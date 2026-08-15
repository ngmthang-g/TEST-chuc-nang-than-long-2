    if (!ScalarGetter(ac, "get_IsAutoPathing", ap, pathing, detail, cap)) return false;
    s.autoPathing = pathing ? 1 : 0; s.validMask |= ValidAutoPath;

    const MethodInfo* getName = FindMethod(lc, "get_Name", 0);
    if (getName) {
        Il2CppObject* no = nullptr;
        wchar_t ignored[128]{};
        if (InvokeObject(getName, leader, no, ignored, _countof(ignored)) && no)
            (void)CopyString(reinterpret_cast<Il2CppString*>(no), s.characterName, _countof(s.characterName));
    }

    SetText(detail, cap, L"STATE map="); AppendInt(detail, cap, s.mapID);
    Append(detail, cap, L" pos="); AppendInt(detail, cap, s.x); Append(detail, cap, L","); AppendInt(detail, cap, s.y);
    Append(detail, cap, L" riding="); AppendInt(detail, cap, s.riding);
    Append(detail, cap, L" autoPath="); AppendInt(detail, cap, s.autoPathing);
    return true;
}

bool SafeForAction(const Classes& c, wchar_t* detail, std::size_t cap) {
    int ready = 0, waiting = 0;
    if (!Transition(c, ready, waiting, detail, cap)) return false;
    if (!ready || waiting) { SetText(detail, cap, L"Action bị chặn: đang chuyển map"); return false; }
    return true;
}

bool ToggleRide(bool desiredRiding, wchar_t* detail, std::size_t cap) {
    Classes c{}; if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;
    Il2CppObject* leader = nullptr; Il2CppClass* lc = nullptr;
    if (!GetLeader(c, leader, lc, detail, cap)) return false;
    int riding = 0;
    if (!ScalarGetter(lc, "get_IsRiding", leader, riding, detail, cap)) return false;
    if ((riding != 0) == desiredRiding) { SetText(detail, cap, L"Ride state đã đúng; không toggle lại"); return true; }

    const MethodInfo* getSlot = ExactMethod(c.gameApi, "get_CurrentMountSlot", 0, true);
    const MethodInfo* toggle = ExactMethod(c.gameApi, "SendToggleRideState", 1, true, "System.Int32");
    if (!getSlot || !toggle) { SetText(detail, cap, L"Không resolve được API lên/xuống ngựa"); return false; }
    std::int64_t slot64 = 0;
    if (!InvokeScalar(getSlot, nullptr, slot64, detail, cap) || slot64 < 0 || slot64 > INT32_MAX) return false;
    std::int32_t slot = static_cast<std::int32_t>(slot64);
    void* args[] = { &slot };
    if (!InvokeVoid(toggle, nullptr, args, detail, cap)) return false;
    SetText(detail, cap, desiredRiding ? L"Đã gửi lệnh lên ngựa" : L"Đã gửi lệnh xuống ngựa");
    return true;
}

bool StartPath(int mapID, int x, int y, wchar_t* detail, std::size_t cap) {
    if (mapID <= 0) { SetText(detail, cap, L"MapID đích không hợp lệ"); return false; }
    Classes c{}; if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;
    Il2CppObject* ap = nullptr; Il2CppClass* ac = nullptr;
    if (!AutoPathInstance(c, ap, ac, detail, cap)) return false;
    const MethodInfo* start = ExactMethod(ac, "StartAutoPath", 3, false,
                                          "System.Int32", "System.Int32", "System.Int32");
    if (!start) { SetText(detail, cap, L"Không resolve đúng StartAutoPath(Int32,Int32,Int32)"); return false; }
    std::int32_t m = mapID, px = x, py = y;
    void* args[] = { &m, &px, &py };
    if (!InvokeVoid(start, ap, args, detail, cap)) return false;
    SetText(detail, cap, L"Đã gửi AutoPath tới map="); AppendInt(detail, cap, mapID);
    Append(detail, cap, L" x="); AppendInt(detail, cap, x); Append(detail, cap, L" y="); AppendInt(detail, cap, y);
    return true;
}

bool StopPath(wchar_t* detail, std::size_t cap) {
    Classes c{}; if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;
    const MethodInfo* stop = ExactMethod(c.gameApi, "StopAutoPath", 0, true);
    if (!stop) { SetText(detail, cap, L"Không resolve được LuaSystemAPI_Game.StopAutoPath()"); return false; }
    if (!InvokeVoid(stop, nullptr, nullptr, detail, cap)) return false;
    SetText(detail, cap, L"Đã gửi StopAutoPath");
    return true;
}

bool EnsureShared() {
    if (g_shared) return true;
    wchar_t name[96]{}; MappingName(GetCurrentProcessId(), name, _countof(name));
    g_mapping = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, name);
    if (!g_mapping) return false;
    g_shared = reinterpret_cast<SharedBlock*>(MapViewOfFile(g_mapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedBlock)));
    if (!g_shared || g_shared->magic != kMagic || g_shared->protocolVersion != kProtocolVersion ||
        g_shared->targetPid != GetCurrentProcessId()) {
        if (g_shared) UnmapViewOfFile(g_shared);
        if (g_mapping) CloseHandle(g_mapping);
        g_shared = nullptr; g_mapping = nullptr; return false;
    }
    InterlockedExchange(&g_shared->bridgeLoaded, 1);
    return true;
}

void ProcessRequest() {
    if (!EnsureShared()) return;
    const LONG seq = g_shared->requestSeq;
    if (seq <= 0 || seq == g_shared->completedSeq) return;
    if (InterlockedCompareExchange(&g_shared->bridgeBusy, 1, 0) != 0) return;

    Response r{};
    r.callbackThreadId = GetCurrentThreadId();
    wchar_t detail[512]{};
    bool ok = false;
    if (r.callbackThreadId != g_shared->targetWindowThreadId) {
        SetText(detail, _countof(detail), L"Sai callback thread; action bị chặn");
        r.errorCode = 1001;
    } else {
        const Command cmd = static_cast<Command>(g_shared->request.command);
        switch (cmd) {
            case Command::ReadState:
                ok = ReadState(r.snapshot, detail, _countof(detail)); r.errorCode = ok ? 0 : 1101; break;
            case Command::ToggleRide:
                ok = ToggleRide(g_shared->request.arg0 != 0, detail, _countof(detail)); r.errorCode = ok ? 0 : 1201; break;
            case Command::StartPath:
                ok = StartPath(g_shared->request.arg0, g_shared->request.arg1, g_shared->request.arg2,
                               detail, _countof(detail)); r.errorCode = ok ? 0 : 1301; break;
            case Command::StopPath:
                ok = StopPath(detail, _countof(detail)); r.errorCode = ok ? 0 : 1401; break;
            case Command::ClickNpc:
                ok = ClickNpc(g_shared->request.arg0, detail, _countof(detail)); r.errorCode = ok ? 0 : 1501; break;
            case Command::InspectHealDialog:
                ok = InspectHealDialog(r.snapshot, detail, _countof(detail)); r.errorCode = ok ? 0 : 1601; break;
            case Command::ClickHealDialogChoice:

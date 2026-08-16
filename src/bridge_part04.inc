    return CopyString(reinterpret_cast<Il2CppString*>(value), out, cap);
}

bool WalkForButton(Il2CppObject* object, const wchar_t* wanted, Il2CppObject*& found,
                   int depth, wchar_t* detail, std::size_t cap) {
    found = nullptr;
    if (!object || depth > 32) return true;
    Il2CppClass* oc = g_api.object_get_class(object);
    if (!oc) return true;

    wchar_t text[256]{};
    if (ReadUiText(object, text, _countof(text)) && TextEq(text, wanted)) {
        const MethodInfo* click = FindMethod(oc, "HandleClickEvent", 0);
        if (click && !StaticMethod(click)) { found = object; return true; }
    }

    const MethodInfo* getChildren = FindMethod(oc, "get_CoreChildren", 0);
    if (!getChildren) return true;
    Il2CppObject* children = nullptr;
    wchar_t ignored[128]{};
    if (!InvokeObject(getChildren, object, children, ignored, _countof(ignored)) || !children) return true;
    int count = 0;
    if (!GetListCount(children, count, ignored, _countof(ignored))) return true;
    if (count > 512) count = 512;
    for (int i = 0; i < count; ++i) {
        Il2CppObject* child = nullptr;
        if (!GetListItem(children, i, child, ignored, _countof(ignored)) || !child) continue;
        if (!WalkForButton(child, wanted, found, depth + 1, detail, cap)) return false;
        if (found) return true;
    }
    return true;
}

bool FindButtonInUi(const Classes& c, const wchar_t* uiName, const wchar_t* buttonText,
                    Il2CppObject*& button, wchar_t* detail, std::size_t cap) {
    button = nullptr;
    Il2CppObject* root = nullptr;
    if (!FindUiRoot(c, uiName, root, detail, cap) || !root) return true;
    return WalkForButton(root, buttonText, button, 0, detail, cap);
}

bool FindLuaUiObjectV113(const Classes& c, const wchar_t* uiName, Il2CppObject*& ui,
                         wchar_t* detail, std::size_t cap) {
    ui = nullptr;
    if (!c.guiApi) { SetText(detail, cap, L"LuaSystemAPI_GUI chưa resolve"); return false; }
    Il2CppString* name = NewString(uiName);
    if (!name) { SetText(detail, cap, L"Không tạo được tên Lua UI"); return false; }
    void* args[] = { name };

    const MethodInfo* find = ExactMethod(c.guiApi, "FindUI", 1, true, "System.String");
    if (find) {
        wchar_t probe[128]{};
        if (!InvokeObjectArgs(find, nullptr, args, ui, probe, _countof(probe))) {
            SetText(detail, cap, probe); return false;
        }
        if (ui) return true;
    }

    const MethodInfo* mainFind = ExactMethod(c.guiApi, "MainFindUI", 1, true, "System.String");
    if (mainFind) {
        wchar_t probe[128]{};
        if (!InvokeObjectArgs(mainFind, nullptr, args, ui, probe, _countof(probe))) {
            SetText(detail, cap, probe); return false;
        }
        if (ui) return true;
    }

    SetText(detail, cap, L"Không tìm thấy Lua UI object: "); Append(detail, cap, uiName);
    return true;
}

const MethodInfo* ResolveExecuteUiObjectV113(Il2CppClass* executorClass) {
    if (!executorClass) return nullptr;
    const MethodInfo* m = FindMethod(executorClass, "ExecuteUIObject", 3);
    if (!m) m = FindMethod(executorClass, "ExecuteUiObject", 3);
    if (!m || StaticMethod(m)) return nullptr;
    return m;
}

bool InvokeLuaUiActionV113(const Classes& c, const wchar_t* uiName, const wchar_t* functionName,
                           Il2CppObject* argument, bool hasArgument,
                           wchar_t* detail, std::size_t cap) {
    if (!c.executor) { SetText(detail, cap, L"MonoBehaviourExecutor chưa resolve"); return false; }

    Il2CppObject* ui = nullptr;
    if (!FindLuaUiObjectV113(c, uiName, ui, detail, cap) || !ui) return false;

    const MethodInfo* getInstance = FindMethod(c.executor, "get_Instance", 0);
    if (!getInstance || !StaticMethod(getInstance)) {
        SetText(detail, cap, L"Không resolve MonoBehaviourExecutor.Instance"); return false;
    }
    Il2CppObject* executor = nullptr;
    if (!InvokeObject(getInstance, nullptr, executor, detail, cap) || !executor) return false;

    const MethodInfo* execute = ResolveExecuteUiObjectV113(c.executor);
    if (!execute) {
        SetText(detail, cap, L"Không resolve ExecuteUIObject/ExecuteUiObject(3 args)");
        return false;
    }

    Il2CppString* function = NewString(functionName);
    if (!function) { SetText(detail, cap, L"Không tạo được tên Lua function"); return false; }
    Il2CppObject* luaArgs = nullptr;
    if (!CreateObjectArrayV113(argument, hasArgument, luaArgs, detail, cap) || !luaArgs) return false;

    void* args[] = { ui, function, luaArgs };
    if (!InvokeVoid(execute, executor, args, detail, cap)) return false;

    SetText(detail, cap, L"Đã gọi Lua action: "); Append(detail, cap, uiName);
    Append(detail, cap, L"."); Append(detail, cap, functionName);
    return true;
}

bool InspectHealDialog(Snapshot& s, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap)) return false;
    if (!c.guiApi && !c.executor) { SetText(detail, cap, L"Không resolve được lớp UI"); return false; }
    s = {};
    s.validMask |= ValidHealDialog;
    Il2CppObject* b = nullptr;
    if (!FindButtonInUi(c, L"GameDialog", L"Trị liệu", b, detail, cap)) return false;
    if (b) s.healDialogMask |= HealDialogTreatment;
    b = nullptr;
    if (!FindButtonInUi(c, L"MessageBox", L"Xác nhận", b, detail, cap)) return false;
    if (!b && !FindButtonInUi(c, L"GameDialog", L"Xác nhận", b, detail, cap)) return false;
    if (b) s.healDialogMask |= HealDialogConfirm;
    b = nullptr;
    if (!FindButtonInUi(c, L"GameDialog", L"Ta biết rồi", b, detail, cap)) return false;
    if (b) s.healDialogMask |= HealDialogKnow;
    SetText(detail, cap, L"HEAL_DIALOG mask="); AppendInt(detail, cap, static_cast<int>(s.healDialogMask));
    return true;
}

bool ClickNpc(int npcID, wchar_t* detail, std::size_t cap) {
    if (npcID <= 0) { SetText(detail, cap, L"NPC ID không hợp lệ"); return false; }
    Classes c{}; if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;
    const MethodInfo* click = ExactMethod(c.gameApi, "ClickNPC", 1, true, "System.Int32");
    if (!click) { SetText(detail, cap, L"Không resolve được ClickNPC(Int32)"); return false; }
    std::int32_t id = npcID; void* args[] = { &id };
    if (!InvokeVoid(click, nullptr, args, detail, cap)) return false;
    SetText(detail, cap, L"Đã gọi ClickNPC npcID="); AppendInt(detail, cap, npcID);
    return true;
}

bool ClickHealDialogChoice(HealDialogChoice choice, wchar_t* detail, std::size_t cap) {
    Classes c{}; if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;

    if (choice == HealDialogChoice::Treatment || choice == HealDialogChoice::Know) {
        const wchar_t* label = choice == HealDialogChoice::Treatment ? L"Trị liệu" : L"Ta biết rồi";
        Il2CppObject* button = nullptr;
        if (!FindButtonInUi(c, L"GameDialog", label, button, detail, cap)) return false;
        if (!button) { SetText(detail, cap, L"Chưa thấy GameDialog button sống: "); Append(detail, cap, label); return false; }
        // Asset-confirmed business callback. Do NOT call UIButton.HandleClickEvent here:
        // GameDialog.FunctionButtonClicked reads button.Tag, sends CMD_SHOW_GAMEDIALOG, then destroys dialog.
        if (!InvokeLuaUiActionV113(c, L"GameDialog", L"FunctionButtonClicked",
                                   button, true, detail, cap)) return false;
        Append(detail, cap, L" • label="); Append(detail, cap, label);
        return true;
    }

    if (choice == HealDialogChoice::Confirm) {
        // Preferred asset-confirmed confirmation path.
        Il2CppObject* messageBox = nullptr;
        wchar_t probe[192]{};
        if (!FindLuaUiObjectV113(c, L"MessageBox", messageBox, probe, _countof(probe))) {
            SetText(detail, cap, probe); return false;
        }
        if (messageBox) {
            return InvokeLuaUiActionV113(c, L"MessageBox", L"ButtonOKClicked",
                                         nullptr, false, detail, cap);
        }

        // Conservative server-variant fallback: if confirm is another GameDialog choice,
        // use the same exact GameDialog business callback rather than generic HandleClickEvent.
        Il2CppObject* button = nullptr;
        if (!FindButtonInUi(c, L"GameDialog", L"Xác nhận", button, detail, cap)) return false;
        if (!button) { SetText(detail, cap, L"Chưa thấy MessageBox hoặc GameDialog Xác nhận"); return false; }
        return InvokeLuaUiActionV113(c, L"GameDialog", L"FunctionButtonClicked",
                                     button, true, detail, cap);
    }

    SetText(detail, cap, L"HealDialogChoice không hợp lệ");
    return false;
}

bool ReadState(Snapshot& s, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap)) return false;
    static std::uint32_t seq = 0;
    s = {}; s.sequence = ++seq;
    int ready = 0, waiting = 0;
    if (!Transition(c, ready, waiting, detail, cap)) return false;
    s.mapReady = ready ? 1 : 0;
    s.waitingChangeMap = waiting ? 1 : 0;
    s.validMask |= ValidMapTransition;
    if (!ready || waiting) {
        SetText(detail, cap, L"Đang chuyển map; không đọc object sâu và không gửi action");
        return true;
    }

    Il2CppObject* leader = nullptr; Il2CppClass* lc = nullptr;
    if (!GetLeader(c, leader, lc, detail, cap)) return false;
    int role = 0, map = 0, x = 0, y = 0, riding = 0;
    if (!ScalarGetter(lc, "get_RoleID", leader, role, detail, cap) || role <= 0) return false;
    if (!ScalarGetter(lc, "get_MapID", leader, map, detail, cap) || map <= 0) return false;
    if (!ReadPosition(leader, lc, x, y, detail, cap)) return false;
    if (!ScalarGetter(lc, "get_IsRiding", leader, riding, detail, cap)) return false;
    s.roleID = role; s.mapID = map; s.x = x; s.y = y; s.riding = riding ? 1 : 0;
    s.validMask |= ValidIdentity | ValidMap | ValidPosition | ValidRiding;

    int moving = 0;
    if (StaticScalar(c.gameApi, "IsMoving", moving, detail, cap)) {
        s.moving = moving ? 1 : 0; s.validMask |= ValidMoving;
    }

    Il2CppObject* ap = nullptr; Il2CppClass* ac = nullptr;
    if (!AutoPathInstance(c, ap, ac, detail, cap)) return false;
    int pathing = 0;

namespace {

bool IsWsV117(wchar_t c) {
    return c == L' ' || c == L'\t' || c == L'\r' || c == L'\n' || c == 0x00A0;
}

void NormalizeSemanticTextV117(const wchar_t* in, wchar_t* out, std::size_t cap) {
    if (!out || cap == 0) return;
    out[0] = 0;
    if (!in) return;
    std::size_t n = 0;
    bool inTag = false;
    bool pendingSpace = false;
    for (const wchar_t* p = in; *p && n + 1 < cap; ++p) {
        wchar_t c = *p;
        if (c == L'<') { inTag = true; continue; }
        if (inTag) { if (c == L'>') inTag = false; continue; }
        if (IsWsV117(c)) { if (n) pendingSpace = true; continue; }
        if (pendingSpace && n + 1 < cap) { out[n++] = L' '; pendingSpace = false; }
        if (c >= L'A' && c <= L'Z') c = static_cast<wchar_t>(c - L'A' + L'a');
        out[n++] = c;
    }
    while (n && out[n - 1] == L' ') --n;
    out[n] = 0;
}

bool SemanticTextEqV117(const wchar_t* a, const wchar_t* b) {
    wchar_t na[256]{}, nb[256]{};
    NormalizeSemanticTextV117(a, na, _countof(na));
    NormalizeSemanticTextV117(b, nb, _countof(nb));
    return wcscmp(na, nb) == 0;
}

bool FirstTextInSubtreeV117(Il2CppObject* object, wchar_t* out, std::size_t cap, int depth) {
    if (!object || !out || cap == 0 || depth > 6) return false;
    wchar_t text[256]{};
    if (ReadUiText(object, text, _countof(text))) {
        wchar_t normalized[256]{};
        NormalizeSemanticTextV117(text, normalized, _countof(normalized));
        if (normalized[0]) {
            wcsncpy_s(out, cap, normalized, _TRUNCATE);
            return true;
        }
    }
    Il2CppClass* oc = g_api.object_get_class(object);
    if (!oc) return false;
    const MethodInfo* getChildren = FindMethod(oc, "get_CoreChildren", 0);
    if (!getChildren) return false;
    Il2CppObject* children = nullptr;
    wchar_t ignored[128]{};
    if (!InvokeObject(getChildren, object, children, ignored, _countof(ignored)) || !children) return false;
    int count = 0;
    if (!GetListCount(children, count, ignored, _countof(ignored))) return false;
    if (count > 128) count = 128;
    for (int i = 0; i < count; ++i) {
        Il2CppObject* child = nullptr;
        if (!GetListItem(children, i, child, ignored, _countof(ignored)) || !child) continue;
        if (FirstTextInSubtreeV117(child, out, cap, depth + 1)) return true;
    }
    return false;
}

struct DialogScanV117 {
    int clickableCount = 0;
    wchar_t labels[320]{};
};

void AddObservedLabelV117(DialogScanV117& scan, const wchar_t* label) {
    if (!label || !label[0]) return;
    if (scan.labels[0]) Append(scan.labels, _countof(scan.labels), L" | ");
    Append(scan.labels, _countof(scan.labels), label);
}

bool WalkForButtonV117(Il2CppObject* object, const wchar_t* wanted, Il2CppObject*& found,
                       DialogScanV117& scan, int depth) {
    if (!object || depth > 32 || found) return true;
    Il2CppClass* oc = g_api.object_get_class(object);
    if (!oc) return true;

    const MethodInfo* click = FindMethod(oc, "HandleClickEvent", 0);
    if (click && !StaticMethod(click)) {
        ++scan.clickableCount;
        wchar_t label[192]{};
        if (FirstTextInSubtreeV117(object, label, _countof(label), 0)) {
            AddObservedLabelV117(scan, label);
            if (SemanticTextEqV117(label, wanted)) {
                found = object;
                return true;
            }
        }
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
        if (!WalkForButtonV117(child, wanted, found, scan, depth + 1)) return false;
        if (found) return true;
    }
    return true;
}

bool FindButtonInUi(const Classes& c, const wchar_t* uiName, const wchar_t* buttonText,
                    Il2CppObject*& button, wchar_t* detail, std::size_t cap) {
    button = nullptr;
    Il2CppObject* root = nullptr;
    if (!FindUiRoot(c, uiName, root, detail, cap)) return false;
    if (!root) {
        Il2CppObject* luaUi = nullptr;
        wchar_t probe[160]{};
        if (!FindLuaUiObjectV113(c, uiName, luaUi, probe, _countof(probe))) {
            SetText(detail, cap, probe);
            return false;
        }
        SetText(detail, cap, luaUi ? L"DIALOG_V117: Lua UI tồn tại nhưng UIRoot chưa sẵn sàng: "
                                   : L"DIALOG_V117: chưa thấy Lua UI/UIRoot: ");
        Append(detail, cap, uiName);
        return true;
    }

    DialogScanV117 scan{};
    if (!WalkForButtonV117(root, buttonText, button, scan, 0)) return false;
    if (button) {
        SetText(detail, cap, L"DIALOG_V117 MATCH • UI="); Append(detail, cap, uiName);
        Append(detail, cap, L" • wanted="); Append(detail, cap, buttonText);
        Append(detail, cap, L" • clickable="); AppendInt(detail, cap, scan.clickableCount);
        Append(detail, cap, L" • labels="); Append(detail, cap, scan.labels);
        return true;
    }

    SetText(detail, cap, L"DIALOG_V117 NO MATCH • UI="); Append(detail, cap, uiName);
    Append(detail, cap, L" • wanted="); Append(detail, cap, buttonText);
    Append(detail, cap, L" • clickable="); AppendInt(detail, cap, scan.clickableCount);
    Append(detail, cap, L" • labels="); Append(detail, cap, scan.labels[0] ? scan.labels : L"<none>");
    return true;
}

bool LuaUiExistsV117(const Classes& c, const wchar_t* uiName, bool& exists,
                     wchar_t* detail, std::size_t cap) {
    exists = false;
    Il2CppObject* ui = nullptr;
    wchar_t probe[192]{};
    if (!FindLuaUiObjectV113(c, uiName, ui, probe, _countof(probe))) {
        SetText(detail, cap, probe);
        return false;
    }
    exists = ui != nullptr;
    return true;
}

bool InspectHealDialog(Snapshot& s, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap)) return false;
    if (!c.guiApi && !c.executor) { SetText(detail, cap, L"Không resolve được lớp UI"); return false; }
    s = {};
    s.validMask |= ValidHealDialog;

    bool gameDialogExists = false;
    if (!LuaUiExistsV117(c, L"GameDialog", gameDialogExists, detail, cap)) return false;

    Il2CppObject* b = nullptr;
    wchar_t treatmentProbe[384]{};
    if (!FindButtonInUi(c, L"GameDialog", L"Trị liệu", b, treatmentProbe, _countof(treatmentProbe))) return false;
    if (b) {
        s.healDialogMask |= HealDialogTreatment;
    } else if (gameDialogExists) {
        // Critical v1.1.7 guard: once the server-driven GameDialog exists, do not reopen NPC.
        // Force the FSM into the action attempt; the action path will either resolve the live
        // button on a later tick or fail with the detailed observed-label diagnostic.
        s.healDialogMask |= HealDialogTreatment;
    }

    b = nullptr;
    if (!FindButtonInUi(c, L"MessageBox", L"Xác nhận", b, detail, cap)) return false;
    if (!b && !FindButtonInUi(c, L"GameDialog", L"Xác nhận", b, detail, cap)) return false;
    if (b) s.healDialogMask |= HealDialogConfirm;

    b = nullptr;
    if (!FindButtonInUi(c, L"GameDialog", L"Ta biết rồi", b, detail, cap)) return false;
    if (b) s.healDialogMask |= HealDialogKnow;

    SetText(detail, cap, L"HEAL_DIALOG_V117 mask="); AppendInt(detail, cap, static_cast<int>(s.healDialogMask));
    Append(detail, cap, gameDialogExists ? L" • GameDialog=present" : L" • GameDialog=absent");
    Append(detail, cap, L" • treatmentProbe={"); Append(detail, cap, treatmentProbe); Append(detail, cap, L"}");
    return true;
}

} // namespace

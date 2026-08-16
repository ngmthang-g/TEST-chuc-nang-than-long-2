namespace {

bool IsWsV118(wchar_t c) { return c == L' ' || c == L'\t' || c == L'\r' || c == L'\n' || c == 0x00A0; }

void NormalizeSemanticTextV118(const wchar_t* in, wchar_t* out, std::size_t cap) {
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
        if (IsWsV118(c)) { if (n) pendingSpace = true; continue; }
        if (pendingSpace && n + 1 < cap) { out[n++] = L' '; pendingSpace = false; }
        if (c >= L'A' && c <= L'Z') c = static_cast<wchar_t>(c - L'A' + L'a');
        out[n++] = c;
    }
    while (n && out[n - 1] == L' ') --n;
    out[n] = 0;
}

bool SemanticTextEqV118(const wchar_t* a, const wchar_t* b) {
    wchar_t na[256]{}, nb[256]{};
    NormalizeSemanticTextV118(a, na, _countof(na));
    NormalizeSemanticTextV118(b, nb, _countof(nb));
    return wcscmp(na, nb) == 0;
}

struct DialogScanV118 {
    int clickableCount = 0;
    int textCount = 0;
    wchar_t labels[320]{};
};

void AddObservedLabelV118(DialogScanV118& scan, const wchar_t* label) {
    if (!label || !label[0]) return;
    if (scan.labels[0]) Append(scan.labels, _countof(scan.labels), L" | ");
    Append(scan.labels, _countof(scan.labels), label);
}

bool SubtreeHasSemanticTextV118(Il2CppObject* object, const wchar_t* wanted,
                                DialogScanV118& scan, int depth) {
    if (!object || depth > 8) return false;
    wchar_t text[256]{};
    if (ReadUiText(object, text, _countof(text))) {
        wchar_t normalized[256]{};
        NormalizeSemanticTextV118(text, normalized, _countof(normalized));
        if (normalized[0]) {
            ++scan.textCount;
            AddObservedLabelV118(scan, normalized);
            if (SemanticTextEqV118(normalized, wanted)) return true;
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
        if (SubtreeHasSemanticTextV118(child, wanted, scan, depth + 1)) return true;
    }
    return false;
}

bool WalkForButtonV118(Il2CppObject* object, const wchar_t* wanted,
                       Il2CppObject*& found, DialogScanV118& scan, int depth) {
    if (!object || depth > 32 || found) return true;
    Il2CppClass* oc = g_api.object_get_class(object);
    if (!oc) return true;
    const MethodInfo* click = FindMethod(oc, "HandleClickEvent", 0);
    if (click && !StaticMethod(click)) {
        ++scan.clickableCount;
        if (SubtreeHasSemanticTextV118(object, wanted, scan, 0)) {
            found = object;
            return true;
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
        if (!WalkForButtonV118(child, wanted, found, scan, depth + 1)) return false;
        if (found) return true;
    }
    return true;
}

bool ReadButtonSelectionIdV118(Il2CppObject* button, std::int32_t& selectionID,
                               wchar_t* detail, std::size_t cap) {
    selectionID = 0;
    if (!button) { SetText(detail, cap, L"selectionID: button=null"); return false; }
    Il2CppClass* bc = g_api.object_get_class(button);
    if (!bc) { SetText(detail, cap, L"selectionID: class=null"); return false; }

    const MethodInfo* getTag = FindMethod(bc, "get_Tag", 0);
    if (getTag && !StaticMethod(getTag)) {
        const Il2CppType* rt = g_api.method_get_return_type(getTag);
        char* tn = rt ? g_api.type_get_name(rt) : nullptr;
        if (tn) {
            if (Eq(tn, "System.Int32") || Eq(tn, "System.UInt32") || Eq(tn, "System.Int64")) {
                std::int64_t value = 0;
                const bool ok = InvokeScalar(getTag, button, value, detail, cap);
                g_api.free_fn(tn);
                if (ok && value > 0 && value <= 2147483647LL) {
                    selectionID = static_cast<std::int32_t>(value);
                    return true;
                }
                SetText(detail, cap, L"selectionID: numeric Tag invalid");
                return false;
            }
            if (Eq(tn, "System.String")) {
                Il2CppObject* value = nullptr;
                wchar_t text[64]{};
                const bool ok = InvokeObject(getTag, button, value, detail, cap) && value &&
                                CopyString(reinterpret_cast<Il2CppString*>(value), text, _countof(text));
                g_api.free_fn(tn);
                if (!ok) return false;
                std::int64_t parsed = 0;
                for (const wchar_t* p = text; *p; ++p) {
                    if (*p < L'0' || *p > L'9') { SetText(detail, cap, L"selectionID: string Tag not numeric"); return false; }
                    parsed = parsed * 10 + (*p - L'0');
                    if (parsed > 2147483647LL) { SetText(detail, cap, L"selectionID: string Tag overflow"); return false; }
                }
                if (parsed > 0) { selectionID = static_cast<std::int32_t>(parsed); return true; }
                SetText(detail, cap, L"selectionID: string Tag invalid");
                return false;
            }
            if (Eq(tn, "System.Object")) {
                Il2CppObject* boxed = nullptr;
                const bool ok = InvokeObject(getTag, button, boxed, detail, cap) && boxed;
                g_api.free_fn(tn);
                if (!ok) return false;
                void* raw = g_api.object_unbox(boxed);
                if (!raw) { SetText(detail, cap, L"selectionID: object Tag cannot unbox"); return false; }
                const std::int32_t value = *reinterpret_cast<const std::int32_t*>(raw);
                if (value > 0) { selectionID = value; return true; }
                SetText(detail, cap, L"selectionID: object Tag invalid");
                return false;
            }
            g_api.free_fn(tn);
        }
    }

    const char* names[] = {"Tag", "tag", "<Tag>k__BackingField"};
    FieldInfo* tagField = nullptr;
    for (Il2CppClass* cls = bc; cls && !tagField; cls = g_api.class_get_parent(cls)) {
        for (const char* name : names) {
            tagField = g_api.class_get_field_from_name(cls, name);
            if (tagField) break;
        }
    }
    if (!tagField) { SetText(detail, cap, L"selectionID: Tag unresolved"); return false; }
    const Il2CppType* ft = g_api.field_get_type(tagField);
    char* ftn = ft ? g_api.type_get_name(ft) : nullptr;
    if (!ftn) { SetText(detail, cap, L"selectionID: Tag type unresolved"); return false; }
    bool ok = false;
    if (Eq(ftn, "System.Int32")) {
        std::int32_t value = 0;
        g_api.field_get_value(button, tagField, &value);
        if (value > 0) { selectionID = value; ok = true; }
    } else if (Eq(ftn, "System.UInt32")) {
        std::uint32_t value = 0;
        g_api.field_get_value(button, tagField, &value);
        if (value > 0 && value <= 2147483647u) { selectionID = static_cast<std::int32_t>(value); ok = true; }
    } else if (Eq(ftn, "System.Int64")) {
        std::int64_t value = 0;
        g_api.field_get_value(button, tagField, &value);
        if (value > 0 && value <= 2147483647LL) { selectionID = static_cast<std::int32_t>(value); ok = true; }
    }
    g_api.free_fn(ftn);
    if (!ok) SetText(detail, cap, L"selectionID: Tag value/type unsupported");
    return ok;
}

bool FindButtonInUi(const Classes& c, const wchar_t* uiName, const wchar_t* buttonText,
                    Il2CppObject*& button, wchar_t* detail, std::size_t cap) {
    button = nullptr;
    Il2CppObject* root = nullptr;
    if (!FindUiRoot(c, uiName, root, detail, cap)) return false;
    if (!root) {
        Il2CppObject* luaUi = nullptr;
        wchar_t probe[160]{};
        if (!FindLuaUiObjectV113(c, uiName, luaUi, probe, _countof(probe))) { SetText(detail, cap, probe); return false; }
        SetText(detail, cap, luaUi ? L"DIALOG_V118: Lua UI present but UIRoot not ready: " : L"DIALOG_V118: Lua UI/UIRoot absent: ");
        Append(detail, cap, uiName);
        return true;
    }

    DialogScanV118 scan{};
    if (!WalkForButtonV118(root, buttonText, button, scan, 0)) return false;
    SetText(detail, cap, button ? L"DIALOG_V118 MATCH • UI=" : L"DIALOG_V118 NO MATCH • UI=");
    Append(detail, cap, uiName);
    Append(detail, cap, L" • wanted="); Append(detail, cap, buttonText);
    Append(detail, cap, L" • clickable="); AppendInt(detail, cap, scan.clickableCount);
    Append(detail, cap, L" • texts="); AppendInt(detail, cap, scan.textCount);
    Append(detail, cap, L" • labels="); Append(detail, cap, scan.labels[0] ? scan.labels : L"<none>");

    if (button && wcscmp(uiName, L"GameDialog") == 0) {
        std::int32_t selectionID = 0;
        wchar_t idProbe[160]{};
        if (ReadButtonSelectionIdV118(button, selectionID, idProbe, _countof(idProbe))) {
            Append(detail, cap, L" • selectionID="); AppendInt(detail, cap, selectionID);
        } else {
            Append(detail, cap, L" • selectionID=UNRESOLVED("); Append(detail, cap, idProbe); Append(detail, cap, L")");
        }
    }
    return true;
}

bool LuaUiExistsV118(const Classes& c, const wchar_t* uiName, bool& exists, wchar_t* detail, std::size_t cap) {
    exists = false;
    Il2CppObject* ui = nullptr;
    wchar_t probe[192]{};
    if (!FindLuaUiObjectV113(c, uiName, ui, probe, _countof(probe))) { SetText(detail, cap, probe); return false; }
    exists = ui != nullptr;
    return true;
}

bool InspectHealDialog(Snapshot& s, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap)) return false;
    if (!c.guiApi && !c.executor) { SetText(detail, cap, L"No UI class resolved"); return false; }
    s = {};
    s.validMask |= ValidHealDialog;

    bool gameDialogExists = false;
    if (!LuaUiExistsV118(c, L"GameDialog", gameDialogExists, detail, cap)) return false;
    if (gameDialogExists) s.healDialogMask |= HealDialogPresent;

    Il2CppObject* b = nullptr;
    wchar_t treatmentProbe[384]{};
    if (!FindButtonInUi(c, L"GameDialog", L"Trị liệu", b, treatmentProbe, _countof(treatmentProbe))) return false;
    SetText(s.healDialogDetail, _countof(s.healDialogDetail), treatmentProbe);
    if (b) s.healDialogMask |= HealDialogTreatment;

    b = nullptr;
    if (!FindButtonInUi(c, L"MessageBox", L"Xác nhận", b, detail, cap)) return false;
    if (!b && !FindButtonInUi(c, L"GameDialog", L"Xác nhận", b, detail, cap)) return false;
    if (b) s.healDialogMask |= HealDialogConfirm;

    b = nullptr;
    if (!FindButtonInUi(c, L"GameDialog", L"Ta biết rồi", b, detail, cap)) return false;
    if (b) s.healDialogMask |= HealDialogKnow;

    SetText(detail, cap, L"HEAL_DIALOG_V118 mask=");
    AppendInt(detail, cap, static_cast<int>(s.healDialogMask));
    Append(detail, cap, gameDialogExists ? L" • GameDialog=present" : L" • GameDialog=absent");
    Append(detail, cap, L" • treatmentProbe={"); Append(detail, cap, treatmentProbe); Append(detail, cap, L"}");
    return true;
}

} // namespace

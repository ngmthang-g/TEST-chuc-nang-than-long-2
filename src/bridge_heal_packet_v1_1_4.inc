namespace {

bool ReadButtonSelectionIdV114(Il2CppObject* button, std::int32_t& selectionID,
                               wchar_t* detail, std::size_t cap) {
    selectionID = 0;
    if (!button) { SetText(detail, cap, L"Button=null khi đọc selectionID"); return false; }
    Il2CppClass* bc = g_api.object_get_class(button);
    if (!bc) { SetText(detail, cap, L"Không lấy được class của GameDialog button"); return false; }

    // Preferred semantic route: UIButton/UIObject Tag property. GameDialog Lua stores
    // server-provided selectionID in this Tag when it creates each dynamic button.
    const MethodInfo* getTag = FindMethod(bc, "get_Tag", 0);
    if (getTag && !StaticMethod(getTag)) {
        const Il2CppType* rt = g_api.method_get_return_type(getTag);
        char* tn = rt ? g_api.type_get_name(rt) : nullptr;
        if (tn) {
            if (Eq(tn, "System.Int32") || Eq(tn, "System.UInt32") || Eq(tn, "System.Int64")) {
                std::int64_t value = 0;
                const bool ok = InvokeScalar(getTag, button, value, detail, cap);
                g_api.free_fn(tn);
                if (!ok || value <= 0 || value > INT32_MAX) {
                    SetText(detail, cap, L"Tag numeric không phải selectionID hợp lệ");
                    return false;
                }
                selectionID = static_cast<std::int32_t>(value);
                return true;
            }
            if (Eq(tn, "System.String")) {
                Il2CppObject* value = nullptr;
                wchar_t text[64]{};
                const bool ok = InvokeObject(getTag, button, value, detail, cap) && value &&
                                CopyString(reinterpret_cast<Il2CppString*>(value), text, _countof(text));
                g_api.free_fn(tn);
                if (!ok) return false;
                int parsed = 0;
                for (const wchar_t* p = text; *p; ++p) {
                    if (*p < L'0' || *p > L'9') { SetText(detail, cap, L"Tag string không phải số"); return false; }
                    if (parsed > (INT32_MAX - (*p - L'0')) / 10) { SetText(detail, cap, L"Tag string vượt Int32"); return false; }
                    parsed = parsed * 10 + (*p - L'0');
                }
                if (parsed <= 0) { SetText(detail, cap, L"Tag string selectionID không hợp lệ"); return false; }
                selectionID = parsed;
                return true;
            }
            if (Eq(tn, "System.Object")) {
                Il2CppObject* boxed = nullptr;
                const bool ok = InvokeObject(getTag, button, boxed, detail, cap) && boxed;
                g_api.free_fn(tn);
                if (!ok) return false;
                void* raw = g_api.object_unbox(boxed);
                if (!raw) { SetText(detail, cap, L"Tag object không unbox được"); return false; }
                const std::int32_t value = *reinterpret_cast<const std::int32_t*>(raw);
                if (value <= 0) { SetText(detail, cap, L"Tag object selectionID không hợp lệ"); return false; }
                selectionID = value;
                return true;
            }
            g_api.free_fn(tn);
        }
    }

    // Metadata-driven fallback for clients where Tag is exposed as a field/backing field.
    const char* names[] = {"Tag", "tag", "<Tag>k__BackingField"};
    FieldInfo* tagField = nullptr;
    for (Il2CppClass* c = bc; c && !tagField; c = g_api.class_get_parent(c)) {
        for (const char* name : names) {
            tagField = g_api.class_get_field_from_name(c, name);
            if (tagField) break;
        }
    }
    if (!tagField) { SetText(detail, cap, L"Không resolve được UIButton.Tag"); return false; }
    const Il2CppType* ft = g_api.field_get_type(tagField);
    char* ftn = ft ? g_api.type_get_name(ft) : nullptr;
    if (!ftn) { SetText(detail, cap, L"Không đọc được type của UIButton.Tag"); return false; }
    bool ok = false;
    if (Eq(ftn, "System.Int32")) {
        std::int32_t value = 0; g_api.field_get_value(button, tagField, &value);
        if (value > 0) { selectionID = value; ok = true; }
    } else if (Eq(ftn, "System.UInt32")) {
        std::uint32_t value = 0; g_api.field_get_value(button, tagField, &value);
        if (value > 0 && value <= static_cast<std::uint32_t>(INT32_MAX)) { selectionID = static_cast<std::int32_t>(value); ok = true; }
    } else if (Eq(ftn, "System.Int64")) {
        std::int64_t value = 0; g_api.field_get_value(button, tagField, &value);
        if (value > 0 && value <= INT32_MAX) { selectionID = static_cast<std::int32_t>(value); ok = true; }
    }
    g_api.free_fn(ftn);
    if (!ok) SetText(detail, cap, L"UIButton.Tag tồn tại nhưng type/value chưa hỗ trợ");
    return ok;
}

bool SendGameDialogSelectionV114(std::int32_t selectionID, std::int32_t selectedItemID,
                                 wchar_t* detail, std::size_t cap) {
    if (selectionID <= 0) { SetText(detail, cap, L"selectionID không hợp lệ"); return false; }
    const Il2CppImage* image = Image();
    if (!image) { SetText(detail, cap, L"Không mở được Assembly-CSharp khi resolve Network API"); return false; }
    Il2CppClass* network = g_api.class_from_name(image, "FGStudio.LuaSystem.API", "LuaSystemAPI_Network");
    if (!network) { SetText(detail, cap, L"Không resolve LuaSystemAPI_Network"); return false; }

    const MethodInfo* send = ExactMethod(network, "SendPacket", 2, true, "System.Int32", "System.String");
    bool unsignedPacketId = false;
    if (!send) {
        send = ExactMethod(network, "SendPacket", 2, true, "System.UInt32", "System.String");
        unsignedPacketId = send != nullptr;
    }
    if (!send) { SetText(detail, cap, L"Không resolve exact SendPacket(Int32/UInt32,String)"); return false; }

    wchar_t payload[64]{};
    wsprintfW(payload, L"%d:%d", selectionID, selectedItemID);
    Il2CppString* data = NewString(payload);
    if (!data) { SetText(detail, cap, L"Không tạo được payload CMD_SHOW_GAMEDIALOG"); return false; }

    constexpr std::int32_t kCmdShowGameDialog = 100007;
    if (unsignedPacketId) {
        std::uint32_t cmd = static_cast<std::uint32_t>(kCmdShowGameDialog);
        void* args[] = { &cmd, data };
        if (!InvokeVoid(send, nullptr, args, detail, cap)) return false;
    } else {
        std::int32_t cmd = kCmdShowGameDialog;
        void* args[] = { &cmd, data };
        if (!InvokeVoid(send, nullptr, args, detail, cap)) return false;
    }

    SetText(detail, cap, L"Đã gửi CMD_SHOW_GAMEDIALOG 100007 • selectionID=");
    AppendInt(detail, cap, selectionID);
    Append(detail, cap, L" • selectedItemID=");
    AppendInt(detail, cap, selectedItemID);
    return true;
}

bool SendLiveGameDialogChoiceV114(const Classes& c, const wchar_t* label,
                                  wchar_t* detail, std::size_t cap) {
    Il2CppObject* button = nullptr;
    if (!FindButtonInUi(c, L"GameDialog", label, button, detail, cap)) return false;
    if (!button) { SetText(detail, cap, L"Chưa thấy GameDialog button sống: "); Append(detail, cap, label); return false; }
    std::int32_t selectionID = 0;
    if (!ReadButtonSelectionIdV114(button, selectionID, detail, cap)) return false;

    // Lua source verified: GameDialog.FunctionButtonClicked sends
    // CMD_SHOW_GAMEDIALOG with "selectionID:SelectedItemID". For ordinary function
    // choices without an award-item selection, SelectedItemID defaults to -1.
    if (!SendGameDialogSelectionV114(selectionID, -1, detail, cap)) return false;
    Append(detail, cap, L" • label="); Append(detail, cap, label);
    return true;
}

bool ClickHealDialogChoice(cleanroute::HealDialogChoice choice, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;

    if (choice == cleanroute::HealDialogChoice::Treatment)
        return SendLiveGameDialogChoiceV114(c, L"Trị liệu", detail, cap);

    if (choice == cleanroute::HealDialogChoice::Know)
        return SendLiveGameDialogChoiceV114(c, L"Ta biết rồi", detail, cap);

    if (choice == cleanroute::HealDialogChoice::Confirm) {
        // If the server variant exposes Xác nhận as another dynamic GameDialog option,
        // submit its real selectionID directly as well.
        Il2CppObject* dynamicConfirm = nullptr;
        wchar_t probe[192]{};
        if (!FindButtonInUi(c, L"GameDialog", L"Xác nhận", dynamicConfirm, probe, _countof(probe))) {
            SetText(detail, cap, probe); return false;
        }
        if (dynamicConfirm)
            return SendLiveGameDialogChoiceV114(c, L"Xác nhận", detail, cap);

        // MessageBox is callback-based rather than a GameDialog selection. Keep the exact
        // asset-confirmed ButtonOKClicked path for this branch until runtime proves otherwise.
        return ClickHealDialogChoiceV113(choice, detail, cap);
    }

    SetText(detail, cap, L"HealDialogChoice không hợp lệ");
    return false;
}

} // namespace

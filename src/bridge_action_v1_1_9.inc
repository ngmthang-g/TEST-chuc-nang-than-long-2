namespace {

bool SendGameDialogSelectionV119(std::int32_t selectionID, std::int32_t selectedItemID,
                                 wchar_t* detail, std::size_t cap) {
    if (selectionID <= 0) { SetText(detail, cap, L"ACTION_V119: selectionID invalid"); return false; }
    const Il2CppImage* image = Image();
    if (!image) { SetText(detail, cap, L"ACTION_V119: Assembly-CSharp unavailable"); return false; }
    Il2CppClass* network = g_api.class_from_name(image, "FGStudio.LuaSystem.API", "LuaSystemAPI_Network");
    if (!network) { SetText(detail, cap, L"ACTION_V119: LuaSystemAPI_Network unresolved"); return false; }

    const MethodInfo* send = ExactMethod(network, "SendPacket", 2, true, "System.Int32", "System.String");
    bool unsignedPacketId = false;
    if (!send) {
        send = ExactMethod(network, "SendPacket", 2, true, "System.UInt32", "System.String");
        unsignedPacketId = send != nullptr;
    }
    if (!send) { SetText(detail, cap, L"ACTION_V119: SendPacket(Int32/UInt32,String) unresolved"); return false; }

    wchar_t payload[64]{};
    wsprintfW(payload, L"%d:%d", selectionID, selectedItemID);
    Il2CppString* data = NewString(payload);
    if (!data) { SetText(detail, cap, L"ACTION_V119: payload allocation failed"); return false; }

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
    return true;
}

std::int32_t SelectionForChoiceV119(cleanroute::HealDialogChoice choice, const LuaDialogIdsV119& ids) {
    if (choice == cleanroute::HealDialogChoice::Treatment) return ids.treatment;
    if (choice == cleanroute::HealDialogChoice::Confirm) return ids.confirm;
    if (choice == cleanroute::HealDialogChoice::Know) return ids.know;
    return 0;
}

const wchar_t* LabelForChoiceV119(cleanroute::HealDialogChoice choice) {
    if (choice == cleanroute::HealDialogChoice::Treatment) return L"Trị liệu";
    if (choice == cleanroute::HealDialogChoice::Confirm) return L"Xác nhận";
    if (choice == cleanroute::HealDialogChoice::Know) return L"Ta biết rồi";
    return L"?";
}

bool RequireValidatedGameThreadV119(wchar_t* detail, std::size_t cap) {
    // Keep the harmless CTS/MainThread proof as a prerequisite even though this narrow
    // v1.1.9 experiment sends only the semantic network request, not a Unity UI mutation.
    if (!PollMtProofV116(detail, cap)) return false;
    if (!LoadMtExtraApiV116(detail, cap)) return false;
    if (!g_mtApiV116.threadCurrent || !g_mtApiV116.threadCurrent()) {
        SetText(detail, cap, L"ACTION_V119: current hook thread is not IL2CPP-attached");
        return false;
    }
    if (!g_shared || GetCurrentThreadId() != g_shared->targetWindowThreadId) {
        SetText(detail, cap, L"ACTION_V119: callback is not target game window thread");
        return false;
    }
    return true;
}

bool SendCurrentLuaDialogSelectionV119(cleanroute::HealDialogChoice choice,
                                       wchar_t* detail, std::size_t cap) {
    if (!RequireValidatedGameThreadV119(detail, cap)) return false;

    LuaDialogIdsV119 ids{};
    wchar_t probe[320]{};
    if (!ReadLuaDialogIdsV119(ids, probe, _countof(probe))) {
        SetText(detail, cap, L"ACTION_V119 FAIL-CLOSED: cannot re-read live GameDialogData • ");
        Append(detail, cap, probe);
        return false;
    }
    const std::int32_t selectionID = SelectionForChoiceV119(choice, ids);
    if (selectionID <= 0) {
        SetText(detail, cap, L"ACTION_V119 FAIL-CLOSED: requested semantic choice absent in current Selections • label=");
        Append(detail, cap, LabelForChoiceV119(choice));
        Append(detail, cap, L" • live={");
        Append(detail, cap, ids.raw);
        Append(detail, cap, L"}");
        return false;
    }

    wchar_t sendProbe[192]{};
    if (!SendGameDialogSelectionV119(selectionID, -1, sendProbe, _countof(sendProbe))) {
        SetText(detail, cap, sendProbe);
        return false;
    }
    SetText(detail, cap, L"ACTION_V119 SENT semantic CMD_SHOW_GAMEDIALOG • label=");
    Append(detail, cap, LabelForChoiceV119(choice));
    Append(detail, cap, L" • selectionID="); AppendInt(detail, cap, selectionID);
    Append(detail, cap, L" • payload="); AppendInt(detail, cap, selectionID); Append(detail, cap, L":-1");
    Append(detail, cap, L" • source=current Lua GameDialogData.Selections • no UIButton/UIRoot");
    return true;
}

bool ExecuteMessageBoxConfirmV119(const Classes& c, wchar_t* detail, std::size_t cap) {
    if (!RequireValidatedGameThreadV119(detail, cap)) return false;
    Il2CppObject* messageBox = nullptr;
    wchar_t probe[192]{};
    if (!FindLuaUiObjectV113(c, L"MessageBox", messageBox, probe, _countof(probe))) {
        SetText(detail, cap, probe);
        return false;
    }
    if (!messageBox) { SetText(detail, cap, L"ACTION_V119: MessageBox absent"); return false; }
    wchar_t callDetail[192]{};
    if (!InvokeLuaUiActionV113(c, L"MessageBox", L"ButtonOKClicked", nullptr, false,
                               callDetail, _countof(callDetail))) {
        SetText(detail, cap, callDetail);
        return false;
    }
    SetText(detail, cap, L"ACTION_V119 CALLED MessageBox.ButtonOKClicked • no UIButton tree dependency");
    return true;
}

bool ClickHealDialogChoice(cleanroute::HealDialogChoice choice, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;

    if (choice == cleanroute::HealDialogChoice::Treatment ||
        choice == cleanroute::HealDialogChoice::Know) {
        return SendCurrentLuaDialogSelectionV119(choice, detail, cap);
    }

    if (choice == cleanroute::HealDialogChoice::Confirm) {
        bool messageBoxPresent = false;
        wchar_t probe[192]{};
        if (!LuaUiPresentV119(c, L"MessageBox", messageBoxPresent, probe, _countof(probe))) {
            SetText(detail, cap, probe);
            return false;
        }
        if (messageBoxPresent) return ExecuteMessageBoxConfirmV119(c, detail, cap);
        return SendCurrentLuaDialogSelectionV119(choice, detail, cap);
    }

    SetText(detail, cap, L"ACTION_V119: invalid HealDialogChoice");
    return false;
}

} // namespace

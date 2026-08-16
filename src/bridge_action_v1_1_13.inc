namespace {

std::int32_t SelectionForChoiceV123(cleanroute::HealDialogChoice choice, const LuaDialogIdsV123& ids) {
    if (choice == cleanroute::HealDialogChoice::Treatment) return ids.treatment;
    if (choice == cleanroute::HealDialogChoice::Confirm) return ids.confirm;
    if (choice == cleanroute::HealDialogChoice::Know) return ids.know;
    return 0;
}

const wchar_t* LabelForChoiceV123(cleanroute::HealDialogChoice choice) {
    if (choice == cleanroute::HealDialogChoice::Treatment) return L"Trị liệu";
    if (choice == cleanroute::HealDialogChoice::Confirm) return L"Xác nhận";
    if (choice == cleanroute::HealDialogChoice::Know) return L"Ta biết rồi";
    return L"?";
}

bool RequireValidatedGameThreadV123(wchar_t* detail, std::size_t cap) {
    wchar_t probe[256]{};
    if (!RequireValidatedGameThreadV120(probe, _countof(probe))) {
        SetText(detail, cap, L"ACTION_V123 prerequisite fail • ");
        Append(detail, cap, probe);
        return false;
    }
    return true;
}

bool SendCurrentLuaDialogSelectionV123(cleanroute::HealDialogChoice choice,
                                       wchar_t* detail, std::size_t cap) {
    if (!RequireValidatedGameThreadV123(detail, cap)) return false;

    LuaDialogIdsV123 ids{};
    wchar_t probe[640]{};
    if (!ReadLuaDialogIdsV123(ids, probe, _countof(probe))) {
        SetText(detail, cap, L"ACTION_V123 FAIL-CLOSED: cannot re-read live GameDialogData • ");
        Append(detail, cap, probe);
        return false;
    }
    const std::int32_t selectionID = SelectionForChoiceV123(choice, ids);
    if (selectionID <= 0) {
        SetText(detail, cap, L"ACTION_V123 FAIL-CLOSED: semantic choice absent • label=");
        Append(detail, cap, LabelForChoiceV123(choice));
        Append(detail, cap, L" • live={"); Append(detail, cap, ids.raw); Append(detail, cap, L"}");
        return false;
    }

    wchar_t sendProbe[192]{};
    if (!SendGameDialogSelectionV119(selectionID, -1, sendProbe, _countof(sendProbe))) {
        SetText(detail, cap, L"ACTION_V123 send fail • ");
        Append(detail, cap, sendProbe);
        return false;
    }
    SetText(detail, cap, L"ACTION_V123 SENT semantic CMD_SHOW_GAMEDIALOG • label=");
    Append(detail, cap, LabelForChoiceV123(choice));
    Append(detail, cap, L" • selectionID="); AppendInt(detail, cap, selectionID);
    Append(detail, cap, L" • payload="); AppendInt(detail, cap, selectionID); Append(detail, cap, L":-1");
    Append(detail, cap, L" • source=current metatable-aware GameDialogData.Selections");
    return true;
}

bool ExecuteMessageBoxConfirmV123(const Classes& c, wchar_t* detail, std::size_t cap) {
    if (!RequireValidatedGameThreadV123(detail, cap)) return false;
    Il2CppObject* messageBox = nullptr;
    wchar_t probe[192]{};
    if (!FindLuaUiObjectV113(c, L"MessageBox", messageBox, probe, _countof(probe))) {
        SetText(detail, cap, probe);
        return false;
    }
    if (!messageBox) { SetText(detail, cap, L"ACTION_V123: MessageBox absent"); return false; }
    wchar_t callDetail[192]{};
    if (!InvokeLuaUiActionV113(c, L"MessageBox", L"ButtonOKClicked", nullptr, false,
                               callDetail, _countof(callDetail))) {
        SetText(detail, cap, L"ACTION_V123 MessageBox confirm fail • ");
        Append(detail, cap, callDetail);
        return false;
    }
    SetText(detail, cap, L"ACTION_V123 CALLED MessageBox.ButtonOKClicked • semantic callback");
    return true;
}

bool ClickHealDialogChoice(cleanroute::HealDialogChoice choice, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;

    if (choice == cleanroute::HealDialogChoice::Treatment ||
        choice == cleanroute::HealDialogChoice::Know) {
        return SendCurrentLuaDialogSelectionV123(choice, detail, cap);
    }

    if (choice == cleanroute::HealDialogChoice::Confirm) {
        bool messageBoxPresent = false;
        wchar_t probe[192]{};
        if (!LuaUiPresentV119(c, L"MessageBox", messageBoxPresent, probe, _countof(probe))) {
            SetText(detail, cap, probe);
            return false;
        }
        if (messageBoxPresent) return ExecuteMessageBoxConfirmV123(c, detail, cap);
        return SendCurrentLuaDialogSelectionV123(choice, detail, cap);
    }

    SetText(detail, cap, L"ACTION_V123: invalid HealDialogChoice");
    return false;
}

} // namespace

namespace {

bool QueueButtonObjectV118(Il2CppObject* button, const wchar_t* uiName, const wchar_t* label,
                           wchar_t* detail, std::size_t cap) {
    if (!button) { SetText(detail, cap, L"ACTION_V118: button=null"); return false; }
    if (!PollMtProofV116(detail, cap)) return false;
    if (!LoadMtExtraApiV116(detail, cap)) return false;

    Il2CppClass* bc = g_api.object_get_class(button);
    const MethodInfo* click = bc ? FindMethod(bc, "HandleClickEvent", 0) : nullptr;
    if (!click || StaticMethod(click) || !ReturnVoidV116(click)) {
        SetText(detail, cap, L"ACTION_V118: HandleClickEvent() signature mismatch");
        return false;
    }

    Il2CppClass* actionClass = ActionClassV116(detail, cap);
    ActionCtorFnV116 actionCtor = ActionCtorV116(detail, cap);
    if (!actionClass || !actionCtor) return false;
    Il2CppObject* action = g_mtApiV116.objectNew(actionClass);
    if (!action) { SetText(detail, cap, L"ACTION_V118: Action alloc fail"); return false; }
    actionCtor(action, button, click, nullptr);
    const std::uint32_t actionHandle = g_mtApiV116.gcNew(action, false);
    if (!actionHandle) { SetText(detail, cap, L"ACTION_V118: Action GC handle fail"); return false; }

    Il2CppObject* mt = nullptr;
    const MethodInfo* execute = nullptr;
    if (!MainThreadTargetV116(mt, execute, detail, cap)) {
        g_mtApiV116.gcFree(actionHandle);
        return false;
    }
    void* args[] = { action };
    if (!InvokeVoid(execute, mt, args, detail, cap)) {
        g_mtApiV116.gcFree(actionHandle);
        return false;
    }
    g_mtApiV116.gcFree(actionHandle);
    SetText(detail, cap, L"ACTION_V118 ENQUEUED live UIButton.HandleClickEvent • UI=");
    Append(detail, cap, uiName);
    Append(detail, cap, L" • label=");
    Append(detail, cap, label);
    return true;
}

bool QueueVerifiedGameDialogChoiceV118(const Classes& c, const wchar_t* label,
                                       wchar_t* detail, std::size_t cap) {
    // Prove the dispatcher first; then resolve the current dialog/button so we do not
    // hold a UI object across a proof frame or a server-driven GameDialog rebuild.
    if (!PollMtProofV116(detail, cap)) return false;
    if (!LoadMtExtraApiV116(detail, cap)) return false;

    Il2CppObject* button = nullptr;
    wchar_t findDetail[384]{};
    if (!FindButtonInUi(c, L"GameDialog", label, button, findDetail, _countof(findDetail)) || !button) {
        SetText(detail, cap, L"ACTION_V118 FAIL: live GameDialog choice not resolved • ");
        Append(detail, cap, findDetail);
        return false;
    }

    std::int32_t selectionID = 0;
    wchar_t idDetail[160]{};
    if (!ReadButtonSelectionIdV118(button, selectionID, idDetail, _countof(idDetail)) || selectionID <= 0) {
        SetText(detail, cap, L"ACTION_V118 FAIL-CLOSED: matched choice but live selectionID unavailable • ");
        Append(detail, cap, idDetail);
        return false;
    }

    wchar_t queueDetail[256]{};
    if (!QueueButtonObjectV118(button, L"GameDialog", label, queueDetail, _countof(queueDetail))) {
        SetText(detail, cap, queueDetail);
        return false;
    }
    SetText(detail, cap, L"ACTION_V118 ENQUEUED verified GameDialog choice • label=");
    Append(detail, cap, label);
    Append(detail, cap, L" • selectionID=");
    AppendInt(detail, cap, selectionID);
    Append(detail, cap, L" • boundary=MainThread.Execute(Action)");
    return true;
}

bool QueueMessageBoxConfirmV118(const Classes& c, wchar_t* detail, std::size_t cap) {
    if (!PollMtProofV116(detail, cap)) return false;
    if (!LoadMtExtraApiV116(detail, cap)) return false;
    Il2CppObject* button = nullptr;
    wchar_t probe[256]{};
    if (!FindButtonInUi(c, L"MessageBox", L"Xác nhận", button, probe, _countof(probe)) || !button) {
        SetText(detail, cap, L"ACTION_V118: MessageBox confirm absent");
        return false;
    }
    if (!QueueButtonObjectV118(button, L"MessageBox", L"Xác nhận", detail, cap)) return false;
    SetText(detail, cap, L"ACTION_V118 ENQUEUED MessageBox confirm • boundary=MainThread.Execute(Action)");
    return true;
}

bool ClickHealDialogChoice(cleanroute::HealDialogChoice choice, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;

    if (choice == cleanroute::HealDialogChoice::Treatment)
        return QueueVerifiedGameDialogChoiceV118(c, L"Trị liệu", detail, cap);
    if (choice == cleanroute::HealDialogChoice::Know)
        return QueueVerifiedGameDialogChoiceV118(c, L"Ta biết rồi", detail, cap);
    if (choice == cleanroute::HealDialogChoice::Confirm) {
        Il2CppObject* probe = nullptr;
        wchar_t ignored[192]{};
        if (FindButtonInUi(c, L"MessageBox", L"Xác nhận", probe, ignored, _countof(ignored)) && probe)
            return QueueMessageBoxConfirmV118(c, detail, cap);
        return QueueVerifiedGameDialogChoiceV118(c, L"Xác nhận", detail, cap);
    }

    SetText(detail, cap, L"ACTION_V118: invalid HealDialogChoice");
    return false;
}

} // namespace

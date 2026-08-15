                        HealFail(L"ClickNPC(339) thất bại 3 lần; dừng fail-closed");
                    return;
                }
                healPhase_ = Phase::WaitTreatment;
                healPhaseTick_ = now;
                healNpcRetries_ = 0;
                return;
            }
            if (a == Action::Wait) {
                if (s.autoPathing) SetText(status_, L"AUTO TRỊ LIỆU: ĐANG AUTOPATH TỚI NPC");
                return;
            }
            Spot npcTarget = healTarget_;
            SendDecision(a, npcTarget);
            return;
        }

        TickHealDialog();
    }

    void StopRoute() {
        running_ = false;
        healRunning_ = false;
        healPhase_ = Phase::Idle;
        if (bridge_.Attached()) {
            Response r{}; std::wstring ignored;
            (void)bridge_.Call(Command::StopPath, 0, 0, 0, r, ignored, 1200);
        }
        SetText(status_, L"TRẠNG THÁI: ĐÃ DỪNG");
        Log(L"Đã dừng mọi chế độ test/AutoPath. Không tự thay đổi trạng thái ngựa khi bấm Dừng.");
    }

    bool CooldownReady(Action a, DWORD now) {
        DWORD delay = 1500;
        if (a == Action::Mount || a == Action::Dismount) delay = 4000;
        if (a == Action::StartPath) delay = 5000;
        if (a != lastAction_) { lastAction_ = a; lastActionTick_ = 0; }
        return lastActionTick_ == 0 || now - lastActionTick_ >= delay;
    }

    void SendDecision(Action a, const Spot& target) {
        DWORD now = GetTickCount();
        if (!CooldownReady(a, now)) return;
        Response r{}; std::wstring error; bool ok = false;
        switch (a) {
            case Action::Mount:
                ok = bridge_.Call(Command::ToggleRide, 1, 0, 0, r, error, 1800);
                SetText(status_, L"TRẠNG THÁI: ĐANG LÊN NGỰA"); break;
            case Action::Dismount:
                ok = bridge_.Call(Command::ToggleRide, 0, 0, 0, r, error, 1800);
                SetText(status_, L"TRẠNG THÁI: ĐÃ TỚI BÃI • ĐANG XUỐNG NGỰA"); break;
            case Action::StartPath:
                ok = bridge_.Call(Command::StartPath, target.mapID, target.x, target.y, r, error, 2500);
                SetText(status_, L"TRẠNG THÁI: ĐANG TỰ RA BÃI"); break;
            case Action::StopPath:
                ok = bridge_.Call(Command::StopPath, 0, 0, 0, r, error, 1500);
                SetText(status_, L"TRẠNG THÁI: ĐÃ TỚI TỌA ĐỘ • DỪNG AUTOPATH"); break;
            default: return;
        }
        lastActionTick_ = now;
        if (ok) Log(r.detail[0] ? r.detail : L"Action PASS");
        else Log(L"Action fail-closed: " + error);
    }

    void Tick() {
        if (!bridge_.Attached()) {
            if (!running_ && !healRunning_) return;
            std::wstring e; if (!EnsureAttach(e)) { SetText(status_, L"TRẠNG THÁI: MẤT CLIENT"); return; }
        }
        Snapshot s{}; std::wstring error;
        if (!ReadSnapshot(s, error)) {
            if (running_ || healRunning_) SetText(status_, L"TRẠNG THÁI: CHỜ STATE ỔN ĐỊNH");
            return;
        }
        UpdateLive(s);
        if (healRunning_) { TickAutoHeal(s); return; }
        if (!running_) return;

        Spot spot{}; if (!SelectedTarget(spot)) { running_ = false; SetText(status_, L"TRẠNG THÁI: CHƯA CHỌN BÃI"); return; }
        if (!s.mapReady || s.waitingChangeMap) {
            candidateCount_ = 0; qualifiedMap_ = 0;
            SetText(status_, L"TRẠNG THÁI: ĐANG CHUYỂN MAP • KHÔNG GỬI ACTION");
            return;
        }
        const std::uint32_t need = ValidMap | ValidPosition | ValidRiding | ValidAutoPath;
        if ((s.validMask & need) != need) { SetText(status_, L"TRẠNG THÁI: STATE CHƯA ĐỦ"); return; }

        if (qualifiedMap_ != s.mapID) {
            if (candidateMap_ == s.mapID) ++candidateCount_;
            else { candidateMap_ = s.mapID; candidateCount_ = 1; }
            if (candidateCount_ < 2) {
                SetText(status_, L"TRẠNG THÁI: ỔN ĐỊNH MAP 1/2"); return;
            }
            qualifiedMap_ = s.mapID; candidateCount_ = 0;
        }

        State logic{};
        logic.valid = true; logic.mapReady = true; logic.waitingMap = false;
        logic.mapID = s.mapID; logic.x = s.x; logic.y = s.y;
        logic.riding = s.riding != 0; logic.autoPathing = s.autoPathing != 0;
        Target target{spot.mapID, spot.x, spot.y, Tolerance()};
        Action a = Decide(logic, target);
        if (a == Action::Hold) {
            lastAction_ = Action::Hold;
            SetText(status_, L"TRẠNG THÁI: ĐÚNG BÃI • ĐANG GIÁM SÁT TỌA ĐỘ");
            return;
        }
        if (a == Action::Wait) {
            if (s.autoPathing) SetText(status_, L"TRẠNG THÁI: ĐANG AUTOPATH TỚI BÃI");
            return;
        }
        SendDecision(a, spot);
    }

    LRESULT Handle(UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
            case WM_CREATE: BuildUi(); return 0;
            case WM_COMMAND:
                switch (LOWORD(wp)) {
                    case IDC_SCAN: ScanClients(); break;
                    case IDC_SAVE_SPOT: SaveCurrentSpot(); break;
                    case IDC_CAPTURE_HEAL: CaptureHealTarget(); break;
                    case IDC_DELETE_SPOT: DeleteSpot(); break;
                    case IDC_START: StartRoute(); break;
                    case IDC_AUTO_HEAL: StartAutoHeal(); break;
                    case IDC_STOP: StopRoute(); break;
                }
                return 0;
            case WM_TIMER: if (wp == kTimer) Tick(); return 0;
            case WM_DESTROY: bridge_.Close(); PostQuitMessage(0); return 0;
        }
        return DefWindowProcW(hwnd_, msg, wp, lp);
    }

    HWND hwnd_ = nullptr;
    HWND clientCombo_ = nullptr;
    HWND live_ = nullptr;
    HWND spotName_ = nullptr;
    HWND targetCombo_ = nullptr;
    HWND tolerance_ = nullptr;
    HWND status_ = nullptr;
    HWND log_ = nullptr;
    std::vector<GameClient> clients_;
    std::vector<Spot> spots_;
    BridgeClient bridge_;
    bool running_ = false;
    bool healRunning_ = false;
    Spot healTarget_{};
    bool healTargetReady_ = false;
    Phase healPhase_ = Phase::Idle;
    DWORD healPhaseTick_ = 0;
    int healNpcRetries_ = 0;
    int healOpenNpcAttempts_ = 0;
    DWORD healOpenNpcAttemptTick_ = 0;
    int qualifiedMap_ = 0, candidateMap_ = 0, candidateCount_ = 0;
    DWORD lastActionTick_ = 0;
    Action lastAction_ = Action::Wait;
};

} // namespace

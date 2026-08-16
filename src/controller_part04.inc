        healRunning_ = false; healPhase_ = Phase::Idle;
        running_ = true; lastActionTick_ = 0; lastAction_ = Action::Wait;
        qualifiedMap_ = 0; candidateMap_ = 0; candidateCount_ = 0;
        SetText(status_, L"TRẠNG THÁI: ĐANG GIÁM SÁT TỌA ĐỘ");
        Log(L"BẮT ĐẦU • mục tiêu " + target.name + L" • Map " + std::to_wstring(target.mapID) + L" • " + std::to_wstring(target.x) + L"," + std::to_wstring(target.y));
    }

    int HealNpcIdForCapturedMap() const {
        if (healTarget_.mapID == 3) return 463;
        if (healTarget_.mapID == 5) return 339;
        return 0;
    }

    const wchar_t* HealNpcNameForCapturedMap() const {
        if (healTarget_.mapID == 3) return L"Long Phá Thiên";
        if (healTarget_.mapID == 5) return L"Đỗ Thanh Đằng";
        return L"NPC không hỗ trợ";
    }

    void StartAutoHeal() {
        if (!healTargetReady_) {
            Log(L"AUTO TRỊ LIỆU CHƯA CHẠY: chưa có tọa độ NPC. Đứng tại NPC cần test rồi bấm TỰ LẤY TỌA ĐỘ NPC trước.");
            SetText(status_, L"AUTO TRỊ LIỆU: CHƯA CÓ TỌA ĐỘ NPC");
            return;
        }
        const int npcID = HealNpcIdForCapturedMap();
        if (npcID == 0) {
            Log(L"AUTO TRỊ LIỆU CHƯA CHẠY: target đã tự lấy không thuộc Map 3 Lạc Dương hoặc Map 5 Lâu Lan; không đoán NPC.");
            SetText(status_, L"AUTO TRỊ LIỆU: MAP NPC KHÔNG HỖ TRỢ");
            return;
        }
        std::wstring error;
        if (!EnsureAttach(error)) { Log(L"LỖI: " + error); return; }
        running_ = false;
        healRunning_ = true;
        healPhase_ = Phase::Travel;
        healPhaseTick_ = GetTickCount();
        healNpcRetries_ = 0;
        healOpenNpcAttempts_ = 0;
        healOpenNpcAttemptTick_ = 0;
        lastActionTick_ = 0;
        lastAction_ = Action::Wait;
        qualifiedMap_ = 0; candidateMap_ = 0; candidateCount_ = 0;
        SetText(status_, std::wstring(L"AUTO TRỊ LIỆU: ĐANG TỚI ") + HealNpcNameForCapturedMap());
        Log(L"AUTO TRỊ LIỆU START • NPC " + std::to_wstring(npcID) + L" " + HealNpcNameForCapturedMap() +
            L" • tọa độ tự lấy Map " + std::to_wstring(healTarget_.mapID) + L" • " +
            std::to_wstring(healTarget_.x) + L"," + std::to_wstring(healTarget_.y));
    }

    void HealFail(const std::wstring& reason) {
        healRunning_ = false;
        healPhase_ = Phase::Failed;
        SetText(status_, L"AUTO TRỊ LIỆU: FAIL-CLOSED");
        Log(L"AUTO TRỊ LIỆU FAIL: " + reason);
    }

    bool SendHealBridge(Command cmd, int a0, const wchar_t* statusText) {
        Response r{}; std::wstring error;
        bool ok = bridge_.Call(cmd, a0, 0, 0, r, error, 2200);
        if (ok) {
            if (statusText) SetText(status_, statusText);
            Log(r.detail[0] ? r.detail : L"Heal action PASS");
        } else {
            Log(L"Heal action fail-closed: " + error);
        }
        return ok;
    }

    void TickHealDialog() {
        Snapshot ui{}; std::wstring error;
        if (!InspectHealUi(ui, error)) {
            HealFail(L"Không inspect được UI sống: " + error);
            return;
        }
        DialogAction action = DecideDialogAction(healPhase_, ui.healDialogMask);
        if (action != DialogAction::None) {
            HealDialogChoice choice = HealDialogChoice::Treatment;
            const wchar_t* statusText = L"AUTO TRỊ LIỆU: ĐANG CHỌN TRỊ LIỆU";
            if (action == DialogAction::Confirm) {
                choice = HealDialogChoice::Confirm;
                statusText = L"AUTO TRỊ LIỆU: ĐANG XÁC NHẬN";
            } else if (action == DialogAction::Know) {
                choice = HealDialogChoice::Know;
                statusText = L"AUTO TRỊ LIỆU: ĐANG BẤM TA BIẾT RỒI";
            }
            if (!SendHealBridge(Command::ClickHealDialogChoice, static_cast<int>(choice), statusText)) return;
            healPhase_ = AfterDialogAction(action);
            healPhaseTick_ = GetTickCount();
            if (healPhase_ == Phase::Done) {
                healRunning_ = false;
                SetText(status_, L"AUTO TRỊ LIỆU: HOÀN TẤT");
                Log(L"AUTO TRỊ LIỆU DONE • đã đi NPC → Trị liệu → Xác nhận → Ta biết rồi");
            }
            return;
        }

        DWORD now = GetTickCount();
        const DWORD elapsed = now - healPhaseTick_;
        if (healPhase_ == Phase::WaitTreatment) {
            if ((ui.healDialogMask & HealDialogPresent) != 0) {
                SetText(status_, L"AUTO TRỊ LIỆU: DIALOG ĐÃ MỞ • CHỜ NHẬN DIỆN TRỊ LIỆU");
                if (ui.healDialogDetail[0]) Log(ui.healDialogDetail);
                if (elapsed >= 15000) {
                    HealFail(L"GameDialog đã mở nhưng 15 giây vẫn không resolve được lựa chọn Trị liệu; không mở lại NPC");
                }
                return;
            }
            SetText(status_, L"AUTO TRỊ LIỆU: CHỜ DIALOG TRỊ LIỆU");
            if (elapsed >= 4000 && healNpcRetries_ < 2) {
                ++healNpcRetries_;
                const int npcID = HealNpcIdForCapturedMap();
                if (npcID == 0) { HealFail(L"Mất mapping NPC theo map target"); return; }
                if (SendHealBridge(Command::ClickNpc, npcID, L"AUTO TRỊ LIỆU: THỬ MỞ LẠI NPC"))
                    healPhaseTick_ = now;
                return;
            }
            if (elapsed >= 15000) { HealFail(L"Quá hạn chờ GameDialog Trị liệu xuất hiện"); return; }
        } else if (healPhase_ == Phase::WaitConfirm) {
            SetText(status_, L"AUTO TRỊ LIỆU: CHỜ XÁC NHẬN");
            if (elapsed >= 15000) { HealFail(L"Quá hạn chờ Xác nhận"); return; }
        } else if (healPhase_ == Phase::WaitKnow) {
            SetText(status_, L"AUTO TRỊ LIỆU: CHỜ TA BIẾT RỒI");
            if (elapsed >= 15000) { HealFail(L"Quá hạn chờ Ta biết rồi"); return; }
        }
    }

    void TickAutoHeal(const Snapshot& s) {
        if (!s.mapReady || s.waitingChangeMap) {
            candidateCount_ = 0; qualifiedMap_ = 0;
            SetText(status_, L"AUTO TRỊ LIỆU: ĐANG CHUYỂN MAP • KHÔNG GỬI ACTION");
            return;
        }
        const std::uint32_t need = ValidMap | ValidPosition | ValidRiding | ValidAutoPath;
        if ((s.validMask & need) != need) {
            SetText(status_, L"AUTO TRỊ LIỆU: STATE CHƯA ĐỦ");
            return;
        }

        if (qualifiedMap_ != s.mapID) {
            if (candidateMap_ == s.mapID) ++candidateCount_;
            else { candidateMap_ = s.mapID; candidateCount_ = 1; }
            if (candidateCount_ < 2) {
                SetText(status_, L"AUTO TRỊ LIỆU: ỔN ĐỊNH MAP 1/2");
                return;
            }
            qualifiedMap_ = s.mapID; candidateCount_ = 0;
        }

        if (healPhase_ == Phase::Travel || healPhase_ == Phase::OpenNpc) {
            State logic{};
            logic.valid = true; logic.mapReady = true; logic.waitingMap = false;
            logic.mapID = s.mapID; logic.x = s.x; logic.y = s.y;
            logic.riding = s.riding != 0; logic.autoPathing = s.autoPathing != 0;
            Target target{healTarget_.mapID, healTarget_.x, healTarget_.y, kHealTolerance};
            Action a = Decide(logic, target);
            if (a == Action::Hold) {
                healPhase_ = Phase::OpenNpc;
                const DWORD now = GetTickCount();
                if (healOpenNpcAttemptTick_ != 0 && now - healOpenNpcAttemptTick_ < 2000) {
                    SetText(status_, L"AUTO TRỊ LIỆU: CHỜ NHỊP GỌI NPC");
                    return;
                }
                if (healOpenNpcAttempts_ >= 3) {
                    HealFail(L"Không mở được NPC sau 3 lần thử; không spam ClickNPC");
                    return;
                }
                ++healOpenNpcAttempts_;
                healOpenNpcAttemptTick_ = now;
                const int npcID = HealNpcIdForCapturedMap();
                if (npcID == 0) { HealFail(L"Mất mapping NPC theo map target"); return; }
                if (!SendHealBridge(Command::ClickNpc, npcID, L"AUTO TRỊ LIỆU: ĐÃ GỌI NPC • CHỜ TRỊ LIỆU")) {
                    if (healOpenNpcAttempts_ >= 3)
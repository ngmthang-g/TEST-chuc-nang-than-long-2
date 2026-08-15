        addFont(Make(L"STATIC", L"Sai số:", 0, 578, 228, 50, 20, 0));
        tolerance_ = Make(L"EDIT", L"120", WS_BORDER | ES_NUMBER | ES_CENTER, 625, 224, 40, 27, IDC_TOLERANCE); addFont(tolerance_);

        addFont(Make(L"BUTTON", L"BẮT ĐẦU GIỮ BÃI", BS_PUSHBUTTON, 18, 272, 200, 42, IDC_START));
        addFont(Make(L"BUTTON", L"AUTO TRỊ LIỆU", BS_DEFPUSHBUTTON, 228, 272, 215, 42, IDC_AUTO_HEAL));
        addFont(Make(L"BUTTON", L"DỪNG", BS_PUSHBUTTON, 453, 272, 212, 42, IDC_STOP));
        status_ = Make(L"STATIC", L"TRẠNG THÁI: ĐÃ DỪNG", SS_CENTER | SS_CENTERIMAGE | WS_BORDER,
                       18, 326, 647, 48, IDC_STATUS); addFont(status_);
        log_ = Make(L"EDIT", L"TEST v1.1.0: Route sạch + Auto trị liệu Đỗ Thanh Đằng (Lâu Lan).\r\n",
                    WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL,
                    18, 388, 647, 132, IDC_LOG); addFont(log_);
        SetTimer(hwnd_, kTimer, 500, nullptr);
        spots_ = LoadSpots(); RefreshSpotCombo(); ScanClients();
    }

    void Log(const std::wstring& line) {
        int n = GetWindowTextLengthW(log_);
        SendMessageW(log_, EM_SETSEL, n, n);
        std::wstring text = line + L"\r\n";
        SendMessageW(log_, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(text.c_str()));
        SendMessageW(log_, EM_SCROLLCARET, 0, 0);
    }

    void ScanClients() {
        clients_ = FindClients();
        SendMessageW(clientCombo_, CB_RESETCONTENT, 0, 0);
        for (const auto& g : clients_) {
            std::wstring label = L"PID " + std::to_wstring(g.pid) + L"  •  " + g.title;
            SendMessageW(clientCombo_, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(label.c_str()));
        }
        if (!clients_.empty()) SendMessageW(clientCombo_, CB_SETCURSEL, 0, 0);
        Log(L"Quét thấy " + std::to_wstring(clients_.size()) + L" client GameAssembly.dll");
    }

    bool EnsureAttach(std::wstring& error) {
        int index = static_cast<int>(SendMessageW(clientCombo_, CB_GETCURSEL, 0, 0));
        if (index < 0 || index >= static_cast<int>(clients_.size())) { error = L"Chưa chọn client"; return false; }
        const GameClient& g = clients_[static_cast<std::size_t>(index)];
        if (bridge_.AttachedTo(g.pid)) return true;
        if (!bridge_.Attach(g, error)) return false;
        qualifiedMap_ = 0; candidateMap_ = 0; candidateCount_ = 0;
        Log(L"Đã attach PID " + std::to_wstring(g.pid) + L" bằng CleanRoute bridge");
        return true;
    }

    bool ReadSnapshot(Snapshot& s, std::wstring& error) {
        Response r{};
        if (!bridge_.Call(Command::ReadState, 0, 0, 0, r, error, 1800)) return false;
        s = r.snapshot;
        return true;
    }

    bool InspectHealUi(Snapshot& s, std::wstring& error) {
        Response r{};
        if (!bridge_.Call(Command::InspectHealDialog, 0, 0, 0, r, error, 1800)) return false;
        s = r.snapshot;
        return (s.validMask & ValidHealDialog) != 0;
    }

    void UpdateLive(const Snapshot& s) {
        std::wstring name = s.characterName[0] ? s.characterName : L"?";
        std::wstring text = L"NV: " + name + L"   |   Map " + std::to_wstring(s.mapID) +
                            L"   |   X/Y " + std::to_wstring(s.x) + L" / " + std::to_wstring(s.y) +
                            L"   |   Ngựa " + (s.riding ? L"ON" : L"OFF") +
                            L"   |   AutoPath " + (s.autoPathing ? L"ON" : L"OFF");
        if (!s.mapReady || s.waitingChangeMap) text = L"ĐANG CHUYỂN MAP • không gửi action";
        SetText(live_, text);
    }

    void RefreshSpotCombo() {
        SendMessageW(targetCombo_, CB_RESETCONTENT, 0, 0);
        for (const auto& s : spots_) {
            std::wstring label = s.name + L"  •  M" + std::to_wstring(s.mapID) + L"  " +
                                 std::to_wstring(s.x) + L"," + std::to_wstring(s.y);
            SendMessageW(targetCombo_, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(label.c_str()));
        }
        if (!spots_.empty()) SendMessageW(targetCombo_, CB_SETCURSEL, 0, 0);
    }

    int Tolerance() const {
        int v = _wtoi(GetText(tolerance_).c_str());
        if (v < 20) v = 20; if (v > 2000) v = 2000;
        return v;
    }

    bool SelectedTarget(Spot& out) const {
        int i = static_cast<int>(SendMessageW(targetCombo_, CB_GETCURSEL, 0, 0));
        if (i < 0 || i >= static_cast<int>(spots_.size())) return false;
        out = spots_[static_cast<std::size_t>(i)]; return true;
    }

    void SaveCurrentSpot() {
        std::wstring error;
        if (!EnsureAttach(error)) { Log(L"LỖI: " + error); return; }
        Snapshot s{};
        if (!ReadSnapshot(s, error) || !s.mapReady || s.waitingChangeMap ||
            (s.validMask & (ValidMap | ValidPosition)) != (ValidMap | ValidPosition)) {
            Log(L"Chưa lưu được bãi: " + (error.empty() ? L"state chưa ổn định" : error)); return;
        }
        std::wstring name = SanitizeName(GetText(spotName_));
        if (name.empty()) name = L"Bãi M" + std::to_wstring(s.mapID) + L" " + std::to_wstring(s.x) + L"," + std::to_wstring(s.y);
        Spot value{name, s.mapID, s.x, s.y};
        bool replaced = false;
        for (auto& x : spots_) if (_wcsicmp(x.name.c_str(), name.c_str()) == 0) { x = value; replaced = true; break; }
        if (!replaced) spots_.push_back(value);
        if (!SaveSpots(spots_)) { Log(L"Không ghi được file bãi"); return; }
        RefreshSpotCombo();
        for (std::size_t i = 0; i < spots_.size(); ++i) if (_wcsicmp(spots_[i].name.c_str(), name.c_str()) == 0)
            SendMessageW(targetCombo_, CB_SETCURSEL, static_cast<WPARAM>(i), 0);
        Log(L"Đã lưu: " + name + L" • Map " + std::to_wstring(s.mapID) + L" • " + std::to_wstring(s.x) + L"," + std::to_wstring(s.y));
    }

    void DeleteSpot() {
        int i = static_cast<int>(SendMessageW(targetCombo_, CB_GETCURSEL, 0, 0));
        if (i < 0 || i >= static_cast<int>(spots_.size())) return;
        std::wstring name = spots_[static_cast<std::size_t>(i)].name;
        spots_.erase(spots_.begin() + i); SaveSpots(spots_); RefreshSpotCombo();
        Log(L"Đã xóa bãi: " + name);
    }

    void StartRoute() {
        Spot target{}; std::wstring error;
        if (!SelectedTarget(target)) { Log(L"Chưa chọn bãi cần giữ"); return; }
        if (!EnsureAttach(error)) { Log(L"LỖI: " + error); return; }

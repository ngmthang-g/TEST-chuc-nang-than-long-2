        const LONG next = shared_->requestSeq + 1;
        shared_->request = {};
        shared_->request.command = static_cast<std::uint32_t>(command);
        shared_->request.arg0 = a0; shared_->request.arg1 = a1; shared_->request.arg2 = a2;
        MemoryBarrier();
        InterlockedExchange(&shared_->requestSeq, next);
        if (!PostThreadMessageW(game_.threadId, kWakeMessage, 0, 0)) {
            error = L"Không đánh thức được game thread"; return false;
        }
        const DWORD begin = GetTickCount();
        while (GetTickCount() - begin < timeoutMs) {
            if (shared_->completedSeq == next) {
                MemoryBarrier(); out = shared_->response;
                if (!out.ok) { error = out.detail[0] ? out.detail : L"Bridge trả lỗi"; return false; }
                return true;
            }
            Sleep(5);
        }
        error = L"Bridge timeout; không retry trong cùng tick";
        return false;
    }

private:
    GameClient game_{};
    HANDLE mapping_ = nullptr;
    SharedBlock* shared_ = nullptr;
    HMODULE localDll_ = nullptr;
    HHOOK hook_ = nullptr;
    bool attached_ = false;
};

struct Spot {
    std::wstring name;
    int mapID = 0;
    int x = 0;
    int y = 0;
};

std::wstring SpotsPath() { return ExeDir() + L"\\ThanLongCleanRoute.spots.tsv"; }

std::wstring SanitizeName(std::wstring s) {
    for (wchar_t& c : s) if (c == L'\t' || c == L'\r' || c == L'\n') c = L' ';
    while (!s.empty() && s.front() == L' ') s.erase(s.begin());
    while (!s.empty() && s.back() == L' ') s.pop_back();
    return s;
}

std::vector<Spot> LoadSpots() {
    std::vector<Spot> out;
    HANDLE h = CreateFileW(SpotsPath().c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                           nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return out;
    LARGE_INTEGER size{};
    if (!GetFileSizeEx(h, &size) || size.QuadPart < 0 || size.QuadPart > 4 * 1024 * 1024) { CloseHandle(h); return out; }
    std::string bytes(static_cast<std::size_t>(size.QuadPart), '\0');
    DWORD got = 0;
    if (!bytes.empty() && !ReadFile(h, bytes.data(), static_cast<DWORD>(bytes.size()), &got, nullptr)) { CloseHandle(h); return out; }
    CloseHandle(h); bytes.resize(got);
    std::stringstream file(bytes); std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line); std::string n, m, x, y;
        if (!std::getline(ss, n, '\t') || !std::getline(ss, m, '\t') ||
            !std::getline(ss, x, '\t') || !std::getline(ss, y, '\t')) continue;
        Spot s{Wide(n), std::atoi(m.c_str()), std::atoi(x.c_str()), std::atoi(y.c_str())};
        if (!s.name.empty() && s.mapID > 0) out.push_back(s);
    }
    return out;
}

bool SaveSpots(const std::vector<Spot>& spots) {
    std::string body;
    for (auto s : spots) {
        s.name = SanitizeName(s.name);
        body += Utf8(s.name) + "\t" + std::to_string(s.mapID) + "\t" +
                std::to_string(s.x) + "\t" + std::to_string(s.y) + "\r\n";
    }
    HANDLE h = CreateFileW(SpotsPath().c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;
    DWORD done = 0;
    bool ok = body.empty() || (WriteFile(h, body.data(), static_cast<DWORD>(body.size()), &done, nullptr) && done == body.size());
    CloseHandle(h); return ok;
}

class App {
public:
    bool Create(HINSTANCE instance) {
        INITCOMMONCONTROLSEX ic{sizeof(ic), ICC_STANDARD_CLASSES}; InitCommonControlsEx(&ic);
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(wc);
        wc.lpfnWndProc = WndProc;
        wc.hInstance = instance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszClassName = L"ThanLongCleanRouteWindow";
        RegisterClassExW(&wc);
        hwnd_ = CreateWindowExW(0, wc.lpszClassName, kTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                CW_USEDEFAULT, CW_USEDEFAULT, 690, 570, nullptr, nullptr, instance, this);
        return hwnd_ != nullptr;
    }

    void Show(int cmd) { ShowWindow(hwnd_, cmd); UpdateWindow(hwnd_); }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        App* self = reinterpret_cast<App*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        if (msg == WM_NCCREATE) {
            auto* cs = reinterpret_cast<CREATESTRUCTW*>(lp);
            self = reinterpret_cast<App*>(cs->lpCreateParams);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
            self->hwnd_ = hwnd;
        }
        return self ? self->Handle(msg, wp, lp) : DefWindowProcW(hwnd, msg, wp, lp);
    }

    HWND Make(const wchar_t* cls, const wchar_t* text, DWORD style, int x, int y, int w, int h, int id) {
        return CreateWindowExW(0, cls, text, WS_CHILD | WS_VISIBLE | style, x, y, w, h,
                               hwnd_, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), nullptr, nullptr);
    }

    void BuildUi() {
        HFONT font = reinterpret_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
        auto addFont = [font](HWND h){ SendMessageW(h, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE); };
        addFont(Make(L"STATIC", L"CLIENT GAME", 0, 18, 16, 120, 22, 0));
        clientCombo_ = Make(WC_COMBOBOXW, L"", CBS_DROPDOWNLIST | WS_VSCROLL, 18, 40, 510, 260, IDC_CLIENT); addFont(clientCombo_);
        addFont(Make(L"BUTTON", L"QUÉT CLIENT", BS_PUSHBUTTON, 540, 39, 125, 29, IDC_SCAN));
        live_ = Make(L"STATIC", L"Chưa kết nối", SS_LEFT, 18, 78, 647, 45, IDC_LIVE); addFont(live_);

        addFont(Make(L"STATIC", L"LƯU BÃI HIỆN TẠI", 0, 18, 132, 180, 22, 0));
        spotName_ = Make(L"EDIT", L"", WS_BORDER | ES_AUTOHSCROLL, 18, 156, 330, 27, IDC_SPOT_NAME); addFont(spotName_);
        addFont(Make(L"BUTTON", L"LƯU MAP + X/Y HIỆN TẠI", BS_PUSHBUTTON, 358, 155, 210, 29, IDC_SAVE_SPOT));

        addFont(Make(L"STATIC", L"BÃI CẦN GIỮ", 0, 18, 200, 120, 22, 0));
        targetCombo_ = Make(WC_COMBOBOXW, L"", CBS_DROPDOWNLIST | WS_VSCROLL, 18, 224, 430, 240, IDC_TARGET); addFont(targetCombo_);
        addFont(Make(L"BUTTON", L"XÓA BÃI", BS_PUSHBUTTON, 458, 223, 110, 29, IDC_DELETE_SPOT));

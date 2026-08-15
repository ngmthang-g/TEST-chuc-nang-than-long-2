// v1.1.2 runtime fix: the visible NPC GameDialog can exist in the normal Lua UI registry
// while MainFindUI returns null. The old resolver stopped as soon as MainFindUI existed and
// therefore never tried FindUI. Keep the original implementation compiled under a renamed symbol
// for audit, and make all later heal code use this corrected resolver.

bool TryFindUiRootPublicMethod(const Classes& c, const char* methodName, const wchar_t* uiName,
                               Il2CppObject*& root, wchar_t* detail, std::size_t cap) {
    root = nullptr;
    if (!c.guiApi || !methodName || !uiName) return true;
    const MethodInfo* find = ExactMethod(c.guiApi, methodName, 1, true, "System.String");
    if (!find) return true;

    Il2CppString* name = NewString(uiName);
    if (!name) {
        SetText(detail, cap, L"Không tạo được tên Lua UI");
        return false;
    }
    void* args[] = { name };
    Il2CppObject* candidate = nullptr;
    if (!InvokeObjectArgs(find, nullptr, args, candidate, detail, cap)) return false;
    if (!candidate) return true;
    return NormalizeUiRoot(candidate, root, detail, cap);
}

bool FindUiRootViaPublicApi(const Classes& c, const wchar_t* uiName, Il2CppObject*& root,
                            wchar_t* detail, std::size_t cap) {
    root = nullptr;
    wchar_t probe[160]{};

    // Runtime evidence from this client: GameDialog is visible after ClickNPC, but the previous
    // MainFindUI-first resolver returned null. Historical working code also uses FindUI first.
    if (!TryFindUiRootPublicMethod(c, "FindUI", uiName, root, probe, _countof(probe))) {
        SetText(detail, cap, probe);
        return false;
    }
    if (root) {
        SetText(detail, cap, L"UI root qua FindUI: ");
        Append(detail, cap, uiName);
        return true;
    }

    probe[0] = 0;
    if (!TryFindUiRootPublicMethod(c, "MainFindUI", uiName, root, probe, _countof(probe))) {
        SetText(detail, cap, probe);
        return false;
    }
    if (root) {
        SetText(detail, cap, L"UI root qua MainFindUI fallback: ");
        Append(detail, cap, uiName);
    }
    return true;
}

bool FindUiRoot(const Classes& c, const wchar_t* uiName, Il2CppObject*& root,
                wchar_t* detail, std::size_t cap) {
    root = nullptr;
    wchar_t publicDetail[192]{};
    if (!FindUiRootViaPublicApi(c, uiName, root, publicDetail, _countof(publicDetail))) {
        SetText(detail, cap, publicDetail);
        return false;
    }
    if (root) {
        SetText(detail, cap, publicDetail);
        return true;
    }

    wchar_t executorDetail[192]{};
    if (!FindUiRootViaExecutor(c, uiName, root, executorDetail, _countof(executorDetail))) {
        SetText(detail, cap, executorDetail);
        return false;
    }
    if (root) {
        SetText(detail, cap, L"UI root qua executor fallback: ");
        Append(detail, cap, uiName);
        return true;
    }

    SetText(detail, cap, L"Chưa thấy UI qua FindUI -> MainFindUI -> executor: ");
    Append(detail, cap, uiName);
    return true;
}

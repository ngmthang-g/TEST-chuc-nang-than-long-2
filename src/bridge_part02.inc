bool InvokeObject(const MethodInfo* method, void* instance, Il2CppObject*& out, wchar_t* detail, std::size_t cap) {
    out = nullptr;
    if (!method) { SetText(detail, cap, L"Method object chưa resolve"); return false; }
    void* exc = nullptr;
    out = g_api.runtime_invoke(method, instance, nullptr, &exc);
    if (exc) { SetText(detail, cap, L"Managed exception ở object getter"); return false; }
    return true;
}

bool InvokeObjectArgs(const MethodInfo* method, void* instance, void** args, Il2CppObject*& out,
                      wchar_t* detail, std::size_t cap) {
    out = nullptr;
    if (!method) { SetText(detail, cap, L"Method object chưa resolve"); return false; }
    void* exc = nullptr;
    out = g_api.runtime_invoke(method, instance, args, &exc);
    if (exc) { SetText(detail, cap, L"Managed exception ở object call"); return false; }
    return true;
}

Il2CppString* NewString(const wchar_t* text) {
    if (!text || !g_api.string_new_utf16) return nullptr;
    std::int32_t n = 0; while (text[n]) ++n;
    return g_api.string_new_utf16(text, n);
}

bool TextEq(const wchar_t* a, const wchar_t* b) {
    if (!a || !b) return false;
    while (*a == L' ' || *a == L'\t' || *a == L'\r' || *a == L'\n') ++a;
    while (*b == L' ' || *b == L'\t' || *b == L'\r' || *b == L'\n') ++b;
    while (*a && *b) {
        wchar_t ca = *a++, cb = *b++;
        if (ca >= L'A' && ca <= L'Z') ca = static_cast<wchar_t>(ca - L'A' + L'a');
        if (cb >= L'A' && cb <= L'Z') cb = static_cast<wchar_t>(cb - L'A' + L'a');
        if (ca != cb) return false;
    }
    while (*a == L' ' || *a == L'\t' || *a == L'\r' || *a == L'\n') ++a;
    while (*b == L' ' || *b == L'\t' || *b == L'\r' || *b == L'\n') ++b;
    return *a == 0 && *b == 0;
}

bool InvokeScalar(const MethodInfo* method, void* instance, std::int64_t& out, wchar_t* detail, std::size_t cap) {
    out = 0;
    if (!method) { SetText(detail, cap, L"Scalar method chưa resolve"); return false; }
    const Il2CppType* rt = g_api.method_get_return_type(method);
    char* tn = rt ? g_api.type_get_name(rt) : nullptr;
    if (!tn) { SetText(detail, cap, L"Không đọc được return type"); return false; }
    void* exc = nullptr;
    Il2CppObject* boxed = g_api.runtime_invoke(method, instance, nullptr, &exc);
    if (exc || !boxed) { g_api.free_fn(tn); SetText(detail, cap, L"Scalar getter lỗi/null"); return false; }
    void* raw = g_api.object_unbox(boxed);
    if (!raw) { g_api.free_fn(tn); SetText(detail, cap, L"Không unbox scalar"); return false; }
    bool ok = true;
    if (Eq(tn, "System.Boolean")) out = *reinterpret_cast<const std::uint8_t*>(raw) ? 1 : 0;
    else if (Eq(tn, "System.Int32")) out = *reinterpret_cast<const std::int32_t*>(raw);
    else if (Eq(tn, "System.UInt32")) out = *reinterpret_cast<const std::uint32_t*>(raw);
    else if (Eq(tn, "System.Int64")) out = *reinterpret_cast<const std::int64_t*>(raw);
    else ok = false;
    g_api.free_fn(tn);
    if (!ok) SetText(detail, cap, L"Return type scalar chưa hỗ trợ");
    return ok;
}

bool ScalarGetter(Il2CppClass* klass, const char* name, void* instance, std::int32_t& out,
                  wchar_t* detail, std::size_t cap) {
    std::int64_t value = 0;
    if (!InvokeScalar(FindMethod(klass, name, 0), instance, value, detail, cap)) return false;
    if (value < INT32_MIN || value > INT32_MAX) { SetText(detail, cap, L"Scalar vượt Int32"); return false; }
    out = static_cast<std::int32_t>(value);
    return true;
}

bool StaticScalar(Il2CppClass* klass, const char* name, std::int32_t& out,
                  wchar_t* detail, std::size_t cap) {
    const MethodInfo* m = FindMethod(klass, name, 0);
    if (!m || !StaticMethod(m)) { SetText(detail, cap, L"Static getter chưa resolve"); return false; }
    return ScalarGetter(klass, name, nullptr, out, detail, cap);
}

bool InvokeVoid(const MethodInfo* method, void* instance, void** args,
                wchar_t* detail, std::size_t cap) {
    if (!method) { SetText(detail, cap, L"Action method chưa resolve"); return false; }
    void* exc = nullptr;
    (void)g_api.runtime_invoke(method, instance, args, &exc);
    if (exc) { SetText(detail, cap, L"Action ném managed exception"); return false; }
    return true;
}

bool CopyString(Il2CppString* value, wchar_t* out, std::size_t cap) {
    if (!value || !out || cap == 0) return false;
    const int len = g_api.string_length(value);
    const wchar_t* chars = g_api.string_chars(value);
    if (len < 0 || len > 4096 || !chars) return false;
    std::size_t n = static_cast<std::size_t>(len);
    if (n + 1 > cap) n = cap - 1;
    for (std::size_t i = 0; i < n; ++i) out[i] = chars[i];
    out[n] = 0;
    return true;
}

struct Classes {
    Il2CppClass* gameApi = nullptr;
    Il2CppClass* session = nullptr;
    Il2CppClass* shared = nullptr;
    Il2CppClass* autoPath = nullptr;
    Il2CppClass* guiApi = nullptr;
    Il2CppClass* executor = nullptr;
};

bool ResolveClasses(Classes& c, wchar_t* detail, std::size_t cap) {
    if (!g_api.Load(detail, cap)) return false;
    const Il2CppImage* image = Image();
    if (!image) { SetText(detail, cap, L"Không mở được Assembly-CSharp"); return false; }
    c.gameApi = g_api.class_from_name(image, "FGStudio.LuaSystem.API", "LuaSystemAPI_Game");
    c.session = g_api.class_from_name(image, "FGStudio.Game.Logic", "SessionData");
    c.shared = g_api.class_from_name(image, "FGStudio.LuaSystem", "LuaSystemSharedData");
    c.autoPath = g_api.class_from_name(image, "FGStudio.Engine.Logic", "AutoPathManager");
    c.guiApi = g_api.class_from_name(image, "FGStudio.LuaSystem.API", "LuaSystemAPI_GUI");
    c.executor = g_api.class_from_name(image, "FGStudio.LuaSystem.Logic", "MonoBehaviourExecutor");
    if (!c.gameApi || !c.session || !c.shared || !c.autoPath) {
        SetText(detail, cap, L"Thiếu class route bắt buộc trên client này");
        return false;
    }
    return true;
}

bool SafeForAction(const Classes& c, wchar_t* detail, std::size_t cap);

bool Transition(const Classes& c, int& mapReady, int& waiting, wchar_t* detail, std::size_t cap) {
    if (!StaticScalar(c.gameApi, "IsMapReady", mapReady, detail, cap)) return false;
    if (!StaticScalar(c.session, "get_WaitingChangeMap", waiting, detail, cap)) return false;
    return true;
}

bool GetLeader(const Classes& c, Il2CppObject*& leader, Il2CppClass*& leaderClass,
               wchar_t* detail, std::size_t cap) {

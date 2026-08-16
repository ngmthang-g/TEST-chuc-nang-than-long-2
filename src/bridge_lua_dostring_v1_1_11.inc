namespace {

enum class LuaChunkKindV121 : int {
    None = 0,
    String = 1,
    ByteArray = 2,
};

struct LuaDoStringMethodV121 {
    const MethodInfo* method = nullptr;
    int argc = 0;
    LuaChunkKindV121 chunkKind = LuaChunkKindV121::None;
    wchar_t signature[192]{};
};

bool TypeNameV121(const Il2CppType* type, wchar_t* out, std::size_t cap) {
    if (!out || cap == 0) return false;
    out[0] = 0;
    char* name = type ? g_api.type_get_name(type) : nullptr;
    if (!name) return false;
    AppendAsciiV120(out, cap, name);
    g_api.free_fn(name);
    return out[0] != 0;
}

bool ParamTypeNameV121(const MethodInfo* method, std::uint32_t index, wchar_t* out, std::size_t cap) {
    if (!method || index >= g_api.method_get_param_count(method)) return false;
    return TypeNameV121(g_api.method_get_param(method, index), out, cap);
}

bool WideEqV121(const wchar_t* a, const wchar_t* b) {
    if (!a || !b) return false;
    while (*a && *b) {
        if (*a++ != *b++) return false;
    }
    return *a == 0 && *b == 0;
}

void DescribeClassV121(Il2CppClass* klass, wchar_t* out, std::size_t cap) {
    if (!out || cap == 0) return;
    out[0] = 0;
    if (!klass) { SetText(out, cap, L"<null>"); return; }
    (void)LoadLuaManagerReflectV120(nullptr, 0);
    const char* ns = g_luaManagerReflectV120.classGetNamespace
        ? g_luaManagerReflectV120.classGetNamespace(klass) : nullptr;
    const char* name = g_luaReflectV119.classGetName
        ? g_luaReflectV119.classGetName(klass) : nullptr;
    if (ns && *ns) { AppendAsciiV120(out, cap, ns); Append(out, cap, L"."); }
    if (name && *name) AppendAsciiV120(out, cap, name);
    if (!out[0]) SetText(out, cap, L"<?>");
}

void DescribeMethodV121(const MethodInfo* method, wchar_t* out, std::size_t cap) {
    if (!out || cap == 0) return;
    out[0] = 0;
    if (!method) { SetText(out, cap, L"<null>"); return; }
    const char* name = g_luaReflectV119.methodGetName ? g_luaReflectV119.methodGetName(method) : nullptr;
    AppendAsciiV120(out, cap, name ? name : "?");
    Append(out, cap, L"(");
    const std::uint32_t count = g_api.method_get_param_count(method);
    for (std::uint32_t i = 0; i < count; ++i) {
        if (i) Append(out, cap, L",");
        wchar_t typeName[96]{};
        if (ParamTypeNameV121(method, i, typeName, _countof(typeName))) Append(out, cap, typeName);
        else Append(out, cap, L"?");
    }
    Append(out, cap, L")");
    const Il2CppType* ret = g_api.method_get_return_type(method);
    wchar_t retName[96]{};
    if (TypeNameV121(ret, retName, _countof(retName))) {
        Append(out, cap, L"->"); Append(out, cap, retName);
    }
}

int ScoreDoStringV121(const MethodInfo* method, LuaChunkKindV121& kind) {
    kind = LuaChunkKindV121::None;
    if (!method || StaticMethod(method)) return -1;
    const std::uint32_t count = g_api.method_get_param_count(method);
    if (count < 1 || count > 3) return -1;

    wchar_t p0[96]{};
    if (!ParamTypeNameV121(method, 0, p0, _countof(p0))) return -1;
    int score = -1;
    if (WideEqV121(p0, L"System.String")) {
        kind = LuaChunkKindV121::String;
        score = 200;
    } else if (WideEqV121(p0, L"System.Byte[]")) {
        kind = LuaChunkKindV121::ByteArray;
        score = 150;
    } else {
        return -1;
    }

    if (count >= 2) {
        wchar_t p1[96]{};
        if (!ParamTypeNameV121(method, 1, p1, _countof(p1)) || !WideEqV121(p1, L"System.String")) return -1;
        score += 20;
    }
    if (count == 3) {
        const Il2CppType* p2 = g_api.method_get_param(method, 2);
        Il2CppClass* p2Class = p2 ? g_api.class_from_type(p2) : nullptr;
        if (!p2Class || g_api.class_is_valuetype(p2Class)) return -1;
        score += 10;
    }
    score += static_cast<int>(count);
    return score;
}

void RecordDoStringCandidateV121(const MethodInfo* method, wchar_t* trace, std::size_t cap) {
    if (!trace || cap == 0 || !method) return;
    if (trace[0]) Append(trace, cap, L" | ");
    wchar_t sig[192]{};
    DescribeMethodV121(method, sig, _countof(sig));
    Append(trace, cap, sig);
}

void RecordNearMethodV121(const MethodInfo* method, wchar_t* trace, std::size_t cap, int& count) {
    if (!trace || cap == 0 || !method || count >= 8) return;
    const char* name = g_luaReflectV119.methodGetName ? g_luaReflectV119.methodGetName(method) : nullptr;
    if (!name) return;
    bool interesting = false;
    for (const char* p = name; *p; ++p) {
        if ((p[0] == 'D' || p[0] == 'd') && (p[1] == 'o' || p[1] == 'O')) { interesting = true; break; }
        if ((p[0] == 'L' || p[0] == 'l') && (p[1] == 'o' || p[1] == 'O')) { interesting = true; break; }
        if ((p[0] == 'E' || p[0] == 'e') && (p[1] == 'x' || p[1] == 'X')) { interesting = true; break; }
    }
    if (!interesting) return;
    if (trace[0]) Append(trace, cap, L" | ");
    wchar_t sig[192]{}; DescribeMethodV121(method, sig, _countof(sig)); Append(trace, cap, sig);
    ++count;
}

bool SearchDoStringClassV121(Il2CppClass* start, LuaDoStringMethodV121& best,
                             int& bestScore, wchar_t* doTrace, std::size_t doTraceCap,
                             wchar_t* nearTrace, std::size_t nearTraceCap) {
    if (!start || !LoadLuaReflectV119(nullptr, 0)) return false;
    int nearCount = 0;
    for (Il2CppClass* cls = start; cls; cls = g_api.class_get_parent(cls)) {
        void* iter = nullptr;
        while (const MethodInfo* method = g_luaReflectV119.classGetMethods(cls, &iter)) {
            const char* name = g_luaReflectV119.methodGetName(method);
            if (Eq(name, "DoString")) {
                RecordDoStringCandidateV121(method, doTrace, doTraceCap);
                LuaChunkKindV121 kind = LuaChunkKindV121::None;
                const int score = ScoreDoStringV121(method, kind);
                if (score > bestScore) {
                    bestScore = score;
                    best.method = method;
                    best.argc = static_cast<int>(g_api.method_get_param_count(method));
                    best.chunkKind = kind;
                    DescribeMethodV121(method, best.signature, _countof(best.signature));
                }
            } else {
                RecordNearMethodV121(method, nearTrace, nearTraceCap, nearCount);
            }
        }
    }
    return best.method != nullptr;
}

void TryDirectDoStringV121(Il2CppClass* start, LuaDoStringMethodV121& best, int& bestScore,
                           wchar_t* doTrace, std::size_t doTraceCap) {
    if (!start) return;
    const int arities[] = {3, 2, 1};
    for (int argc : arities) {
        const MethodInfo* method = FindMethod(start, "DoString", argc);
        if (!method) continue;
        RecordDoStringCandidateV121(method, doTrace, doTraceCap);
        LuaChunkKindV121 kind = LuaChunkKindV121::None;
        const int score = ScoreDoStringV121(method, kind);
        if (score > bestScore) {
            bestScore = score;
            best.method = method;
            best.argc = argc;
            best.chunkKind = kind;
            DescribeMethodV121(method, best.signature, _countof(best.signature));
        }
    }
}

bool ResolveLuaDoStringV121(Il2CppObject* luaEnv, LuaDoStringMethodV121& out,
                            wchar_t* detail, std::size_t cap) {
    out = {};
    if (!luaEnv) { SetText(detail, cap, L"LUA_DOSTRING_V121: LuaEnv=null"); return false; }
    if (!LoadLuaReflectV119(detail, cap) || !LoadLuaManagerReflectV120(detail, cap)) return false;

    Il2CppClass* actual = g_api.object_get_class(luaEnv);
    const Il2CppImage* image = Image();
    Il2CppClass* declared = nullptr;
    if (image) {
        Il2CppClass* managerClass = g_api.class_from_name(image, "FGStudio.LuaSystem", "LuaSystemManager");
        const MethodInfo* getLuaEnv = managerClass ? FindMethod(managerClass, "get_LuaEnv", 0) : nullptr;
        const Il2CppType* rt = getLuaEnv ? g_api.method_get_return_type(getLuaEnv) : nullptr;
        declared = rt ? g_api.class_from_type(rt) : nullptr;
    }

    wchar_t actualName[128]{}; DescribeClassV121(actual, actualName, _countof(actualName));
    wchar_t declaredName[128]{}; DescribeClassV121(declared, declaredName, _countof(declaredName));
    wchar_t doTrace[384]{};
    wchar_t nearTrace[256]{};
    int bestScore = -1;

    (void)SearchDoStringClassV121(actual, out, bestScore, doTrace, _countof(doTrace), nearTrace, _countof(nearTrace));
    if (declared && declared != actual) {
        (void)SearchDoStringClassV121(declared, out, bestScore, doTrace, _countof(doTrace), nearTrace, _countof(nearTrace));
    }
    // Direct lookup is deliberately retained as a second path because the v1.1.10
    // runtime failure may itself be an iterator/metadata-enumeration visibility issue.
    TryDirectDoStringV121(actual, out, bestScore, doTrace, _countof(doTrace));
    if (declared && declared != actual) TryDirectDoStringV121(declared, out, bestScore, doTrace, _countof(doTrace));

    if (out.method) return true;

    SetText(detail, cap, L"LUA_DOSTRING_V121 unresolved • actual="); Append(detail, cap, actualName);
    Append(detail, cap, L" • declared="); Append(detail, cap, declaredName);
    Append(detail, cap, L" • DoString={"); Append(detail, cap, doTrace[0] ? doTrace : L"none"); Append(detail, cap, L"}");
    Append(detail, cap, L" • near={"); Append(detail, cap, nearTrace[0] ? nearTrace : L"none"); Append(detail, cap, L"}");
    return false;
}

bool CreateUtf8ByteArrayV121(const wchar_t* text, Il2CppObject*& array,
                             wchar_t* detail, std::size_t cap) {
    array = nullptr;
    if (!text || !g_api.module) { SetText(detail, cap, L"LUA_DOSTRING_V121: byte[] input unavailable"); return false; }
    using GetCorlibFn = const Il2CppImage* (__cdecl*)();
    using ArrayNewFn = Il2CppObject* (__cdecl*)(Il2CppClass*, std::uintptr_t);
    auto getCorlib = reinterpret_cast<GetCorlibFn>(GetProcAddress(g_api.module, "il2cpp_get_corlib"));
    auto arrayNew = reinterpret_cast<ArrayNewFn>(GetProcAddress(g_api.module, "il2cpp_array_new"));
    if (!getCorlib || !arrayNew) { SetText(detail, cap, L"LUA_DOSTRING_V121: byte[] IL2CPP exports unavailable"); return false; }
    const Il2CppImage* corlib = getCorlib();
    Il2CppClass* byteClass = corlib ? g_api.class_from_name(corlib, "System", "Byte") : nullptr;
    if (!byteClass) { SetText(detail, cap, L"LUA_DOSTRING_V121: System.Byte unresolved"); return false; }

    int wcharCount = 0; while (text[wcharCount]) ++wcharCount;
    const int byteCount = WideCharToMultiByte(CP_UTF8, 0, text, wcharCount, nullptr, 0, nullptr, nullptr);
    if (byteCount < 0 || byteCount > 1024 * 1024) { SetText(detail, cap, L"LUA_DOSTRING_V121: UTF8 size invalid"); return false; }
    array = arrayNew(byteClass, static_cast<std::uintptr_t>(byteCount));
    if (!array) { SetText(detail, cap, L"LUA_DOSTRING_V121: byte[] allocation failed"); return false; }
    if (byteCount == 0) return true;
    auto* data = reinterpret_cast<char*>(reinterpret_cast<unsigned char*>(array) + sizeof(Il2CppArrayHeaderV113));
    const int written = WideCharToMultiByte(CP_UTF8, 0, text, wcharCount, data, byteCount, nullptr, nullptr);
    if (written != byteCount) { SetText(detail, cap, L"LUA_DOSTRING_V121: UTF8 conversion failed"); return false; }
    return true;
}

bool RunLuaChunkV121(const wchar_t* chunk, wchar_t* result, std::size_t resultCap,
                     wchar_t* resolverRoute, std::size_t resolverRouteCap,
                     wchar_t* detail, std::size_t detailCap) {
    if (!chunk || !result || resultCap == 0) return false;
    result[0] = 0;
    Il2CppObject* luaEnv = nullptr;
    if (!ResolveLuaEnvV120(luaEnv, resolverRoute, resolverRouteCap, detail, detailCap)) return false;

    LuaDoStringMethodV121 resolved{};
    if (!ResolveLuaDoStringV121(luaEnv, resolved, detail, detailCap)) return false;

    Il2CppObject* chunkObject = nullptr;
    if (resolved.chunkKind == LuaChunkKindV121::String) {
        chunkObject = reinterpret_cast<Il2CppObject*>(NewString(chunk));
        if (!chunkObject) { SetText(detail, detailCap, L"LUA_DOSTRING_V121: String chunk allocation failed"); return false; }
    } else if (resolved.chunkKind == LuaChunkKindV121::ByteArray) {
        if (!CreateUtf8ByteArrayV121(chunk, chunkObject, detail, detailCap)) return false;
    } else {
        SetText(detail, detailCap, L"LUA_DOSTRING_V121: unsupported chunk kind"); return false;
    }

    Il2CppString* chunkName = nullptr;
    if (resolved.argc >= 2) {
        chunkName = NewString(L"ThanLongAutoHealV121");
        if (!chunkName) { SetText(detail, detailCap, L"LUA_DOSTRING_V121: chunkName allocation failed"); return false; }
    }

    Il2CppObject* values = nullptr;
    if (resolved.argc == 3) {
        void* args[] = { chunkObject, chunkName, nullptr };
        if (!InvokeObjectArgs(resolved.method, luaEnv, args, values, detail, detailCap)) return false;
    } else if (resolved.argc == 2) {
        void* args[] = { chunkObject, chunkName };
        if (!InvokeObjectArgs(resolved.method, luaEnv, args, values, detail, detailCap)) return false;
    } else if (resolved.argc == 1) {
        void* args[] = { chunkObject };
        if (!InvokeObjectArgs(resolved.method, luaEnv, args, values, detail, detailCap)) return false;
    } else {
        SetText(detail, detailCap, L"LUA_DOSTRING_V121: invalid argc"); return false;
    }

    wchar_t resultDetail[192]{};
    if (!FirstDoStringResultV119(values, result, resultCap, resultDetail, _countof(resultDetail))) {
        SetText(detail, detailCap, L"LUA_DOSTRING_V121: invalid result • method="); Append(detail, detailCap, resolved.signature);
        Append(detail, detailCap, L" • "); Append(detail, detailCap, resultDetail);
        return false;
    }
    return true;
}

struct LuaDialogIdsV121 {
    std::int32_t treatment = 0;
    std::int32_t confirm = 0;
    std::int32_t know = 0;
    wchar_t raw[384]{};
    wchar_t resolver[160]{};
};

bool ReadLuaDialogIdsV121(LuaDialogIdsV121& ids, wchar_t* detail, std::size_t cap) {
    ids = {};
    wchar_t raw[512]{};
    if (!RunLuaChunkV121(kGameDialogProbeLuaV119, raw, _countof(raw),
                         ids.resolver, _countof(ids.resolver), detail, cap)) return false;
    (void)ParseLuaTokenIntV119(raw, L"T=", ids.treatment);
    (void)ParseLuaTokenIntV119(raw, L"C=", ids.confirm);
    (void)ParseLuaTokenIntV119(raw, L"K=", ids.know);
    SetText(ids.raw, _countof(ids.raw), raw);
    return true;
}

bool InspectHealDialog(cleanroute::Snapshot& s, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap)) return false;
    s = {};
    s.validMask |= ValidHealDialog;

    bool gameDialogPresent = false;
    bool messageBoxPresent = false;
    if (!LuaUiPresentV119(c, L"GameDialog", gameDialogPresent, detail, cap)) return false;
    if (!LuaUiPresentV119(c, L"MessageBox", messageBoxPresent, detail, cap)) return false;
    if (gameDialogPresent) s.healDialogMask |= HealDialogPresent;

    LuaDialogIdsV121 ids{};
    wchar_t probe[448]{};
    if (!ReadLuaDialogIdsV121(ids, probe, _countof(probe))) {
        SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V121 PROBE FAIL • ");
        Append(s.healDialogDetail, _countof(s.healDialogDetail), probe);
        SetText(detail, cap, s.healDialogDetail);
        return true;
    }

    if (ids.treatment > 0) s.healDialogMask |= HealDialogTreatment;
    if (ids.confirm > 0 || messageBoxPresent) s.healDialogMask |= HealDialogConfirm;
    if (ids.know > 0) s.healDialogMask |= HealDialogKnow;

    SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V121 • route=");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), ids.resolver[0] ? ids.resolver : L"?");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • GD=");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), gameDialogPresent ? L"present" : L"absent");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • MB=");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), messageBoxPresent ? L"present" : L"absent");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • T="); AppendInt(s.healDialogDetail, _countof(s.healDialogDetail), ids.treatment);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • C="); AppendInt(s.healDialogDetail, _countof(s.healDialogDetail), ids.confirm);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • K="); AppendInt(s.healDialogDetail, _countof(s.healDialogDetail), ids.know);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • raw={"); Append(s.healDialogDetail, _countof(s.healDialogDetail), ids.raw); Append(s.healDialogDetail, _countof(s.healDialogDetail), L"}");
    SetText(detail, cap, s.healDialogDetail);
    return true;
}

} // namespace

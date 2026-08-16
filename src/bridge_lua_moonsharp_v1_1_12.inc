namespace {

struct MoonSharpDoStringV122 {
    const MethodInfo* method = nullptr;
    wchar_t signature[224]{};
};

bool IsMoonSharpDoStringV122(const MethodInfo* method) {
    if (!method || StaticMethod(method) || g_api.method_get_param_count(method) != 3) return false;
    if (!ParamType(method, 0, "System.String")) return false;
    if (!ParamType(method, 1, "MoonSharp.Interpreter.Table")) return false;
    if (!ParamType(method, 2, "System.String")) return false;
    wchar_t returnType[128]{};
    if (!TypeNameV121(g_api.method_get_return_type(method), returnType, _countof(returnType))) return false;
    return WideEqV121(returnType, L"MoonSharp.Interpreter.DynValue");
}

bool ResolveMoonSharpDoStringV122(Il2CppObject* scriptObject, MoonSharpDoStringV122& out,
                                  wchar_t* detail, std::size_t cap) {
    out = {};
    if (!scriptObject) { SetText(detail, cap, L"LUA_MOONSHARP_V122: Script=null"); return false; }
    if (!LoadLuaReflectV119(detail, cap) || !LoadLuaManagerReflectV120(detail, cap)) return false;

    Il2CppClass* actual = g_api.object_get_class(scriptObject);
    wchar_t actualName[160]{};
    DescribeClassV121(actual, actualName, _countof(actualName));
    wchar_t trace[448]{};

    for (Il2CppClass* cls = actual; cls; cls = g_api.class_get_parent(cls)) {
        void* iter = nullptr;
        while (const MethodInfo* method = g_luaReflectV119.classGetMethods(cls, &iter)) {
            const char* name = g_luaReflectV119.methodGetName(method);
            if (!Eq(name, "DoString")) continue;
            if (trace[0]) Append(trace, _countof(trace), L" | ");
            wchar_t sig[224]{};
            DescribeMethodV121(method, sig, _countof(sig));
            Append(trace, _countof(trace), sig);
            if (!out.method && IsMoonSharpDoStringV122(method)) {
                out.method = method;
                SetText(out.signature, _countof(out.signature), sig);
            }
        }
    }

    if (!out.method && actual) {
        const MethodInfo* direct = FindMethod(actual, "DoString", 3);
        if (direct && IsMoonSharpDoStringV122(direct)) {
            out.method = direct;
            DescribeMethodV121(direct, out.signature, _countof(out.signature));
        }
    }

    if (out.method) return true;

    SetText(detail, cap, L"LUA_MOONSHARP_V122 DoString unresolved • actual=");
    Append(detail, cap, actualName);
    Append(detail, cap, L" • required=DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue");
    Append(detail, cap, L" • DoString={");
    Append(detail, cap, trace[0] ? trace : L"none");
    Append(detail, cap, L"}");
    return false;
}

bool InvokeObjectArgsDetailedV122(const MethodInfo* method, void* instance, void** args,
                                  Il2CppObject*& out, wchar_t* detail, std::size_t cap) {
    out = nullptr;
    if (!method) { SetText(detail, cap, L"LUA_MOONSHARP_V122: method=null"); return false; }
    void* exc = nullptr;
    out = g_api.runtime_invoke(method, instance, args, &exc);
    if (!exc) return true;

    SetText(detail, cap, L"LUA_MOONSHARP_V122 managed exception");
    Il2CppObject* exceptionObject = reinterpret_cast<Il2CppObject*>(exc);
    Il2CppClass* exceptionClass = exceptionObject ? g_api.object_get_class(exceptionObject) : nullptr;
    if (exceptionClass) {
        wchar_t className[128]{};
        DescribeClassV121(exceptionClass, className, _countof(className));
        Append(detail, cap, L" • type="); Append(detail, cap, className);
        const MethodInfo* toString = FindMethod(exceptionClass, "ToString", 0);
        if (toString && !StaticMethod(toString)) {
            void* nestedExc = nullptr;
            Il2CppObject* textObject = g_api.runtime_invoke(toString, exceptionObject, nullptr, &nestedExc);
            if (!nestedExc && textObject) {
                wchar_t text[256]{};
                if (CopyString(reinterpret_cast<Il2CppString*>(textObject), text, _countof(text))) {
                    Append(detail, cap, L" • "); Append(detail, cap, text);
                }
            }
        }
    }
    return false;
}

bool MoonSharpDynValueStringV122(Il2CppObject* dynValue, wchar_t* out, std::size_t cap,
                                 wchar_t* detail, std::size_t detailCap) {
    if (!dynValue || !out || cap == 0) {
        SetText(detail, detailCap, L"LUA_MOONSHARP_V122: DynValue=null");
        return false;
    }
    out[0] = 0;
    Il2CppClass* dynClass = g_api.object_get_class(dynValue);
    wchar_t className[128]{};
    DescribeClassV121(dynClass, className, _countof(className));

    const MethodInfo* getString = FindMethod(dynClass, "get_String", 0);
    if (!getString || StaticMethod(getString)) {
        SetText(detail, detailCap, L"LUA_MOONSHARP_V122: DynValue.get_String unresolved • actual=");
        Append(detail, detailCap, className);
        return false;
    }
    wchar_t returnType[96]{};
    if (!TypeNameV121(g_api.method_get_return_type(getString), returnType, _countof(returnType)) ||
        !WideEqV121(returnType, L"System.String")) {
        SetText(detail, detailCap, L"LUA_MOONSHARP_V122: DynValue.get_String return mismatch • actual=");
        Append(detail, detailCap, className);
        return false;
    }

    Il2CppObject* stringObject = nullptr;
    wchar_t getterDetail[192]{};
    if (!InvokeObject(getString, dynValue, stringObject, getterDetail, _countof(getterDetail)) || !stringObject) {
        SetText(detail, detailCap, L"LUA_MOONSHARP_V122: returned DynValue is not String • actual=");
        Append(detail, detailCap, className);
        return false;
    }
    if (!CopyString(reinterpret_cast<Il2CppString*>(stringObject), out, cap)) {
        SetText(detail, detailCap, L"LUA_MOONSHARP_V122: cannot copy DynValue.String");
        return false;
    }
    return true;
}

bool RunLuaChunkV122(const wchar_t* chunk, wchar_t* result, std::size_t resultCap,
                     wchar_t* resolverRoute, std::size_t resolverRouteCap,
                     wchar_t* detail, std::size_t detailCap) {
    if (!chunk || !result || resultCap == 0) return false;
    result[0] = 0;

    Il2CppObject* scriptObject = nullptr;
    if (!ResolveLuaEnvV120(scriptObject, resolverRoute, resolverRouteCap, detail, detailCap)) return false;

    MoonSharpDoStringV122 resolved{};
    if (!ResolveMoonSharpDoStringV122(scriptObject, resolved, detail, detailCap)) return false;

    Il2CppString* code = NewString(chunk);
    Il2CppString* friendlyName = NewString(L"ThanLongAutoHealV122");
    if (!code || !friendlyName) {
        SetText(detail, detailCap, L"LUA_MOONSHARP_V122: String allocation failed");
        return false;
    }

    // Runtime + MoonSharp source verified order:
    // DoString(string code, Table globalContext = null, string codeFriendlyName = null)
    void* args[] = { code, nullptr, friendlyName };
    Il2CppObject* dynValue = nullptr;
    wchar_t invokeDetail[384]{};
    if (!InvokeObjectArgsDetailedV122(resolved.method, scriptObject, args, dynValue,
                                      invokeDetail, _countof(invokeDetail))) {
        SetText(detail, detailCap, L"LUA_MOONSHARP_V122 invoke fail • method=");
        Append(detail, detailCap, resolved.signature);
        Append(detail, detailCap, L" • "); Append(detail, detailCap, invokeDetail);
        return false;
    }
    if (!dynValue) {
        SetText(detail, detailCap, L"LUA_MOONSHARP_V122: DoString returned null DynValue");
        return false;
    }

    wchar_t valueDetail[256]{};
    if (!MoonSharpDynValueStringV122(dynValue, result, resultCap,
                                     valueDetail, _countof(valueDetail))) {
        SetText(detail, detailCap, L"LUA_MOONSHARP_V122 result fail • method=");
        Append(detail, detailCap, resolved.signature);
        Append(detail, detailCap, L" • "); Append(detail, detailCap, valueDetail);
        return false;
    }

    if (resolverRoute && resolverRouteCap) {
        Append(resolverRoute, resolverRouteCap, L" -> MoonSharp.Script.DoString(String,Table,String)");
    }
    return true;
}

struct LuaDialogIdsV122 {
    std::int32_t treatment = 0;
    std::int32_t confirm = 0;
    std::int32_t know = 0;
    wchar_t raw[384]{};
    wchar_t resolver[192]{};
};

bool ReadLuaDialogIdsV122(LuaDialogIdsV122& ids, wchar_t* detail, std::size_t cap) {
    ids = {};
    wchar_t raw[512]{};
    if (!RunLuaChunkV122(kGameDialogProbeLuaV119, raw, _countof(raw),
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

    LuaDialogIdsV122 ids{};
    wchar_t probe[480]{};
    if (!ReadLuaDialogIdsV122(ids, probe, _countof(probe))) {
        SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V122 PROBE FAIL • ");
        Append(s.healDialogDetail, _countof(s.healDialogDetail), probe);
        SetText(detail, cap, s.healDialogDetail);
        return true;
    }

    if (ids.treatment > 0) s.healDialogMask |= HealDialogTreatment;
    if (ids.confirm > 0 || messageBoxPresent) s.healDialogMask |= HealDialogConfirm;
    if (ids.know > 0) s.healDialogMask |= HealDialogKnow;

    SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V122 • route=");
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

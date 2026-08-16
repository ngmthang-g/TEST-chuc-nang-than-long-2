namespace {

using ClassGetFieldsFnV120 = FieldInfo* (__cdecl*)(Il2CppClass*, void**);
using FieldGetNameFnV120 = const char* (__cdecl*)(FieldInfo*);
using ClassGetTypeFnV120 = const Il2CppType* (__cdecl*)(Il2CppClass*);
using TypeGetObjectFnV120 = Il2CppObject* (__cdecl*)(const Il2CppType*);
using ClassGetNamespaceFnV120 = const char* (__cdecl*)(Il2CppClass*);

struct LuaManagerReflectApiV120 {
    ClassGetFieldsFnV120 classGetFields = nullptr;
    FieldGetNameFnV120 fieldGetName = nullptr;
    ClassGetTypeFnV120 classGetType = nullptr;
    TypeGetObjectFnV120 typeGetObject = nullptr;
    ClassGetNamespaceFnV120 classGetNamespace = nullptr;
    bool loaded = false;
};

LuaManagerReflectApiV120 g_luaManagerReflectV120{};

bool LoadLuaManagerReflectV120(wchar_t* detail, std::size_t cap) {
    if (g_luaManagerReflectV120.loaded) return true;
    if (!LoadLuaReflectV119(detail, cap) || !g_api.module) return false;
    g_luaManagerReflectV120.classGetFields = reinterpret_cast<ClassGetFieldsFnV120>(
        GetProcAddress(g_api.module, "il2cpp_class_get_fields"));
    g_luaManagerReflectV120.fieldGetName = reinterpret_cast<FieldGetNameFnV120>(
        GetProcAddress(g_api.module, "il2cpp_field_get_name"));
    g_luaManagerReflectV120.classGetType = reinterpret_cast<ClassGetTypeFnV120>(
        GetProcAddress(g_api.module, "il2cpp_class_get_type"));
    g_luaManagerReflectV120.typeGetObject = reinterpret_cast<TypeGetObjectFnV120>(
        GetProcAddress(g_api.module, "il2cpp_type_get_object"));
    g_luaManagerReflectV120.classGetNamespace = reinterpret_cast<ClassGetNamespaceFnV120>(
        GetProcAddress(g_api.module, "il2cpp_class_get_namespace"));
    if (!g_luaManagerReflectV120.classGetFields || !g_luaManagerReflectV120.fieldGetName) {
        SetText(detail, cap, L"LUA_MANAGER_V120: missing field-enumeration exports");
        return false;
    }
    g_luaManagerReflectV120.loaded = true;
    return true;
}

void AppendAsciiV120(wchar_t* out, std::size_t cap, const char* text) {
    if (!out || cap == 0 || !text) return;
    std::size_t n = 0;
    while (n + 1 < cap && out[n]) ++n;
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(text); *p && n + 1 < cap; ++p) {
        out[n++] = (*p < 0x80) ? static_cast<wchar_t>(*p) : L'?';
    }
    out[n] = 0;
}

bool IsManagerObjectV120(Il2CppClass* managerClass, Il2CppObject* value) {
    if (!managerClass || !value) return false;
    Il2CppClass* actual = g_api.object_get_class(value);
    for (Il2CppClass* c = actual; c; c = g_api.class_get_parent(c)) {
        if (c == managerClass) return true;
    }
    return false;
}

bool IsReferenceFieldV120(FieldInfo* field) {
    if (!field) return false;
    const Il2CppType* type = g_api.field_get_type(field);
    Il2CppClass* fieldClass = type ? g_api.class_from_type(type) : nullptr;
    return fieldClass && !g_api.class_is_valuetype(fieldClass);
}

void RecordStaticFieldV120(Il2CppClass* declaringClass, FieldInfo* field,
                           wchar_t* trace, std::size_t traceCap) {
    if (!trace || traceCap == 0 || !field) return;
    if (trace[0]) Append(trace, traceCap, L" | ");
    const char* className = declaringClass && g_luaReflectV119.classGetName
        ? g_luaReflectV119.classGetName(declaringClass) : nullptr;
    const char* fieldName = g_luaManagerReflectV120.fieldGetName(field);
    if (className) AppendAsciiV120(trace, traceCap, className);
    else Append(trace, traceCap, L"?");
    Append(trace, traceCap, L".");
    if (fieldName) AppendAsciiV120(trace, traceCap, fieldName);
    else Append(trace, traceCap, L"?");

    const Il2CppType* type = g_api.field_get_type(field);
    char* typeName = type ? g_api.type_get_name(type) : nullptr;
    if (typeName) {
        Append(trace, traceCap, L":");
        AppendAsciiV120(trace, traceCap, typeName);
        g_api.free_fn(typeName);
    }
}

bool ScanStaticReferencesV120(Il2CppClass* declaringClass, Il2CppClass* managerClass,
                              Il2CppObject*& manager, int& staticRefCount,
                              wchar_t* trace, std::size_t traceCap) {
    if (!declaringClass || manager || !LoadLuaManagerReflectV120(trace, traceCap)) return manager != nullptr;
    constexpr std::uint32_t kFieldStatic = 0x0010u;
    void* iter = nullptr;
    while (FieldInfo* field = g_luaManagerReflectV120.classGetFields(declaringClass, &iter)) {
        if ((g_luaReflectV119.fieldGetFlags(field) & kFieldStatic) == 0) continue;
        if (!IsReferenceFieldV120(field)) continue;
        ++staticRefCount;
        if (staticRefCount <= 8) RecordStaticFieldV120(declaringClass, field, trace, traceCap);
        Il2CppObject* value = nullptr;
        g_luaReflectV119.fieldStaticGetValue(field, &value);
        if (value && IsManagerObjectV120(managerClass, value)) {
            manager = value;
            return true;
        }
    }
    return false;
}

bool ManagerIsUnityObjectV120(Il2CppClass* managerClass) {
    if (!managerClass || !LoadLuaManagerReflectV120(nullptr, 0)) return false;
    for (Il2CppClass* c = managerClass; c; c = g_api.class_get_parent(c)) {
        const char* name = g_luaReflectV119.classGetName ? g_luaReflectV119.classGetName(c) : nullptr;
        const char* ns = g_luaManagerReflectV120.classGetNamespace
            ? g_luaManagerReflectV120.classGetNamespace(c) : nullptr;
        if (name && (Eq(name, "MonoBehaviour") || Eq(name, "ScriptableObject") ||
                     Eq(name, "Behaviour") || Eq(name, "Component"))) return true;
        if (name && ns && Eq(name, "Object") && Eq(ns, "UnityEngine")) return true;
    }
    return false;
}

const MethodInfo* FindStaticTypeMethodV120(Il2CppClass* klass, const char* name) {
    if (!klass || !name || !LoadLuaReflectV119(nullptr, 0)) return nullptr;
    for (Il2CppClass* c = klass; c; c = g_api.class_get_parent(c)) {
        void* iter = nullptr;
        while (const MethodInfo* method = g_luaReflectV119.classGetMethods(c, &iter)) {
            const char* methodName = g_luaReflectV119.methodGetName(method);
            if (!Eq(methodName, name) || !StaticMethod(method)) continue;
            if (g_api.method_get_param_count(method) != 1) continue;
            if (ParamType(method, 0, "System.Type")) return method;
        }
    }
    return nullptr;
}

bool TryUnityObjectLookupV120(Il2CppClass* managerClass, Il2CppObject*& manager,
                              wchar_t* route, std::size_t routeCap,
                              wchar_t* detail, std::size_t detailCap) {
    manager = nullptr;
    if (!ManagerIsUnityObjectV120(managerClass)) return false;
    if (!LoadLuaManagerReflectV120(detail, detailCap) ||
        !g_luaManagerReflectV120.classGetType || !g_luaManagerReflectV120.typeGetObject) {
        SetText(detail, detailCap, L"LUA_MANAGER_V120: Unity lookup exports unavailable");
        return false;
    }
    Il2CppDomain* domain = g_api.domain_get();
    if (!domain) return false;
    const Il2CppAssembly* assembly = g_api.domain_assembly_open(domain, "UnityEngine.CoreModule");
    if (!assembly) assembly = g_api.domain_assembly_open(domain, "UnityEngine.CoreModule.dll");
    const Il2CppImage* image = assembly ? g_api.assembly_get_image(assembly) : nullptr;
    Il2CppClass* objectClass = image ? g_api.class_from_name(image, "UnityEngine", "Object") : nullptr;
    if (!objectClass) return false;

    const Il2CppType* managerType = g_luaManagerReflectV120.classGetType(managerClass);
    Il2CppObject* typeObject = managerType ? g_luaManagerReflectV120.typeGetObject(managerType) : nullptr;
    if (!typeObject) return false;

    const char* methods[] = {"FindFirstObjectByType", "FindAnyObjectByType", "FindObjectOfType"};
    for (const char* methodName : methods) {
        const MethodInfo* method = FindStaticTypeMethodV120(objectClass, methodName);
        if (!method) continue;
        void* args[] = { typeObject };
        Il2CppObject* value = nullptr;
        wchar_t probe[128]{};
        if (!InvokeObjectArgs(method, nullptr, args, value, probe, _countof(probe)) || !value) continue;
        if (!IsManagerObjectV120(managerClass, value)) continue;
        manager = value;
        SetText(route, routeCap, L"UnityEngine.Object.");
        AppendAsciiV120(route, routeCap, methodName);
        return true;
    }
    return false;
}

bool ResolveLuaSystemManagerV120(Il2CppClass*& managerClass, Il2CppObject*& manager,
                                 wchar_t* route, std::size_t routeCap,
                                 wchar_t* detail, std::size_t cap) {
    managerClass = nullptr;
    manager = nullptr;
    if (route && routeCap) route[0] = 0;
    const Il2CppImage* image = Image();
    if (!image) { SetText(detail, cap, L"LUA_MANAGER_V120: Assembly-CSharp unavailable"); return false; }
    managerClass = g_api.class_from_name(image, "FGStudio.LuaSystem", "LuaSystemManager");
    if (!managerClass) { SetText(detail, cap, L"LUA_MANAGER_V120: LuaSystemManager class unresolved"); return false; }

    const MethodInfo* getInstance = FindMethod(managerClass, "get_Instance", 0);
    if (getInstance && StaticMethod(getInstance)) {
        wchar_t probe[160]{};
        if (InvokeObject(getInstance, nullptr, manager, probe, _countof(probe)) &&
            IsManagerObjectV120(managerClass, manager)) {
            SetText(route, routeCap, L"LuaSystemManager.get_Instance");
            return true;
        }
        manager = nullptr;
    }

    if (!LoadLuaManagerReflectV120(detail, cap)) return false;
    int staticRefCount = 0;
    wchar_t trace[256]{};
    for (Il2CppClass* c = managerClass; c && !manager; c = g_api.class_get_parent(c)) {
        if (ScanStaticReferencesV120(c, managerClass, manager, staticRefCount, trace, _countof(trace))) {
            SetText(route, routeCap, L"static-ref in LuaSystemManager hierarchy");
            return true;
        }
    }

    struct RelatedClassV120 { const char* ns; const char* name; };
    const RelatedClassV120 related[] = {
        {"FGStudio.LuaSystem.API", "LuaSystemAPI_GUI"},
        {"FGStudio.LuaSystem.API", "LuaSystemAPI_Game"},
        {"FGStudio.LuaSystem.API", "LuaSystemAPI_Network"},
        {"FGStudio.LuaSystem", "LuaSystemSharedData"},
        {"FGStudio.LuaSystem.Logic", "MonoBehaviourExecutor"},
    };
    for (const auto& item : related) {
        Il2CppClass* cls = g_api.class_from_name(image, item.ns, item.name);
        if (!cls) continue;
        if (ScanStaticReferencesV120(cls, managerClass, manager, staticRefCount, trace, _countof(trace))) {
            SetText(route, routeCap, L"static-ref in related Lua bridge class");
            return true;
        }
    }

    wchar_t unityDetail[128]{};
    if (TryUnityObjectLookupV120(managerClass, manager, route, routeCap,
                                 unityDetail, _countof(unityDetail))) return true;

    SetText(detail, cap, L"LUA_MANAGER_V120 unresolved • staticRefs=");
    AppendInt(detail, cap, staticRefCount);
    Append(detail, cap, L" • unityCandidate=");
    Append(detail, cap, ManagerIsUnityObjectV120(managerClass) ? L"yes" : L"no");
    Append(detail, cap, L" • fields={");
    Append(detail, cap, trace[0] ? trace : L"none");
    Append(detail, cap, L"}");
    return false;
}

bool ResolveLuaEnvV120(Il2CppObject*& luaEnv, wchar_t* route, std::size_t routeCap,
                       wchar_t* detail, std::size_t cap) {
    luaEnv = nullptr;
    if (route && routeCap) route[0] = 0;
    const Il2CppImage* image = Image();
    if (!image) { SetText(detail, cap, L"LUA_MANAGER_V120: Assembly-CSharp unavailable"); return false; }
    Il2CppClass* managerClass = g_api.class_from_name(image, "FGStudio.LuaSystem", "LuaSystemManager");
    if (!managerClass) { SetText(detail, cap, L"LUA_MANAGER_V120: LuaSystemManager class unresolved"); return false; }
    const MethodInfo* getLuaEnv = FindMethod(managerClass, "get_LuaEnv", 0);
    if (!getLuaEnv) { SetText(detail, cap, L"LUA_MANAGER_V120: get_LuaEnv unresolved"); return false; }

    if (StaticMethod(getLuaEnv)) {
        wchar_t probe[160]{};
        if (InvokeObject(getLuaEnv, nullptr, luaEnv, probe, _countof(probe)) && luaEnv) {
            SetText(route, routeCap, L"static LuaSystemManager.get_LuaEnv");
            return true;
        }
        luaEnv = nullptr;
    }

    Il2CppObject* manager = nullptr;
    wchar_t managerRoute[128]{};
    if (!ResolveLuaSystemManagerV120(managerClass, manager, managerRoute, _countof(managerRoute), detail, cap)) return false;
    if (StaticMethod(getLuaEnv)) {
        SetText(detail, cap, L"LUA_MANAGER_V120: static get_LuaEnv returned null");
        return false;
    }
    if (!InvokeObject(getLuaEnv, manager, luaEnv, detail, cap) || !luaEnv) {
        SetText(detail, cap, L"LUA_MANAGER_V120: instance get_LuaEnv returned null");
        return false;
    }
    SetText(route, routeCap, managerRoute);
    Append(route, routeCap, L" -> get_LuaEnv");
    return true;
}

bool RunLuaChunkV120(const wchar_t* chunk, wchar_t* result, std::size_t resultCap,
                     wchar_t* resolverRoute, std::size_t resolverRouteCap,
                     wchar_t* detail, std::size_t detailCap) {
    if (!chunk || !result || resultCap == 0) return false;
    result[0] = 0;
    Il2CppObject* luaEnv = nullptr;
    if (!ResolveLuaEnvV120(luaEnv, resolverRoute, resolverRouteCap, detail, detailCap)) return false;
    Il2CppClass* envClass = g_api.object_get_class(luaEnv);
    int argc = 0;
    const MethodInfo* doString = FindLuaDoStringV119(envClass, argc, detail, detailCap);
    if (!doString) {
        wchar_t old[192]{}; SetText(old, _countof(old), detail);
        SetText(detail, detailCap, L"LUA_DIALOG_V120: DoString unresolved • "); Append(detail, detailCap, old);
        return false;
    }
    Il2CppString* script = NewString(chunk);
    if (!script) { SetText(detail, detailCap, L"LUA_DIALOG_V120: Lua chunk allocation failed"); return false; }
    Il2CppObject* values = nullptr;
    if (argc == 3) {
        Il2CppString* chunkName = NewString(L"ThanLongAutoHealV120");
        if (!chunkName) { SetText(detail, detailCap, L"LUA_DIALOG_V120: chunkName allocation failed"); return false; }
        void* args[] = { script, chunkName, nullptr };
        if (!InvokeObjectArgs(doString, luaEnv, args, values, detail, detailCap)) return false;
    } else {
        void* args[] = { script };
        if (!InvokeObjectArgs(doString, luaEnv, args, values, detail, detailCap)) return false;
    }
    wchar_t resultDetail[192]{};
    if (!FirstDoStringResultV119(values, result, resultCap, resultDetail, _countof(resultDetail))) {
        SetText(detail, detailCap, L"LUA_DIALOG_V120: invalid DoString result • ");
        Append(detail, detailCap, resultDetail);
        return false;
    }
    return true;
}

struct LuaDialogIdsV120 {
    std::int32_t treatment = 0;
    std::int32_t confirm = 0;
    std::int32_t know = 0;
    wchar_t raw[384]{};
    wchar_t resolver[160]{};
};

bool ReadLuaDialogIdsV120(LuaDialogIdsV120& ids, wchar_t* detail, std::size_t cap) {
    ids = {};
    wchar_t raw[512]{};
    if (!RunLuaChunkV120(kGameDialogProbeLuaV119, raw, _countof(raw),
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

    LuaDialogIdsV120 ids{};
    wchar_t probe[384]{};
    if (!ReadLuaDialogIdsV120(ids, probe, _countof(probe))) {
        SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V120 PROBE FAIL • ");
        Append(s.healDialogDetail, _countof(s.healDialogDetail), probe);
        SetText(detail, cap, s.healDialogDetail);
        return true;
    }

    if (ids.treatment > 0) s.healDialogMask |= HealDialogTreatment;
    if (ids.confirm > 0 || messageBoxPresent) s.healDialogMask |= HealDialogConfirm;
    if (ids.know > 0) s.healDialogMask |= HealDialogKnow;

    SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V120 • route=");
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

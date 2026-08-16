namespace {

using ClassGetMethodsFnV119 = const MethodInfo* (__cdecl*)(Il2CppClass*, void**);
using MethodGetNameFnV119 = const char* (__cdecl*)(const MethodInfo*);
using FieldGetFlagsFnV119 = std::uint32_t (__cdecl*)(FieldInfo*);
using FieldStaticGetValueFnV119 = void (__cdecl*)(FieldInfo*, void*);
using ClassGetNameFnV119 = const char* (__cdecl*)(Il2CppClass*);

struct LuaReflectApiV119 {
    ClassGetMethodsFnV119 classGetMethods = nullptr;
    MethodGetNameFnV119 methodGetName = nullptr;
    FieldGetFlagsFnV119 fieldGetFlags = nullptr;
    FieldStaticGetValueFnV119 fieldStaticGetValue = nullptr;
    ClassGetNameFnV119 classGetName = nullptr;
    bool loaded = false;
};

LuaReflectApiV119 g_luaReflectV119{};

bool LoadLuaReflectV119(wchar_t* detail, std::size_t cap) {
    if (g_luaReflectV119.loaded) return true;
    if (!g_api.Load(detail, cap) || !g_api.module) return false;
    g_luaReflectV119.classGetMethods = reinterpret_cast<ClassGetMethodsFnV119>(
        GetProcAddress(g_api.module, "il2cpp_class_get_methods"));
    g_luaReflectV119.methodGetName = reinterpret_cast<MethodGetNameFnV119>(
        GetProcAddress(g_api.module, "il2cpp_method_get_name"));
    g_luaReflectV119.fieldGetFlags = reinterpret_cast<FieldGetFlagsFnV119>(
        GetProcAddress(g_api.module, "il2cpp_field_get_flags"));
    g_luaReflectV119.fieldStaticGetValue = reinterpret_cast<FieldStaticGetValueFnV119>(
        GetProcAddress(g_api.module, "il2cpp_field_static_get_value"));
    g_luaReflectV119.classGetName = reinterpret_cast<ClassGetNameFnV119>(
        GetProcAddress(g_api.module, "il2cpp_class_get_name"));
    if (!g_luaReflectV119.classGetMethods || !g_luaReflectV119.methodGetName ||
        !g_luaReflectV119.fieldGetFlags || !g_luaReflectV119.fieldStaticGetValue) {
        SetText(detail, cap, L"LUA_DIALOG_V119: thiếu IL2CPP reflection export");
        return false;
    }
    g_luaReflectV119.loaded = true;
    return true;
}

bool ResolveLuaSystemManagerV119(Il2CppClass*& managerClass, Il2CppObject*& manager,
                                 wchar_t* detail, std::size_t cap) {
    managerClass = nullptr;
    manager = nullptr;
    const Il2CppImage* image = Image();
    if (!image) { SetText(detail, cap, L"LUA_DIALOG_V119: Assembly-CSharp chưa sẵn sàng"); return false; }
    managerClass = g_api.class_from_name(image, "FGStudio.LuaSystem", "LuaSystemManager");
    if (!managerClass) { SetText(detail, cap, L"LUA_DIALOG_V119: không resolve LuaSystemManager"); return false; }

    const MethodInfo* getInstance = FindMethod(managerClass, "get_Instance", 0);
    if (getInstance && StaticMethod(getInstance)) {
        wchar_t probe[160]{};
        if (InvokeObject(getInstance, nullptr, manager, probe, _countof(probe)) && manager) return true;
    }

    if (!LoadLuaReflectV119(detail, cap)) return false;
    constexpr std::uint32_t kFieldStatic = 0x0010u;
    const char* names[] = {"Instance", "instance", "_instance", "<Instance>k__BackingField"};
    for (Il2CppClass* cls = managerClass; cls && !manager; cls = g_api.class_get_parent(cls)) {
        for (const char* name : names) {
            FieldInfo* field = g_api.class_get_field_from_name(cls, name);
            if (!field) continue;
            if ((g_luaReflectV119.fieldGetFlags(field) & kFieldStatic) == 0) continue;
            Il2CppObject* value = nullptr;
            g_luaReflectV119.fieldStaticGetValue(field, &value);
            if (value) { manager = value; break; }
        }
    }
    if (!manager) {
        SetText(detail, cap, L"LUA_DIALOG_V119: LuaSystemManager instance unresolved");
        return false;
    }
    return true;
}

bool ResolveLuaEnvV119(Il2CppObject*& luaEnv, wchar_t* detail, std::size_t cap) {
    luaEnv = nullptr;
    Il2CppClass* managerClass = nullptr;
    Il2CppObject* manager = nullptr;
    if (!ResolveLuaSystemManagerV119(managerClass, manager, detail, cap)) return false;
    Il2CppClass* actual = manager ? g_api.object_get_class(manager) : managerClass;
    if (!actual) actual = managerClass;
    const MethodInfo* getLuaEnv = FindMethod(actual, "get_LuaEnv", 0);
    if (!getLuaEnv) getLuaEnv = FindMethod(managerClass, "get_LuaEnv", 0);
    if (!getLuaEnv) { SetText(detail, cap, L"LUA_DIALOG_V119: get_LuaEnv() unresolved"); return false; }
    void* instance = StaticMethod(getLuaEnv) ? nullptr : manager;
    if (!StaticMethod(getLuaEnv) && !instance) {
        SetText(detail, cap, L"LUA_DIALOG_V119: LuaEnv getter cần manager instance");
        return false;
    }
    if (!InvokeObject(getLuaEnv, instance, luaEnv, detail, cap) || !luaEnv) {
        SetText(detail, cap, L"LUA_DIALOG_V119: LuaEnv=null");
        return false;
    }
    return true;
}

const MethodInfo* FindLuaDoStringV119(Il2CppClass* luaEnvClass, int& argc,
                                      wchar_t* detail, std::size_t cap) {
    argc = 0;
    if (!luaEnvClass || !LoadLuaReflectV119(detail, cap)) return nullptr;
    const MethodInfo* fallbackOne = nullptr;
    for (Il2CppClass* cls = luaEnvClass; cls; cls = g_api.class_get_parent(cls)) {
        void* iter = nullptr;
        while (const MethodInfo* method = g_luaReflectV119.classGetMethods(cls, &iter)) {
            const char* name = g_luaReflectV119.methodGetName(method);
            if (!Eq(name, "DoString") || StaticMethod(method)) continue;
            const std::uint32_t count = g_api.method_get_param_count(method);
            if (count == 3 && ParamType(method, 0, "System.String") && ParamType(method, 1, "System.String")) {
                argc = 3;
                return method;
            }
            if (count == 1 && ParamType(method, 0, "System.String") && !fallbackOne) fallbackOne = method;
        }
    }
    if (fallbackOne) { argc = 1; return fallbackOne; }
    SetText(detail, cap, L"LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)");
    return nullptr;
}

bool FirstDoStringResultV119(Il2CppObject* arrayObject, wchar_t* out, std::size_t cap,
                             wchar_t* detail, std::size_t detailCap) {
    if (!arrayObject || !out || cap == 0) {
        SetText(detail, detailCap, L"LUA_DIALOG_V119: DoString result array=null");
        return false;
    }
    auto* header = reinterpret_cast<Il2CppArrayHeaderV113*>(arrayObject);
    if (header->maxLength < 1 || header->maxLength > 64) {
        SetText(detail, detailCap, L"LUA_DIALOG_V119: DoString result count invalid");
        return false;
    }
    auto** slots = reinterpret_cast<Il2CppObject**>(
        reinterpret_cast<unsigned char*>(arrayObject) + sizeof(Il2CppArrayHeaderV113));
    Il2CppObject* first = slots[0];
    if (!first) { SetText(detail, detailCap, L"LUA_DIALOG_V119: first Lua result=null"); return false; }
    if (g_luaReflectV119.classGetName) {
        Il2CppClass* fc = g_api.object_get_class(first);
        const char* name = fc ? g_luaReflectV119.classGetName(fc) : nullptr;
        if (name && !Eq(name, "String")) {
            SetText(detail, detailCap, L"LUA_DIALOG_V119: first Lua result không phải String");
            return false;
        }
    }
    if (!CopyString(reinterpret_cast<Il2CppString*>(first), out, cap)) {
        SetText(detail, detailCap, L"LUA_DIALOG_V119: không copy được Lua result String");
        return false;
    }
    return true;
}

bool RunLuaChunkV119(const wchar_t* chunk, wchar_t* result, std::size_t resultCap,
                     wchar_t* detail, std::size_t detailCap) {
    if (!chunk || !result || resultCap == 0) return false;
    result[0] = 0;
    Il2CppObject* luaEnv = nullptr;
    if (!ResolveLuaEnvV119(luaEnv, detail, detailCap)) return false;
    Il2CppClass* ec = g_api.object_get_class(luaEnv);
    int argc = 0;
    const MethodInfo* doString = FindLuaDoStringV119(ec, argc, detail, detailCap);
    if (!doString) return false;
    Il2CppString* script = NewString(chunk);
    if (!script) { SetText(detail, detailCap, L"LUA_DIALOG_V119: không tạo được Lua chunk String"); return false; }
    Il2CppObject* values = nullptr;
    if (argc == 3) {
        Il2CppString* chunkName = NewString(L"ThanLongAutoHealV119");
        if (!chunkName) { SetText(detail, detailCap, L"LUA_DIALOG_V119: chunkName alloc fail"); return false; }
        void* args[] = { script, chunkName, nullptr };
        if (!InvokeObjectArgs(doString, luaEnv, args, values, detail, detailCap)) return false;
    } else {
        void* args[] = { script };
        if (!InvokeObjectArgs(doString, luaEnv, args, values, detail, detailCap)) return false;
    }
    return FirstDoStringResultV119(values, result, resultCap, detail, detailCap);
}

static const wchar_t kGameDialogProbeLuaV119[] = LR"LUA(
local function norm(s)
    if type(s) ~= "string" then return "" end
    s = string.gsub(s, "<[^>]*>", " ")
    s = string.gsub(s, "%s+", " ")
    s = string.gsub(s, "^%s+", "")
    s = string.gsub(s, "%s+$", "")
    return string.lower(s)
end
local wanted = { ["trị liệu"]="T", ["xác nhận"]="C", ["ta biết rồi"]="K" }
local found = { T=0, C=0, K=0 }
local where = { T="", C="", K="" }
local samples = {}
local seen = {}
local nodes = 0
local function addSample(s)
    if type(s) == "string" and #samples < 8 then samples[#samples+1] = s end
end
local function scanSelections(sels, path)
    if type(sels) ~= "table" then return end
    for k,v in pairs(sels) do
        if type(v) == "string" then
            addSample(path .. "[" .. tostring(k) .. "]=" .. v)
            local tag = wanted[norm(v)]
            local id = tonumber(k)
            if tag and id and id > 0 and found[tag] == 0 then
                found[tag] = id
                where[tag] = path
            end
        end
    end
end
local function scan(t, path, depth)
    if type(t) ~= "table" or seen[t] or depth > 6 or nodes >= 320 then return end
    seen[t] = true
    nodes = nodes + 1
    scanSelections(rawget(t, "Selections"), path .. ".Selections")
    local priority = {"GameDialogData","gameDialogData","CurrentGameDialogData","currentGameDialogData","DialogData","dialogData","GameDialog","gameDialog"}
    for _,key in ipairs(priority) do
        local v = rawget(t, key)
        if type(v) == "table" then scan(v, path .. "." .. key, depth + 1) end
    end
    for k,v in pairs(t) do
        if type(v) == "table" then
            local ks = string.lower(tostring(k))
            if string.find(ks, "dialog", 1, true) or string.find(ks, "selection", 1, true) then
                scan(v, path .. "." .. tostring(k), depth + 1)
            end
        end
    end
    for k,v in pairs(t) do
        if type(v) == "table" then scan(v, path .. "." .. tostring(k), depth + 1) end
    end
end
local gd = nil
local af = nil
if GUI and GUI.FindUI then
    gd = GUI.FindUI("GameDialog")
    af = GUI.FindUI("AutoFight_Main")
end
scan(gd, "GameDialog", 0)
scan(af, "AutoFight_Main", 0)
return "T="..tostring(found.T)..";C="..tostring(found.C)..";K="..tostring(found.K)..
       ";GD="..type(gd)..";AF="..type(af)..";N="..tostring(nodes)..
       ";WT="..where.T..";WC="..where.C..";WK="..where.K..";S="..table.concat(samples, " | ")
)LUA";

bool ParseLuaTokenIntV119(const wchar_t* text, const wchar_t* token, std::int32_t& value) {
    value = 0;
    if (!text || !token) return false;
    const wchar_t* p = wcsstr(text, token);
    if (!p) return false;
    while (*token) { ++p; ++token; }
    std::int64_t parsed = 0;
    bool any = false;
    while (*p >= L'0' && *p <= L'9') {
        any = true;
        parsed = parsed * 10 + (*p - L'0');
        if (parsed > 2147483647LL) return false;
        ++p;
    }
    if (!any) return false;
    value = static_cast<std::int32_t>(parsed);
    return true;
}

struct LuaDialogIdsV119 {
    std::int32_t treatment = 0;
    std::int32_t confirm = 0;
    std::int32_t know = 0;
    wchar_t raw[384]{};
};

bool ReadLuaDialogIdsV119(LuaDialogIdsV119& ids, wchar_t* detail, std::size_t cap) {
    ids = {};
    wchar_t raw[512]{};
    if (!RunLuaChunkV119(kGameDialogProbeLuaV119, raw, _countof(raw), detail, cap)) return false;
    (void)ParseLuaTokenIntV119(raw, L"T=", ids.treatment);
    (void)ParseLuaTokenIntV119(raw, L"C=", ids.confirm);
    (void)ParseLuaTokenIntV119(raw, L"K=", ids.know);
    SetText(ids.raw, _countof(ids.raw), raw);
    return true;
}

bool LuaUiPresentV119(const Classes& c, const wchar_t* name, bool& present,
                      wchar_t* detail, std::size_t cap) {
    present = false;
    Il2CppObject* ui = nullptr;
    wchar_t probe[192]{};
    if (!FindLuaUiObjectV113(c, name, ui, probe, _countof(probe))) {
        SetText(detail, cap, probe);
        return false;
    }
    present = ui != nullptr;
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

    LuaDialogIdsV119 ids{};
    wchar_t probe[256]{};
    if (!ReadLuaDialogIdsV119(ids, probe, _countof(probe))) {
        SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V119 PROBE FAIL • ");
        Append(s.healDialogDetail, _countof(s.healDialogDetail), probe);
        SetText(detail, cap, s.healDialogDetail);
        return true;
    }

    if (ids.treatment > 0) s.healDialogMask |= HealDialogTreatment;
    if (ids.confirm > 0 || messageBoxPresent) s.healDialogMask |= HealDialogConfirm;
    if (ids.know > 0) s.healDialogMask |= HealDialogKnow;

    SetText(s.healDialogDetail, _countof(s.healDialogDetail), L"LUA_DIALOG_V119 • GameDialog=");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), gameDialogPresent ? L"present" : L"absent");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • MessageBox=");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), messageBoxPresent ? L"present" : L"absent");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • T="); AppendInt(s.healDialogDetail, _countof(s.healDialogDetail), ids.treatment);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • C="); AppendInt(s.healDialogDetail, _countof(s.healDialogDetail), ids.confirm);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • K="); AppendInt(s.healDialogDetail, _countof(s.healDialogDetail), ids.know);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L" • raw={");
    Append(s.healDialogDetail, _countof(s.healDialogDetail), ids.raw);
    Append(s.healDialogDetail, _countof(s.healDialogDetail), L"}");
    SetText(detail, cap, s.healDialogDetail);
    return true;
}

} // namespace

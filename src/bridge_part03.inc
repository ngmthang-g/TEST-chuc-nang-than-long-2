    const MethodInfo* getLeader = ExactMethod(c.shared, "get_LeaderRoleData", 0, true);
    if (!getLeader || !InvokeObject(getLeader, nullptr, leader, detail, cap) || !leader) {
        SetText(detail, cap, L"LeaderRoleData chưa sẵn sàng"); return false;
    }
    leaderClass = g_api.object_get_class(leader);
    if (!leaderClass) { SetText(detail, cap, L"Không lấy được class LeaderRoleData"); return false; }
    return true;
}

bool ReadPosition(Il2CppObject* leader, Il2CppClass* leaderClass, int& x, int& y,
                  wchar_t* detail, std::size_t cap) {
    if (ScalarGetter(leaderClass, "get_PosX", leader, x, detail, cap) &&
        ScalarGetter(leaderClass, "get_PosY", leader, y, detail, cap)) return true;
    FieldInfo* field = nullptr;
    for (Il2CppClass* c = leaderClass; c; c = g_api.class_get_parent(c)) {
        field = g_api.class_get_field_from_name(c, "roleData");
        if (field) break;
    }
    if (!field) { SetText(detail, cap, L"Không resolve được PosX/PosY"); return false; }
    const Il2CppType* ft = g_api.field_get_type(field);
    Il2CppClass* fc = ft ? g_api.class_from_type(ft) : nullptr;
    if (!fc || g_api.class_is_valuetype(fc)) { SetText(detail, cap, L"roleData backing không hợp lệ"); return false; }
    Il2CppObject* backing = nullptr;
    g_api.field_get_value(leader, field, &backing);
    if (!backing) { SetText(detail, cap, L"roleData backing=null"); return false; }
    Il2CppClass* bc = g_api.object_get_class(backing);
    return bc && ScalarGetter(bc, "get_PosX", backing, x, detail, cap) &&
                 ScalarGetter(bc, "get_PosY", backing, y, detail, cap);
}

bool AutoPathInstance(const Classes& c, Il2CppObject*& instance, Il2CppClass*& actual,
                      wchar_t* detail, std::size_t cap) {
    const MethodInfo* getInstance = ExactMethod(c.autoPath, "get_Instance", 0, true);
    if (!getInstance || !InvokeObject(getInstance, nullptr, instance, detail, cap) || !instance) {
        SetText(detail, cap, L"AutoPathManager.Instance chưa sẵn sàng"); return false;
    }
    actual = g_api.object_get_class(instance);
    if (!actual) { SetText(detail, cap, L"Không lấy được class AutoPathManager"); return false; }
    return true;
}

bool GetListItem(Il2CppObject* list, int index, Il2CppObject*& item, wchar_t* detail, std::size_t cap) {
    item = nullptr;
    if (!list) return false;
    Il2CppClass* lc = g_api.object_get_class(list);
    if (!lc) return false;
    const MethodInfo* getItem = ExactMethod(lc, "get_Item", 1, false, "System.Int32");
    if (!getItem) return false;
    std::int32_t i = index;
    void* args[] = { &i };
    return InvokeObjectArgs(getItem, list, args, item, detail, cap);
}

bool GetListCount(Il2CppObject* list, int& count, wchar_t* detail, std::size_t cap) {
    count = 0;
    if (!list) return false;
    Il2CppClass* lc = g_api.object_get_class(list);
    if (!lc) return false;
    std::int64_t v = 0;
    const MethodInfo* getCount = FindMethod(lc, "get_Count", 0);
    if (!getCount || !InvokeScalar(getCount, list, v, detail, cap) || v < 0 || v > 4096) return false;
    count = static_cast<int>(v);
    return true;
}

bool NormalizeUiRoot(Il2CppObject* candidate, Il2CppObject*& root, wchar_t* detail, std::size_t cap) {
    root = nullptr;
    if (!candidate) return true;
    Il2CppClass* cc = g_api.object_get_class(candidate);
    if (!cc) return false;
    if (FindMethod(cc, "get_CoreChildren", 0)) { root = candidate; return true; }
    const MethodInfo* getRoot = FindMethod(cc, "get_UIRoot", 0);
    if (getRoot) return InvokeObject(getRoot, candidate, root, detail, cap);
    return true;
}

bool FindUiRootViaPublicApi(const Classes& c, const wchar_t* uiName, Il2CppObject*& root,
                            wchar_t* detail, std::size_t cap) {
    root = nullptr;
    if (!c.guiApi) return true;
    const MethodInfo* find = ExactMethod(c.guiApi, "MainFindUI", 1, true, "System.String");
    if (!find) find = ExactMethod(c.guiApi, "FindUI", 1, true, "System.String");
    if (!find) return true;
    Il2CppString* name = NewString(uiName);
    if (!name) return false;
    void* args[] = { name };
    Il2CppObject* candidate = nullptr;
    if (!InvokeObjectArgs(find, nullptr, args, candidate, detail, cap)) return false;
    return NormalizeUiRoot(candidate, root, detail, cap);
}

bool FindUiRootViaExecutor(const Classes& c, const wchar_t* uiName, Il2CppObject*& root,
                           wchar_t* detail, std::size_t cap) {
    root = nullptr;
    if (!c.executor) return true;
    Il2CppObject* executor = nullptr;
    const MethodInfo* getInstance = FindMethod(c.executor, "get_Instance", 0);
    if (getInstance && StaticMethod(getInstance)) {
        if (!InvokeObject(getInstance, nullptr, executor, detail, cap)) return false;
    }
    const MethodInfo* findRoots = FindMethod(c.executor, "FindScriptUIRoots", 1);
    if (!findRoots) return true;
    void* instance = StaticMethod(findRoots) ? nullptr : executor;
    if (!StaticMethod(findRoots) && !instance) return true;
    Il2CppString* name = NewString(uiName);
    if (!name) return false;
    void* args[] = { name };
    Il2CppObject* roots = nullptr;
    if (!InvokeObjectArgs(findRoots, instance, args, roots, detail, cap) || !roots) return true;
    int count = 0;
    if (!GetListCount(roots, count, detail, cap) || count <= 0) return true;
    Il2CppObject* candidate = nullptr;
    if (!GetListItem(roots, 0, candidate, detail, cap)) return false;
    return NormalizeUiRoot(candidate, root, detail, cap);
}

bool FindUiRoot(const Classes& c, const wchar_t* uiName, Il2CppObject*& root,
                wchar_t* detail, std::size_t cap) {
    root = nullptr;
    wchar_t ignored[128]{};
    if (FindUiRootViaPublicApi(c, uiName, root, ignored, _countof(ignored)) && root) return true;
    if (FindUiRootViaExecutor(c, uiName, root, detail, cap) && root) return true;
    root = nullptr;
    return true;
}

bool ReadUiText(Il2CppObject* object, wchar_t* out, std::size_t cap) {
    if (!object || !out || cap == 0) return false;
    Il2CppClass* oc = g_api.object_get_class(object);
    if (!oc) return false;
    const MethodInfo* getText = FindMethod(oc, "get_Text", 0);
    if (!getText) return false;
    Il2CppObject* value = nullptr; wchar_t ignored[128]{};
    if (!InvokeObject(getText, object, value, ignored, _countof(ignored)) || !value) return false;

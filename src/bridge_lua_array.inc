// v1.1.3: minimal same-process Object[] support for MonoBehaviourExecutor.ExecuteUIObject.
// This file is included after bridge_part01.inc, where IL2CPP aliases + g_api are available.

struct Il2CppArrayHeaderV113 {
    void* klass;
    void* monitor;
    void* bounds;
    std::uintptr_t maxLength;
};

using Il2CppGetCorlibFnV113 = const Il2CppImage* (__cdecl*)();
using Il2CppArrayNewFnV113 = Il2CppObject* (__cdecl*)(Il2CppClass*, std::uintptr_t);
using Il2CppWriteBarrierFnV113 = void (__cdecl*)(Il2CppObject*, void**, void*);

bool CreateObjectArrayV113(Il2CppObject* first, bool hasFirst, Il2CppObject*& array,
                           wchar_t* detail, std::size_t cap) {
    array = nullptr;
    if (!g_api.module) { SetText(detail, cap, L"IL2CPP module chưa load"); return false; }

    auto getCorlib = reinterpret_cast<Il2CppGetCorlibFnV113>(
        GetProcAddress(g_api.module, "il2cpp_get_corlib"));
    auto arrayNew = reinterpret_cast<Il2CppArrayNewFnV113>(
        GetProcAddress(g_api.module, "il2cpp_array_new"));
    if (!getCorlib || !arrayNew) {
        SetText(detail, cap, L"Thiếu il2cpp_get_corlib/il2cpp_array_new cho Lua args");
        return false;
    }

    const Il2CppImage* corlib = getCorlib();
    Il2CppClass* objectClass = corlib ? g_api.class_from_name(corlib, "System", "Object") : nullptr;
    if (!objectClass) { SetText(detail, cap, L"Không resolve được System.Object"); return false; }

    array = arrayNew(objectClass, hasFirst ? 1u : 0u);
    if (!array) { SetText(detail, cap, L"Không tạo được System.Object[]"); return false; }
    if (!hasFirst) return true;
    if (!first) { SetText(detail, cap, L"Lua arg object=null"); return false; }

    auto** slot = reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(array) + sizeof(Il2CppArrayHeaderV113));
    auto barrier = reinterpret_cast<Il2CppWriteBarrierFnV113>(
        GetProcAddress(g_api.module, "il2cpp_gc_wbarrier_set_field"));
    if (barrier) barrier(array, slot, first);
    else *slot = first;
    return true;
}

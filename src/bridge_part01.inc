#include <windows.h>
#include <cstdint>
#include <cstddef>
#include <climits>
#include "protocol.h"

using namespace cleanroute;

namespace {

using Il2CppDomain = void;
using Il2CppAssembly = void;
using Il2CppImage = void;
using Il2CppClass = void;
using MethodInfo = void;
using FieldInfo = void;
using Il2CppType = void;
using Il2CppObject = void;
using Il2CppString = void;

HANDLE g_mapping = nullptr;
SharedBlock* g_shared = nullptr;

template <typename T>
bool Resolve(HMODULE module, const char* name, T& out) {
    out = nullptr;
    FARPROC p = GetProcAddress(module, name);
    if (!p) return false;
    static_assert(sizeof(p) == sizeof(out), "pointer-size mismatch");
    const unsigned char* s = reinterpret_cast<const unsigned char*>(&p);
    unsigned char* d = reinterpret_cast<unsigned char*>(&out);
    for (std::size_t i = 0; i < sizeof(out); ++i) d[i] = s[i];
    return out != nullptr;
}

bool Eq(const char* a, const char* b) {
    if (!a || !b) return false;
    while (*a && *b) { if (*a++ != *b++) return false; }
    return *a == *b;
}

void SetText(wchar_t* out, std::size_t cap, const wchar_t* text) {
    if (!out || cap == 0) return;
    std::size_t i = 0;
    if (text) while (i + 1 < cap && text[i]) { out[i] = text[i]; ++i; }
    out[i] = 0;
}

void Append(wchar_t* out, std::size_t cap, const wchar_t* text) {
    if (!out || !text || cap == 0) return;
    std::size_t n = 0; while (n + 1 < cap && out[n]) ++n;
    std::size_t i = 0; while (n + 1 < cap && text[i]) out[n++] = text[i++];
    out[n] = 0;
}

void AppendInt(wchar_t* out, std::size_t cap, int value) {
    wchar_t tmp[32]{}; wsprintfW(tmp, L"%d", value); Append(out, cap, tmp);
}

struct Api {
    HMODULE module = nullptr;
    Il2CppDomain* (__cdecl* domain_get)() = nullptr;
    const Il2CppAssembly* (__cdecl* domain_assembly_open)(Il2CppDomain*, const char*) = nullptr;
    const Il2CppImage* (__cdecl* assembly_get_image)(const Il2CppAssembly*) = nullptr;
    Il2CppClass* (__cdecl* class_from_name)(const Il2CppImage*, const char*, const char*) = nullptr;
    const MethodInfo* (__cdecl* class_get_method_from_name)(Il2CppClass*, const char*, int) = nullptr;
    Il2CppClass* (__cdecl* class_get_parent)(Il2CppClass*) = nullptr;
    std::uint32_t (__cdecl* method_get_flags)(const MethodInfo*, std::uint32_t*) = nullptr;
    std::uint32_t (__cdecl* method_get_param_count)(const MethodInfo*) = nullptr;
    const Il2CppType* (__cdecl* method_get_param)(const MethodInfo*, std::uint32_t) = nullptr;
    const Il2CppType* (__cdecl* method_get_return_type)(const MethodInfo*) = nullptr;
    char* (__cdecl* type_get_name)(const Il2CppType*) = nullptr;
    void (__cdecl* free_fn)(void*) = nullptr;
    Il2CppObject* (__cdecl* runtime_invoke)(const MethodInfo*, void*, void**, void**) = nullptr;
    void* (__cdecl* object_unbox)(Il2CppObject*) = nullptr;
    Il2CppClass* (__cdecl* object_get_class)(Il2CppObject*) = nullptr;
    FieldInfo* (__cdecl* class_get_field_from_name)(Il2CppClass*, const char*) = nullptr;
    const Il2CppType* (__cdecl* field_get_type)(FieldInfo*) = nullptr;
    void (__cdecl* field_get_value)(Il2CppObject*, FieldInfo*, void*) = nullptr;
    Il2CppClass* (__cdecl* class_from_type)(const Il2CppType*) = nullptr;
    bool (__cdecl* class_is_valuetype)(const Il2CppClass*) = nullptr;
    std::int32_t (__cdecl* string_length)(Il2CppString*) = nullptr;
    const wchar_t* (__cdecl* string_chars)(Il2CppString*) = nullptr;
    Il2CppString* (__cdecl* string_new_utf16)(const wchar_t*, std::int32_t) = nullptr;

    bool Load(wchar_t* detail, std::size_t cap) {
        if (module) return true;
        module = GetModuleHandleW(L"GameAssembly.dll");
        if (!module) { SetText(detail, cap, L"GameAssembly.dll chưa sẵn sàng"); return false; }
#define NEED(symbol) do { if (!Resolve(module, "il2cpp_" #symbol, symbol)) { SetText(detail, cap, L"Thiếu IL2CPP export bắt buộc"); return false; } } while (0)
        NEED(domain_get); NEED(domain_assembly_open); NEED(assembly_get_image); NEED(class_from_name);
        NEED(class_get_method_from_name); NEED(class_get_parent); NEED(method_get_flags);
        NEED(method_get_param_count); NEED(method_get_param); NEED(method_get_return_type);
        NEED(type_get_name); NEED(runtime_invoke); NEED(object_unbox); NEED(object_get_class);
        NEED(class_get_field_from_name); NEED(field_get_type); NEED(field_get_value);
        NEED(class_from_type); NEED(class_is_valuetype); NEED(string_length); NEED(string_chars);
        NEED(string_new_utf16);
#undef NEED
        if (!Resolve(module, "il2cpp_free", free_fn)) { SetText(detail, cap, L"Thiếu il2cpp_free"); return false; }
        return true;
    }
};

Api g_api;

const Il2CppImage* Image() {
    Il2CppDomain* domain = g_api.domain_get ? g_api.domain_get() : nullptr;
    if (!domain) return nullptr;
    const Il2CppAssembly* assembly = g_api.domain_assembly_open(domain, "Assembly-CSharp");
    if (!assembly) assembly = g_api.domain_assembly_open(domain, "Assembly-CSharp.dll");
    return assembly ? g_api.assembly_get_image(assembly) : nullptr;
}

bool StaticMethod(const MethodInfo* method) {
    if (!method) return false;
    constexpr std::uint32_t StaticFlag = 0x0010;
    std::uint32_t iflags = 0;
    return (g_api.method_get_flags(method, &iflags) & StaticFlag) != 0;
}

const MethodInfo* FindMethod(Il2CppClass* klass, const char* name, int argc) {
    for (Il2CppClass* c = klass; c; c = g_api.class_get_parent(c)) {
        if (const MethodInfo* m = g_api.class_get_method_from_name(c, name, argc)) return m;
    }
    return nullptr;
}

bool ParamType(const MethodInfo* m, std::uint32_t index, const char* expected) {
    if (!m || index >= g_api.method_get_param_count(m)) return false;
    const Il2CppType* t = g_api.method_get_param(m, index);
    char* n = t ? g_api.type_get_name(t) : nullptr;
    if (!n) return false;
    bool ok = Eq(n, expected);
    g_api.free_fn(n);
    return ok;
}

const MethodInfo* ExactMethod(Il2CppClass* klass, const char* name, int argc, bool isStatic,
                              const char* p0 = nullptr, const char* p1 = nullptr, const char* p2 = nullptr) {
    const MethodInfo* m = FindMethod(klass, name, argc);
    if (!m || StaticMethod(m) != isStatic) return nullptr;
    if (argc > 0 && p0 && !ParamType(m, 0, p0)) return nullptr;
    if (argc > 1 && p1 && !ParamType(m, 1, p1)) return nullptr;
    if (argc > 2 && p2 && !ParamType(m, 2, p2)) return nullptr;
    return m;
}


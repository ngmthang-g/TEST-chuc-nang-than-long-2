namespace {

enum class MtProofStateV116 : int { Uninitialized = 0, Pending = 1, Verified = 2, Failed = 3 };

using ObjectNewFnV116 = Il2CppObject* (__cdecl*)(Il2CppClass*);
using GcHandleNewFnV116 = std::uint32_t (__cdecl*)(Il2CppObject*, bool);
using GcHandleGetTargetFnV116 = Il2CppObject* (__cdecl*)(std::uint32_t);
using GcHandleFreeFnV116 = void (__cdecl*)(std::uint32_t);
using ThreadCurrentFnV116 = void* (__cdecl*)();
using ActionCtorFnV116 = void (*)(Il2CppObject*, Il2CppObject*, const MethodInfo*, const MethodInfo*);

struct MtExtraApiV116 {
    ObjectNewFnV116 objectNew = nullptr;
    GcHandleNewFnV116 gcNew = nullptr;
    GcHandleGetTargetFnV116 gcGet = nullptr;
    GcHandleFreeFnV116 gcFree = nullptr;
    ThreadCurrentFnV116 threadCurrent = nullptr;
    bool loaded = false;
};

MtExtraApiV116 g_mtApiV116{};
MtProofStateV116 g_mtProofV116 = MtProofStateV116::Uninitialized;
std::uint32_t g_mtProofCtsHandleV116 = 0;
std::uint32_t g_mtProofActionHandleV116 = 0;
DWORD g_mtProofStartedV116 = 0;

bool LoadMtExtraApiV116(wchar_t* detail, std::size_t cap) {
    if (g_mtApiV116.loaded) return true;
    if (!g_api.Load(detail, cap) || !g_api.module) return false;
    if (!Resolve(g_api.module, "il2cpp_object_new", g_mtApiV116.objectNew) ||
        !Resolve(g_api.module, "il2cpp_gchandle_new", g_mtApiV116.gcNew) ||
        !Resolve(g_api.module, "il2cpp_gchandle_get_target", g_mtApiV116.gcGet) ||
        !Resolve(g_api.module, "il2cpp_gchandle_free", g_mtApiV116.gcFree) ||
        !Resolve(g_api.module, "il2cpp_thread_current", g_mtApiV116.threadCurrent)) {
        SetText(detail, cap, L"MAINTHREAD: thiếu IL2CPP object/gchandle/thread export");
        return false;
    }
    g_mtApiV116.loaded = true;
    return true;
}

bool ReturnVoidV116(const MethodInfo* method) {
    if (!method) return false;
    const Il2CppType* rt = g_api.method_get_return_type(method);
    char* n = rt ? g_api.type_get_name(rt) : nullptr;
    if (!n) return false;
    const bool ok = Eq(n, "System.Void");
    g_api.free_fn(n);
    return ok;
}

const Il2CppImage* CorlibImageV116() {
    Il2CppDomain* domain = g_api.domain_get ? g_api.domain_get() : nullptr;
    if (!domain) return nullptr;
    const char* names[] = {"mscorlib", "mscorlib.dll", "System.Private.CoreLib", "System.Private.CoreLib.dll"};
    for (const char* name : names) {
        const Il2CppAssembly* a = g_api.domain_assembly_open(domain, name);
        if (a) {
            const Il2CppImage* image = g_api.assembly_get_image(a);
            if (image) return image;
        }
    }
    return nullptr;
}

Il2CppClass* ActionClassV116(wchar_t* detail, std::size_t cap) {
    const Il2CppImage* corlib = CorlibImageV116();
    if (!corlib) { SetText(detail, cap, L"MAINTHREAD: không resolve corlib image"); return nullptr; }
    Il2CppClass* c = g_api.class_from_name(corlib, "System", "Action");
    if (!c) { SetText(detail, cap, L"MAINTHREAD: không resolve System.Action"); return nullptr; }
    const MethodInfo* ctor = FindMethod(c, ".ctor", 2);
    if (!ctor || StaticMethod(ctor)) { SetText(detail, cap, L"MAINTHREAD: System.Action .ctor(2) mismatch"); return nullptr; }
    return c;
}

bool MainThreadTargetV116(Il2CppObject*& instance, const MethodInfo*& execute,
                          wchar_t* detail, std::size_t cap) {
    instance = nullptr; execute = nullptr;
    const Il2CppImage* image = Image();
    if (!image) { SetText(detail, cap, L"MAINTHREAD: Assembly-CSharp chưa sẵn sàng"); return false; }
    Il2CppClass* c = g_api.class_from_name(image, "FGStudio.Engine.Utilities", "MainThread");
    if (!c) { SetText(detail, cap, L"MAINTHREAD: không resolve FGStudio.Engine.Utilities.MainThread"); return false; }
    const MethodInfo* getInstance = ExactMethod(c, "get_Instance", 0, true);
    execute = ExactMethod(c, "Execute", 1, false, "System.Action");
    if (!getInstance || !execute || !ReturnVoidV116(execute)) {
        SetText(detail, cap, L"MAINTHREAD: get_Instance/Execute(Action) signature mismatch"); return false;
    }
    if (!InvokeObject(getInstance, nullptr, instance, detail, cap) || !instance) {
        SetText(detail, cap, L"MAINTHREAD: Instance=null"); return false;
    }
    return true;
}

ActionCtorFnV116 ActionCtorV116(wchar_t* detail, std::size_t cap) {
    if (!g_api.module) { SetText(detail, cap, L"MAINTHREAD: GameAssembly module=null"); return nullptr; }
    // Frozen-client donor, cross-checked by System.Action metadata before use.
    constexpr std::uintptr_t kActionCtorRva = 0x49F810u;
    return reinterpret_cast<ActionCtorFnV116>(reinterpret_cast<std::uintptr_t>(g_api.module) + kActionCtorRva);
}

void CleanupMtProofV116() {
    if (g_mtApiV116.gcFree) {
        if (g_mtProofActionHandleV116) g_mtApiV116.gcFree(g_mtProofActionHandleV116);
        if (g_mtProofCtsHandleV116) g_mtApiV116.gcFree(g_mtProofCtsHandleV116);
    }
    g_mtProofActionHandleV116 = 0;
    g_mtProofCtsHandleV116 = 0;
}

bool BeginMtProofV116(wchar_t* detail, std::size_t cap) {
    if (!LoadMtExtraApiV116(detail, cap)) return false;
    if (!g_mtApiV116.threadCurrent || !g_mtApiV116.threadCurrent()) {
        SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: hook thread không phải IL2CPP-attached thread");
        g_mtProofV116 = MtProofStateV116::Failed;
        return false;
    }

    const Il2CppImage* corlib = CorlibImageV116();
    if (!corlib) { SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: corlib"); g_mtProofV116 = MtProofStateV116::Failed; return false; }
    Il2CppClass* ctsClass = g_api.class_from_name(corlib, "System.Threading", "CancellationTokenSource");
    if (!ctsClass) { SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: CancellationTokenSource class"); g_mtProofV116 = MtProofStateV116::Failed; return false; }
    const MethodInfo* ctor = FindMethod(ctsClass, ".ctor", 0);
    const MethodInfo* cancel = FindMethod(ctsClass, "Cancel", 0);
    const MethodInfo* getter = FindMethod(ctsClass, "get_IsCancellationRequested", 0);
    if (!ctor || !cancel || !getter || StaticMethod(ctor) || StaticMethod(cancel) || !ReturnVoidV116(cancel)) {
        SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: CTS method signature"); g_mtProofV116 = MtProofStateV116::Failed; return false;
    }

    Il2CppObject* cts = g_mtApiV116.objectNew(ctsClass);
    if (!cts) { SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: CTS alloc"); g_mtProofV116 = MtProofStateV116::Failed; return false; }
    if (!InvokeVoid(ctor, cts, nullptr, detail, cap)) { g_mtProofV116 = MtProofStateV116::Failed; return false; }
    std::int64_t before = 1;
    if (!InvokeScalar(getter, cts, before, detail, cap) || before != 0) {
        SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: CTS initial state != false"); g_mtProofV116 = MtProofStateV116::Failed; return false;
    }

    g_mtProofCtsHandleV116 = g_mtApiV116.gcNew(cts, false);
    if (!g_mtProofCtsHandleV116) { SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: CTS GC handle"); g_mtProofV116 = MtProofStateV116::Failed; return false; }

    Il2CppClass* actionClass = ActionClassV116(detail, cap);
    ActionCtorFnV116 actionCtor = ActionCtorV116(detail, cap);
    if (!actionClass || !actionCtor) { CleanupMtProofV116(); g_mtProofV116 = MtProofStateV116::Failed; return false; }
    Il2CppObject* action = g_mtApiV116.objectNew(actionClass);
    if (!action) { CleanupMtProofV116(); SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: Action alloc"); g_mtProofV116 = MtProofStateV116::Failed; return false; }
    actionCtor(action, cts, cancel, nullptr);
    g_mtProofActionHandleV116 = g_mtApiV116.gcNew(action, false);
    if (!g_mtProofActionHandleV116) { CleanupMtProofV116(); SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: Action GC handle"); g_mtProofV116 = MtProofStateV116::Failed; return false; }

    Il2CppObject* mt = nullptr; const MethodInfo* execute = nullptr;
    if (!MainThreadTargetV116(mt, execute, detail, cap)) { CleanupMtProofV116(); g_mtProofV116 = MtProofStateV116::Failed; return false; }
    void* args[] = { action };
    if (!InvokeVoid(execute, mt, args, detail, cap)) { CleanupMtProofV116(); g_mtProofV116 = MtProofStateV116::Failed; return false; }

    g_mtProofV116 = MtProofStateV116::Pending;
    g_mtProofStartedV116 = GetTickCount();
    SetText(detail, cap, L"MAINTHREAD_PROOF PENDING: CTS.Cancel Action đã enqueue; chờ Unity Update");
    return true;
}

bool PollMtProofV116(wchar_t* detail, std::size_t cap) {
    if (g_mtProofV116 == MtProofStateV116::Verified) return true;
    if (g_mtProofV116 == MtProofStateV116::Failed) { SetText(detail, cap, L"MAINTHREAD_PROOF FAILED trước đó"); return false; }
    if (g_mtProofV116 == MtProofStateV116::Uninitialized) {
        if (!BeginMtProofV116(detail, cap)) return false;
        return false;
    }
    if (!g_mtProofCtsHandleV116 || !g_mtApiV116.gcGet) {
        CleanupMtProofV116(); g_mtProofV116 = MtProofStateV116::Failed;
        SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: mất CTS root"); return false;
    }
    Il2CppObject* cts = g_mtApiV116.gcGet(g_mtProofCtsHandleV116);
    if (!cts) {
        CleanupMtProofV116(); g_mtProofV116 = MtProofStateV116::Failed;
        SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: CTS target=null"); return false;
    }
    Il2CppClass* cc = g_api.object_get_class(cts);
    const MethodInfo* getter = cc ? FindMethod(cc, "get_IsCancellationRequested", 0) : nullptr;
    std::int64_t value = 0;
    if (!getter || !InvokeScalar(getter, cts, value, detail, cap)) return false;
    if (value != 0) {
        CleanupMtProofV116();
        g_mtProofV116 = MtProofStateV116::Verified;
        SetText(detail, cap, L"MAINTHREAD_PROOF PASS: CTS false->true qua MainThread.Execute/Unity Update");
        return true;
    }
    if (GetTickCount() - g_mtProofStartedV116 > 5000) {
        CleanupMtProofV116(); g_mtProofV116 = MtProofStateV116::Failed;
        SetText(detail, cap, L"MAINTHREAD_PROOF FAIL: callback timeout 5000ms");
        return false;
    }
    SetText(detail, cap, L"MAINTHREAD_PROOF PENDING: chưa thấy CTS callback");
    return false;
}

bool QueueLiveButtonV116(const Classes& c, const wchar_t* uiName, const wchar_t* label,
                         wchar_t* detail, std::size_t cap) {
    if (!PollMtProofV116(detail, cap)) return false;
    if (!LoadMtExtraApiV116(detail, cap)) return false;
    Il2CppObject* button = nullptr;
    if (!FindButtonInUi(c, uiName, label, button, detail, cap) || !button) {
        SetText(detail, cap, L"MAINTHREAD: không thấy button sống "); Append(detail, cap, label); return false;
    }
    Il2CppClass* bc = g_api.object_get_class(button);
    const MethodInfo* click = bc ? FindMethod(bc, "HandleClickEvent", 0) : nullptr;
    if (!click || StaticMethod(click) || !ReturnVoidV116(click)) {
        SetText(detail, cap, L"MAINTHREAD: HandleClickEvent() signature mismatch"); return false;
    }
    Il2CppClass* actionClass = ActionClassV116(detail, cap);
    ActionCtorFnV116 actionCtor = ActionCtorV116(detail, cap);
    if (!actionClass || !actionCtor) return false;
    Il2CppObject* action = g_mtApiV116.objectNew(actionClass);
    if (!action) { SetText(detail, cap, L"MAINTHREAD: Action alloc fail"); return false; }
    actionCtor(action, button, click, nullptr);
    const std::uint32_t actionHandle = g_mtApiV116.gcNew(action, false);
    if (!actionHandle) { SetText(detail, cap, L"MAINTHREAD: Action GC handle fail"); return false; }
    Il2CppObject* mt = nullptr; const MethodInfo* execute = nullptr;
    if (!MainThreadTargetV116(mt, execute, detail, cap)) { g_mtApiV116.gcFree(actionHandle); return false; }
    void* args[] = { action };
    if (!InvokeVoid(execute, mt, args, detail, cap)) { g_mtApiV116.gcFree(actionHandle); return false; }
    // Execute() synchronously enqueues into a managed ConcurrentQueue<Action>; the queue now owns a strong reference.
    g_mtApiV116.gcFree(actionHandle);
    SetText(detail, cap, L"MAINTHREAD ENQUEUED UIButton.HandleClickEvent • UI="); Append(detail, cap, uiName);
    Append(detail, cap, L" • label="); Append(detail, cap, label);
    return true;
}

bool ClickHealDialogChoice(cleanroute::HealDialogChoice choice, wchar_t* detail, std::size_t cap) {
    Classes c{};
    if (!ResolveClasses(c, detail, cap) || !SafeForAction(c, detail, cap)) return false;

    if (choice == cleanroute::HealDialogChoice::Treatment)
        return QueueLiveButtonV116(c, L"GameDialog", L"Trị liệu", detail, cap);
    if (choice == cleanroute::HealDialogChoice::Know)
        return QueueLiveButtonV116(c, L"GameDialog", L"Ta biết rồi", detail, cap);
    if (choice == cleanroute::HealDialogChoice::Confirm) {
        Il2CppObject* probe = nullptr;
        wchar_t ignored[128]{};
        if (FindButtonInUi(c, L"MessageBox", L"Xác nhận", probe, ignored, _countof(ignored)) && probe)
            return QueueLiveButtonV116(c, L"MessageBox", L"Xác nhận", detail, cap);
        return QueueLiveButtonV116(c, L"GameDialog", L"Xác nhận", detail, cap);
    }
    SetText(detail, cap, L"HealDialogChoice không hợp lệ");
    return false;
}

} // namespace

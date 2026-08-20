from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def read(path: str) -> str:
    return (ROOT / path).read_text(encoding="utf-8")


version = read("VERSION.txt").strip()
controller = read("src/controller.cpp")
bridge = read("src/bridge.cpp")
protocol = read("src/protocol.h")
scoring = read("src/background_ui_logic.h")
tests = read("src/background_ui_logic_test.cpp")
trade_logic = read("src/trade_coordinator_logic.h")
trade_tests = read("src/trade_coordinator_logic_test.cpp")
cmake = read("CMakeLists.txt")
workflow = read(".github/workflows/build.yml")
knowledge = read("PROJECT_KNOWLEDGE.md")
changelog = read("CHANGELOG.md")
readme = read("README.md")

assert version == "v0.6.2"
assert "Thần Long Item Consolidator v0.6.2" in controller
assert "project(ThanLongItemConsolidator VERSION 0.6.2" in cmake
assert "kProtocolVersion = 0x00010602u" in protocol
assert "verify_v062_logic.py" in workflow
assert "ThanLongItemConsolidator_v0.6.2.exe" in workflow
assert "ThanLongItemConsolidator-v0.6.2-win-x64" in workflow

# Keep the v0.5 message-thread bridge and single in-flight boundary.
assert "CreateRemoteThread" not in controller
assert "CreateRemoteThread" not in bridge
assert "WH_GETMESSAGE" in controller
assert "callbackThreadId != g_shared->targetWindowThreadId" in bridge
assert "Bridge còn bận sau timeout; không gửi chồng request" in controller

# Runtime-passed P1/P2 guards must remain intact.
assert "Role::ConfirmMap" in bridge
assert 'ScalarGetter(leaderClass, "get_IsDeath"' in bridge
assert 'ExactMethod(control.klass, "HandleClickEvent", 0, false)' in bridge
assert "Có nhiều control cùng điểm; fail-closed" in bridge

# BUG-002: layered AUTO lookup + staged, non-blocking semantic fallback.
assert 'InvokeLuaAction("TopIcon", start ? "AutoTrainClick" : "AutoStopClick"' in bridge
assert "FindUiObjectInRegistry" in bridge
assert "UIObject.instances không có UI active đúng tên" in bridge
assert "Role::AutoRoot" in bridge
assert "Role::AutoTrain" in bridge
assert "Role::AutoStop" in bridge
assert "ActionResult::MenuOpened" in bridge
assert "g_shared->request.arg0 != 0" in bridge
assert "kPriorityAutoMenuWaitMs = 650" in controller
assert "priorityAutoMenuOpenedTick" in controller
assert "menuAlreadyOpened ? 1 : 0" in controller
assert "Sleep(kPriorityAutoMenuWaitMs)" in controller  # manual TEST only
priority_start = controller.index("bool PriorityAutoClick")
priority_end = controller.index("bool RunPriorityAutoPass", priority_start)
priority = controller[priority_start:priority_end]
assert "Sleep(" not in priority
assert "SetCursorPos(" not in priority and "SendInput(" not in priority
for role in ("AutoRoot", "AutoTrain", "AutoStop"):
    assert role in scoring
assert "unrelatedStop" in tests

# BUG-004: preserve v0.5 pause semantics and add one shared-latch delivery fallback.
pause_start = controller.index("void ToggleGlobalPause()")
pause_end = controller.index("void PersistSelectedEditorSafeBeforeSwitch", pause_start)
pause = controller[pause_start:pause_end]
for invariant in (
    "globalPaused_ = !globalPaused_",
    "Command::StopPath",
    'a.runtime.status = L"TẠM DỪNG F4"',
    'item->runtime.status = L"Tiếp tục sau F4"',
    "HandlePauseHotkeyPress",
    "GetAsyncKeyState(VK_F4)",
    "pauseKeyLatched_",
):
    assert invariant in pause
assert "RegisterHotKey(hwnd_, kPauseHotkeyId, MOD_NOREPEAT, VK_F4)" in controller
assert "if (pauseHotkeyRegistered_) UnregisterHotKey" in controller

# BUG-003: sell safety ceiling/no-progress are not completion and raw pointer order is
# only the final deterministic tie-break after semantic/natural ordering.
assert "SafetyLimit = 7" in protocol
assert "NoProgress = 8" in protocol
assert "BagItemScore" in scoring and "NaturalItemIndex" in scoring
collect_start = bridge.index("bool CollectSafeBagItems")
collect_end = bridge.index("bool SellNextBagItem", collect_start)
collect = bridge[collect_start:collect_end]
assert collect.index("BagItemScore") < collect.index("reinterpret_cast<std::uintptr_t>")
assert collect.index("NaturalItemIndex") < collect.index("reinterpret_cast<std::uintptr_t>")
assert "ActionResult::SafetyLimit" in bridge
assert "ActionResult::NoProgress" in bridge
assert "không coi là đã hết item" in bridge
assert "sellInitialFreeBag" in controller
assert "sellVerifiedSold" in controller
assert "sellExhausted" in controller
assert "HasVerifiedSellCompletion(rt.sellInitialFreeBag" in controller
assert "currentFreeBagSpace > initialFreeBagSpace" in trade_logic
assert "verifiedSold > 0 && exhausted" in trade_logic
assert "HasVerifiedSellCompletion(0, 1, 1, true)" in trade_tests
phase10_start = controller.index("if (rt.sellPhase == 10)")
phase10_end = controller.index("return true;", phase10_start) + len("return true;")
phase10 = controller[phase10_start:phase10_end]
assert "sellPhase = 8" not in phase10
assert "freeBagSpace > 0" not in phase10

# Durable evidence must distinguish runtime pass/fail instead of flattening v0.6.1.
for path in (
    "docs/bugs/BUG_REGISTRY.md",
    "docs/evidence/EVIDENCE_REGISTRY.md",
    "docs/decisions/DECISIONS.md",
    "docs/features/BACKGROUND_UI_ACTIONS.md",
    "docs/history/VERSION_v0.6.2.md",
):
    assert (ROOT / path).is_file(), path
assert "v0.6.1 XN Lâu Lan | RUNTIME PASS" in knowledge
assert "v0.6.1 Đầu thai | RUNTIME PASS" in knowledge
assert "v0.6.1 AUTO | RUNTIME FAIL" in knowledge
assert "v0.6.2 | BUILD PASS / RUNTIME UNTESTED" in knowledge
assert "Windows CI run 280" in knowledge and "17d5aa7" in knowledge
assert "F4 is USER-REPORTED FAIL" in changelog
assert "v0.6.2" in readme

# Preserve already published historical packages.
assert (ROOT / "release/ThanLongItemConsolidator-v0.6-win-x64.zip").is_file()
assert (ROOT / "release/ThanLongItemConsolidator-v0.6.1-win-x64.zip").is_file()

print("v0.6.2 AUTO/F4/sell fail-closed audit PASS")

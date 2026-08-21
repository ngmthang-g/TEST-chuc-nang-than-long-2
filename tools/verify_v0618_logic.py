#!/usr/bin/env python3
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

def r(path):
    return (ROOT / path).read_text(encoding="utf-8")

def req(condition, message):
    if not condition:
        raise SystemExit("FAIL: " + message)

def main():
    c = r("src/controller.cpp")
    g = r("src/travel_fight_guard_logic.h")
    gt = r("src/travel_fight_guard_logic_test.cpp")
    h = r("src/internal_ui_click_logic.h")
    it = r("src/internal_ui_click_logic_test.cpp")
    p = r("src/protocol.h")
    w = r(".github/workflows/build.yml")

    req(r("VERSION.txt").strip() == "v0.6.1.8", "version")
    req("kProtocolVersion = 0x00010618u" in p, "protocol")
    req("ThanLongItemConsolidator-v0.6.1.8-win-x64" in w and
        "verify_v0618_logic.py" in w, "workflow")

    # Preserve the runtime-successful v0.6.1.7 hidden AUTO menu lifecycle.
    req("AutoMenuChoicePlan" in h and "AutoMenuChoicePoint::Choice" in h,
        "generic AUTO menu choice plan missing")
    req("requestedSlot == ClickSlot::Attack || requestedSlot == ClickSlot::StopAuto2" in c,
        "StopAuto2 not kept as two-phase menu sequence")
    req("? ClickSlot::AutoMenu : requestedSlot" in c,
        "phase 1 does not open AUTO before choice")
    req("click 2/2: DỪNG AUTO 2" in c,
        "Stop completion is not after second click")
    req("AutoMenuChoicePlan" in it, "native menu-choice test missing")

    # Approved item 2: cross-map ownership survives intermediate maps, but must be
    # released at the physical final destination so P3 Train is not blocked forever.
    req("IsPhysicalRouteCompletion" in g and "IsPhysicalRouteCompletion" in gt,
        "physical route-completion invariant missing")
    req("CompleteToolOwnedRoute" in c, "route completion cleanup missing")
    req(c.count("CompleteToolOwnedRoute(rt, true") >= 2,
        "route completion cleanup not wired to normal + robust arrival")
    req("if (keepToolOwnedCrossMapRoute)" in c and "rt.crossMapRouteMoved = true" in c,
        "intermediate-map route ownership preservation missing")

    # Approved item 5: Map 87 guard is only proactive for a real destination outside M87.
    req("ShouldGuardUnderworldExit" in g and "ShouldGuardUnderworldExit" in gt,
        "M87 exit-scope invariant missing")
    req("CurrentTravelDestinationMap" in c and "ShouldGuardUnderworldExit(" in c,
        "M87 guard not scoped by active travel destination")

    # Approved item 7: known dead helpers/fields are fully removed.
    dead_tokens = (
        "ActiveSellClickSequenceAccount",
        "ClickSlotNow",
        "RunPriorityAutoPass",
        "RunPriorityLauLanGateConfirmPass",
        "RunPriorityRevivePass",
        "suppressRouteSinceTick",
        "trainRecoveryTick",
        "priorityAutoReason",
        "sellTriggeredByFullBag",
    )
    for token in dead_tokens:
        req(token not in c, "dead code/state remains: " + token)

    # Approved item 8: tradeTxn_ + tradeHeld are the only business serialization layer.
    for token in ("AcquireTradeWorkflowLock", "ReleaseTradeWorkflowLock", "ReleaseTradeWorkflowLockCore"):
        req(token not in c, "no-op trade lock remains: " + token)
    req("tradeTxn_ itself is the only business serialization primitive" in c,
        "tradeTxn-only serialization contract missing")

    # Never regress to physical mouse ownership while cleaning.
    for token in ("SetCursorPos", "SendInput", "WH_MOUSE_LL", "SetForegroundWindow"):
        req(token not in c, "physical input regression: " + token)

    print("PASS: v0.6.1.8 scoped route/M87/dead-code/trade-lock maintenance audit")

if __name__ == "__main__":
    main()

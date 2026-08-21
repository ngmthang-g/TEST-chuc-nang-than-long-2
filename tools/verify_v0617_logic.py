#!/usr/bin/env python3
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def r(p): return (ROOT/p).read_text(encoding="utf-8")
def req(c,m):
    if not c: raise SystemExit("FAIL: "+m)
def main():
    c=r("src/controller.cpp"); h=r("src/internal_ui_click_logic.h"); t=r("src/internal_ui_click_logic_test.cpp")
    p=r("src/protocol.h"); w=r(".github/workflows/build.yml")
    req(r("VERSION.txt").strip()=="v0.6.1.7","version")
    req("AutoMenuChoicePlan" in h and "AutoMenuChoicePoint::Choice" in h,"generic AUTO menu choice plan missing")
    req("requestedSlot == ClickSlot::Attack || requestedSlot == ClickSlot::StopAuto2" in c,"StopAuto2 not promoted to two-phase menu sequence")
    req("? ClickSlot::AutoMenu : requestedSlot" in c,"phase 1 does not open AUTO before choice")
    req("click 2/2: DỪNG AUTO 2" in c,"Stop completion is not after second click")
    req("AutoMenuChoicePlan" in t,"native menu-choice test missing")
    req("QueuePriorityAutoClick(a, ClickSlot::StopAuto2" in c,"Travel Guard no longer requests stop")
    req("kProtocolVersion = 0x00010617u" in p,"protocol")
    req("ThanLongItemConsolidator-v0.6.1.7-win-x64" in w and "verify_v0617_logic.py" in w,"workflow")
    for tok in ("SetCursorPos","SendInput","WH_MOUSE_LL","SetForegroundWindow"):
        req(tok not in c,"physical input regression: "+tok)
    print("PASS: v0.6.1.7 AUTO->StopAuto2 hidden menu lifecycle audit")
if __name__=="__main__": main()

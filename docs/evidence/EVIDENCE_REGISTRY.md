# EVIDENCE REGISTRY

## EVID-001 — NPC 339 opens but Treatment action does not advance

**Type:** USER_RUNTIME + SCREENSHOT/OBSERVATION  
**Date / Version:** 2026-08-16 / v1.1.2–v1.1.4 series  
**Source:** user live client test, Đỗ Thanh Đằng / Lâu Lan  
**Observation:** route and `ClickNPC(339)` open the intended dialog; Treatment action produces visible flicker but no transition to confirmation/service completion.  
**Supports:** route/NPC-open works; decisive Treatment action path fails.  
**Does NOT Prove:** exact root cause, packet correctness, or thread boundary by itself.  
**Confidence:** CONFIRMED runtime behavior.

## EVID-002 — Different NPC/map reproduces same Treatment failure

**Type:** USER_RUNTIME  
**Date / Version:** 2026-08-16 / v1.1.5-test  
**Source:** user live test, Long Phá Thiên NPC 463 / Lạc Dương Map 3  
**Observation:** tool again opens the NPC/dialog and the Treatment screen/dialog flickers, with no Treatment progression, matching the symptom at NPC 339/Lâu Lan.  
**Supports:** NPC 339/Lâu Lan is not a sufficient sole explanation; a common action/integration layer is more plausible.  
**Does NOT Prove:** that NPC 463 is a canonical healer family or the exact shared root cause.  
**Confidence:** CONFIRMED runtime symptom; inference about shared root cause is LIKELY.

## EVID-003 — Canonical MainThread dispatcher contract

**Type:** SOURCE / REVERSE_ENGINEERING (canonical client KB)  
**Date / Version:** 2026-08-16 / used for v1.1.6  
**Source:** canonical client `contexts/BUILD_MAINTHREAD_BRIDGE.md`, `contracts/MAINTHREAD_BRIDGE_V1.md`, `analysis/30_EXTERNAL_ACTION_BRIDGE_BLUEPRINT.md`  
**Observation:** `MainThread.Execute(System.Action)` enqueues managed Action; normal Unity `Update()` drains and invokes it. Current-style WH_GETMESSAGE hook may be a valid producer context, but mutable callbacks should be queued and the hook must return before waiting for callback proof.  
**Supports:** v1.1.6 experiment moving the broken UI event from direct/re-entrant hook invocation to queued Unity Update execution.  
**Does NOT Prove:** that this is the actual root cause until runtime Treatment behavior changes.  
**Confidence:** CONFIRMED client mechanism; v1.1.6 causal theory remains LIKELY.

## EVID-004 — GameDialog lifecycle can visually reconstruct/flicker

**Type:** SOURCE (canonical client KB)  
**Date / Version:** 2026-08-16  
**Source:** canonical `database/UI_PACKET_LIFECYCLE.md`  
**Observation:** inbound `CMD_SHOW_GAMEDIALOG` destroys an existing GameDialog, then may call `AutoFight_Main:PutGameDialog(data)` and `GUI.CallUI("GameDialog", data)` to create the next/current dialog.  
**Supports:** visible flicker is compatible with dialog reconstruction and is not itself Treatment success proof.  
**Does NOT Prove:** why the server returns/reconstructs the same state in failed tool runs.  
**Confidence:** CONFIRMED source semantics.

# EVIDENCE REGISTRY

## EVID-001 — NPC dialog opens but full Treatment flow does not advance

**Type:** USER_RUNTIME + SCREENSHOT/OBSERVATION  
**Date / Version:** 2026-08-16 / early v1.1.x series  
**Source:** user live client test, Đỗ Thanh Đằng / Lâu Lan  
**Observation:** route and `ClickNPC(339)` open the intended visible dialog; full auto flow does not progress to confirmed Treatment completion.  
**Supports:** route/NPC-open partial subsystem works; BUG-001 is after NPC interaction.  
**Does NOT Prove:** that every attempted Treatment action layer was actually reached/executed.  
**Confidence:** CONFIRMED runtime behavior.

## EVID-002 — Different NPC/map reproduces same overall failure symptom

**Type:** USER_RUNTIME  
**Date / Version:** 2026-08-16 / v1.1.5-test  
**Source:** user live test, Long Phá Thiên NPC 463 / Lạc Dương Map 3  
**Observation:** NPC/dialog opens and the same visible flicker/no-progression symptom is reproduced.  
**Supports:** NPC 339/Lâu Lan is not a sufficient sole explanation.  
**Does NOT Prove:** exact shared root cause or that NPC 463 is a canonical healer family.  
**Confidence:** CONFIRMED symptom; root-cause inference only LIKELY.

## EVID-003 — Canonical MainThread dispatcher contract

**Type:** SOURCE / REVERSE_ENGINEERING (canonical client KB)  
**Date / Version:** 2026-08-16 / used for v1.1.6  
**Source:** canonical client MainThread docs  
**Observation:** `MainThread.Execute(System.Action)` enqueues managed Action and Unity `Update()` drains/invokes it.  
**Supports:** v1.1.6 action-boundary experiment.  
**Does NOT Prove:** that action boundary is BUG-001 root cause.  
**Confidence:** CONFIRMED mechanism.

## EVID-004 — GameDialog lifecycle may destroy/recreate visible UI

**Type:** SOURCE (canonical client KB)  
**Date / Version:** 2026-08-16  
**Source:** canonical UI/runtime action documentation  
**Observation:** inbound `CMD_SHOW_GAMEDIALOG` destroys current GameDialog, forwards dialog data to AutoFight_Main and creates the next GameDialog UI.  
**Supports:** visible flicker can be reconstruction/reopen and is not success proof.  
**Does NOT Prove:** what caused a particular reconstruction.  
**Confidence:** CONFIRMED source semantics.

## EVID-005 — Original runtime log shows repeated ClickNPC while still waiting for Treatment

**Type:** USER_RUNTIME + SCREENSHOT/LOG + SOURCE CORRELATION  
**Date / Version:** 2026-08-16 / early auto-heal test  
**Source:** user screenshot/log plus current controller source  
**Observation:** after the dialog is visibly open, log contains repeated `Đã gọi ClickNPC npcID=339` while status remains `AUTO TRỊ LIỆU: CHỜ DIALOG TRỊ LIỆU`. Current controller only performs this retry in `WaitTreatment` when `DecideDialogAction` does not see the Treatment bit.  
**Supports:** the observer can fail to recognize a visible GameDialog/Treatment choice, causing the controller itself to reopen the NPC and generate the flicker. This is a direct reason to isolate discovery before redesigning actions again.  
**Does NOT Prove:** whether a second independent action problem remains after discovery is fixed.  
**Confidence:** CONFIRMED repeated retry behavior; discovery-as-root-cause is LIKELY pending v1.1.7 runtime.

## EVID-006 — Shared old discovery assumption

**Type:** SOURCE  
**Date / Version:** 2026-08-16 / audit before v1.1.7  
**Source:** `src/bridge_part04.inc`  
**Observation:** old `WalkForButton()` recognizes a button only if the same object both returns matching `get_Text()` and has an instance `HandleClickEvent()`. The same discovery primitive feeds `InspectHealDialog` and the later action helpers.  
**Supports:** multiple action experiments were not independent of a common discovery dependency. Visible label text held on a descendant/dynamic clone can be missed.  
**Does NOT Prove:** actual runtime hierarchy shape until v1.1.7 reports observed candidates.  
**Confidence:** CONFIRMED implementation fact; runtime impact LIKELY.

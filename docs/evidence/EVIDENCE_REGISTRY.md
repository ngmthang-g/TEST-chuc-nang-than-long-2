# EVIDENCE REGISTRY

## EVID-001 — NPC dialog opens but full Treatment flow does not advance
**Type:** USER_RUNTIME  
Early tests confirm raw route and `ClickNPC(339)` can open the intended dialog; full Treatment has no runtime PASS. This does not prove each historical action layer was reached.

## EVID-002 — Alternate NPC/map reproduces overall symptom
**Type:** USER_RUNTIME / v1.1.5  
NPC 463 / Lạc Dương reproduced flicker/no-progress, weakening an NPC-339-only theory.

## EVID-003 — MainThread dispatcher contract
**Type:** CANONICAL CLIENT SOURCE  
`MainThread.Execute(System.Action)` queues a managed action and Unity Update drains/invokes it. Mechanism VERIFIED; not itself proof of BUG-001 cause.

## EVID-004 — GameDialog is server-driven and rebuilt
**Type:** CANONICAL CLIENT SOURCE  
Inbound GameDialog processing can destroy current UI and create the next dialog while forwarding data to AutoFight_Main. Visual flicker/rebuild is not success proof.

## EVID-005 — Historical repeated ClickNPC while waiting for Treatment
**Type:** USER_RUNTIME + SOURCE CORRELATION  
Old logs showed repeated NPC interactions while waiting for Treatment. This established a real observer/retry coupling bug path.

## EVID-006 — Old same-object discovery assumption
**Type:** SOURCE  
Old button walker required matching text and click handler on the same object. Multiple historical action experiments shared this observer dependency.

## EVID-007 — Pre-v1.1.8 user flicker report
**Type:** USER_RUNTIME  
User reported current tool still flickered before the build-valid v1.1.8 handoff; exact older artifact lineage was unknown.

## EVID-008 — v1.1.7 source/CI gaps
**Type:** SOURCE + CI  
`FirstTextInSubtreeV117` returned after first text, WaitTreatment retained transient-absence retry, and final v1.1.7 run `31925922772` failed controller compilation.

## EVID-009 — v1.1.8 build-valid artifact
**Type:** CI / BUILD  
Source run `31926671467` and final-head run `31926929828` passed architecture audit, self-tests, bridge/controller compilation and artifact upload.

## EVID-010 — v1.1.8 runtime isolates failure to UIRoot observer representation
**Type:** USER_RUNTIME + EXACT LOG  
**Date:** 2026-08-16  
**Artifact:** delivered v1.1.8  
**Context:** PID 4324, NPC 339, Map 5, captured coordinate `9442,5510`.

Observed:
- mount / AutoPath / dismount succeeded;
- exactly one `Đã gọi ClickNPC npcID=339`;
- current GameDialog opened;
- every probe for ~15 seconds returned `DIALOG_V118 NO MATCH • UI=GameDialog • wanted=Trị liệu • clickable=0 • texts=0 • labels=<none>`;
- controller failed closed with no second NPC interaction.

Supports:
1. WaitTreatment anti-reopen is runtime PASS for this transaction;
2. bridge-visible `UIRoot/CoreChildren` has zero relevant text/clickable nodes for the live dynamic GameDialog;
3. v1.1.8 never reaches Treatment selectionID/action, so this test does not disprove the action/server request.

Confidence: **CONFIRMED runtime evidence**.

## EVID-011 — Runtime `Selections` is the stronger canonical source
**Type:** CANONICAL CLIENT SOURCE/KB  
Relevant Auto Heal/GameDialog docs establish `Selections[selectionID]=visibleText`; built-in AutoFight dialog logic inspects current selections; command `100007` submits `selectionID:SelectedItemID`. This supports moving v1.1.9 observation from UIRoot to live Lua dialog data. Exact live table path remains runtime evidence.

## EVID-012 — v1.1.9 source builds and packages successfully
**Type:** CI / BUILD ARTIFACT  
**Date / Version:** 2026-08-16 / v1.1.9-test  
**Source commit:** `1eac3b9eb55dae9a80d6fcba847c7bd7281fe3b7`  
**Actions run:** `31932086373`  
**Observation:** architecture audit PASS; Route FSM PASS; Heal FSM PASS; bridge DLL + PE verification PASS; controller EXE PASS; artifact upload PASS.  
**Artifact:** `ThanLongTestAutoHeal-v1.1.9`, ID `9259620117`.  
**ZIP SHA-256:** `823f04f60fad78720f3742d7e93a7c31b6e382f504fe44982c41cf9a967d648a`.  
**EXE SHA-256:** `3228bf03c493329af00c8014b47c9f486469a39b24b2c09add5df2061a450ad8`.  
**DLL SHA-256:** `22019c4c9f4f4aaf29db3c4c328d9a948d285f41186698c981335cc039df7ea9`.  
**Supports:** v1.1.9 source is build-valid and ready for live Lua-dialog probing.  
**Does NOT Prove:** that LuaEnv probe finds live selections, that Treatment is accepted, or that the full heal flow completes.  
**Confidence:** CONFIRMED BUILD; **RUNTIME UNTESTED**.

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
Source/final v1.1.8 runs passed architecture audit, self-tests, bridge/controller compilation and artifact upload.

## EVID-010 — v1.1.8 runtime isolates UIRoot representation failure
**Type:** USER_RUNTIME + EXACT LOG  
**Date:** 2026-08-16  
GameDialog opens after one NPC interaction; for ~15 seconds every probe returns `DIALOG_V118 NO MATCH ... clickable=0 • texts=0 • labels=<none>`; controller fails closed without another NPC interaction. This confirms anti-reopen for the transaction and confirms the bridge-visible UIRoot representation is empty for the relevant dynamic dialog nodes.

## EVID-011 — Runtime `Selections` is the stronger canonical source
**Type:** CANONICAL CLIENT SOURCE/KB  
GameDialog docs establish `Selections[selectionID]=visibleText`; built-in AutoFight dialog logic inspects current selections; command `100007` submits `selectionID:SelectedItemID`.

## EVID-012 — v1.1.9 source builds and packages successfully
**Type:** CI / BUILD ARTIFACT  
Source commit `1eac3b9eb55dae9a80d6fcba847c7bd7281fe3b7`, run `31932086373`: architecture audit, FSM tests, bridge/controller build and upload PASS. Artifact `ThanLongTestAutoHeal-v1.1.9`, ID `9259620117`. This proved build validity only.

## EVID-013 — v1.1.9 runtime fails specifically at LuaSystemManager instance resolution
**Type:** USER_RUNTIME + SCREENSHOT/EXACT LOG + SOURCE CORRELATION  
**Date / Version:** 2026-08-16 / delivered v1.1.9  
**Context:** PID 4324, Map 5, NPC 339 Đỗ Thanh Đằng. The captured UI status says the GameDialog is open and it is waiting for live Treatment selection. Log shows `Đã gọi ClickNPC npcID=339` followed by repeated:

`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`

**Supports:** the failure is before LuaEnv/DoString/Selections/action. Source audit confirms v1.1.9 `ResolveLuaEnvV119()` first required `ResolveLuaSystemManagerV119()`, whose only instance paths were `get_Instance` and four named static fields.  
**Does NOT Prove:** DoString failure, missing Treatment text/ID, packet rejection, MessageBox behavior or server refusal.  
**Confidence:** **CONFIRMED runtime boundary + source implementation fact**.

## EVID-014 — Canonical KB does not verify a LuaSystemManager singleton contract
**Type:** CANONICAL CLIENT KB  
`LuaSystemManager` high-value members include `get_LuaEnv/set_LuaEnv`; IL2CPP reflection/static-field APIs are verified. The current canonical docs do not claim a stable `get_Instance` or a particular singleton backing-field name for LuaSystemManager. This supports correcting the resolver rather than declaring Lua data inaccessible.

## EVID-015 — v1.1.10 source-bearing resolver build passes
**Type:** CI / BUILD ARTIFACT  
**Source-bearing commit:** `d35517385266d4fa75011374966816a0e8d5ada1`  
**Actions run:** `31933118883`  
Architecture audit PASS; Route FSM PASS; Heal FSM PASS; bridge DLL + PE verification PASS; controller build PASS; artifact upload PASS.  
Artifact `ThanLongTestAutoHeal-v1.1.10`, ID `9259895908`, ZIP digest `sha256:4b43c205bedd4177288b562ed7df20b08adb5f295b6b1601a11699e0bb80ef60`.  
**Supports:** the new resolver/fallback code compiles/packages correctly.  
**Does NOT Prove:** that the live client resolves LuaEnv, that T is found, or that Treatment completes.  
**Confidence:** CONFIRMED BUILD; **RUNTIME UNTESTED**.
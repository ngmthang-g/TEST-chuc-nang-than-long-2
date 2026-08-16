# EVIDENCE REGISTRY

## EVID-001 — NPC dialog opens but full Treatment flow does not advance
**Type:** USER_RUNTIME  
Early tests confirm raw route and `ClickNPC(339)` can open the intended dialog; full Treatment has no runtime PASS. This does not prove each historical action layer was reached.

## EVID-002 — Alternate NPC/map reproduces overall symptom
**Type:** USER_RUNTIME / v1.1.5  
NPC 463 / Lạc Dương reproduced flicker/no-progress, weakening an NPC-339-only theory.

## EVID-003 — MainThread dispatcher contract
**Type:** CANONICAL CLIENT SOURCE  
`MainThread.Execute(System.Action)` queues a managed action and Unity Update drains/invokes Actions. Mechanism VERIFIED; not itself proof of BUG-001 cause.

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
Log shows one `ClickNPC npcID=339` followed by repeated:
`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`.

**Supports:** failure is before LuaEnv/DoString/Selections/action.  
**Does NOT Prove:** DoString failure, missing Treatment text/ID, packet rejection, MessageBox behavior or server refusal.  
**Confidence:** CONFIRMED runtime boundary + source implementation fact.

## EVID-014 — Canonical KB does not verify a LuaSystemManager singleton contract
**Type:** CANONICAL CLIENT KB  
`LuaSystemManager` high-value members include `get_LuaEnv/set_LuaEnv`; canonical docs did not establish a stable `get_Instance` or particular singleton backing-field name. This supported the v1.1.10 resolver correction.

## EVID-015 — v1.1.10 source/final builds pass
**Type:** CI / BUILD ARTIFACT  
v1.1.10 builds pass architecture audit, FSM tests, bridge/controller compile and artifact upload. BUILD evidence does not establish runtime success.

## EVID-016 — v1.1.10 runtime reaches LuaEnv then fails at old DoString resolver
**Type:** USER_RUNTIME + EXACT LOG + SOURCE CORRELATION  
**Date / Version:** 2026-08-16 / delivered v1.1.10  
Exact user log:
```text
Đã gửi AutoPath tới map=5 x=9454 y=5477
Đã gửi lệnh xuống ngựa
Đã gọi ClickNPC npcID=339
LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)
```
The failure repeats.

**Supports:** `RunLuaChunkV120` calls `ResolveLuaEnvV120` before DoString lookup; therefore manager/LuaEnv resolution is RUNTIME PARTIAL PASS and old DoString resolver is the first confirmed failing stage.  
**Does NOT Prove:** Lua chunk execution, current Selections/Treatment ID, `ACTION_V120`, packet/server result.  
**Confidence:** CONFIRMED runtime boundary + source control-flow correlation.

## EVID-017 — Upstream xLua exposes both String and Byte[] DoString overloads
**Type:** EXTERNAL PRIMARY SOURCE / TARGETED RESEARCH  
**Date / Used by:** 2026-08-16 / v1.1.11 investigation  
Official Tencent xLua `LuaEnv.cs` contains both `DoString(byte[] chunk, string chunkName, LuaTable env)` and `DoString(string chunk, string chunkName, LuaTable env)`.

**Supports:** a robust resolver should inspect live runtime metadata rather than assume only one String-first overload.  
**Does NOT Prove:** that this game client ships exactly the same xLua version or that either overload survived IL2CPP stripping.  
**Confidence:** CONFIRMED upstream source fact; exact-client applicability = guidance only.

## EVID-018 — v1.1.11 source-bearing build and artifact handoff package pass
**Type:** CI / BUILD ARTIFACT + ARTIFACT INSPECTION  
**Date / Version:** 2026-08-16 / v1.1.11-test  
**Source branch commit:** `95f285f929a32c9748342a3480748a5b79d1a4d0`  
**PR Actions run:** `31935080947`  
**Result:** architecture audit PASS; Route FSM PASS; Heal FSM PASS; bridge DLL/PE PASS; controller PASS; handoff/knowledge packaging PASS; artifact upload PASS.  
**Artifact:** `ThanLongTestAutoHeal-v1.1.11`, ID `9260424284`, ZIP digest `sha256:2e085f606c5df0fed5933c3eff13cec0b544d923ea18e94b1892e392cc2cb8ae`.  
**EXE SHA-256:** `ee7ff5aeb66c6e9715d7f73522834ced6d56484dc4933259b441af6d19959764`.  
**DLL SHA-256:** `d30b30757442b9671606f83301509c539eb22ad8da6e9b93638cf24f2a9eff5f`.

Artifact inspection found exactly the intended nine handoff members: EXE, bridge DLL, `AI_PROJECT_HANDOFF_FULL.md`, `AI_START_HERE.md`, V2 protocol, client rules TXT, `PROJECT_KNOWLEDGE.md`, `CHANGELOG.md`, `BUILD_EVIDENCE.txt`.

A packaging-evidence correction is applied before final handoff because PR-event `GITHUB_SHA` identifies the checkout/merge commit. Final `BUILD_EVIDENCE.txt` records branch `SOURCE_HEAD_SHA` separately from `CHECKOUT_SHA`.

**Supports:** v1.1.11 source compiles and the previously missing artifact knowledge bundle is actually present.  
**Does NOT Prove:** any v1.1.11 runtime behavior in the game.  
**Confidence:** CONFIRMED BUILD/PACKAGING; RUNTIME UNTESTED.

# EVIDENCE REGISTRY

## EVID-001 — NPC dialog opens but full Treatment flow does not advance
**Type:** USER_RUNTIME  
Early tests confirm raw route and `ClickNPC(339)` can open intended dialog; full Treatment has no runtime PASS.

## EVID-002 — Alternate NPC/map reproduces overall symptom
**Type:** USER_RUNTIME / v1.1.5  
NPC 463 / Lạc Dương reproduced no-progress symptom.

## EVID-003 — MainThread dispatcher contract
**Type:** CANONICAL CLIENT SOURCE  
`MainThread.Execute(System.Action)` queues a managed action and Unity Update drains/invokes it.

## EVID-004 — GameDialog is server-driven and rebuilt
**Type:** CANONICAL CLIENT SOURCE  
Inbound GameDialog processing may destroy/recreate UI; flicker is not success proof.

## EVID-005 — Historical repeated ClickNPC while waiting for Treatment
**Type:** USER_RUNTIME + SOURCE CORRELATION  
Old logs/source established a real retry/rebuild pollution path.

## EVID-006 — Old same-object discovery assumption
**Type:** SOURCE  
Historical button walker coupled text and click-handler discovery on same object.

## EVID-007 — Pre-v1.1.8 user flicker report
**Type:** USER_RUNTIME  
Flicker/no-progress persisted in older artifacts.

## EVID-008 — v1.1.7 source/CI gaps
**Type:** SOURCE + CI  
First-text-only matching, residual retry and final controller compile failure made v1.1.7 invalid handoff.

## EVID-009 — v1.1.8 build-valid artifact
**Type:** CI / BUILD  
Architecture audit, tests and binaries passed.

## EVID-010 — v1.1.8 runtime isolates UIRoot representation failure
**Type:** USER_RUNTIME + EXACT LOG  
After one NPC open, every observer sample reported `clickable=0 • texts=0 • labels=<none>` until fail-closed. Anti-reopen passed for that transaction; UIRoot/CoreChildren representation failed.

## EVID-011 — Runtime Selections is stronger GameDialog source
**Type:** CANONICAL CLIENT SOURCE/KB  
`Selections[selectionID]=visibleText`; command `100007` submits `selectionID:SelectedItemID`.

## EVID-012 — v1.1.9 source build passes
**Type:** CI / BUILD ARTIFACT  
Build/package success only; runtime remained separate.

## EVID-013 — v1.1.9 runtime fails at LuaSystemManager instance resolution
**Type:** USER_RUNTIME + EXACT LOG + SOURCE CORRELATION  
After GameDialog open, repeated `LuaSystemManager instance unresolved`. Returned object/DoString/Selections/action not reached.

## EVID-014 — Canonical KB lacked stable LuaSystemManager singleton contract
**Type:** CANONICAL CLIENT KB  
`get_LuaEnv/set_LuaEnv` documented but no stable singleton getter/field contract verified.

## EVID-015 — v1.1.10 source/final builds pass
**Type:** CI / BUILD ARTIFACT  
Build validity only.

## EVID-016 — v1.1.10 runtime reaches returned object then old DoString lookup fails
**Type:** USER_RUNTIME + EXACT LOG + SOURCE CORRELATION  
`RunLuaChunkV120()` reaches `ResolveLuaEnvV120()` then reports old DoString unresolved. Manager -> returned object = runtime PARTIAL PASS; old DoString resolver = first confirmed failing stage.

## EVID-017 — Upstream xLua DoString research
**Type:** EXTERNAL PRIMARY SOURCE / FAILED-GUIDANCE HISTORY  
Official Tencent xLua exposes String/Byte[] DoString shapes. This guided V121 diagnostics, but later live runtime proves current returned client object is MoonSharp. xLua is not current engine identity at this boundary.

## EVID-018 — v1.1.11 build + self-contained handoff artifact pass
**Type:** CI / BUILD ARTIFACT + ARTIFACT INSPECTION  
Source commit `95f285f929a32c9748342a3480748a5b79d1a4d0`, run `31935080947` PASS. Final pre-v1.1.12 HEAD `4c9b0d5dceaa3828e3452aee339b3a03ceb0ef3b`, run `31935342682` PASS, artifact ID `9260507355`. ZIP contained EXE/DLL + handoff/startup/protocol/rules/current knowledge/changelog/build evidence.

## EVID-019 — v1.1.11 runtime identifies MoonSharp and exact live DoString signature
**Type:** USER_RUNTIME + EXACT LOG + RUNTIME METADATA  
**Date / Version:** 2026-08-16 / delivered v1.1.11  
Repeated:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

**Supports:** actual/declared returned object is `MoonSharp.Interpreter.Script`; current metadata exposes `DoString(String,Table,String)->DynValue`; V121 reached method enumeration.  
**Source correlation:** V121 required parameter index 1 to be `System.String`; live index 1 is `MoonSharp.Interpreter.Table`, so correct method was rejected before invocation.  
**Does NOT Prove:** chunk execution, current Selections, Treatment ID, action/packet acceptance or server result.  
**Confidence:** CONFIRMED runtime identity/signature + confirmed source rejection condition.

## EVID-020 — Official MoonSharp source matches runtime contract
**Type:** EXTERNAL PRIMARY SOURCE / TARGETED RESEARCH  
Official MoonSharp `Script.cs` defines `DoString(string code, Table globalContext = null, string codeFriendlyName = null)` returning `DynValue`. Official `DynValue.cs` exposes `String` property.  
**Supports:** V122 argument order `{code, null Table, friendlyName}` and result extraction through `get_String()`.  
**Does NOT Prove:** GameDialog probe will find Treatment.  
**Confidence:** CONFIRMED upstream API shape, directly consistent with live metadata.

## EVID-021 — v1.1.12 initial CI failure is an audit false-positive
**Type:** CI / BUILD LOG  
**Commit:** `ad2f0403863251330b1aca80eb1eba9681b58c9a`  
**Run:** `31937607280`  
Architecture audit failed before compiler with `v1.1.12 MoonSharp resolver missing MoonSharp.Interpreter.Script`. Source did not hardcode this literal because current method signature is metadata-validated. Artifact upload skipped.  
**Supports:** initial v1.1.12 build state = CI FAILED; failure is build-audit configuration, not MoonSharp C++ compile/runtime behavior.  
**Confidence:** CONFIRMED.

## EVID-022 — v1.1.12 corrected source-bearing build passes
**Type:** CI / BUILD ARTIFACT + ARTIFACT INSPECTION  
**Correction commit:** `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1`  
**Run:** `31937703988`  
**Result:** architecture audit PASS; Route FSM PASS; Heal FSM PASS; bridge DLL/PE PASS; controller EXE PASS; knowledge packaging PASS; artifact upload PASS.  
**Artifact:** `ThanLongTestAutoHeal-v1.1.12`, ID `9261162703`, ZIP digest `sha256:d25f999934be62152cf02fd0251f533743f3e9eaa0d4178f74c2fa1007d85c19`.  
**Verified members:** exactly 9 required files.  
**EXE SHA-256:** `32a3977469a26f5807be192a620630fba9ea3242e265706522fd7d93bb27c823`.  
**DLL SHA-256:** `553c1d245c06c614561b9c2c1e6368daf30cb671b46602aec60b1938f326a037`.  
`BUILD_EVIDENCE.txt` records source head `33a3b56...`, checkout SHA `3855f781...`, run `31937703988`, BUILD PASS and RUNTIME UNTESTED.  
**Does NOT Prove:** V122 runtime success.  
**Confidence:** CONFIRMED BUILD; RUNTIME UNTESTED.

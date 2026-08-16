# EVIDENCE REGISTRY

## EVID-001 — NPC dialog opens but full Treatment flow does not advance
**Type:** USER_RUNTIME  
Early tests confirm raw route and `ClickNPC(339)` can open the intended dialog; full Treatment has no runtime PASS.

## EVID-002 — Alternate NPC/map reproduces overall symptom
**Type:** USER_RUNTIME / v1.1.5  
NPC 463 / Lạc Dương reproduced the overall no-progress symptom.

## EVID-003 — MainThread dispatcher contract
**Type:** CANONICAL CLIENT SOURCE  
`MainThread.Execute(System.Action)` queues a managed action and Unity Update drains/invokes it.

## EVID-004 — GameDialog is server-driven and rebuilt
**Type:** CANONICAL CLIENT SOURCE  
Inbound GameDialog processing can destroy/recreate the current UI; visual flicker is not success proof.

## EVID-005 — Historical repeated ClickNPC while waiting for Treatment
**Type:** USER_RUNTIME + SOURCE CORRELATION  
Old logs/source established a real retry/rebuild pollution path.

## EVID-006 — Old same-object discovery assumption
**Type:** SOURCE  
Historical button walker coupled text and click handler discovery on the same object.

## EVID-007 — Pre-v1.1.8 user flicker report
**Type:** USER_RUNTIME  
Flicker/no-progress persisted in older artifacts before the v1.1.8 isolation work.

## EVID-008 — v1.1.7 source/CI gaps
**Type:** SOURCE + CI  
First-text-only matching, residual retry, and final controller compile failure made v1.1.7 an invalid handoff.

## EVID-009 — v1.1.8 build-valid artifact
**Type:** CI / BUILD  
Architecture audit, tests and binaries passed.

## EVID-010 — v1.1.8 runtime isolates UIRoot representation failure
**Type:** USER_RUNTIME + EXACT LOG  
**Date / Version:** 2026-08-16 / v1.1.8  
After one NPC open, every observer sample reported `clickable=0 • texts=0 • labels=<none>` until fail-closed. Anti-reopen passed for that transaction; UIRoot/CoreChildren representation failed.

## EVID-011 — Runtime Selections is the stronger GameDialog source
**Type:** CANONICAL CLIENT SOURCE/KB  
`Selections[selectionID]=visibleText`; command `100007` submits `selectionID:SelectedItemID`.

## EVID-012 — v1.1.9 source build passes
**Type:** CI / BUILD ARTIFACT  
Source built/packages; runtime result remained separate.

## EVID-013 — v1.1.9 runtime fails at LuaSystemManager instance resolution
**Type:** USER_RUNTIME + EXACT LOG + SOURCE CORRELATION  
After GameDialog open, repeated `LuaSystemManager instance unresolved`. This is before returned script object/DoString/Selections/action.

## EVID-014 — Canonical KB lacked a stable LuaSystemManager singleton contract
**Type:** CANONICAL CLIENT KB  
`get_LuaEnv/set_LuaEnv` were documented but no stable singleton getter/field contract was verified.

## EVID-015 — v1.1.10 source/final builds pass
**Type:** CI / BUILD ARTIFACT  
Build validity only.

## EVID-016 — v1.1.10 runtime reaches returned script object then old DoString resolver fails
**Type:** USER_RUNTIME + EXACT LOG + SOURCE CORRELATION  
`RunLuaChunkV120()` reaches `ResolveLuaEnvV120()` and then reports old DoString unresolved. This supports manager -> returned object = RUNTIME PARTIAL PASS and old DoString resolver = first confirmed failure.

## EVID-017 — Upstream xLua DoString research
**Type:** EXTERNAL PRIMARY SOURCE / FAILED-GUIDANCE HISTORY  
Official Tencent xLua exposes String/Byte[] DoString shapes. This influenced v1.1.11, but later runtime proves the actual returned client object is MoonSharp, so xLua is not current engine identity at this boundary.

## EVID-018 — v1.1.11 build + self-contained handoff artifact pass
**Type:** CI / BUILD ARTIFACT + ARTIFACT INSPECTION  
Source commit `95f285f929a32c9748342a3480748a5b79d1a4d0`, run `31935080947` PASS. Final pre-v1.1.12 HEAD `4c9b0d5dceaa3828e3452aee339b3a03ceb0ef3b`, run `31935342682` PASS, artifact ID `9260507355`. ZIP included EXE/DLL + consolidated handoff/startup/protocol/rules/current knowledge/changelog/build evidence.

## EVID-019 — v1.1.11 runtime identifies MoonSharp and exact live DoString signature
**Type:** USER_RUNTIME + EXACT LOG + RUNTIME METADATA  
**Date / Version:** 2026-08-16 / delivered v1.1.11  
Repeated exact diagnostic:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

**Supports:**
- actual/declared returned object is `MoonSharp.Interpreter.Script`;
- current metadata exposes `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
- V121 reached method enumeration.

**Source correlation:** V121 `ScoreDoStringV121` required parameter index 1 to be `System.String` for multi-argument shapes. The runtime method's index 1 is `MoonSharp.Interpreter.Table`, so the valid method was rejected before invocation.

**Does NOT Prove:** Lua chunk execution, current Selections, Treatment ID, semantic packet acceptance or server result.

**Confidence:** CONFIRMED runtime identity/signature + confirmed source rejection condition.

## EVID-020 — Official MoonSharp source matches the runtime contract
**Type:** EXTERNAL PRIMARY SOURCE / TARGETED RESEARCH  
**Date / Used by:** 2026-08-16 / v1.1.12  
Official MoonSharp `Script.cs` defines `DoString(string code, Table globalContext = null, string codeFriendlyName = null)` and returns `DynValue`. Official `DynValue.cs` exposes `String` via a property getter.

**Supports:** v1.1.12 invocation order `{code, null Table, friendlyName}` and result extraction through `DynValue.get_String()`.

**Does NOT Prove:** that the bounded GameDialog probe itself will find Treatment; that remains runtime work after invocation succeeds.

**Confidence:** CONFIRMED upstream API shape and directly consistent with live client metadata.

## EVID-021 — v1.1.12 build evidence
**Type:** CI / BUILD  
**Status:** PENDING at source commit creation. Update after CI; do not promote runtime state.

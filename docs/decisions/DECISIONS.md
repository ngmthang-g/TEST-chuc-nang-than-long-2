# DECISION REGISTRY

## DEC-001 — Mandatory per-version startup knowledge
**Status:** ACTIVE  
Read `AI_START_HERE.md`, V2 protocol, client-analysis rules, consolidated handoff/current knowledge/changelog, affected docs and current source before modifying a version.

## DEC-002 — Canonical client KB before reverse
**Status:** ACTIVE  
Use `clinent-game-than-long-DATA-2222/AI_INDEX.md` routing and exact VERIFIED/database facts first. No repeated broad reverse.

## DEC-003 — Runtime coordinates in this test lab
**Status:** ACTIVE  
Do not infer healer X/Y as universal truth. Current test target includes Map 5 -> NPC 339 Đỗ Thanh Đằng.

## DEC-004 — MainThread remains preferred production mutation boundary
**Status:** ACTIVE  
Prefer game-owned managed action dispatch for Unity/UI mutations; narrow semantic request proof does not revoke this architecture.

## DEC-005 — MainThread proof is asynchronous
**Status:** ACTIVE  
Never enqueue and synchronously wait inside the same hook request.

## DEC-006 — No WaitTreatment NPC reopen while dialog observation is unresolved
**Status:** ACTIVE / RUNTIME SUPPORTED  
The old reopen retry polluted server-driven dialog state. v1.1.8 removed it.

## DEC-007 — Dynamic GameDialog identity is current runtime state
**Status:** ACTIVE  
Never hardcode or cache a universal Treatment selectionID.

## DEC-008 — UIRoot/UIButton is abandoned as active observer for this GameDialog
**Status:** ACTIVE  
Runtime v1.1.8 proved that representation empty for the needed dynamic content.

## DEC-009 — Use current Selections + exact GameDialog request as narrow proof
**Status:** ACTIVE FOR TEST LAB ONLY  
Observe current Selections, re-read immediately before action, then submit `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentSelectionID>:-1`.

## DEC-010 — MessageBox confirmation is its semantic callback when present
**Status:** ACTIVE  
Do not force MessageBox into the GameDialog selection model if a live MessageBox exists.

## DEC-011 — Resolve returned script object without assuming a LuaSystemManager singleton
**Status:** ACTIVE / RUNTIME SUPPORTED  
The V120 direction progressed beyond the v1.1.9 singleton failure and is protected absent contrary evidence.

## DEC-012 — Metadata-first DoString resolution
**Status:** ACTIVE PRINCIPLE / ENGINE SHAPE SUPERSEDED  
Do not guess RVA. Runtime metadata exposed the actual MoonSharp method and disproved the earlier xLua parameter model for the returned object.

## DEC-013 — Every artifact carries one-file handoff + current knowledge bundle
**Status:** ACTIVE  
Every delivered CI artifact contains exactly the 9-file self-describing bundle.

## DEC-014 — Use exact live MoonSharp Script.DoString contract
**Date / Version:** 2026-08-16 / v1.1.12  
**Status:** ACTIVE / RUNTIME PASS FOR EXECUTION BOUNDARY  
Use the runtime-confirmed `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`, invoke `DoString(code,null,friendlyName)`, and read the returned string through `DynValue.get_String()`. v1.1.12 runtime confirms this execution/result path works.

## DEC-015 — Preserve MoonSharp execution; replace only raw-only GameDialog observation
**Date / Version:** 2026-08-16 / v1.1.13  
**Status:** ACTIVE EXPERIMENT

**Trigger:** delivered v1.1.12 executes the Lua probe successfully and returns:
`GD=table;AF=table;T=0;C=0;K=0;N=4;S=`.

**Source correction:** V122 `N` is `nodes`, the number of traversed tables. It is not a count of selections.

**Source finding:** V122 reuses a probe that reads semantic fields through `rawget`, including `rawget(t,"Selections")`. `rawget` bypasses `__index`/metatable lookup.

**Decision:**
1. freeze the V120/V122 returned-Script + MoonSharp DoString + DynValue.String execution boundary;
2. change only the read-only current-dialog observer;
3. use normal indexing inside `pcall` for semantic keys;
4. inspect bounded table children and bounded table-valued `__index` metatable chains;
5. check canonical semantic names `Selections`, `GameDialogData`, `CurrentGameDialogData`, `DialogData` and variants;
6. emit representation diagnostics `NODE`, `ST`, `SV`, `MT`, `KS`, `S` so the next runtime log identifies whether a Selections table was actually reached;
7. remain fail-closed and issue no GameDialog mutation until a positive current semantic ID is re-read immediately before action.

**Hypothesis status:** the data being hidden behind normal indexing/metatable/nested representation is LIKELY, not CONFIRMED until V123 runtime.

**Rejected:**
- changing MoonSharp invocation again despite V122 runtime PASS;
- interpreting `N=4` as four choices;
- reverting to UIRoot;
- hardcoding Treatment ID;
- guessing packet failure before a live selection is sent;
- broad binary reverse before V123 representation diagnostics are exhausted.

**Consequences:** active observer/action markers become `LUA_DIALOG_V123` and `ACTION_V123`. The next runtime classification is driven by `T`, `ST`, `SV`, `MT`, `KS`, and `S`.

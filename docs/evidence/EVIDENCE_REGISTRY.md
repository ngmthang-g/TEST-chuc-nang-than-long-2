# EVIDENCE REGISTRY

## EVID-001 — NPC dialog opens but complete Treatment flow is not yet proven
**Type:** USER_RUNTIME  
Route/NPC open has runtime evidence; full Treatment has no known-good version.

## EVID-002 — Alternate NPC/map reproduced the historical no-progress symptom
**Type:** USER_RUNTIME / v1.1.5  
NPC 463 / Lạc Dương weakened an NPC-339-only explanation.

## EVID-003 — MainThread dispatcher contract
**Type:** CANONICAL CLIENT SOURCE  
`MainThread.Execute(System.Action)` queues managed work drained by Unity Update. Keep as preferred production mutation boundary.

## EVID-004 — GameDialog is server-driven and rebuilt
**Type:** CANONICAL CLIENT SOURCE  
Inbound GameDialog handling may destroy/recreate the current UI. Visual flicker is not completion proof.

## EVID-005 — Historical WaitTreatment reopen pollution
**Type:** USER_RUNTIME + SOURCE  
Old controller retry could re-open the NPC while observation was unresolved. v1.1.8 removed this path.

## EVID-006 — UIRoot representation failed for the live dynamic GameDialog
**Type:** USER_RUNTIME / v1.1.8  
GameDialog existed while observer repeatedly saw `clickable=0 • texts=0 • labels=<none>`. UIRoot/UIButton is abandoned for this feature.

## EVID-007 — Canonical current Selections contract
**Type:** CANONICAL CLIENT SOURCE/KB  
`GameDialogData.Selections[selectionID]=visibleText`; `CMD_SHOW_GAMEDIALOG=100007` submits `selectionID:SelectedItemID`; current ID is runtime state.

## EVID-008 — v1.1.9 failed before returned script object
**Type:** USER_RUNTIME + SOURCE  
After GameDialog open: `LuaSystemManager instance unresolved`. DoString/Selections/action were not reached.

## EVID-009 — v1.1.10 progresses to object returned by get_LuaEnv
**Type:** USER_RUNTIME + SOURCE  
The next failure was old DoString lookup, proving the returned-object resolver progressed beyond the prior singleton boundary.

## EVID-010 — v1.1.11 identifies MoonSharp and exact DoString metadata
**Type:** USER_RUNTIME + RUNTIME METADATA  
Repeated runtime diagnostics report `actual=MoonSharp.Interpreter.Script`, `declared=MoonSharp.Interpreter.Script`, and `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`. V121 source rejected the real overload before invocation because it expected parameter #2 String.

## EVID-011 — Official MoonSharp API matches live metadata
**Type:** EXTERNAL PRIMARY SOURCE / TARGETED RESEARCH  
MoonSharp defines `Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null) -> DynValue` and exposes `DynValue.String`. Used to implement V122 argument/result handling; exact client runtime remained source of truth.

## EVID-012 — v1.1.12 build lineage
**Type:** CI / BUILD  
Initial V122 run `31937607280` failed before compiler because architecture audit required an unnecessary class-name literal. Corrected source commit `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1`, run `31937703988`, passed architecture audit, Route/Heal tests, bridge/controller build, packaging and artifact upload. Final v1.1.12 HEAD `03b33595e122217baa4c006a0fe8998af3395d44`, run `31938051044`, also passed.

## EVID-013 — v1.1.12 runtime passes MoonSharp execution/result and isolates observer representation
**Type:** USER_RUNTIME + EXACT LOG + SOURCE CORRELATION  
**Date / Version:** 2026-08-16 / delivered v1.1.12  
After one `ClickNPC npcID=339`, repeated:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

**Supports:** current Script resolution PASS; exact MoonSharp DoString invocation PASS; DynValue.String extraction PASS; GameDialog and AutoFight_Main are returned as Lua tables; MessageBox absent; V122 observer does not expose T/C/K.  
**Does NOT Prove:** that no Treatment choice exists, that four selections exist, or that packet/server rejects Treatment.  
**Confidence:** CONFIRMED runtime boundary.

## EVID-014 — V122 `N=4` means four traversed table nodes, not four selections
**Type:** SOURCE CORRECTION  
In `kGameDialogProbeLuaV119`, `nodes` increments once per table accepted by `scan()`, and the returned diagnostic writes `N=nodes`. Therefore the runtime `N=4` is table traversal count only. Any statement that V122 observed four selections is incorrect.

## EVID-015 — V122 semantic field lookup is raw-only
**Type:** SOURCE  
V122 reuses the V119 probe, which calls `rawget(t,"Selections")` and `rawget(t,key)` for priority fields. Lua `rawget` bypasses `__index`/metatable resolution. This creates a concrete observer limitation consistent with `GD=table`, `AF=table`, `S=` empty.

**Supports:** a normal-indexing/metatable-aware read-only observer is a targeted next experiment.  
**Does NOT Prove:** that the live data definitely resides behind `__index`; that remains a hypothesis until V123 runtime.

## EVID-016 — Canonical client still defines Selections as the semantic source
**Type:** CANONICAL CLIENT KB  
Current canonical docs continue to verify `GameDialogData.Selections[selectionID]=visibleText` and built-in automation inspecting current Selections. No contrary runtime evidence yet establishes a changed server contract.

## EVID-017 — v1.1.13 experiment scope
**Type:** SOURCE DESIGN  
V123 preserves the runtime-proven MoonSharp execution primitive and changes only the read-only table observer: normal indexing under `pcall`, bounded child traversal, bounded metatable/table-`__index` traversal, and diagnostics `NODE/ST/SV/MT/KS/S`. Mutation remains fail-closed until a positive current semantic ID is re-read.

## Current evidence boundary
Earliest unresolved stage:
`MoonSharp execution PASS -> DynValue.String PASS -> current Lua table representation/access -> current Selections -> live Treatment ID -> action -> follow-up/result`.

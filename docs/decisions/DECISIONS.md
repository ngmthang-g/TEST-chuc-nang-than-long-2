# DECISION REGISTRY

## DEC-001 — Mandatory per-version startup knowledge
**Status:** ACTIVE  
Every version starts from `AI_START_HERE.md`, V2 protocol, client-analysis TXT, consolidated handoff/current knowledge/changelog, affected docs/source.

## DEC-002 — Canonical client KB before reverse
**Status:** ACTIVE  
Use `clinent-game-than-long-DATA-2222/AI_INDEX.md` routing and exact VERIFIED/database facts before targeted binary work. No repeated broad reverse.

## DEC-003 — Runtime coordinates are user-captured in this test lab
**Status:** ACTIVE  
Do not infer/hardcode healer X/Y as universal truth. Current map mapping includes Map 5 -> NPC 339 Đỗ Thanh Đằng.

## DEC-004 — MainThread remains preferred production mutation boundary
**Status:** ACTIVE  
Prefer game-owned managed action dispatch for live Unity/UI mutations. Narrow semantic GameDialog proof does not revoke this architecture.

## DEC-005 — MainThread proof is asynchronous
**Status:** ACTIVE  
Never enqueue and synchronously wait inside the same hook request.

## DEC-006 — Never reopen NPC from WaitTreatment merely because observation is unresolved
**Status:** ACTIVE / RUNTIME SUPPORTED  
v1.1.8 removed the pollution path; user runtime showed one NPC interaction for the tested transaction.

## DEC-007 — Dynamic GameDialog identity comes from current runtime state
**Status:** ACTIVE  
Never hardcode Treatment selectionID.

## DEC-008 — Abandon UIRoot/UIButton as active observer for this dynamic GameDialog
**Status:** ACTIVE  
Runtime v1.1.8 proved the bridge-visible tree had no relevant nodes while GameDialog existed.

## DEC-009 — Use current Selections + exact semantic GameDialog request as narrow proof
**Status:** ACTIVE FOR TEST LAB ONLY  
Observe current `Selections`; immediately re-read before submitting `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentSelectionID>:-1`.

## DEC-010 — MessageBox confirmation is not automatically a GameDialog selection
**Status:** ACTIVE  
If live MessageBox exists, use semantic `ButtonOKClicked()`; otherwise current GameDialog confirmation uses its live selection ID.

## DEC-011 — Resolve the returned script object without assuming a LuaSystemManager singleton
**Date / Version:** 2026-08-16 / v1.1.10  
**Status:** ACTIVE / RUNTIME PARTIAL PASS  
Resolve `get_LuaEnv` directly when static; otherwise use metadata-validated manager candidates. Latest runtime progresses beyond this boundary, so do not redesign it without contrary evidence.

## DEC-012 — Resolve DoString from live metadata, not a guessed RVA/signature
**Date / Version:** 2026-08-16 / v1.1.11  
**Status:** SUPERSEDED IN ENGINE-SPECIFIC SHAPE BY DEC-014; metadata-first principle remains ACTIVE  
v1.1.11 added actual-class/signature diagnostics instead of hardcoding an RVA. Its xLua-inspired accepted parameter model was later disproven for this returned object by V121 runtime.

## DEC-013 — Every artifact contains one-file AI handoff + current knowledge bundle
**Date / Version:** 2026-08-16 / v1.1.11  
**Status:** ACTIVE  
Every CI artifact includes EXE/DLL, `AI_PROJECT_HANDOFF_FULL.md`, startup, V2 protocol, client rules, project knowledge, changelog, and generated `BUILD_EVIDENCE.txt`.

## DEC-014 — Treat the returned runtime object as MoonSharp Script and invoke the exact live contract
**Date / Version:** 2026-08-16 / v1.1.12  
**Status:** ACTIVE

**Trigger:** delivered v1.1.11 repeatedly reports:
`actual=MoonSharp.Interpreter.Script`, `declared=MoonSharp.Interpreter.Script`, and
`DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`.

**Source correlation:** V121 did enumerate this method but rejected it because its scorer required parameter #2 (`index 1`) to be `System.String`; the live method has `MoonSharp.Interpreter.Table` there.

**Decision:**
1. retain the runtime-proven V120 returned-object resolver;
2. require the current live metadata shape `DoString(String, MoonSharp.Interpreter.Table, String) -> MoonSharp.Interpreter.DynValue`;
3. invoke in exact order `DoString(code, null, friendlyName)`;
4. consume the returned `DynValue` through `get_String()` because the bounded diagnostic probe returns one string;
5. emit managed exception/result diagnostics if invocation/result extraction fails;
6. only after successful V122 probe parse current T/C/K and then re-read the current selection immediately before action.

**Primary-source cross-check:** official MoonSharp source defines the same DoString parameter order/defaults and DynValue String getter.

**Rejected:**
- continuing xLua `LuaEnv.DoString` argument assumptions;
- swapping MoonSharp Table/friendly-name parameter order;
- raw native RVA calls;
- reverting to UIRoot;
- hardcoding Treatment ID;
- claiming packet/server failure before the current live selection is sent.

**Consequences:** active markers become `LUA_MOONSHARP_V122`, `LUA_DIALOG_V122`, `ACTION_V122`. xLua remains historical failed-guidance evidence, not current engine identity for this boundary.

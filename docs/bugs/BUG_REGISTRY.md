# BUG REGISTRY

## BUG-001 — GameDialog Trị liệu mở được nhưng chưa tiến sang service/result

**Status:** OPEN  
**Severity:** HIGH for Auto Heal feature  
**First Observed:** v1.1.0-test  
**Last Runtime Tested:** v1.1.12-test delivered artifact, 2026-08-16  
**Last Known-Good:** NONE for complete Treatment flow  
**Related Feature:** Auto trị liệu NPC

### Runtime-confirmed working
- raw NPC coordinate capture;
- route/mount/AutoPath/dismount;
- `ClickNPC(339)` opens intended server-driven GameDialog;
- tested v1.1.8 transaction has no WaitTreatment NPC reopen;
- current returned script object resolves as `MoonSharp.Interpreter.Script`;
- v1.1.12 exact MoonSharp `DoString(String,Table,String)` invocation executes;
- v1.1.12 `DynValue.String` extraction returns the full Lua probe string;
- current `GameDialog` and `AutoFight_Main` are reachable as Lua tables during the open dialog.

### Runtime-confirmed failures / boundaries
#### v1.1.8 — UIRoot representation
GameDialog existed but UIRoot/CoreChildren walker saw no relevant buttons/text. Abandoned.

#### v1.1.9 — manager singleton assumption
Failed before returned object/DoString/Selections.

#### v1.1.11 — accepted DoString shape
Runtime exposed the real MoonSharp overload, but V121 rejected it before invocation because it expected parameter #2 String instead of MoonSharp Table.

#### v1.1.12 — raw-only current Selections observation
Exact repeated runtime line after one NPC open:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

This proves the execution/result boundary passed and isolates the next unresolved layer to current Lua dialog representation/access.

### Important correction
V122 `N` is the number of table nodes traversed, not a selection count. `N=4` must never be cited as evidence of four dialog choices.

### Source correlation
V122 uses `rawget(t,"Selections")` and rawget for priority semantic fields. Lua rawget bypasses `__index`/metatable lookup. Canonical client knowledge still verifies `GameDialogData.Selections[selectionID]=visibleText`.

**Current root-cause hypothesis:** V122 is observing the correct live script objects through a representation/access path that is too raw/narrow. Confidence: LIKELY, not CONFIRMED until V123 runtime.

### Canonical semantic source of truth
- `GameDialogData.Selections[selectionID]=visibleText`;
- no universal Treatment selectionID;
- `CMD_SHOW_GAMEDIALOG=100007` payload `selectionID:SelectedItemID`;
- ordinary no-item selection commonly uses `-1` item;
- current ID must be re-read immediately before action.

### Attempts / lineage
1. v1.1.0–v1.1.2: initial UIRoot/button experiments.
2. v1.1.3: GameDialog callback experiment; shared discovery dependency remained.
3. v1.1.4: live Tag + exact packet experiment; shared button discovery still gated reachability.
4. v1.1.5: alternate NPC/map reproduced symptom.
5. v1.1.6: CTS/MainThread experiment.
6. v1.1.7: final CI failed; source retained gaps.
7. v1.1.8: anti-reopen runtime PASS, UIRoot representation runtime FAIL.
8. v1.1.9: manager singleton assumption runtime FAIL.
9. v1.1.10: returned-object resolver progressed.
10. v1.1.11: MoonSharp identity/signature discovered; real method rejected before invoke.
11. v1.1.12: exact MoonSharp call/result implemented; runtime PASS through DynValue.String, observer returns T/C/K=0.
12. v1.1.13: metatable-aware/normal-indexing observer + bounded representation diagnostics; runtime pending.

### v1.1.13 strategy
- preserve V120/V122 execution boundary;
- replace only V122 raw-only semantic field access;
- normal indexing under `pcall`;
- bounded child-table scan;
- bounded `getmetatable` + table-valued `__index` scan;
- diagnostics distinguish `NODE`, `ST`, `SV`, `MT`, `KS`, `S`;
- only act when live T is positive and re-read immediately before send.

### Root-cause status
**CONFIRMED resolved component:** WaitTreatment NPC reopen removed in tested v1.1.8 transaction.  
**CONFIRMED failed component:** v1.1.8 UIRoot representation.  
**CONFIRMED failed component:** v1.1.9 singleton assumption.  
**CONFIRMED runtime identity:** MoonSharp Script + exact DoString signature.  
**CONFIRMED runtime PASS:** v1.1.12 exact DoString invocation + DynValue.String extraction.  
**CONFIRMED observation result:** V122 returns T/C/K=0 and no selection samples via raw-only probe.  
**LIKELY current cause:** semantic fields hidden behind normal indexing/metatable/nested representation not reached by V122 rawget.  
**UNKNOWN:** live Treatment ID, mutable action result, follow-up sequence, completion proof.

### Current workaround
None. Do not port Auto Heal to production.

### Fixed in
UNKNOWN until complete runtime PASS.

### Next diagnostic step
Run v1.1.13 and inspect first `LUA_DIALOG_V123`:
- `T>0` -> action stage reached;
- `T=0, ST>0, SV>0` -> Selections reached, inspect value/text samples;
- `T=0, ST=0` -> inspect `MT` and `KS` representation diagnostics;
- no packet/server diagnosis before `ACTION_V123 SENT`.

### Do-not-do
- no broad reverse;
- no UIRoot depth/label tuning;
- no hardcoded Treatment ID;
- no WaitTreatment ClickNPC retry;
- no fixed Sleep as success proof;
- do not alter proven MoonSharp invocation while diagnosing table representation;
- do not call V122 N a selection count;
- no packet/server blame before a current live ID is sent.

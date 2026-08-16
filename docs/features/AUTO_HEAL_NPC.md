# FEATURE: AUTO HEAL NPC / TRỊ LIỆU

## Goal
`route -> one NPC open -> observe current server GameDialog data -> current Treatment selection -> one semantic action -> fresh follow-up -> result proof`.
Only port after repeated runtime PASS.

## Mandatory startup / artifact handoff
Read `AI_START_HERE.md`, V2 protocol, client-analysis TXT, `AI_PROJECT_HANDOFF_FULL.md`, project knowledge/changelog, this feature + BUG/DEC/EVID/history/investigation + current source. Use canonical client repo `clinent-game-than-long-DATA-2222` before any targeted reverse work.

## Protected working path
- raw user-captured MapID/X/Y;
- mount / route / StopPath / dismount;
- Map 5 -> NPC 339 Đỗ Thanh Đằng;
- semantic NPC interaction/open;
- no WaitTreatment NPC reopen after successful initial open in tested v1.1.8 transaction.

## Canonical semantic facts
- `GameDialogData.Selections[selectionID]=visibleText`;
- no global Treatment selectionID;
- built-in AutoFight/FuBen logic inspects current Selections;
- `CMD_SHOW_GAMEDIALOG=100007`;
- payload `selectionID:SelectedItemID`, ordinary no-item choice commonly `<currentID>:-1`;
- MessageBox OK uses semantic callback.

## Runtime layer findings
### v1.1.8
GameDialog opened but bridge-visible UIRoot observer returned no relevant nodes. UIRoot/UIButton abandoned.

### v1.1.9
GameDialog opened then manager singleton resolution failed before returned script object/DoString/Selections.

### v1.1.10
Returned-object resolver progressed to the object from `get_LuaEnv`.

### v1.1.11
Runtime proved that object is `MoonSharp.Interpreter.Script` and exposed exact `DoString(String,Table,String)->DynValue`; V121 rejected the real method before invocation because of a wrong accepted-shape model.

### v1.1.12 — current runtime proof
After one NPC open, repeated:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

Confirmed:
- MoonSharp Script resolution PASS;
- exact DoString invocation PASS;
- DynValue.String extraction PASS;
- GameDialog and AutoFight_Main are reachable as Lua tables;
- V122 does not expose current T/C/K.

**Correction:** V122 `N` is table-node count, not selection count. `N=4` does not establish four choices.

## v1.1.12 observer weakness
The V122 probe accesses semantic fields with `rawget`, which bypasses `__index`/metatable lookup. This is now the earliest targeted observer weakness. Canonical Selections contract remains valid unless contrary runtime evidence appears.

## Current implementation — v1.1.13
### Protected MoonSharp execution
`bridge_lua_manager_v1_1_10.inc` + `bridge_lua_moonsharp_v1_1_12.inc` remain the proven execution layer. V123 reuses `RunLuaChunkV122()`.

### New read-only observer
`src/bridge_lua_dialog_v1_1_13.inc`:
- uses normal indexing under `pcall` through `safeget`;
- checks semantic field names including `Selections` and `GameDialogData` variants;
- boundedly scans child tables;
- boundedly inspects `getmetatable()` and table-valued `__index`;
- checks canonical global GameDialogData names if exposed;
- matches current numeric selection key to normalized visible text;
- if a selection value is unexpectedly a table, records bounded diagnostic text-field candidates without promoting that shape to a verified client fact;
- marker `LUA_DIALOG_V123`;
- diagnostics: `NODE`, `ST`, `SV`, `MT`, `KS`, `S`.

Interpretation:
- `ST` = number of Selections tables observed;
- `SV` = number of values enumerated inside Selections tables;
- `NODE` = traversed tables;
- `MT` = metatables seen;
- `KS` = key/type samples;
- `S` = selection samples.

### Action
`src/bridge_action_v1_1_13.inc`:
- re-runs V123 immediately before action;
- refuses cached/guessed/absent ID;
- submits canonical `<currentID>:-1` only when current `T>0`;
- marker `ACTION_V123`;
- confirmation remains MessageBox callback or current live GameDialog confirmation ID.

## Build/runtime state
- v1.1.12 final build PASS; runtime updated as above.
- v1.1.13 source/CI: PENDING at source commit creation.
- v1.1.13 runtime: UNTESTED.
- full Auto Heal: NO KNOWN-GOOD VERSION.

## Required next log
After one NPC open capture `LUA_DIALOG_V123`:
- `T>0` -> capture `ACTION_V123` and next GameDialog/MessageBox/HP/money;
- `T=0,ST>0,SV>0` -> inspect `S` only;
- `T=0,ST=0` -> inspect `MT`/`KS` to locate current field representation.

## Do not break
- no guessed/cached ID;
- no UIRoot tuning;
- no WaitTreatment NPC reopen;
- no fixed sleep success proof;
- no broad reverse;
- do not touch runtime-proven MoonSharp invocation while diagnosing table representation;
- do not treat V122 N as selection count;
- BUILD PASS is not Runtime PASS;
- artifact handoff bundle is mandatory.

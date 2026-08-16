# PROJECT KNOWLEDGE

## Identity / mandatory startup
- Repo: `ngmthang-g/TEST-chuc-nang-than-long-2`; branch `agent/auto-tri-lieu-v1.1.0`.
- Current version: `v1.1.13-test`; full Auto trị liệu known-good: **NONE**.
- Every version: `AI_START_HERE.md` -> V2 protocol -> client-analysis TXT -> `AI_PROJECT_HANDOFF_FULL.md` -> this file -> `CHANGELOG.md` -> affected feature/BUG/DEC/EVID/history/investigation -> source/tests.
- Canonical client facts start at `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`; do not broad reverse solved facts.
- BUILD/CI PASS != RUNTIME PASS.

## Artifact handoff contract
Every v1.1.11+ delivered CI artifact contains exactly 9 files: versioned EXE, bridge DLL, consolidated handoff, startup pointer, V2 protocol, client-analysis rules, project knowledge, changelog and generated `BUILD_EVIDENCE.txt`.

## Runtime-protected path
- raw MapID/X/Y capture;
- mount / AutoPath / stop / dismount;
- Map 5 NPC 339 Đỗ Thanh Đằng;
- semantic NPC interaction opens intended GameDialog;
- WaitTreatment does not reopen NPC after successful initial open in tested v1.1.8 transaction.

## Layer history
### v1.1.8
UIRoot/CoreChildren/UIButton representation failed despite GameDialog presence. Do not return to tree-depth/text tuning.

### v1.1.9
LuaSystemManager singleton assumption failed before returned object/DoString/Selections.

### v1.1.10
Metadata-driven returned-object resolver progressed beyond manager lookup and reached the object returned by `get_LuaEnv`.

### v1.1.11
Runtime identified that object as `MoonSharp.Interpreter.Script` and enumerated `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`. V121 rejected the correct method because its accepted-shape model expected parameter #2 String.

### v1.1.12 — latest runtime, 2026-08-16
Exact user log after one NPC open repeatedly reports:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

This establishes:
- current Script resolver PASS for the transaction;
- exact MoonSharp DoString invocation PASS;
- DynValue.String extraction PASS;
- GameDialog table present;
- AutoFight_Main table present;
- MessageBox absent;
- V122 observer did not find semantic choices.

### Critical correction: `N=4`
V122 source defines `N` as `nodes`, the number of table nodes visited by `scan()`. **It is not a selection count.** Never state that the runtime exposed four selections from this log.

### Earliest unresolved layer after v1.1.12
V122 accesses semantic fields with `rawget(t,"Selections")` and rawget priority keys. `rawget` bypasses normal `__index`/metatable lookup. Canonical client knowledge still verifies `GameDialogData.Selections[selectionID]=visibleText`; therefore the active hypothesis is a **Lua representation/access mismatch**. It is LIKELY but not yet CONFIRMED until V123 runtime.

## Canonical GameDialog contract
- `GameDialogData.Selections[selectionID] = visibleText`;
- IDs are live server/runtime state; never hardcode Treatment ID;
- `CMD_SHOW_GAMEDIALOG = 100007`;
- payload `selectionID:SelectedItemID`, ordinary no-item choice commonly `<currentID>:-1`;
- built-in AutoFight/FuBen flows inspect current Selections;
- MessageBox OK follows semantic callback.

## v1.1.13 active design
Protected execution primitive:
- `src/bridge_lua_manager_v1_1_10.inc`;
- `src/bridge_lua_moonsharp_v1_1_12.inc` under legacy `InspectHealDialog` naming but with reusable `RunLuaChunkV122()`.

Active observer/action:
- `src/bridge_lua_dialog_v1_1_13.inc`;
- `src/bridge_action_v1_1_13.inc`.

Observer changes only:
1. `safeget(t,key)` uses normal indexing inside `pcall`;
2. searches current `Selections`, `GameDialogData`, `CurrentGameDialogData`, `DialogData` and case variants;
3. bounded recursive table scan;
4. bounded `getmetatable` + table-valued `__index` scan;
5. canonical global `GameDialogData` names checked if exposed;
6. `Selections` numeric/current keys matched against normalized visible text;
7. bounded diagnostics distinguish:
   - `NODE` table nodes;
   - `ST` Selections tables;
   - `SV` values in Selections;
   - `MT` metatables;
   - `KS` key/type samples;
   - `S` selection samples.

Action remains fail-closed:
- re-read V123 immediately before action;
- require positive current semantic ID;
- send `CMD_SHOW_GAMEDIALOG=100007` only with `<currentID>:-1`;
- marker `ACTION_V123`;
- no guessed/cached ID.

## Build state
- v1.1.12 final HEAD `03b33595e122217baa4c006a0fe8998af3395d44` CI/BUILD PASS, runtime now updated by the user as above.
- v1.1.13 source/CI: **PENDING at source commit creation**.
- v1.1.13 runtime: **UNTESTED**.

## Hard rules
- no hardcoded Treatment ID;
- no inferred NPC X/Y as universal truth;
- no WaitTreatment NPC reopen;
- no UIRoot tuning;
- no fixed sleep as completion proof;
- one mutable action per PID;
- do not modify the MoonSharp execution boundary while diagnosing table representation unless contrary runtime evidence appears;
- do not interpret `N` from V122 as selection count;
- do not blame packet/server until a current live ID is actually sent;
- preserve failed attempts and BUILD/RUNTIME distinction.

## BUG-001 status
OPEN.
- anti-reopen component: runtime PASS for tested v1.1.8 transaction;
- UIRoot representation: runtime FAIL;
- v1.1.9 singleton assumption: runtime FAIL;
- v1.1.10 returned-object resolver: progressed;
- v1.1.11 engine/signature identification: runtime CONFIRMED MoonSharp;
- v1.1.12 MoonSharp invoke/result: runtime PASS;
- v1.1.12 rawget-only current Selections observation: failed to expose T/C/K;
- v1.1.13 metatable-aware observer: current experiment;
- live Treatment ID / action / server follow-up: UNKNOWN.

## Next runtime evidence
Read the first `LUA_DIALOG_V123` after one NPC open:
- `T>0` -> evaluate `ACTION_V123` and next server/UI state;
- `T=0, ST>0, SV>0` -> observer reached Selections; inspect `S` only;
- `T=0, ST=0` -> inspect `MT` and `KS` to locate exact current field representation;
- no packet/server diagnosis before `ACTION_V123 SENT`.

See `AI_PROJECT_HANDOFF_FULL.md`, `docs/history/VERSION_v1.1.13.md`, `docs/investigations/V122_SELECTIONS_RUNTIME_FINDING.md`, feature/BUG/EVID/DEC registries.

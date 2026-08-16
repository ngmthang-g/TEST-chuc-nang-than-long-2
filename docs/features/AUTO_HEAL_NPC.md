# FEATURE: AUTO HEAL NPC / TRỊ LIỆU

## Goal
`route -> one NPC open -> observe current server GameDialog data -> current Treatment selection -> one semantic action -> fresh follow-up -> result proof`.
Only port after repeated runtime PASS.

## Protected working path
Runtime-confirmed partial path includes runtime MapID/X/Y capture, mount/route/stop/dismount, Map 5 NPC 339 Đỗ Thanh Đằng, semantic NPC open, and no WaitTreatment NPC reopen in the tested v1.1.8 transaction.

## Canonical facts
- `GameDialogData.Selections[selectionID]=visibleText`;
- no global Treatment ID;
- built-in AutoFight/FuBen logic inspects current selections;
- `CMD_SHOW_GAMEDIALOG=100007` with `selectionID:SelectedItemID`;
- ordinary no-item selection commonly `<currentID>:-1`;
- MessageBox OK follows semantic callback.

## Current runtime boundary — v1.1.12
After one NPC open:
`LUA_DIALOG_V122 ... GD=present • MB=absent • T=0 • C=0 • K=0 • raw={...GD=table;AF=table;N=4;...;S=}`.

This establishes runtime PASS for current MoonSharp Script resolution, exact DoString invocation, DynValue.String extraction, and GameDialog/AutoFight_Main table presence.

**Correction:** `N=4` is table-node count, not four selections.

V122 observer uses rawget for `Selections` and priority keys, which bypasses `__index`/metatable lookup. It failed to expose current T/C/K.

## v1.1.13 observer
`src/bridge_lua_dialog_v1_1_13.inc` preserves proven MoonSharp execution and changes only read-only representation access:
- normal indexing under `pcall`;
- semantic key variants;
- bounded child tables;
- bounded metatable/table-`__index` scan;
- canonical globals when exposed;
- diagnostics `NODE`, `ST`, `SV`, `MT`, `KS`, `S`;
- marker `LUA_DIALOG_V123`.

Interpretation:
- `ST` Selections tables reached;
- `SV` values enumerated in Selections;
- `NODE` traversed tables;
- `MT` metatables observed;
- `KS` key/type samples;
- `S` selection samples.

## v1.1.13 action
`src/bridge_action_v1_1_13.inc` re-runs V123 immediately before action, refuses absent/cached/guessed ID, and only then submits canonical `<currentID>:-1`. Marker `ACTION_V123`. MessageBox callback behavior remains unchanged.

## Build status
Source-bearing commit `79235f61ded9d393694be807c996128b082f67b4`, run `31939000139`: **CI/BUILD PASS**. Artifact ID `9261527342`; ZIP SHA-256 `1cb886860b998b955efe68164bf5379642a2c2bde739acb425c7a9a650ecee5f`; EXE SHA-256 `764fc10e98afd0fcd608bd9dd50c9fa86e51b467524cab4aebbe5001646d037a`; DLL SHA-256 `8d8b9e3feeecf0be509610ad990bbe3852761fb750d1862aee09d5f51eadb7f7`; exactly 9 required files verified.

Runtime v1.1.13: **UNTESTED**. BUG-001 OPEN.

## Required next log
- `T>0` -> capture `ACTION_V123` and fresh next state;
- `T=0,ST>0,SV>0` -> inspect `S`;
- `T=0,ST=0` -> inspect `MT`/`KS`;
- no packet/server diagnosis before `ACTION_V123 SENT`.

## Do not break
No guessed ID, no UIRoot tuning, no WaitTreatment reopen, no fixed-sleep success proof, no broad reverse, no change to runtime-proven MoonSharp execution without contrary evidence, and never interpret V122 `N` as selection count.

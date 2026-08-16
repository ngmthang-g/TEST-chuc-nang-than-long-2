# FEATURE: AUTO HEAL NPC / TRỊ LIỆU

## Goal
`route -> one NPC open -> observe current server GameDialog data -> current Treatment selection -> one semantic action -> fresh follow-up -> result proof`.
Only port after repeated runtime PASS.

## Mandatory startup
Read `AI_START_HERE.md`, V2 protocol, client-analysis TXT, project knowledge/changelog, this feature + BUG/DEC/EVID/history + current source. Use `clinent-game-than-long-DATA-2222/AI_INDEX.md` before targeted reverse work.

## Protected working path
- raw user-captured MapID/X/Y;
- mount / route / StopPath / dismount;
- Map 5 -> NPC 339 Đỗ Thanh Đằng;
- Map 3 -> NPC 463 alternate test candidate;
- semantic NPC interaction/open;
- no WaitTreatment NPC reopen after successful initial open.

## Runtime layer findings
### v1.1.8
GameDialog opens but UIRoot observer reports `clickable=0`, `texts=0`, `labels=<none>` until fail-closed. Therefore UIRoot/UIButton is not the active Treatment observer anymore.

### v1.1.9
User screenshot/live log shows GameDialog opens and then repeatedly:

`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`

This is a narrower runtime failure: v1.1.9 does not reach LuaEnv/DoString/Selections/Treatment ID/action. The Lua selection model itself remains untested by this run.

## Current implementation — v1.1.10
### Resolver
`src/bridge_lua_manager_v1_1_10.inc` fixes only the failing Lua manager/LuaEnv boundary.

Order:
1. resolve `LuaSystemManager` + `get_LuaEnv` semantically;
2. if getter is static, invoke it directly without manager instance;
3. otherwise try semantic `get_Instance` if exposed;
4. enumerate bounded static reference fields using verified IL2CPP field APIs; validate actual runtime class before accepting a manager candidate;
5. if class hierarchy proves a Unity object, try typed Unity object lookup;
6. fail closed with `LUA_MANAGER_V120` diagnostics if unresolved.

This avoids the v1.1.9 assumption that a named singleton path must exist.

### Lua dialog observer
Once LuaEnv resolves, v1.1.10 reuses the bounded runtime probe:
- `GUI.FindUI("GameDialog")` / `GUI.FindUI("AutoFight_Main")`;
- find current `Selections`;
- match `Trị liệu`, `Xác nhận`, `Ta biết rồi`;
- return T/C/K and resolver route as `LUA_DIALOG_V120`.

### GameDialog action
`src/bridge_action_v1_1_10.inc`:
- requires safe current game/managed thread prerequisite;
- re-reads current selections immediately before action;
- rejects absent/stale/guessed ID;
- submits canonical `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentID>:-1`;
- logs `ACTION_V120`;
- waits for a fresh server/UI state.

### Confirmation
If a live MessageBox exists, use semantic `ButtonOKClicked()`. Otherwise a current GameDialog `Xác nhận` selection uses its actual current ID.

## Canonical facts
- `Selections[selectionID]=visibleText`.
- no global Treatment ID.
- built-in AutoFight dialog logic inspects current selections.
- GameDialog request ID `100007`, payload `selectionID:SelectedItemID`.
- canonical KB verifies LuaSystemManager `get_LuaEnv/set_LuaEnv`, but not a stable singleton getter/field name.
- IL2CPP snapshot exposes metadata/static-field reflection used by the resolver.

## Safety / isolation
v1.1.10 does NOT:
- return to UIRoot/button scanning;
- add NPC reopen retries;
- hardcode Treatment ID;
- raw-scan heap/pointers;
- change route/state machine semantics;
- declare downstream packet/server failure before those stages are reached.

## Build status
Source-bearing v1.1.10 commit `d35517385266d4fa75011374966816a0e8d5ada1`, Actions run `31933118883`: **CI/BUILD PASS**. Artifact `ThanLongTestAutoHeal-v1.1.10`, ID `9259895908`, ZIP SHA-256 `4b43c205bedd4177288b562ed7df20b08adb5f295b6b1601a11699e0bb80ef60`.

Runtime v1.1.10: **UNTESTED**.

## Required next log
After the single NPC open, capture one of:
- `LUA_DIALOG_V120 • route=... • T=<id> ...` — LuaEnv/DoString reached;
- or exact `LUA_MANAGER_V120 ...` — resolver still unresolved.
If `T>0`, then capture `MAINTHREAD_PROOF`, `ACTION_V120`, next GameDialog/MessageBox and HP/money/result state.

## Do not break
- no guessed/cached ID;
- no UIRoot tuning;
- no WaitTreatment NPC reopen;
- no fixed sleep success proof;
- no broad reverse;
- BUILD PASS is not Runtime PASS.
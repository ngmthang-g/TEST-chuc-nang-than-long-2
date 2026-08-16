# FEATURE: AUTO HEAL NPC / TRỊ LIỆU

## Goal
`route -> one NPC open -> observe current server GameDialog data -> current Treatment selection -> one semantic action -> fresh follow-up -> result proof`.
Only port after repeated runtime PASS.

## Mandatory startup
Read `AI_START_HERE.md`, V2 protocol, client-analysis TXT, project knowledge/changelog, this feature + BUG/DEC/EVID/history + current source. Use `clinent-game-than-long-DATA-2222/AI_INDEX.md` before any targeted reverse work.

## Protected working path
- raw user-captured MapID/X/Y;
- mount / route / StopPath / dismount;
- Map 5 -> NPC 339 Đỗ Thanh Đằng;
- Map 3 -> NPC 463 alternate test candidate;
- semantic NPC interaction/open.

## v1.1.8 runtime conclusion
User test proves:
- travel/open works;
- exactly one NPC interaction occurs, so WaitTreatment anti-reopen works for that transaction;
- GameDialog presence exists but UIRoot scanner returns `clickable=0`, `texts=0`, `labels=<none>` until 15-second fail-closed;
- no Treatment selection ID/action is reached.

Therefore UIRoot/UIButton is no longer the active observer path. The stale `TEST v1.1.0` startup line in v1.1.8 was only an old UI literal; v1.1.9 corrects it.

## Current implementation — v1.1.9
### Observer
`src/bridge_lua_dialog_v1_1_9.inc`:
- resolve current Lua environment;
- dynamically resolve `DoString`;
- run bounded read-only Lua probe;
- inspect `GUI.FindUI("GameDialog")` and `GUI.FindUI("AutoFight_Main")` runtime tables;
- find `Selections` maps;
- match `Trị liệu`, `Xác nhận`, `Ta biết rồi`;
- return current T/C/K IDs and paths/samples as `LUA_DIALOG_V119`.

### GameDialog action
`src/bridge_action_v1_1_9.inc`:
- require safe current managed game context and CTS/MainThread proof prerequisite;
- re-read live selections immediately before action;
- reject absent/stale/guessed ID;
- submit canonical `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentID>:-1`;
- emit `ACTION_V119`;
- observe the next real state instead of assuming success.

### Confirmation
If a live `MessageBox` exists, use semantic `ButtonOKClicked()`. Otherwise a current GameDialog `Xác nhận` selection uses its live current ID.

## Why v1.1.9 is a real new experiment
v1.1.4 already knew the packet contract, but still depended on button discovery to obtain the ID. v1.1.8 runtime confirms this UI representation can be empty. v1.1.9 removes that common dependency by obtaining identity from runtime `Selections` itself.

## Canonical facts
- `Selections[selectionID]=visibleText`.
- current selection ID is runtime/server state; no global Treatment ID.
- built-in AutoFight dialog logic inspects current selections.
- GameDialog request ID = `100007`, payload = `selectionID:SelectedItemID`.
- ordinary function selection commonly uses `SelectedItemID=-1`.
- MessageBox OK is callback-based.
- `MainThread.Execute(System.Action)` remains preferred for live Unity/UI mutation architecture.

## Scope note
The v1.1.9 direct semantic GameDialog request is a narrow test-lab proof after the current UIButton representation failed. It is not blanket permission to bypass the production MainThread architecture for arbitrary actions.

## Build status
- source commit `1eac3b9eb55dae9a80d6fcba847c7bd7281fe3b7`.
- run `31932086373`: **CI/BUILD PASS**.
- artifact `ThanLongTestAutoHeal-v1.1.9`, ID `9259620117`.
- ZIP SHA-256 `823f04f60fad78720f3742d7e93a7c31b6e382f504fe44982c41cf9a967d648a`.
- EXE SHA-256 `3228bf03c493329af00c8014b47c9f486469a39b24b2c09add5df2061a450ad8`.
- DLL SHA-256 `22019c4c9f4f4aaf29db3c4c328d9a948d285f41186698c981335cc039df7ea9`.
- runtime v1.1.9: **UNTESTED**.

## Required next log
`one ClickNPC -> LUA_DIALOG_V119 -> T=<live id> -> MAINTHREAD_PROOF -> ACTION_V119 -> next GameDialog/MessageBox/result -> HP/money proof`.

## Do not break
- no guessed ID;
- no UIRoot depth/label tuning for this confirmed empty-representation failure;
- no stale ID;
- no WaitTreatment NPC reopen;
- no fixed sleep as success proof;
- no broad reverse;
- BUILD PASS is not Runtime PASS.

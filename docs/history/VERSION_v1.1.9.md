# VERSION v1.1.9-test — Lua GameDialogData / Selections observer

Date: 2026-08-16

## Runtime evidence that triggered this version
User tested the delivered v1.1.8 artifact against NPC 339 / Map 5.

Exact important behavior:
- mount / AutoPath / dismount succeeded;
- only one `Đã gọi ClickNPC npcID=339` occurred;
- GameDialog opened;
- for ~15 seconds observer repeated `DIALOG_V118 NO MATCH • UI=GameDialog • wanted=Trị liệu • clickable=0 • texts=0 • labels=<none>`;
- tool failed closed and did not reopen NPC.

This proves two separate things:
1. v1.1.8 anti-reopen component works in the tested transaction;
2. the active UIRoot/CoreChildren representation is empty for the dynamic GameDialog content required by this feature, so more UIButton/text-tree matching is the wrong next step.

The `TEST v1.1.0` startup text visible in that v1.1.8 log was a stale controller string only; v1.1.9 fixes the version label.

## Canonical knowledge used
Read-first sources from `ngmthang-g/clinent-game-than-long-DATA-2222` establish:
- `GameDialog.Selections[selectionID] = visibleText`;
- current IDs are server/runtime data;
- built-in AutoFight dialog flows inspect current selections;
- `CMD_SHOW_GAMEDIALOG=100007`;
- payload is `selectionID:SelectedItemID`, normally current ID with `-1` for ordinary function choices;
- MessageBox OK is callback-based.

No broad binary reverse was performed.

## Implementation
### `src/bridge_lua_dialog_v1_1_9.inc`
- resolves current LuaSystemManager / LuaEnv;
- dynamically resolves an instance `LuaEnv.DoString` overload;
- executes a bounded read-only Lua probe;
- starts from `GUI.FindUI("GameDialog")` and `GUI.FindUI("AutoFight_Main")`;
- searches runtime Lua tables for `Selections`;
- matches `Trị liệu`, `Xác nhận`, `Ta biết rồi`;
- returns current T/C/K IDs plus path/sample diagnostics under `LUA_DIALOG_V119`.

### `src/bridge_action_v1_1_9.inc`
- requires current SafeForAction/managed game context and the harmless CTS/MainThread proof prerequisite;
- re-reads current Lua selections immediately before action;
- rejects stale/guessed IDs;
- submits the canonical current GameDialog selection request with `<liveID>:-1`;
- logs `ACTION_V119`;
- if a real MessageBox is current, uses semantic `ButtonOKClicked()` rather than pretending it is a GameDialog selection.

### Wiring
- v1.1.8 UIRoot observer is kept under legacy names only for lineage/old helper wiring;
- v1.1.9 owns active `InspectHealDialog` and `ClickHealDialogChoice`;
- protocol bumped to `0x00010109`;
- controller title/startup text/artifact naming corrected to v1.1.9;
- WaitTreatment still has no NPC reopen retry.

## Why this is not merely v1.1.4 again
v1.1.4 knew the exact GameDialog request but still depended on button discovery to obtain the current selection ID. Because the shared discovery layer was unproven, its full-chain runtime failure did not independently establish that the request stage was reached.

v1.1.9 obtains the current ID from runtime `Selections` data. Only a log showing both a live ID and `ACTION_V119` justifies reasoning about server/request behavior.

## Build evidence
- source commit: `1eac3b9eb55dae9a80d6fcba847c7bd7281fe3b7`.
- GitHub Actions run: `31932086373`.
- architecture audit: PASS.
- Route FSM self-test: PASS.
- Heal FSM self-test: PASS.
- bridge DLL build + PE verification: PASS.
- controller EXE build: PASS.
- artifact upload: PASS.
- artifact: `ThanLongTestAutoHeal-v1.1.9`, ID `9259620117`.
- ZIP SHA-256: `823f04f60fad78720f3742d7e93a7c31b6e382f504fe44982c41cf9a967d648a`.
- EXE SHA-256: `3228bf03c493329af00c8014b47c9f486469a39b24b2c09add5df2061a450ad8`.
- DLL SHA-256: `22019c4c9f4f4aaf29db3c4c328d9a948d285f41186698c981335cc039df7ea9`.

BUILD/CI PASS is not Runtime PASS.

## Runtime status
**UNTESTED** for v1.1.9. BUG-001 remains OPEN.

## Next required trace
`one ClickNPC -> LUA_DIALOG_V119 -> live T ID -> MAINTHREAD_PROOF -> ACTION_V119 -> next dialog/MessageBox/result -> HP/money proof`.

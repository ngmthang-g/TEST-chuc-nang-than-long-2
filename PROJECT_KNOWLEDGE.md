# PROJECT KNOWLEDGE

## Identity / startup
- Repo: `ngmthang-g/TEST-chuc-nang-than-long-2`; branch `agent/auto-tri-lieu-v1.1.0`.
- Current version: `v1.1.9-test`; full Auto trị liệu known-good: **NONE**.
- Before every version read: `AI_START_HERE.md` -> `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md` -> `AI_CLIENT_ANALYSIS_RULES.txt` -> this file -> `CHANGELOG.md` -> affected BUG/DEC/EVID/feature/history -> current source.
- Client facts must start from `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`; no broad reverse for facts already in canonical VERIFIED/database docs.
- BUILD/CI PASS != RUNTIME PASS.

## v1.1.8 runtime evidence — 2026-08-16
User tested delivered v1.1.8 on PID 4324, Map 5, NPC 339 Đỗ Thanh Đằng, captured raw coordinate `9442,5510`.

Observed:
- mount / AutoPath / dismount PASS;
- exactly one `Đã gọi ClickNPC npcID=339`;
- GameDialog opened;
- for the full 15-second guard every observer line was `DIALOG_V118 NO MATCH • UI=GameDialog • wanted=Trị liệu • clickable=0 • texts=0 • labels=<none>`;
- final fail closed explicitly did not call NPC again.

Therefore:
1. v1.1.8 anti-reopen behavior is **RUNTIME PASS for this transaction**.
2. v1.1.8 UIRoot/CoreChildren observer is **RUNTIME FAILED as the representation for this dynamic GameDialog**: dialog presence exists but zero scanned text/clickable nodes exist.
3. Treatment selection/action was never reached, so this run does not prove an action/server failure.
4. The startup line `TEST v1.1.0` was only a stale controller string; v1.1.9 corrects the visible version.

## Canonical GameDialog contract
- `GameDialog.Selections[selectionID] = visibleText`.
- Current IDs are server/runtime data; never hardcode Treatment ID.
- Built-in AutoFight dialog flows inspect current server-provided selections.
- `CMD_SHOW_GAMEDIALOG = 100007`; payload `selectionID:SelectedItemID`, commonly current ID with `-1` for an ordinary function choice.
- MessageBox OK follows its stored callback path.

## v1.1.9 active design
Stop repairing UIRoot/button matching for Treatment. Active flow is:

`one NPC open -> Lua runtime dialog probe -> enumerate current Selections -> match semantic text -> re-read current Selections at action time -> submit current selection -> wait fresh server/UI state`.

New active files:
- `src/bridge_lua_dialog_v1_1_9.inc`: resolves current Lua environment, runs a bounded read-only probe starting from `GUI.FindUI("GameDialog")` and `GUI.FindUI("AutoFight_Main")`, finds runtime `Selections`, returns T/C/K live IDs and path/sample diagnostics as `LUA_DIALOG_V119`.
- `src/bridge_action_v1_1_9.inc`: requires current safe managed game context/CTS proof, re-reads live selections immediately before action, refuses cached/guessed IDs, submits the canonical current GameDialog selection, logs `ACTION_V119`. Current MessageBox confirm uses semantic `ButtonOKClicked()`.
- v1.1.8 UIRoot observer remains only for lineage/legacy helper wiring, not active Treatment observation.
- protocol = `0x00010109`; controller title/start log/artifact names = v1.1.9.

The direct GameDialog semantic request in v1.1.9 is a **test-lab proof only**, not a blanket replacement for the preferred MainThread production mutation architecture.

## Build evidence
- v1.1.7 final run `31925922772`: FAILED.
- v1.1.8: BUILD PASS; runtime anti-reopen partial PASS + observer representation FAIL.
- v1.1.9 source commit: `1eac3b9eb55dae9a80d6fcba847c7bd7281fe3b7`.
- Actions run `31932086373`: **CI/BUILD PASS** including architecture audit, Route/Heal self-tests, bridge DLL/PE check, controller build and artifact upload.
- Artifact: `ThanLongTestAutoHeal-v1.1.9`, ID `9259620117`.
- ZIP SHA-256: `823f04f60fad78720f3742d7e93a7c31b6e382f504fe44982c41cf9a967d648a`.
- EXE SHA-256: `3228bf03c493329af00c8014b47c9f486469a39b24b2c09add5df2061a450ad8`.
- DLL SHA-256: `22019c4c9f4f4aaf29db3c4c328d9a948d285f41186698c981335cc039df7ea9`.
- v1.1.9 runtime: **UNTESTED**.

## Hard rules
- no inferred NPC X/Y;
- no fixed Treatment selection ID;
- no stale UI pointer or stale selection ID across dialog transitions;
- no fixed sleep as success proof;
- no WaitTreatment NPC reopen;
- do not return to UIRoot depth/label tuning for this confirmed empty-representation failure;
- one mutable action in flight per PID;
- preserve implementation/build/runtime evidence separately.

## BUG-001 current status
OPEN. Confirmed resolved component: no controller-driven reopen in the tested v1.1.8 transaction. Confirmed failing component: active v1.1.8 UIRoot observer sees zero actionable nodes despite GameDialog presence. Unknown: live Lua selection path and server/follow-up result in v1.1.9.

## Next runtime evidence
Capture from first NPC open:
`LUA_DIALOG_V119` (T/C/K + raw paths/samples) -> `MAINTHREAD_PROOF` -> `ACTION_V119` live ID/result -> next GameDialog/MessageBox -> HP/money/final state.

See `docs/features/AUTO_HEAL_NPC.md`, `docs/bugs/BUG_REGISTRY.md`, `docs/evidence/EVIDENCE_REGISTRY.md`, `docs/decisions/DECISIONS.md`, `docs/history/VERSION_v1.1.9.md`.

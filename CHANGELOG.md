# CHANGELOG

## [v1.1.9-test] - 2026-08-16

### Trigger / Runtime Evidence
- User tested delivered v1.1.8 against NPC 339 / Map 5.
- Route, mount, AutoPath, dismount and NPC interaction succeeded.
- Exactly one `Đã gọi ClickNPC npcID=339` occurred: v1.1.8 WaitTreatment anti-reopen PASS for this transaction.
- For the next 15 seconds every observer probe returned `DIALOG_V118 NO MATCH • UI=GameDialog • wanted=Trị liệu • clickable=0 • texts=0 • labels=<none>`.
- Controller failed closed and did not reopen NPC.
- Therefore v1.1.8 dies at observer representation before Treatment selection/action; this run does not prove MainThread UIButton or server request failure.
- Stale startup literal `TEST v1.1.0` was an old UI string only; corrected in v1.1.9.

### Investigation / Correction
- Stop repairing UIRoot depth/text/button matching for this dynamic GameDialog. The tested bridge representation contains zero relevant nodes while GameDialog is present.
- Canonical client KB gives stronger state: `GameDialog.Selections[selectionID]=visibleText`; built-in AutoFight dialog logic inspects current server selections.
- Exact GameDialog request remains canonical: command `100007`, payload `selectionID:SelectedItemID`.
- v1.1.4 knew that request but still relied on the shared button observer to obtain live selectionID, so its old failure did not independently prove request execution.

### Added / Changed
- Added `src/bridge_lua_dialog_v1_1_9.inc`:
  - resolves current Lua environment and `DoString`;
  - runs bounded read-only probe from `GUI.FindUI("GameDialog")` / `GUI.FindUI("AutoFight_Main")`;
  - searches runtime tables for `Selections`;
  - matches Treatment/Confirm/Acknowledge text and reports current T/C/K IDs plus paths/samples as `LUA_DIALOG_V119`.
- Added `src/bridge_action_v1_1_9.inc`:
  - requires safe managed game context + CTS/MainThread proof prerequisite;
  - re-reads current selections immediately before action;
  - refuses cached/static IDs;
  - submits current GameDialog selection using the canonical request and emits `ACTION_V119`;
  - MessageBox confirmation uses semantic `ButtonOKClicked()` when MessageBox is current.
- v1.1.8 UIRoot observer retained only under legacy names for lineage/old helper wiring.
- Protocol bumped to `0x00010109`.
- Controller title/startup log/artifact naming corrected to v1.1.9.
- WaitTreatment still contains no NPC reopen retry.

### Scope
The v1.1.9 direct semantic GameDialog request is a narrow test-lab proof after the current UIButton representation failed. It is not a blanket replacement for the preferred production MainThread architecture.

### Build
- Source commit: `1eac3b9eb55dae9a80d6fcba847c7bd7281fe3b7`.
- GitHub Actions run `31932086373`: **CI/BUILD PASS**.
- Architecture audit PASS; Route FSM PASS; Heal FSM PASS; bridge DLL build/PE verification PASS; controller build PASS; artifact upload PASS.
- Artifact `ThanLongTestAutoHeal-v1.1.9`, ID `9259620117`.
- ZIP SHA-256 `823f04f60fad78720f3742d7e93a7c31b6e382f504fe44982c41cf9a967d648a`.
- EXE SHA-256 `3228bf03c493329af00c8014b47c9f486469a39b24b2c09add5df2061a450ad8`.
- DLL SHA-256 `22019c4c9f4f4aaf29db3c4c328d9a948d285f41186698c981335cc039df7ea9`.

### Runtime
- v1.1.9: **UNTESTED**.
- BUG-001 remains OPEN.

### Next Evidence
`one ClickNPC -> LUA_DIALOG_V119 -> live T ID -> MAINTHREAD_PROOF -> ACTION_V119 -> next GameDialog/MessageBox/result -> HP/money proof`.

## [v1.1.8-test] - 2026-08-16
- Added all-descendant UIRoot scan, live button Tag gate, MainThread queued UIButton action and removed WaitTreatment NPC reopen.
- Source run `31926671467` and final-head run `31926929828`: BUILD PASS.
- Runtime update from user test: travel/open PASS; anti-reopen PASS for tested transaction; full Treatment FAIL because active UIRoot observer returned `clickable=0/texts=0` despite GameDialog presence and never reached selection/action.

## [v1.1.7-test] - 2026-08-16
- Added descendant observer/presence guard.
- Later audit found first-text-only matching and remaining transient-absence NPC retry.
- final run `31925922772`: CI FAILED at controller compile; no valid handoff artifact.

## [v1.1.6-test] - 2026-08-16
- Added mandatory project-memory files/startup pointer.
- Added CTS proof + `MainThread.Execute(System.Action)` experiment.
- final run `31924151093`: CI/BUILD PASS.

## [v1.1.5-test] - 2026-08-16
- Alternate NPC 463 / Lạc Dương reproduced same overall Treatment symptom.
- run `31909204317`: BUILD PASS; runtime full flow FAIL.

## [v1.1.4-test] - 2026-08-16
- Tried live Tag/selectionID + source-verified GameDialog request.
- Later correction: shared button discovery still gated live ID/reachability, so request failure was not independently proven.

## [v1.1.3-test] - 2026-08-16
- Tried `GameDialog.FunctionButtonClicked(liveButton)` via ExecuteUIObject; full flow failed, shared discovery remained.

## [v1.1.2-test] - 2026-08-16
- Changed UI-root discovery order; route/NPC open partial PASS, Treatment failed.

## [v1.1.1-test] - 2026-08-16
- Removed inferred X/Y; added runtime coordinate capture.

## [v1.1.0-test] - 2026-08-16
- Initial Auto Heal route/state-machine test.

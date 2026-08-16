# CHANGELOG

## [v1.1.10-test] - 2026-08-16

### Trigger / runtime evidence
- User tested delivered v1.1.9 on PID 4324 / Map 5 / NPC 339.
- NPC dialog opens; tool status remains `DIALOG ĐÃ MỞ • CHỜ LIVE SELECTION TRỊ LIỆU`.
- Repeated exact failure: `LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`.
- Therefore v1.1.9 fails before `LuaEnv.DoString`, `Selections`, Treatment selectionID and `ACTION_V119`; no packet/server failure is established.

### Investigation / correction
- Source audit found `ResolveLuaEnvV119()` always required a manager instance before checking whether `get_LuaEnv` itself is static.
- The v1.1.9 manager resolver only tried semantic `get_Instance` plus four guessed static field names.
- Canonical client KB verifies `LuaSystemManager.get_LuaEnv/set_LuaEnv` and IL2CPP reflection exports, but does not verify a stable manager singleton contract.
- No broad binary reverse was performed; fix is bounded metadata/runtime resolution at the exact failing boundary.

### Added / changed
- Added `src/bridge_lua_manager_v1_1_10.inc`.
- First tries **static `LuaSystemManager.get_LuaEnv` directly**.
- If an instance is required: tries semantic `get_Instance`, then enumerates static reference fields on the manager hierarchy and known Lua bridge classes. Candidate values are accepted only when runtime class validation proves LuaSystemManager/derived.
- Adds typed Unity object lookup fallback only when LuaSystemManager is actually in the UnityEngine.Object hierarchy.
- Failure now emits `LUA_MANAGER_V120` diagnostics instead of generic instance-unresolved.
- Adds `LUA_DIALOG_V120` resolver-route diagnostics and `ACTION_V120` re-read/send path.
- v1.1.9 implementation retained under legacy names for lineage.
- Protocol/title/startup log/artifact bumped to v1.1.10.
- Route/NPC/no-WaitTreatment-reopen behavior unchanged.

### Build
- Source-bearing build commit `d35517385266d4fa75011374966816a0e8d5ada1`.
- Actions run `31933118883`: **CI/BUILD PASS**.
- Architecture audit PASS; Route FSM PASS; Heal FSM PASS; bridge DLL + PE verification PASS; controller compile PASS; artifact upload PASS.
- Artifact `ThanLongTestAutoHeal-v1.1.10`, ID `9259895908`.
- ZIP SHA-256 `4b43c205bedd4177288b562ed7df20b08adb5f295b6b1601a11699e0bb80ef60`.

### Runtime
- v1.1.10: **RUNTIME UNTESTED**.
- BUG-001 remains OPEN.

### Next evidence
`one NPC open -> LUA_MANAGER_V120/LUA_DIALOG_V120 -> T=<live id> -> MAINTHREAD_PROOF -> ACTION_V120 -> next server/UI/result state`.

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
- Stopped repairing UIRoot depth/text/button matching for this dynamic GameDialog.
- Canonical client KB gives stronger state: `GameDialog.Selections[selectionID]=visibleText`; built-in AutoFight dialog logic inspects current server selections.
- Exact GameDialog request: command `100007`, payload `selectionID:SelectedItemID`.
- v1.1.4 knew that request but still relied on shared button observer to obtain live selectionID, so its old failure did not independently prove request execution.

### Added / Changed
- Added `src/bridge_lua_dialog_v1_1_9.inc` and `src/bridge_action_v1_1_9.inc`.
- Intended flow: resolve LuaEnv/DoString -> inspect current Selections -> re-read current ID -> semantic request.
- v1.1.8 UIRoot observer retained only as lineage.
- Protocol/title/artifact bumped to v1.1.9.

### Build
- Source commit `1eac3b9eb55dae9a80d6fcba847c7bd7281fe3b7`.
- run `31932086373`: **CI/BUILD PASS**; artifact ID `9259620117`.

### Runtime update
- **RUNTIME FAIL at resolver boundary**: user v1.1.9 log repeatedly reports `LuaSystemManager instance unresolved` after GameDialog is open.
- LuaEnv/DoString/Selections/action are NOT reached.

## [v1.1.8-test] - 2026-08-16
- Added all-descendant UIRoot scan, live button Tag gate, MainThread queued UIButton action and removed WaitTreatment NPC reopen.
- Source/final builds PASS.
- Runtime: travel/open PASS; anti-reopen PASS for tested transaction; full Treatment FAIL because active UIRoot observer returned `clickable=0/texts=0` despite GameDialog presence.

## [v1.1.7-test] - 2026-08-16
- Descendant observer/presence guard; later audit found first-text-only matching and transient-absence retry.
- final CI `31925922772` FAILED; no valid handoff artifact.

## [v1.1.6-test] - 2026-08-16
- Added mandatory project-memory files/startup pointer and CTS/MainThread proof experiment.
- CI/BUILD PASS.

## [v1.1.5-test] - 2026-08-16
- Alternate NPC 463 / Lạc Dương reproduced overall Treatment symptom; BUILD PASS.

## [v1.1.4-test] - 2026-08-16
- Tried live Tag/selectionID + exact GameDialog request; later correction: shared button discovery still gated reachability.

## [v1.1.3-test] - 2026-08-16
- Tried `GameDialog.FunctionButtonClicked(liveButton)` through ExecuteUIObject; shared discovery remained.

## [v1.1.2-test] - 2026-08-16
- Changed UI-root discovery order; route/NPC open partial PASS, Treatment failed.

## [v1.1.1-test] - 2026-08-16
- Removed inferred X/Y; added runtime coordinate capture.

## [v1.1.0-test] - 2026-08-16
- Initial Auto Heal route/state-machine test.
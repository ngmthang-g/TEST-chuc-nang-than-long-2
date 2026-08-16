# CHANGELOG

## [v1.1.8-test] - 2026-08-16

### Requested
- Continue Auto trị liệu instead of stopping at unfinished v1.1.7.
- Keep `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md` and `AI_CLIENT_ANALYSIS_RULES.txt` as mandatory per-version startup memory.
- Use `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md` and canonical KB first; no broad reverse.
- Investigate current user report that opening the NPC still causes Treatment-dialog flicker.

### Investigation / Correction
- v1.1.7 final CI run `31925922772` was not pending: it completed **FAIL** at controller compilation because the `kTitle` macro/scope wiring created an unused global and left `controller_part02.inc` unable to see `kTitle`.
- v1.1.7 descendant observer did not actually search all labels: `FirstTextInSubtreeV117` stopped at the first non-empty descendant text.
- v1.1.7 still retained a `WaitTreatment` retry that called `ClickNPC` after 4 seconds whenever Lua GameDialog presence was temporarily absent. A server-driven destroy/recreate interval could therefore still reopen the NPC and recreate visible flicker.
- Canonical client data confirms stronger semantic identity: current `Selections[selectionID] = visibleText`; generated current button stores the same selectionID in `Tag`.
- New user flicker report is recorded as current runtime evidence, but exact tested artifact/version was not stated and is therefore not falsely assigned to v1.1.7.

### Added / Changed / Fixed
- Added `src/bridge_dialog_v1_1_8.inc`.
- Added all-descendant semantic label matching (`SubtreeHasSemanticTextV118`).
- Added live GameDialog `Tag`/selectionID extraction and explicit diagnostics.
- Added `src/bridge_action_v1_1_8.inc` semantic gate: GameDialog choices fail closed unless freshly matched current button yields `selectionID > 0`.
- Retained v1.1.6 CTS/MainThread proof and `MainThread.Execute(System.Action)` as mutation boundary.
- Removed `WaitTreatment` NPC reopen loop. After initial NPC interaction the transaction waits for current dialog state and times out fail-closed instead of calling `ClickNPC` again.
- Removed broken `kTitle` macro override; canonical title now lives in `controller_part01.inc` as v1.1.8.
- Updated artifact names to v1.1.8.
- Architecture audit uses encoding-safe symbol checks for v1.1.8 logic.

### Build
- Inherited v1.1.7 final run `31925922772`: **CI FAILED** (`kTitle` compile error).
- v1.1.8 source commit: `1da643b8384dfa64a2523938dffb4ddd9885b181`.
- GitHub Actions run `31926671467`: **CI PASS / BUILD PASS**.
- Architecture audit, Route FSM tests, Heal FSM tests, bridge DLL/PE verification, controller EXE build and artifact upload all PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.8`, artifact ID `9258076757`.
- Artifact digest: `sha256:390ae27248eebee99db7b1a6a463e291a797150a23de770aeed6d29aed8cd9d5`.
- BUILD/CI PASS is not promoted to runtime status.

### Runtime
- v1.1.8: **RUNTIME UNTESTED**.
- Latest user observation before v1.1.8: Treatment dialog still flickers when NPC is opened; tested artifact/version UNKNOWN.
- Awaiting: one-transaction proof, `DIALOG_V118`, live `selectionID`, `MAINTHREAD_PROOF`, `ACTION_V118`, next dialog/result state.

### Related Bugs / Evidence
- BUG-001 remains OPEN.
- EVID-007: latest user flicker report, artifact version unknown.
- EVID-008: source audit proves v1.1.7 first-text-only matcher + residual WaitTreatment ClickNPC retry.

### Next Version Notes
- Do not change packet or rotate NPC again before one v1.1.8 runtime trace proves which stage is reached.
- If live selectionID is resolved and `ACTION_V118` is enqueued but no state changes, only then compare the exact business callback/request against manual behavior.

## [v1.1.7-test] - 2026-08-16

### Requested
- Keep uploaded V2 knowledge protocol MD and client-analysis TXT as mandatory per-version memory/read files.
- Continue investigating Auto trị liệu because opening the NPC still produces the visible Treatment-dialog flicker/no progression.
- Use canonical `clinent-game-than-long-DATA-2222` knowledge first; no broad reverse.

### Investigation / Correction
- Re-audited the original runtime screenshot/log and shared source instead of changing the action callback again.
- Found a common dependency across the old experiments: `InspectHealDialog -> FindButtonInUi -> WalkForButton` only matched when the **same UI object** had both matching `get_Text()` and `HandleClickEvent()`.
- Original runtime evidence showed repeated `ClickNPC` while status remained `CHỜ DIALOG TRỊ LIỆU` even though GameDialog was visibly open.
- Controller only performs this NPC retry when Treatment is not detected.
- New causal theory: observer misses the dynamic button/UI root, controller reopens NPC, and the repeated dialog reconstruction itself creates the flicker.
- Corrected historical wording: old full-chain runtime failures remain real, but it is not proven that every attempted Treatment action layer was actually reached.

### Added / Changed / Fixed
- Added `src/bridge_dialog_v1_1_7.inc`.
- Dynamic clickable discovery now searches descendant label text rather than requiring Text + HandleClickEvent on identical object.
- Normalizes whitespace, NBSP and simple markup tags.
- Logs clickable candidate count/labels with `DIALOG_V117 MATCH/NO MATCH` diagnostics.
- Distinguishes Lua GameDialog presence from UIRoot readiness.
- If GameDialog already exists during WaitTreatment, tool no longer interprets temporary button-resolution failure as a reason to reopen NPC.
- Kept v1.1.6 MainThread action boundary unchanged so v1.1.7 isolates the observer/discovery variable.
- Updated controller title/build artifact to v1.1.7.

### Files / Modules
- `src/bridge_dialog_v1_1_7.inc`
- `src/bridge.cpp`
- `src/controller.cpp`
- `build.cmd`
- `.github/workflows/build.yml`
- `PROJECT_KNOWLEDGE.md`
- `docs/features/AUTO_HEAL_NPC.md`
- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/history/VERSION_v1.1.7.md`

### Build
- Initial v1.1.7 CI run `31925301297`: **CI FAILED before compiler** because architecture audit searched for a Vietnamese UTF-8 literal and PowerShell runner text decoding did not match it.
- Correction: audit changed to encoding-safe ASCII symbols (`FirstTextInSubtreeV117`, `WalkForButtonV117`, `DIALOG_V117`, `gameDialogExists`).
- Final v1.1.7 run `31925922772`: **CI FAILED** at controller compile (`kTitle` scope/wiring). This correction was discovered during v1.1.8 work.

### Runtime
- Status: **RUNTIME UNTESTED as a verified v1.1.7 artifact**.
- Confirmed working inherited: target capture, route, dismount, NPC opening.

### Related Bugs / Evidence
- BUG-001 remains OPEN.
- EVID-005: original repeated ClickNPC while visible dialog exists.
- EVID-006: shared old matcher required same object Text + HandleClickEvent.

## [v1.1.6-test] - 2026-08-16

### Requested
- Make V2 engineering-lineage protocol and client-analysis TXT mandatory startup memory.
- Re-investigate Treatment after v1.1.5 same symptom on second NPC/map.

### Added / Changed
- Added `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`, `AI_CLIENT_ANALYSIS_RULES.txt`, `AI_START_HERE.md`.
- Added MainThread queued action experiment with harmless CTS proof.
- Historical direct packet helper removed from active compilation while preserved in source/history.

### Build
- Initial build failed because inactive historical helper became unused under `-Werror`.
- Final run `31924151093`: CI/BUILD PASS; Route 8/8; Heal 7/7; bridge/PE/controller PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.6`.

### Runtime
- Preserved status before v1.1.7 correction: RUNTIME UNTESTED / no artifact-version-confirmed log.

## [v1.1.5-test] - 2026-08-16

### Changed
- Added Long Phá Thiên `463` / Lạc Dương `MapID 3` as second NPC/map test.
- Kept Đỗ Thanh Đằng `339` / Lâu Lan `MapID 5`.
- Coordinates remained raw user-captured runtime values.

### Build
- Run `31909204317`: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.5`.

### Runtime
- RUNTIME FAIL full Treatment flow: same visible flicker/no progression on second NPC/map.

## [v1.1.4-test] - 2026-08-16
- Tried live Tag/selectionID + source-verified `CMD_SHOW_GAMEDIALOG=100007` payload.
- BUILD PASS; full runtime flow did not progress on NPC 339.
- Later correction: action-stage reachability is not proven independently because shared discovery remained.

## [v1.1.3-test] - 2026-08-16
- Tried `GameDialog.FunctionButtonClicked(liveButton)` via ExecuteUIObject.
- Full runtime flow did not progress.
- Later correction: shared discovery remained a common dependency.

## [v1.1.2-test] - 2026-08-16
- Changed UI root discovery order.
- Route + NPC open PARTIAL PASS; Treatment full flow still failed.

## [v1.1.1-test] - 2026-08-16
- Removed inferred/hardcoded NPC X/Y.
- Added `TỰ LẤY TỌA ĐỘ NPC` and persistence.

## [v1.1.0-test] - 2026-08-16
- Initial Auto Heal route/state-machine test.
- NPC 339 + dynamic GameDialog/MessageBox inspection.

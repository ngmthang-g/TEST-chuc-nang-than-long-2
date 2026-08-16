# CHANGELOG

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
- Final build result: pending at this changelog write; must be updated before handoff.

### Runtime
- Status: **RUNTIME UNTESTED**.
- Confirmed working inherited: target capture, route, dismount, NPC opening.
- Awaiting proof: exact `DIALOG_V117` discovery branch and whether flicker stops before action.

### Related Bugs / Evidence
- BUG-001 remains OPEN.
- EVID-005: original repeated ClickNPC while visible dialog exists.
- EVID-006: shared old matcher required same object Text + HandleClickEvent.

### Next Version Notes
- If `DIALOG_V117 MATCH` appears and MainThread action is definitely enqueued but no state transition follows, then do the narrow manual-vs-tool action/request trace.
- If `NO MATCH` reports useful labels or Lua UI/Root mismatch, fix that exact observer layer only.
- Do not rotate NPCs or broad-reverse before this evidence.

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

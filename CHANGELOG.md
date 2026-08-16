# CHANGELOG

## [v1.1.6-test] - 2026-08-16

### Requested
- Make the uploaded V2 engineering-lineage protocol and client-analysis TXT mandatory startup memory for every version.
- Re-investigate Treatment after v1.1.5 still flickers/no-progress on the second NPC/map.

### Added / Changed / Fixed
- Added root `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`.
- Added root `AI_CLIENT_ANALYSIS_RULES.txt`.
- Added `AI_START_HERE.md` with mandatory per-version read order and canonical client routing.
- Added `src/bridge_mainthread_v1_1_6.inc`.
- v1.1.6 constructs a legitimate `System.Action(target=live UIButton, callback=HandleClickEvent)` and enqueues it through `FGStudio.Engine.Utilities.MainThread.Execute(Action)` for later Unity Update execution instead of treating direct hook mutation as authoritative.
- Added harmless MainThread bridge proof using `CancellationTokenSource.Cancel()`; real button action is blocked until later polling observes `IsCancellationRequested false -> true`.
- Historical v1.1.4 direct packet helper remains in source/history but is no longer compiled into active v1.1.6 bridge.
- Removed obsolete duplicate v1.1.5 CI workflow.

### Investigation
- v1.1.5 Long Phá Thiên/Lạc Dương shows the same Treatment-dialog flicker/non-progression as Đỗ Thanh Đằng/Lâu Lan.
- This strongly weakens/disproves NPC 339/Lâu Lan as the sole root cause.
- Canonical `BUILD_MAINTHREAD_BRIDGE` / `MAINTHREAD_BRIDGE_V1` says the message hook is a producer context, but mutable callbacks should still be queued through `MainThread.Execute(Action)` and execute later in normal Unity Update.
- Root cause status: **LIKELY common action execution-boundary problem; not yet CONFIRMED**.

### Files / Modules
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
- `AI_CLIENT_ANALYSIS_RULES.txt`
- `AI_START_HERE.md`
- `PROJECT_KNOWLEDGE.md`
- `docs/features/AUTO_HEAL_NPC.md`
- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/decisions/DECISIONS.md`
- `docs/history/VERSION_v1.1.6.md`
- `src/bridge_mainthread_v1_1_6.inc`
- `src/bridge.cpp`
- `src/controller_part01.inc`
- `build.cmd`
- `.github/workflows/build.yml`
- removed `.github/workflows/build_v1_1_5.yml`

### Build
- Initial CI run `31923691864` / commit `be99b7d...`: **FAILED** because historical `ClickHealDialogChoiceV114` became unused and `-Werror,-Wunused-function` rejected the bridge compile.
- Correction: removed v1.1.4 helper from active compilation; retained v1.1.3 helper with explicit `[[maybe_unused]]` reference because its compilation unit still supplies shared current UI/state/NPC logic.
- Final source commit: `e54d537d8870c6e7e131816f3b7e78b60f10a62f`.
- Final GitHub Actions run: `31924151093` — **CI PASS / BUILD PASS**.
- Architecture audit: PASS.
- Route FSM: **8/8 PASS**.
- Heal FSM: **7/7 PASS**.
- Bridge DLL + PE verification: PASS (`0x2022`).
- Controller EXE: PASS.
- Artifact upload: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.6`.
- Artifact ID: `9257330034`.
- Artifact ZIP SHA256: `452e0093090b4ce3b7c00f7e4fc815b384d880554ebdfc4ff6b01bbee20cbd34`.

### Runtime
- Status: **RUNTIME UNTESTED** for v1.1.6.
- Confirmed working inherited: coordinate capture, route, dismount, NPC opening.
- Still failing in last tested version: Treatment progression.
- Awaiting test: CTS MainThread proof and queued live UIButton action.

### Regression / Known-Good / Related Bugs
- BUG-001 remains OPEN.
- No full Auto Heal known-good version exists.
- v1.1.5 A/B test is RUNTIME FAIL at Treatment on NPC 463/Lạc Dương.

### Next Version Notes
- If CTS proof fails, fix only the indicated delegate/MainThread stage.
- If CTS proof passes but Treatment still flickers, perform a targeted manual-vs-tool GameDialog/request/state trace; do not rotate NPCs again or broad-reverse the client.

## [v1.1.5-test] - 2026-08-16

### Changed
- Added Long Phá Thiên `463` / Lạc Dương `MapID 3` as second NPC/map isolation target.
- Existing Đỗ Thanh Đằng `339` / Lâu Lan `MapID 5` remains available.
- NPC selected from captured target map: Map 3 -> 463, Map 5 -> 339.
- Coordinates remain raw user-captured runtime values; no hardcoded NPC X/Y.
- Kept v1.1.4 semantic packet action unchanged to isolate NPC/map.

### Build
- GitHub Actions run `31909204317`: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.5`.
- SHA256: `652a12e2f454d1d8bee6d5025512825d98262e38a567d69b6597b1143daedf93`.

### Runtime
- **RUNTIME FAIL at Treatment** on NPC 463/Lạc Dương: user reports same visible flicker and no progression as NPC 339/Lâu Lan.
- Route/NPC-open remains partial working behavior.

## [v1.1.4-test] - 2026-08-16

### Changed
- `Trị liệu` / `Ta biết rồi`: read live `UIButton.Tag` to obtain runtime selectionID.
- Sent source-verified `CMD_SHOW_GAMEDIALOG=100007`, payload `selectionID:selectedItemID`, default `-1`.

### Build
- GitHub Actions run `31906609147`: PASS.

### Runtime
- RUNTIME FAIL on NPC 339/Lâu Lan: same flicker/no Treatment transition.

## [v1.1.3-test] - 2026-08-16

### Changed
- Used `GameDialog.FunctionButtonClicked(liveButton)` via `ExecuteUIObject`; MessageBox semantic callback for confirmation.

### Runtime
- RUNTIME FAIL at Treatment: same visible flicker/no transition.

## [v1.1.2-test] - 2026-08-16

### Fixed
- UI root discovery order adjustment.

### Runtime
- PARTIAL PASS: route + `ClickNPC(339)` opens correct NPC.
- FAIL: Treatment does not advance.

## [v1.1.1-test] - 2026-08-16

### Changed
- Removed inferred/hardcoded NPC X/Y.
- Added `TỰ LẤY TỌA ĐỘ NPC` and persistence.

## [v1.1.0-test] - 2026-08-16

### Added
- Initial Auto Heal route/state-machine test.
- NPC 339 and dynamic GameDialog/MessageBox inspection.

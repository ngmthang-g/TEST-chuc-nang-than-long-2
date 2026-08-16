# VERSION v1.1.6-test — 2026-08-16

## A. Identity / Lineage
- Version: `v1.1.6-test`
- Based On: `v1.1.5-test`
- Reason Created: v1.1.5 reproduced the same Treatment flicker/no-progress on Long Phá Thiên/Lạc Dương as Đỗ Thanh Đằng/Lâu Lan, shifting investigation from NPC-specific theory to the common external action execution boundary.
- Previous Runtime State: route/NPC opening partial working; Treatment RUNTIME FAIL.
- Last Known-Good: no full Auto Heal known-good; route/NPC-open partial known-good only.
- Regression From: none; continuing unresolved BUG-001.
- Supersedes: v1.1.5 as current test version.
- Related BUG: BUG-001.
- Related Feature: `docs/features/AUTO_HEAL_NPC.md`.

## B. User Requests
1. Add the uploaded V2 project-knowledge MD and client-analysis TXT to persistent files every version must read.
2. Tool still only flickers the Treatment screen after opening NPC; re-study operation/action mechanism and find another solution.

## C. State Before Modification
Working:
- raw runtime coordinate capture;
- route/AutoPath;
- StopPath/dismount;
- NPC dialog opening.

Broken:
- Treatment never advances to the real next service/confirmation state.

User runtime evidence:
- NPC 339/Lâu Lan fails with flicker.
- NPC 463/Lạc Dương also fails with the same flicker in v1.1.5.

Inherited failed approaches:
- direct `UIButton.HandleClickEvent`;
- resolver-only changes;
- `GameDialog.FunctionButtonClicked` through ExecuteUIObject;
- direct source-verified `CMD_SHOW_GAMEDIALOG` using live selectionID;
- changing NPC/map while holding action layer constant.

## D. Investigation / Root Cause
### Mandatory source route used
- uploaded V2 protocol;
- uploaded/client `AI_CLIENT_ANALYSIS_RULES.txt`;
- project `PROJECT_KNOWLEDGE.md` + `CHANGELOG.md`;
- canonical client `AI_INDEX.md`, `AI_BOOTSTRAP.md`, `AUTO_TOOL_SCOPE.md`, `AI_ROUTER.md`;
- `contexts/BUILD_AUTO_HEAL.md` and REQUIRED Auto Heal docs;
- because the common mutation bridge became the suspected layer, additionally `contexts/BUILD_MAINTHREAD_BRIDGE.md`, `contracts/MAINTHREAD_BRIDGE_V1.md`, `analysis/30_EXTERNAL_ACTION_BRIDGE_BLUEPRINT.md`.

### Technical finding
Canonical client knowledge verifies:
- current-style WH_GETMESSAGE hook can be a valid managed Unity-main-thread producer context;
- gameplay/UI mutations should still be queued through `FGStudio.Engine.Utilities.MainThread.Execute(System.Action)` and execute later during normal Unity Update;
- enqueue then synchronously wait in the same hook is forbidden because that blocks the future Update which drains the queue;
- legitimate `System.Action` construction with managed target + callback MethodInfo is solved for this frozen client;
- isolated `CancellationTokenSource.Cancel()` is the canonical first live proof.

### Root Cause
**LIKELY, not CONFIRMED:** earlier Treatment actions used a common re-entrant/external invocation boundary rather than normal game-owned Unity Update dispatch. Identical symptoms across two NPC/maps support a common bridge issue, but only runtime evidence can confirm causality.

### Disproven/Weakened Theory
“NPC 339/Lâu Lan alone is broken” is not a sufficient explanation after v1.1.5 reproduced the same symptom at NPC 463/Lạc Dương.

## E. Changes Made
### Mandatory memory files
Added:
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
- `AI_CLIENT_ANALYSIS_RULES.txt`
- `AI_START_HERE.md`

`AI_START_HERE.md` makes the per-version read sequence explicit and routes client analysis through the canonical KB rather than broad reverse.

### MainThread queued-action bridge
Added `src/bridge_mainthread_v1_1_6.inc`.

Implementation:
1. Resolve `il2cpp_object_new`, strong GC-handle APIs and `il2cpp_thread_current` by export name.
2. Resolve corlib `System.Action`, `CancellationTokenSource`, and `FGStudio.Engine.Utilities.MainThread` semantically.
3. Use frozen Action constructor locator `GameAssembly + 0x49F810` only as build-specific donor after semantic class/method cross-checks.
4. Begin harmless proof: allocate/init CTS -> verify false -> root CTS -> construct/root `Action(target=CTS, callback=Cancel)` -> `MainThread.Execute(Action)` -> return from hook.
5. Later request polls CTS. PASS only when false -> true is observed; timeout after 5000 ms fails closed.
6. Only after proof PASS: resolve current live button -> resolve zero-arg instance `HandleClickEvent()` -> construct `Action(target=button, callback=HandleClickEvent)` -> enqueue through `MainThread.Execute` -> return and let observer/state machine detect the next real state.

Treatment uses `GameDialog / Trị liệu`; acknowledgement uses `GameDialog / Ta biết rồi`; confirmation prefers `MessageBox / Xác nhận`, then GameDialog fallback.

### Active implementation wiring
- `src/bridge.cpp` retains the v1.1.3 helper only as a historical/reference implementation while still compiling shared UI discovery/ReadState/ClickNPC code from `bridge_part04.inc`.
- The v1.1.4 direct packet helper file remains in source/history but is intentionally **not compiled** into the active v1.1.6 bridge after runtime failure in v1.1.4/v1.1.5.
- v1.1.6 `ClickHealDialogChoice` is the active dispatcher implementation.

### Build/CI
`build.cmd` now:
- requires the mandatory V2 protocol and TXT files;
- audits the MainThread proof/action helper;
- builds `ThanLongTestAutoHeal_v1.1.6.exe`.

`.github/workflows/build.yml` uploads `ThanLongTestAutoHeal-v1.1.6`.
Obsolete duplicate `build_v1_1_5.yml` workflow removed.

## F. Important Implementation Details
- Proof is asynchronous across hook requests/timer ticks.
- A first attempt may return/log `MAINTHREAD_PROOF PENDING`; this is not runtime success and does not advance the heal phase.
- Do not enqueue another proof while one is pending.
- External roots protect CTS/Action through proof; queued Action has a managed queue reference after `Execute()`.
- This version intentionally does **not** migrate route/ClickNPC because they are runtime-working; it isolates the broken Treatment mutation boundary.
- UI flicker remains only an observation, never success proof.

## G. Files / Components Changed
### Added
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
- `AI_CLIENT_ANALYSIS_RULES.txt`
- `AI_START_HERE.md`
- `src/bridge_mainthread_v1_1_6.inc`
- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/decisions/DECISIONS.md`
- `docs/features/AUTO_HEAL_NPC.md`
- `docs/history/VERSION_v1.1.6.md`

### Modified
- `src/bridge.cpp`
- `src/controller_part01.inc`
- `build.cmd`
- `.github/workflows/build.yml`
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
- `README.md`

### Removed
- `.github/workflows/build_v1_1_5.yml` obsolete duplicate workflow.

## H. Build / CI History
### Initial Build
- Commit: `be99b7d56be0dbbcf3acac4b1d7c2d597ff166ae`
- Run: `31923691864`
- Result: **BUILD FAILED / CI FAILED** at bridge compile.
- Cause: historical v1.1.4 helper `ClickHealDialogChoiceV114` remained compiled but became unused after v1.1.6 replaced the active action path; `-Werror,-Wunused-function` blocked the build.

### Correction
- Removed v1.1.4 helper from active compilation while preserving its source/history file.
- Added an explicit `[[maybe_unused]]` reference for the retained v1.1.3 historical helper because `bridge_part04.inc` still supplies shared UI/state/NPC functions used by current code.

### Final Build
- Source commit: `e54d537d8870c6e7e131816f3b7e78b60f10a62f`
- GitHub Actions run: `31924151093`
- Architecture audit: PASS.
- Route FSM: **8/8 PASS**.
- Heal FSM: **7/7 PASS**.
- Bridge DLL compile: PASS.
- Bridge PE verification: PASS (`characteristics=0x2022`).
- Controller EXE compile: PASS.
- Artifact upload: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.6`.
- Artifact ID: `9257330034`.
- Artifact ZIP SHA256: `452e0093090b4ce3b7c00f7e4fc815b384d880554ebdfc4ff6b01bbee20cbd34`.
- Final CI state for code: **CI PASS / BUILD PASS**.

## I. Runtime Result
- RUNTIME: **UNTESTED** for v1.1.6.
- Confirmed Working inherited: route/NPC-open partial subsystem.
- Still Failing in last tested version: Treatment progression.
- Awaiting Test:
  1. CTS MainThread proof false->true;
  2. queued `GameDialog/Trị liệu` event;
  3. real next UI/server state.

## J. Regression / Revert / Failed Attempts
### v1.1.5 isolation result
- Feature: Treatment action.
- What Stayed The Same: v1.1.4 direct semantic action layer.
- What Changed: NPC/map only (339/Map5 -> 463/Map3).
- Result: same flicker/non-progression.
- Root Cause Status: NPC-specific sole cause DISPROVEN/strongly weakened.
- Evidence: EVID-002.

### Why v1.1.6 is not retrying FAILED-002
v1.1.0–v1.1.2 called `UIButton.HandleClickEvent()` directly within the hook request path. v1.1.6 uses that authentic event only as a managed Action callback executed later by the game-owned MainThread Update queue. The execution boundary is materially different and is the variable under test.

## K. Known-Good Established
None for full Auto Heal. BUILD PASS is not runtime evidence.

## L. Remaining Bugs / New Knowledge / Decisions
- BUG-001 OPEN.
- EVID-002 records cross-NPC identical runtime failure.
- EVID-003 records canonical MainThread contract.
- DEC-004 activates queued MainThread UI mutation for the broken step.
- DEC-005 requires two-phase async proof/poll.

## M. Handoff
### Inspect first
1. `AI_START_HERE.md`.
2. `PROJECT_KNOWLEDGE.md`.
3. BUG-001 and EVID-002/EVID-003.
4. `src/bridge_mainthread_v1_1_6.inc`.

### Do not change first
- route/coordinate capture/ClickNPC working path;
- NPC IDs/maps;
- semantic GameDialog facts already verified in canonical KB;
- arbitrary delays.

### Next evidence needed
Runtime log beginning when Treatment is visible, especially:
- `MAINTHREAD_PROOF PENDING`
- `MAINTHREAD_PROOF PASS` or exact proof failure stage
- `MAINTHREAD ENQUEUED UIButton.HandleClickEvent • UI=GameDialog • label=Trị liệu`
- resulting UI/state.

If proof PASS + enqueued click still reproduces the same flicker, the next investigation is a targeted comparison of one manual human Treatment action versus the queued tool action at current GameDialog data/outbound request/inbound lifecycle. No broad reverse is justified.

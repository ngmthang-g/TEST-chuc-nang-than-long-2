# VERSION v1.1.8-test

## A. Identity / Lineage
- Version: `v1.1.8-test`
- Date: 2026-08-16
- Based On: v1.1.7-test branch state at `554b67b495f9097433bcbbf3b333c7b01dc42a6d`
- Reason Created: user reports Treatment dialog still flickers when NPC is opened; continue instead of stopping at unverified v1.1.7.
- Last Known-Good full Auto Heal: NONE / UNKNOWN.
- Regression From: not established; full Treatment has never been known-good.
- Supersedes: v1.1.7-test experiment.
- Related BUG: BUG-001.

## B. User Requests
- Keep the uploaded `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md` and `AI_CLIENT_ANALYSIS_RULES.txt` as mandatory startup memory for every version.
- Use `ngmthang-g/clinent-game-than-long-DATA-2222` as the canonical client research repository and start from `AI_INDEX.md`; do not broad reverse.
- Continue fixing Auto trị liệu because opening the NPC still causes visible flicker/no progression.

## C. State Before Modification
- Route/dismount/NPC open is runtime-partial-known-good.
- Full Treatment chain has no known-good version.
- v1.1.7 attempted descendant-label discovery but still allowed `WaitTreatment` to retry `ClickNPC` whenever Lua GameDialog presence was transiently absent for 4 seconds.
- v1.1.7 `FirstTextInSubtreeV117` returned after the first non-empty descendant label, so it did not actually prove that all descendant labels were searched.
- v1.1.7 final CI run `31925922772` FAILED while building the controller because the `kTitle` macro/scope wiring produced both an unused `kTitle` and an undeclared identifier in `controller_part02.inc`.
- User's newest runtime report confirms the visible flicker symptom remains, but the exact tested artifact/version was not stated; do not falsely assign that report to a specific binary.

## D. Investigation / Root Cause
Root Cause: **LIKELY / still not runtime-confirmed**.

Current stronger model:
1. Server-driven GameDialog may destroy/recreate during lifecycle transitions.
2. v1.1.7 observer could miss the desired label because it only used the first non-empty text in each clickable subtree.
3. Even after adding `HealDialogPresent`, controller still retried `ClickNPC` if `FindUI("GameDialog")` transiently returned absent.
4. Reopening the NPC can itself recreate the dialog and present as flicker.
5. Canonical source says the real semantic identity is the current `Selections[selectionID]`; generated live button `Tag` carries that current selectionID.

This does NOT yet prove that no second action-stage issue exists.

## E. Changes Made
### Observer
- Added `src/bridge_dialog_v1_1_8.inc`.
- Replaced first-text-only subtree logic with `SubtreeHasSemanticTextV118`, which considers every reachable text descendant until the desired semantic text is found.
- Keeps whitespace/NBSP/simple-markup normalization.
- Adds `ReadButtonSelectionIdV118` to read the current dynamic GameDialog `Tag` as the live selectionID.
- Diagnostics now emit `DIALOG_V118 ... selectionID=...` or an explicit unresolved reason.

### Action gate
- Added `src/bridge_action_v1_1_8.inc`.
- Preserves v1.1.6 CTS/MainThread proof and `MainThread.Execute(System.Action)` queue.
- Treatment/Know/GameDialog-confirm actions are fail-closed unless the freshly resolved live button also yields `selectionID > 0`.
- The same freshly resolved button object is used to construct the queued `System.Action`; it is not cached across a UI transition.
- MessageBox confirm remains a live-button MainThread action because MessageBox does not use GameDialog selectionID semantics.

### Controller anti-flicker rule
- Removed the `WaitTreatment` loop that retried `ClickNPC` after 4 seconds.
- After a successful initial NPC interaction request, that transaction waits for current GameDialog state and fails closed after 15 seconds instead of reopening the NPC.
- Existing OpenNpc transport-failure retry code is retained; the known visible-dialog WaitTreatment retry is removed.

### Build wiring
- Removed the broken `kTitle` macro override in `controller.cpp` and put the v1.1.8 title back in the canonical `controller_part01.inc` definition.
- Build artifact renamed to `ThanLongTestAutoHeal_v1.1.8.exe` / `ThanLongTestAutoHeal-v1.1.8`.
- Architecture audit uses ASCII symbol checks only for implementation assertions, avoiding the v1.1.7 PowerShell UTF-8 matching failure.

## F. Important Implementation Details
- Current client truth: `GameDialog.Selections[selectionID] = visibleText`; generated button `Tag = selectionID`.
- No fixed/global Treatment selection ID is introduced.
- v1.1.8 still uses the live generated UIButton as the final queued callback target, but only after proving its current semantic selection ID; direct packet dispatch is NOT re-enabled in this version.
- `MAINTHREAD_PROOF PASS` remains a prerequisite for queued gameplay/UI mutation.
- Timeout is a failure guard, not success proof.

## G. Files / Components Changed
Added:
- `src/bridge_dialog_v1_1_8.inc`
- `src/bridge_action_v1_1_8.inc`
- `docs/history/VERSION_v1.1.8.md`

Modified:
- `src/bridge.cpp`
- `src/controller.cpp`
- `src/controller_part01.inc`
- `src/controller_part04.inc`
- `build.cmd`
- `.github/workflows/build.yml`
- project knowledge/history registries.

## H. Build / CI History
- Inherited v1.1.7 final run `31925922772`: **CI FAILED** at controller compile due `kTitle` wiring.
- v1.1.8 source commit: `1da643b8384dfa64a2523938dffb4ddd9885b181`.
- GitHub Actions run `31926671467`: **CI PASS / BUILD PASS**.
- Architecture audit: PASS.
- Route FSM self-test: PASS (8/8 inherited test suite).
- Heal FSM self-test: PASS (7/7 inherited test suite).
- Bridge DLL build + PE verification: PASS.
- Controller EXE build: PASS.
- Artifact upload: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.8`, artifact ID `9258076757`, SHA-256 digest `390ae27248eebee99db7b1a6a463e291a797150a23de770aeed6d29aed8cd9d5`.

## I. Runtime Result
- RUNTIME: **UNTESTED for v1.1.8**.
- User current symptom before v1.1.8: Treatment dialog still flickers when opening NPC; exact artifact version not stated.
- Awaiting test: whether repeated NPC reopen disappears; exact `DIALOG_V118` labels/selectionID; `ACTION_V118` enqueue; next real dialog/result state.

## J. Regression / Failed Attempts
- v1.1.7 theory that descendant-label observer + presence guard fully eliminated flicker is not established and is weakened by the new user report plus source audit.
- Do not rewrite v1.1.3/v1.1.4/v1.1.6 action experiments as independently disproven; shared observer/reachability uncertainty remains.

## K. Known-Good Established
None for full Auto Heal.

## L. Remaining Bugs / New Knowledge
- BUG-001 remains OPEN.
- New source fact: v1.1.7 only inspected the first non-empty label per clickable subtree.
- New source fact: `WaitTreatment` could still reopen NPC on transient GameDialog absence.
- Exact live Treatment/confirmation sequence and success proof remain runtime/server evidence gaps.

## M. Handoff
Inspect first:
1. runtime log from first NPC interaction through `DIALOG_V118`, `selectionID=`, `MAINTHREAD_PROOF`, `ACTION_V118`.
2. whether any second `ClickNPC` appears after the initial successful NPC-open call.
3. the exact next server/UI state after Treatment.

Do not first:
- broad reverse the client;
- hardcode a Treatment selectionID;
- add fixed sleeps;
- rotate more NPCs before collecting the current dialog/action log.

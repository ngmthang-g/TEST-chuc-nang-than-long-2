# VERSION v1.1.7-test — Dynamic GameDialog observer/discovery correction

## A. Identity / Lineage
- Version: v1.1.7-test
- Date: 2026-08-16
- Based On: v1.1.6-test
- Reason Created: repeated visible Treatment-dialog flicker/no progression; re-audit shows the common dialog discovery/retry layer was never isolated from the action experiments.
- Last Known-Good full Auto Heal: NONE / UNKNOWN.
- Supersedes: v1.1.6 diagnostic assumption that action execution boundary was the primary likely cause.
- Related BUG: BUG-001.
- Related Feature: Auto Heal NPC.

## B. User Requests
- Make uploaded `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED` MD and client-analysis TXT mandatory memory/read material for every version.
- Continue investigation because opening the NPC still produces a Treatment-screen/dialog flicker instead of progressing.
- Read canonical `clinent-game-than-long-DATA-2222` knowledge first and do not broad reverse the client.

## C. State Before Modification
Runtime-confirmed partial behavior:
- target coordinate capture works;
- route/dismount works;
- NPC interaction opens a visible GameDialog.

Observed failure lineage:
- multiple versions visibly flicker/reopen around the Treatment dialog and do not progress.
- original runtime screenshot/log showed repeated `ClickNPC` calls while controller status remained `CHỜ DIALOG TRỊ LIỆU`.

Important inherited implementation:
- `InspectHealDialog()` used `FindButtonInUi()` -> `WalkForButton()`.
- old `WalkForButton()` only matched when the same object both exposed `get_Text()==wanted` and `HandleClickEvent()`.
- controller retries `ClickNPC` while WaitTreatment has no Treatment mask.

## D. Investigation / Root Cause
Root Cause status: **LIKELY, not yet runtime-confirmed**.

New evidence-based hypothesis:
1. The server-driven `GameDialog` can already be visible.
2. Old observer may fail to recognize the dynamic button because its visible text can be on a descendant/label or because Lua UI exists before normalized `UIRoot` is available.
3. `healDialogMask` remains zero.
4. controller interprets that as “dialog not ready” and calls `ClickNPC` again.
5. repeated NPC interaction destroys/recreates/reopens the same dialog, producing the visible flicker.

This matters because earlier versions changed action callback/packet paths while retaining the same observer/discovery primitive. Therefore the statement “all action mechanisms were definitely executed and failed” was too strong. User runtime evidence proves non-progression/flicker, but does not by itself prove each action layer was reached.

Canonical support:
- `TCPCmdHandler` destroys/recreates `GameDialog` on incoming `CMD_SHOW_GAMEDIALOG`.
- canonical Auto Heal context requires WAIT_GAMEDIALOG -> enumerate/observe current runtime dialog -> choose semantic action.
- UI source says server/runtime dialog state is stronger proof than fixed timing/repeated clicks.

No broad binary analysis was used for this correction.

## E. Changes Made
### `src/bridge_dialog_v1_1_7.inc`
New robust discovery layer:
- normalizes whitespace/NBSP and strips simple markup tags;
- treats a clickable UI object as a button candidate when it has `HandleClickEvent()`;
- searches the candidate's descendant subtree for visible text instead of requiring text and click handler on the identical object;
- records clickable candidate count and observed labels in diagnostic detail;
- distinguishes `Lua UI exists but UIRoot not ready` from `UI absent`;
- once `GameDialog` itself exists in WaitTreatment, prevents the old logic from reopening the NPC merely because the exact button has not been resolved in that tick.

### `src/bridge.cpp`
- preserves old v1.1.6 discovery functions under legacy names for lineage;
- activates the v1.1.7 observer/discovery layer;
- retains v1.1.6 MainThread queued action boundary so this version changes the observer variable first rather than rewriting action semantics again.

### `src/controller.cpp`
- version title changed to v1.1.7.

### Build workflow
- artifact renamed to `ThanLongTestAutoHeal-v1.1.7`.
- architecture audit checks mandatory knowledge files and v1.1.7 dialog observer symbols.

## F. Important Implementation Details
New intended flow:

`ClickNPC once -> Lua GameDialog presence proof -> robust clickable+descendant-label scan -> resolve current Treatment button -> MainThread proof/action -> wait real next state`

Critical anti-loop behavior:

`GameDialog present` must no longer be interpreted as “NPC needs reopening” just because exact button discovery is temporarily incomplete.

## G. Files / Components Changed
Added:
- `src/bridge_dialog_v1_1_7.inc`
- `docs/history/VERSION_v1.1.7.md`

Modified:
- `src/bridge.cpp`
- `src/controller.cpp`
- `build.cmd`
- `.github/workflows/build.yml`
- knowledge/feature/bug/evidence docs for v1.1.7.

## H. Build / CI History
- CI: PENDING at initial history creation.
- Runtime: UNTESTED.

## I. Runtime Result
RUNTIME: UNTESTED.

Expected diagnostic outcomes:
1. `DIALOG_V117 MATCH ... labels=...Trị liệu...` -> discovery fixed; action layer is genuinely reached.
2. `Lua UI tồn tại nhưng UIRoot chưa sẵn sàng` -> root normalization/timing is exact remaining observer issue; NPC should not be reopened.
3. `DIALOG_V117 NO MATCH ... labels=...` -> observed labels identify hierarchy/text mismatch without binary reverse.
4. MainThread action enqueued but no server transition -> only then action boundary/manual-vs-tool trace becomes the next target.

## J. Regression / Failed Attempts Correction
Previous classification corrected:
- old versions are still RUNTIME FAIL for full Treatment progression;
- however “each action mechanism itself definitely failed” is **NOT PROVEN** unless logs show that action was actually reached.
- repeated `ClickNPC` while WaitTreatment is strong evidence that observer failure can generate the same flicker before action execution.

## K. Known-Good Established
No new known-good until runtime test.

## L. Remaining Bugs / New Knowledge / Decisions
- BUG-001 remains OPEN.
- New evidence/correction: observer/discovery is now a first-class suspect and must be proven before further action redesign.

## M. Handoff
Test v1.1.7 and preserve log from first `ClickNPC` onward.
Most useful lines:
- `DIALOG_V117 MATCH`
- `DIALOG_V117 NO MATCH`
- `Lua UI tồn tại nhưng UIRoot chưa sẵn sàng`
- `MAINTHREAD_PROOF`
- `MAINTHREAD ENQUEUED`

Do not broad reverse or rotate NPC again until this observer evidence is collected.

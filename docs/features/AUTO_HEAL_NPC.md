# FEATURE: AUTO HEAL NPC / TRỊ LIỆU

## Purpose
Build a reliable internal flow:
`route -> NPC interaction -> observe current server GameDialog -> resolve current Treatment selection -> one action -> real follow-up -> state proof`.
Only port after repeated runtime PASS.

## Mandatory Startup
Before every version read:
1. `AI_START_HERE.md`
2. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
3. `AI_CLIENT_ANALYSIS_RULES.txt`
4. `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. this feature + BUG/DEC/EVID docs + current source.

Canonical client research route starts from `clinent-game-than-long-DATA-2222/AI_INDEX.md`; do not broad reverse when exact facts exist in VERIFIED/database.

## Current Implementation — v1.1.8
Protected working path:
- user captures raw MapID/X/Y;
- route / stop path / dismount;
- Map 5 -> NPC 339 Đỗ Thanh Đằng;
- Map 3 -> NPC 463 Long Phá Thiên;
- semantic NPC interaction/open.

Current dialog/action path:

`initial ClickNPC -> wait current GameDialog -> all-descendant semantic scan -> read live button.Tag selectionID -> MainThread queued live action -> wait next real state`.

### v1.1.8 observer correction
v1.1.7 said it searched the clickable subtree, but `FirstTextInSubtreeV117` actually returned after the first non-empty text. v1.1.8 replaces that with `SubtreeHasSemanticTextV118`, so later sibling/descendant labels can match.

For a matched GameDialog choice v1.1.8 also reads current `Tag` and records the dynamic `selectionID`. This is a semantic gate, not a hardcoded ID.

### v1.1.8 anti-reopen rule
`WaitTreatment` no longer retries `ClickNPC` after a timing interval. After the initial NPC interaction returns success, absence/unresolved GameDialog is treated as a state to observe until timeout; timeout fails closed instead of reopening the NPC.

This is specifically intended to remove controller-driven dialog reconstruction as a source of visible flicker.

### v1.1.8 action gate
- retains v1.1.6 CTS/MainThread proof;
- freshly resolves the live button after proof;
- requires live `selectionID > 0` for GameDialog Treatment/Know/GameDialog-confirm choices;
- queues `UIButton.HandleClickEvent` through `MainThread.Execute(System.Action)` on that same fresh object;
- never caches the button across server/UI transitions;
- MessageBox confirm remains a live MessageBox button action and is not assigned a GameDialog selectionID.

Direct packet dispatch is NOT re-enabled in v1.1.8. The first goal is to prove exact current selection identity and action reachability without changing the proven dispatcher boundary again.

## Current Runtime / Build Status
- Route/NPC open: RUNTIME PARTIAL PASS.
- Full Treatment: no known-good version.
- v1.1.5: RUNTIME FAIL overall at Treatment/flicker on second NPC/map.
- v1.1.6: CI/BUILD PASS; runtime artifact-version result remains unconfirmed in preserved logs.
- v1.1.7: final CI `31925922772` FAILED at controller compile; not a valid build handoff.
- latest user report before v1.1.8: flicker still occurs; exact tested artifact/version UNKNOWN.
- v1.1.8 source commit `1da643b8384dfa64a2523938dffb4ddd9885b181`: **CI/BUILD PASS** in run `31926671467`.
- v1.1.8 artifact: `ThanLongTestAutoHeal-v1.1.8` (artifact ID `9258076757`).
- v1.1.8: **RUNTIME UNTESTED**.

## Critical Evidence Correction
Confirmed:
- visible server dialog opens;
- full Treatment chain has not passed;
- original logs showed repeated `ClickNPC` while waiting for Treatment;
- v1.1.7 source retained a transient-absence reopen loop and first-text-only descendant selection.

Not proven:
- that every historical Treatment callback/packet experiment was actually reached;
- that removing the reopen loop alone fixes the complete Treatment flow;
- exact live follow-up sequence after Treatment.

## Canonical Client Facts
- Dynamic `GameDialog` is server/runtime-driven.
- `Selections[selectionID] = visibleText`.
- generated current button `Tag = selectionID`.
- selection action contract is `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`.
- inbound GameDialog lifecycle may destroy/recreate existing UI.
- canonical Auto Heal flow is wait/observe current dialog -> current semantic choice -> state proof.
- visual flicker is not success proof.

## Current Known-Good
No full Auto Heal known-good.
Protected partial behavior:
- coordinate capture;
- route;
- StopPath/dismount;
- NPC interaction/open.

## Version Timeline
### v1.1.0
Initial route + NPC + dialog experiment. Full Treatment runtime FAIL.

### v1.1.1
Removed inferred X/Y; added runtime capture.

### v1.1.2
Changed UI root discovery order. NPC opening confirmed; Treatment still not progressed.

### v1.1.3
Tried `GameDialog.FunctionButtonClicked(liveButton)` through ExecuteUIObject. Overall runtime still failed; shared discovery remained.

### v1.1.4
Tried runtime Tag/selectionID + source-verified GameDialog packet. Overall runtime failed on NPC 339; shared discovery remained.

### v1.1.5
A/B NPC/map test using 463/Lạc Dương. Same overall symptom; NPC-339-only theory weakened.

### v1.1.6
Introduced CTS proof + `MainThread.Execute(System.Action)` queued UIButton action. CI/BUILD PASS. Runtime proof not preserved as confirmed in current evidence.

### v1.1.7
Added descendant-label discovery and GameDialog-present guard. Later source audit found first-text-only matching and a remaining WaitTreatment ClickNPC retry when Lua UI presence was transiently absent. Final CI also failed at `kTitle` controller compile.

### v1.1.8
Adds all-descendant semantic matching, live selectionID gate, preserves MainThread boundary, and removes WaitTreatment NPC reopen. CI/BUILD PASS in run `31926671467`; runtime untested.

## Failed / Unsafe Mechanisms
- inferred/hardcoded coordinate scaling — FAILED.
- treating fixed delay as UI readiness — unsafe.
- repeated `ClickNPC` from WaitTreatment — removed in v1.1.8.
- claiming an action mechanism failed without proof that the action stage was reached — DEPRECATED reasoning.
- v1.1.7 first-text-only subtree matcher — superseded.

## Important APIs / Constants
- `LuaSystemAPI_Game.ClickNPC(Int32)`.
- `GameDialog.Selections[selectionID] = visibleText`.
- generated GameDialog button `Tag = selectionID`.
- `CMD_SHOW_GAMEDIALOG = 100007`.
- `FGStudio.Engine.Utilities.MainThread.Execute(System.Action)`.
- v1.1.8 diagnostics: `DIALOG_V118`, `selectionID=`, `MAINTHREAD_PROOF`, `ACTION_V118`.

## Do-Not-Break Rules
- mandatory startup files every version;
- canonical KB first; targeted binary only for an exact remaining gap;
- no guessed fixed Treatment selection ID;
- no hardcoded/inferred NPC X/Y;
- no stale UI pointer across transition;
- no fixed Sleep as success proof;
- one mutable action at a time;
- do not reopen NPC from WaitTreatment on temporary dialog absence;
- BUILD/CI PASS != RUNTIME PASS.

## Open Questions
1. Does v1.1.8 emit exactly one successful NPC-open call per transaction after the initial open?
2. What exact live selectionID accompanies visible `Trị liệu`?
3. Does `ACTION_V118` advance to a new GameDialog, MessageBox or immediate result?
4. If an action is definitely enqueued and no state transition follows, what exact manual-vs-tool business callback/request differs?
5. What HP/money/dialog evidence establishes successful Treatment?

## Next Diagnostic Step
Run the build-valid v1.1.8 artifact and preserve logs from first NPC call through `DIALOG_V118`, live `selectionID`, `MAINTHREAD_PROOF`, `ACTION_V118`, and the next UI/result. Do not rotate NPCs or broad reverse before this evidence.

# BUG REGISTRY

## BUG-001 — GameDialog Trị liệu nháy nhưng không tiến sang trạng thái kế tiếp

**Status:** OPEN  
**Severity:** HIGH for Auto Heal feature  
**First Observed:** v1.1.0-test  
**Last Runtime Report:** 2026-08-16 current user report; exact tested artifact/version UNKNOWN  
**Last Known-Good:** NONE for full Treatment action  
**First Confirmed-Bad:** v1.1.0-test  
**Related Feature:** Auto trị liệu NPC

### Confirmed Behavior
- Raw target capture / route / dismount / NPC opening works sufficiently to display the intended server-driven dialog.
- Full Treatment chain has not reached runtime PASS.
- Original runtime evidence showed repeated `ClickNPC` while status remained `CHỜ DIALOG TRỊ LIỆU` even though the intended dialog was visibly open.
- NPC 463/Lạc Dương reproduced the same overall flicker/no-progress symptom, so NPC 339/Lâu Lan alone is not a sufficient explanation.
- Latest user report says the tool still flickers the Treatment dialog when opening NPC; the report does not identify the exact artifact/version.

### v1.1.8 Correction of v1.1.7 Assumptions
v1.1.7 improved discovery but still left two concrete gaps:
1. `FirstTextInSubtreeV117` stopped at the first non-empty descendant text, so a later `Trị liệu` label inside the same clickable subtree could be missed.
2. `WaitTreatment` still called `ClickNPC` again after 4 seconds whenever Lua GameDialog presence was transiently absent. Because canonical `CMD_SHOW_GAMEDIALOG` handling destroys/recreates GameDialog, a transient absence could re-enable the reopen loop.

Also, final v1.1.7 CI run `31925922772` failed at controller compilation (`kTitle` wiring), so v1.1.7 cannot be treated as a build-valid runtime fix.

### Canonical Support
- `GameDialog.Selections[selectionID] = visibleText`.
- Generated dynamic button stores current `selectionID` in `Tag`.
- GameDialog request is `CMD_SHOW_GAMEDIALOG=100007`, payload `selectionID:SelectedItemID`.
- Inbound GameDialog lifecycle destroys/recreates current UI for new server dialog data.
- Correct automation pattern is `observe current dialog -> resolve current semantic choice -> one action -> fresh state proof`.

### Attempts / Lineage
1. v1.1.0–v1.1.2: generic UIButton/action experiments; full runtime FAIL.
2. v1.1.2: UI root order change; full runtime FAIL.
3. v1.1.3: `GameDialog.FunctionButtonClicked` through ExecuteUIObject; full runtime FAIL, shared discovery remained.
4. v1.1.4: live Tag/selectionID + source-verified packet; full runtime FAIL on 339, shared discovery remained.
5. v1.1.5: different NPC/map; same overall symptom.
6. v1.1.6: MainThread queued UIButton action after CTS proof; CI/BUILD PASS, runtime artifact-version assignment not preserved.
7. v1.1.7: descendant observer/presence guard; final CI FAILED and source still retained first-text-only + transient-absence reopen gaps.
8. v1.1.8: all-descendant observer + live selectionID gate + removal of WaitTreatment NPC reopen; source commit `1da643b8384dfa64a2523938dffb4ddd9885b181`, **CI/BUILD PASS** in run `31926671467`, artifact ID `9258076757`; RUNTIME UNTESTED.

### v1.1.8 Fix Strategy
- search every reachable text descendant under each live clickable candidate;
- normalize semantic text;
- read live GameDialog `Tag` and require `selectionID > 0` before enqueueing a GameDialog choice;
- use the same freshly resolved button for the queued `System.Action`;
- retain proven CTS/MainThread dispatcher boundary;
- after initial successful NPC interaction, never reopen NPC from `WaitTreatment`; time out fail-closed instead;
- log `DIALOG_V118`, live `selectionID`, `MAINTHREAD_PROOF`, `ACTION_V118` so the next runtime test shows the exact reached stage.

### Root Cause
**LIKELY:** observer/retry coupling contributes directly to flicker by reopening/reconstructing the server dialog.

**UNKNOWN:** whether an additional action/business-request issue remains after v1.1.8 successfully resolves a live selection and queues it.

### Current Workaround
None. Do not port Auto Heal to production.

### Fixed In
UNKNOWN until runtime confirms behavior.

### Runtime Verified In
NONE.

### Next Diagnostic Step
Run the build-valid v1.1.8 artifact and capture one transaction from the first NPC interaction. Required evidence:
- count of `ClickNPC` calls;
- `DIALOG_V118 MATCH/NO MATCH`;
- exact live `selectionID` or unresolved reason;
- `MAINTHREAD_PROOF PASS/PENDING/FAIL`;
- `ACTION_V118 ENQUEUED` or fail-closed reason;
- next GameDialog/MessageBox/result state.

### Do-Not-Do
- Do not broad reverse the client.
- Do not rotate more NPCs before current-stage evidence.
- Do not add fixed sleeps to hide a state/discovery bug.
- Do not hardcode a global Treatment selectionID.
- Do not equate visible flicker with a successful Treatment click.
- Do not claim an action path failed unless logs prove that path was reached.

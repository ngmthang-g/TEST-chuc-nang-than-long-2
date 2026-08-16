# BUG REGISTRY

## BUG-001 — GameDialog Trị liệu mở được nhưng không tiến sang service/result

**Status:** OPEN  
**Severity:** HIGH for Auto Heal feature  
**First Observed:** v1.1.0-test  
**Last Runtime Tested:** v1.1.8-test delivered artifact, 2026-08-16  
**Last Known-Good:** NONE for full Treatment flow  
**Related Feature:** Auto trị liệu NPC

### Runtime-confirmed working
- raw NPC coordinate capture;
- route/mount/AutoPath/dismount;
- `ClickNPC(339)` opens the intended server-driven GameDialog;
- v1.1.8 tested transaction issued exactly one `ClickNPC`; the old WaitTreatment reopen loop did not recur.

### Runtime-confirmed failing in v1.1.8
After the single NPC open, observer repeatedly logged:

`DIALOG_V118 NO MATCH • UI=GameDialog • wanted=Trị liệu • clickable=0 • texts=0 • labels=<none>`

for the full 15-second guard, then failed closed without reopening NPC.

This establishes that the bridge's active `FindUI -> UIRoot/CoreChildren -> UIButton/Text` representation is not exposing the dynamic GameDialog nodes needed by the scanner in this runtime state.

### Critical correction
The old hypothesis “flicker/no-progress is mainly repeated NPC reopen” was only partly correct.
- Controller-driven reopen was a real historical bug path and is now absent in the v1.1.8 runtime transaction.
- Full Treatment still fails because v1.1.8 dies earlier at observer representation.
- Therefore do not keep repairing UIButton text matching/depth against the same empty UIRoot representation.

### Canonical semantic source of truth
- `GameDialog.Selections[selectionID] = visibleText`.
- Built-in AutoFight quest/FuBen logic stores/inspects current server dialog selections.
- `CMD_SHOW_GAMEDIALOG = 100007`.
- Request payload: `selectionID:SelectedItemID`.
- No universal static Treatment selectionID exists.

### Attempts / Lineage
1. v1.1.0–v1.1.2: initial UIRoot/button experiments; full runtime FAIL.
2. v1.1.3: `GameDialog.FunctionButtonClicked` through ExecuteUIObject; full runtime FAIL, shared discovery remained.
3. v1.1.4: live Tag + exact packet experiment; full runtime FAIL, but old discovery still gated reachability.
4. v1.1.5: alternate NPC/map reproduced same overall symptom.
5. v1.1.6: CTS/MainThread queued UIButton experiment; BUILD PASS, action-stage runtime evidence not preserved.
6. v1.1.7: observer redesign; final CI failed and source still had first-text/retry gaps.
7. v1.1.8: all-descendant scan + live Tag gate + no WaitTreatment reopen. BUILD PASS. **Runtime:** single NPC open PASS, but GameDialog UIRoot scan returned clickable=0/texts=0 until timeout.
8. v1.1.9: abandon UIRoot as active Treatment observer. Read live Lua GameDialog data/Selections and re-read semantic selection at action time.

### v1.1.9 Strategy
Observer:
`GUI.FindUI(GameDialog/AutoFight_Main) inside Lua runtime -> bounded table scan -> current Selections -> match semantic text -> actual live ID`.

Action:
`re-read current Selections -> require current requested choice -> SendPacket(100007, liveID:-1) -> wait fresh server/UI state`.

MessageBox confirmation remains callback-based via `ButtonOKClicked()` if MessageBox exists.

### Root cause status
**CONFIRMED component:** v1.1.8 UIRoot/button observer representation is unusable for this live dynamic GameDialog (`0 clickable / 0 texts` despite dialog presence).

**CONFIRMED resolved component:** tested v1.1.8 transaction no longer reopens NPC from WaitTreatment.

**UNKNOWN:** exact live Lua table path for `Selections` and server acceptance/follow-up after v1.1.9 sends the current semantic selection.

### Current Workaround
None. Do not port Auto Heal to production.

### Fixed In
UNKNOWN.

### Runtime Verified In
NONE for complete Treatment flow.

### Next Diagnostic Step
Run v1.1.9 and capture:
- `LUA_DIALOG_V119` including T/C/K and raw path/sample data;
- `MAINTHREAD_PROOF`;
- `ACTION_V119` live selectionID/payload or exact failure;
- subsequent GameDialog/MessageBox/result/HP/money state.

### Do-Not-Do
- no broad client reverse;
- no more UIRoot depth/label tuning for this same GameDialog failure;
- no fixed Treatment selectionID;
- no WaitTreatment `ClickNPC` retry;
- no fixed Sleep as success proof;
- no claim that packet/server failed until v1.1.9 proves a live selection was actually sent.

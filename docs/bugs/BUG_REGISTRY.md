# BUG REGISTRY

## BUG-001 — GameDialog Trị liệu nháy nhưng không tiến sang trạng thái kế tiếp

**Status:** OPEN  
**Severity:** HIGH for Auto Heal feature  
**First Observed:** v1.1.0-test  
**Last Runtime Tested:** v1.1.5-test from preserved evidence; v1.1.6/v1.1.7 require explicit artifact-version confirmation before assigning runtime result.  
**Last Known-Good:** NONE for full Treatment action  
**First Confirmed-Bad:** v1.1.0-test  
**Related Feature:** Auto trị liệu NPC

### Confirmed Behavior
- Raw target capture / route / dismount / NPC opening works sufficiently to display the intended server-driven dialog.
- Full Treatment chain has not reached runtime PASS.
- The original runtime log/screenshot showed repeated `ClickNPC` calls while status remained `CHỜ DIALOG TRỊ LIỆU`.
- NPC 463/Lạc Dương reproduced the same overall flicker/no-progress symptom, weakening the theory that NPC 339/Lâu Lan alone is defective.

### Critical Correction — v1.1.7 investigation
The previous BUG entry over-attributed the failure to the mutable-action execution boundary.

Shared code audit shows v1.1.0–v1.1.6 reused the same core button discovery assumption:

`WalkForButton -> require same object get_Text()==wanted AND HandleClickEvent()`.

If the dynamic UIButton stores visible text in a descendant label, or Lua UI is registered before normalized `UIRoot` is available, `InspectHealDialog` can return no Treatment bit despite the dialog being visibly open. Controller then retries `ClickNPC`, which can recreate/reopen the dialog and itself produce the visible flicker.

Therefore:
- RUNTIME FAIL/non-progression remains CONFIRMED for tested versions;
- “each attempted action callback/packet definitely executed and failed” is **NOT PROVEN** without action-stage logs;
- observer/discovery/retry loop is now **LIKELY** and must be tested before another action redesign.

### Canonical Support
- Server `CMD_SHOW_GAMEDIALOG` lifecycle destroys/recreates current GameDialog before opening new data.
- Dynamic GameDialog is server/runtime data; current dialog presence and semantic selections should be observed rather than inferred from timing.
- Canonical Auto Heal flow is `WAIT_GAMEDIALOG -> observe current selection -> one action -> state proof`.

### Attempts / Lineage
1. v1.1.0–v1.1.2: generic UIButton/action experiments; full runtime FAIL. Action-stage reachability is not fully preserved in logs.
2. v1.1.2: UI root order change; full runtime FAIL.
3. v1.1.3: `GameDialog.FunctionButtonClicked` through ExecuteUIObject; full runtime FAIL, but shared discovery remained.
4. v1.1.4: Tag/selectionID + source-verified packet; full runtime FAIL on 339, but shared discovery remained.
5. v1.1.5: different NPC/map; same overall symptom.
6. v1.1.6: MainThread queued UIButton action after CTS proof; BUILD/CI PASS, runtime version assignment not yet confirmed by preserved user log.
7. v1.1.7: robust dialog observer/discovery; BUILD/CI pending, runtime untested.

### v1.1.7 Fix Strategy
- find clickable candidate by `HandleClickEvent`;
- inspect its descendant subtree for visible label text;
- normalize whitespace/NBSP/simple markup;
- report observed clickable labels;
- distinguish Lua UI presence from UIRoot readiness;
- once GameDialog exists, do not reopen NPC solely because exact Treatment button was unresolved on that tick;
- retain current MainThread action boundary so observer and action variables are not changed simultaneously.

### Root Cause
**LIKELY:** dialog discovery/retry loop contributes directly to flicker and may prevent the Treatment action from being reached.

**UNKNOWN:** whether an additional action-boundary or semantic request problem remains after discovery is fixed.

### Current Workaround
None. Do not port Auto Heal to production.

### Fixed In
UNKNOWN until runtime confirms v1.1.7 behavior.

### Runtime Verified In
NONE.

### Next Diagnostic Step
Run v1.1.7 and capture log beginning with the first NPC interaction. Determine which branch occurs:
- `DIALOG_V117 MATCH`;
- `DIALOG_V117 NO MATCH` with observed labels;
- Lua UI exists / UIRoot not ready;
- MainThread action enqueued but no state transition.

Only the last branch justifies returning to manual-vs-tool action/request tracing.

### Do-Not-Do
- Do not broad reverse the client.
- Do not rotate additional NPCs before observer evidence.
- Do not add fixed sleeps to hide a discovery bug.
- Do not equate visible flicker with a successful Treatment click.
- Do not claim an action path failed unless logs prove that action path was actually reached.

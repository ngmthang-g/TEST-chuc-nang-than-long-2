# BUG REGISTRY

## BUG-001 — GameDialog Trị liệu nháy nhưng không tiến sang trạng thái kế tiếp

**Status:** OPEN  
**Severity:** HIGH for Auto Heal feature  
**First Observed:** v1.1.0-test  
**Last Runtime Tested:** v1.1.5-test  
**Last Known-Good:** NONE for full Treatment action  
**First Confirmed-Bad:** v1.1.0-test  
**Related Feature:** Auto trị liệu NPC

### Known Evidence
- v1.1.0–v1.1.2 direct `UIButton.HandleClickEvent()` path: NPC opens, Treatment UI flickers, no progression.
- v1.1.3 `GameDialog.FunctionButtonClicked(liveButton)` via `ExecuteUIObject`: same symptom.
- v1.1.4 live `selectionID` + source-verified `CMD_SHOW_GAMEDIALOG 100007` payload `selectionID:-1` on NPC 339/Lâu Lan: same symptom.
- v1.1.5 unchanged v1.1.4 action layer at NPC 463/Lạc Dương: user reports same symptom. This strongly weakens/disproves NPC 339/Lâu Lan as the sole cause.

### Unknowns
- Whether the old direct/hook action boundary caused the failed behavior.
- Whether v1.1.6 MainThread-queued event reproduces the real user click path successfully.
- Exact post-Treatment server/UI transition after a genuinely accepted action.

### Root Cause
**LIKELY but not CONFIRMED:** common external action execution-boundary issue. The symptom survives NPC/map changes and multiple source-correct semantic action attempts. Canonical client bridge docs require mutable callbacks to execute through `MainThread.Execute(Action)` and later Unity Update rather than direct re-entrant mutation from the Windows message hook.

### Attempts
1. v1.1.0–1.1.2: direct generic UIButton click — FAILED.
2. v1.1.2: UI resolver-only adjustment — FAILED.
3. v1.1.3: exact GameDialog Lua callback through ExecuteUIObject — FAILED.
4. v1.1.4: live selectionID + verified direct semantic packet — FAILED at runtime on NPC 339.
5. v1.1.5: A/B NPC/map isolation with NPC 463 — FAILED with same symptom.
6. v1.1.6: CTS-proven `MainThread.Execute(System.Action)` + queued live UIButton event — **BUILD PASS / CI PASS**, runtime untested. Final code run `31924151093`, source commit `e54d537d8870c6e7e131816f3b7e78b60f10a62f`.

### Current Workaround
None. Do not port Auto Heal to production.

### Fixed In
UNKNOWN.

### Runtime Verified In
NONE.

### Next Diagnostic Step
Run v1.1.6 and preserve log from `MAINTHREAD_PROOF` through Treatment. If proof PASS + queued click still fails, compare one legitimate manual click against tool action using targeted GameDialog/request/state tracing only.

### Do-Not-Do
- Do not broad reverse the entire client.
- Do not rotate more NPCs merely to mask the common symptom.
- Do not restore fixed/hardcoded Treatment selection IDs.
- Do not treat UI flicker as success.
- Do not call direct `HandleClickEvent()` re-entrantly from the hook and claim it is the same experiment as v1.1.6.

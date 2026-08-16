# FEATURE: AUTO HEAL NPC / TRỊ LIỆU

## Purpose
Test a reliable internal flow:
`route -> NPC interaction -> current GameDialog Treatment choice -> real follow-up -> state proof`.
Only port to the production tool after repeated runtime PASS.

## Current Implementation
v1.1.6 keeps the already-working route/NPC-open path and changes only the broken UI mutation execution boundary:

`live GameDialog -> find current UIButton by semantic text -> prove external Action queue with CTS -> construct System.Action(target=button, callback=HandleClickEvent) -> MainThread.Execute(Action) -> return from hook -> Unity Update invokes callback -> observe next UI/state`.

NPC target position remains raw user-captured MapID/X/Y in this test UX.
Current MapID->NPC test mapping:
- Map 5 -> NPC 339 Đỗ Thanh Đằng.
- Map 3 -> NPC 463 Long Phá Thiên.

## Current Runtime Status
- Route/NPC open: RUNTIME PARTIAL PASS.
- Full Treatment: no known-good version.
- v1.1.0–v1.1.5: RUNTIME FAIL at Treatment.
- v1.1.6: RUNTIME UNTESTED until CI artifact is tested.

## Current Known-Good
No full Auto Heal known-good.
Protected partial behavior:
- coordinate capture;
- route to target;
- StopPath/dismount;
- semantic NPC interaction/open.
Do not rewrite these while debugging the Treatment boundary unless new evidence implicates them.

## Related REQ / BUG / DEC
- BUG-001: Treatment flicker/no progression.
- DEC-001: mandatory startup knowledge.
- DEC-002: canonical client KB first.
- DEC-003: user-captured target coordinates in this test lab.
- DEC-004: queue broken UI mutation via MainThread instead of direct hook mutation.
- DEC-005: asynchronous proof/poll rule.

## Version Timeline
### v1.1.0
- request: initial Auto trị liệu.
- implementation: route + ClickNPC + direct live UIButton event.
- build: PASS.
- runtime: NPC opens; Treatment FAIL/flicker.

### v1.1.1
- change: remove inferred X/Y; add runtime coordinate capture.
- runtime: route target correction retained.

### v1.1.2
- change: UI discovery/resolver fallback.
- runtime: NPC open PARTIAL PASS; Treatment still FAIL.

### v1.1.3
- change: exact `GameDialog.FunctionButtonClicked(liveButton)` through ExecuteUIObject.
- runtime: same flicker/no progression.

### v1.1.4
- change: bypass click callback; read live Tag/selectionID and send source-verified GameDialog packet.
- runtime: same flicker/no progression on NPC 339/Lâu Lan.

### v1.1.5
- change: controlled NPC/map A/B test; same action layer on NPC 463/Lạc Dương.
- build: PASS, run `31909204317`.
- runtime: same flicker/no progression. NPC-specific-only theory strongly weakened/disproven.

### v1.1.6
- inherited problem: common symptom survives two NPCs and multiple semantic action styles.
- investigation: canonical MainThread contract says message hook is a producer context; mutable callbacks should execute later through `MainThread.Execute(Action)` / Unity Update.
- change: CTS proof + queued live UIButton event.
- build/runtime: pending at document creation.

## Failed / Reverted Approaches
- inferred coordinate scaling — FAILED.
- direct UIButton event inside hook — FAILED.
- resolver-only fix — FAILED.
- ExecuteUIObject GameDialog callback — FAILED.
- direct source-verified GameDialog packet from current bridge — FAILED at runtime for tested flow.
- changing NPC/map while keeping same action layer — did not change symptom.

## Important APIs / Constants
- `LuaSystemAPI_Game.ClickNPC(Int32)`.
- Dynamic dialog truth: `Selections[selectionID] = visibleText`.
- `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`.
- `FGStudio.Engine.Utilities.MainThread.Execute(System.Action)`.
- v1.1.6 isolated proof: `CancellationTokenSource.Cancel()` -> `IsCancellationRequested false -> true`.

## Do-Not-Break Rules
- no broad client reverse when canonical KB already has exact fact;
- no guessed fixed Treatment selection ID;
- no hardcoded/inferred X/Y in current test flow;
- no stale UIButton pointer across dialog transition;
- no fixed sleep as success proof;
- no synchronous waiting for an enqueued Action in the same WH_GETMESSAGE callback;
- one mutable action at a time;
- visual flicker is not success proof.

## Open Questions
1. Does the CTS proof pass on the live client repeatedly?
2. Does MainThread-queued HandleClickEvent produce a true Treatment transition?
3. If not, what differs between one manual human click and the queued action at GameDialog data/request/state level?
4. What confirmation UI/state follows a genuinely accepted Treatment?

## Next Diagnostic Step
Test v1.1.6 and capture all log lines containing `MAINTHREAD_PROOF` and `MAINTHREAD ENQUEUED`. If proof PASS but Treatment still fails, perform one narrow manual-vs-tool trace of current GameDialog selections/outbound request/inbound dialog lifecycle. Do not test more NPCs as the next step.

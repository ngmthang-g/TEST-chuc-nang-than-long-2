# FEATURE: Background UI actions

## Purpose

Perform Confirm, Revive, AUTO and Auto Sell actions without moving the physical mouse.

## Current implementation

The controller retains the v0.5 state machines. The per-PID Bridge resolves live managed UI objects at action time and invokes Button/Toggle/UIRect/Lua callbacks. It does not cache child controls across UI transitions.

## Current runtime status

- v0.6 Confirm: RUNTIME FAIL, BUG-001.
- v0.6 Auto Sell: RUNTIME FAIL, BUG-001.
- v0.6 Revive: UNKNOWN / not reported.
- v0.6 AUTO: UNKNOWN / not reported.
- v0.6.1 Confirm: RUNTIME PASS.
- v0.6.1 Revive: RUNTIME PASS.
- v0.6.1 AUTO: RUNTIME FAIL at `TopIcon` name lookup.
- v0.6.1 Auto Sell: RUNTIME PARTIAL/FAIL at item callbacks and completion proof.
- v0.6.2: BUILD/RUNTIME UNTESTED until CI and live retest.

## Version timeline

### v0.6

- Added semantic callbacks learned from donor v0.8.4.
- Build/CI passed.
- Runtime exposed an aggregate resolver gate before control selection.

### v0.6.1

- Split basic UI and Lua capabilities.
- Added metadata fallback and exact diagnostics.
- Kept state machines, retry timing and mouse-free policy unchanged.

### v0.6.2

- AUTO lookup layers `FindUI/MainFindUI` → active `UIObject.instances` → current two-stage `AUTO` root and `Đánh quái/Dừng` controls.
- Production two-stage fallback waits in controller state; it never sleeps the game callback thread and re-resolves every control.
- F4 retains v0.5 behavior and gains a shared-latch state-polling delivery fallback.
- Sell candidates prefer actionable item handler/logical cell order.
- Sell safety ceiling/no-progress are not completion; one free slot cannot leave fail phase.

## Do-not-break rules

- Resolve current controls for every action; never retain stale UI pointers.
- Ambiguous equal-score candidates must fail closed.
- Confirm must remain MessageBox-scoped.
- Revive must recheck `IsDeath` immediately.
- Sell must verify progress using fresh `FreeBagSpace`; 90 callbacks is a failure ceiling, never exhaustion proof.
- Sell completion must include positive verified progress and a session-relative proof; phase 10 must not auto-resume from one free slot.
- Button/Toggle paths must not require Lua Executor.

## Open architectural item

The frozen-client KB recommends an asynchronous legitimate `System.Action -> MainThread.Execute` boundary. This tool's current direct hook mutation is not promoted to production-verified until the separate CTS proof succeeds.

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
- v0.6.1: RUNTIME UNTESTED.

## Version timeline

### v0.6

- Added semantic callbacks learned from donor v0.8.4.
- Build/CI passed.
- Runtime exposed an aggregate resolver gate before control selection.

### v0.6.1

- Split basic UI and Lua capabilities.
- Added metadata fallback and exact diagnostics.
- Kept state machines, retry timing and mouse-free policy unchanged.

## Do-not-break rules

- Resolve current controls for every action; never retain stale UI pointers.
- Ambiguous equal-score candidates must fail closed.
- Confirm must remain MessageBox-scoped.
- Revive must recheck `IsDeath` immediately.
- Sell must verify progress using fresh `FreeBagSpace` and retain the 3-failure/90-callback guards.
- Button/Toggle paths must not require Lua Executor.

## Open architectural item

The frozen-client KB recommends an asynchronous legitimate `System.Action -> MainThread.Execute` boundary. This tool's current direct hook mutation is not promoted to production-verified until the separate CTS proof succeeds.

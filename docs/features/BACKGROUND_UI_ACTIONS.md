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
- v0.6.1: BUILD PASS (Windows CI 274), RUNTIME UNTESTED.
- v0.6.1.1: BUILD PASS; RUNTIME FAIL at Unity geometry class gate before item hit-test.
- v0.6.1.2: multi-assembly geometry resolver source complete; CI/live runtime pending.

## Version timeline

### v0.6

- Added semantic callbacks learned from donor v0.8.4.
- Build/CI passed.
- Runtime exposed an aggregate resolver gate before control selection.

### v0.6.1

- Split basic UI and Lua capabilities.
- Added metadata fallback and exact diagnostics.
- Kept state machines, retry timing and mouse-free policy unchanged.

### v0.6.1.1

- Preserved the working semantic shop chain through Equipment tab.
- Replaced only automatic bag-item name/ancestor enumeration with one manually captured fixed-cell hit-test.
- Invokes one current live callback per scheduler tick; no physical mouse input.
- Restored adaptive Step 5 lifetime: 90 callbacks initially, then stable post-sale `FreeBagSpace` for later sessions, capped at 90.

### v0.6.1.2

- Preserved the entire v0.6.1.1 item path.
- Resolved geometry classes independently across CoreModule, UIModule and legacy UnityEngine.dll.
- Added exact missing-class and assembly-availability diagnostics.

## Do-not-break rules

- Resolve current controls for every action; never retain stale UI pointers.
- Ambiguous equal-score candidates must fail closed.
- Confirm must remain MessageBox-scoped.
- Revive must recheck `IsDeath` immediately.
- Sell completion must still be verified using stable fresh `FreeBagSpace`; the fixed-slot hotfix retains the 90-callback ceiling.
- Button/Toggle paths must not require Lua Executor.

## Open architectural item

The frozen-client KB recommends an asynchronous legitimate `System.Action -> MainThread.Execute` boundary. This tool's current direct hook mutation is not promoted to production-verified until the separate CTS proof succeeds.

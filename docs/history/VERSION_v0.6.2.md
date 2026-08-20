# VERSION v0.6.2

## Scope

- Version: v0.6.2
- Direct base: v0.6.1 on the v0.5.0 product line
- Trigger: EVID-002 live results
- Changed areas: AUTO internal action resolution, F4 delivery, sell item ordering/completion proof
- Protected areas: runtime-passed Confirm/Revive, route/death/trade/rotation and unrelated v0.5 behavior

## Evidence disposition

- Promote v0.6.1 Confirm and Revive to RUNTIME PASS.
- Mark v0.6.1 AUTO RUNTIME FAIL at fixed-name lookup.
- Mark v0.6.1 Auto Sell RUNTIME PARTIAL/FAIL: semantic stages pass, item sale/completion does not.
- Mark F4 USER-REPORTED FAIL; retain UNKNOWN exact delivery cause because the old source still contains the v0.5 function and registration.

## Implementation

### AUTO

1. Invoke exact current `TopIcon.AutoTrainClick/AutoStopClick` through GUI lookup.
2. If GUI lookup returns null, scan current active `UIObject.instances` for exact `TopIcon` and invoke the same action.
3. If unavailable, Bridge callbacks current AUTO root and returns `MenuOpened`.
4. Controller keeps the request pending, waits 650 ms without blocking the game thread, then calls Bridge with stage 2.
5. Bridge re-enumerates and selects semantic `Đánh quái`/`Dừng`; generic unrelated `Dừng` is rejected.
6. Existing FSM verifies authoritative AutoFight ON/OFF.

### F4

- `ToggleGlobalPause` body stays equal in behavior/text to v0.5.
- `RegisterHotKey` + `WM_HOTKEY` stays primary.
- Timer polling reads F4 edge/state every 250 ms.
- WM and polling share `pauseKeyLatched_`, preventing a double toggle from one press.

### Auto Sell

- Sort controls by item-action confidence and natural cell index before pointer tie-break.
- Protocol distinguishes `NoCandidate`, `SafetyLimit` and `NoProgress`.
- Record session initial free space and verified sold count.
- Close/return only after positive progress + exhaustion + stable increase.
- Safety/no-progress/close failure remains phase 10; manual creation of one free slot cannot resume travel.

## Verification contract

- `tools/verify_v062_logic.py` statically protects all changes and unchanged runtime boundaries.
- MSVC x64 must compile controller, Bridge and four test executables.
- Background UI tests cover AUTO roles, unrelated Stop rejection and natural bag-cell ordering.
- Final Windows package is the unchanged artifact from CI run 282 (`32394836115`) on source commit `a1f81d84af4488d79db1eaf1a656280b2ade0945`: PASS; artifact `9416247108`; ZIP SHA-256 `53057686ad6857d244f0feba566fb00b4d5263872bd813d036470a13b29d8079`.
- CI PASS is build evidence only. Live status remains RUNTIME UNTESTED until one-PID retest.

## Required live log sequence

- F4: one pause log per press, then one resume log.
- AUTO path 1 or fallback: optional `AUTO root` intermediate log, then callback PASS, then snapshot state change.
- Sell: item callbacks show increasing `sold verified`; `SafetyLimit`/`NoProgress` must not close/return; successful exhaustion must log initial→final FreeBag.
- Regression: XN and Revive remain PASS.

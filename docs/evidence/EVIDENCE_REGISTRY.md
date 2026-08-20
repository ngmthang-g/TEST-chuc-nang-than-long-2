# EVIDENCE REGISTRY

## EVID-002 — v0.6.1 reaches Equipment tab but item stage is wrong

- Type: USER_RUNTIME / DESCRIPTION + SCREENSHOTS
- Date/version: 2026-08-20 / v0.6.1
- Character/PID: MộcBăngMi / PID 12252
- Confidence: High for the stage boundary; no claim that a specific UI class/name is absent

### Observation

The user reports that NPC/shop/Sell/Quick Sell/Equipment tab actions are correct, but the following item-cell stage does not repeat one fixed cell as v0.5 did. A partial sale can leave the bag full while the flow closes UI and returns toward training. The user clarifies that Quick Sell merely makes each item-cell click sell immediately and remaining equipment collapses into the same cell.

### Source corroboration

v0.6.1's `SellNextBagItem` searches control names/ancestors and stops on `NoCandidate`; it does not read the recorded Step 5 coordinate and does not assign the stable post-sale `FreeBagSpace` back to `sellStep5LearnedRepeat`.

### Does not prove

- The exact live name/handler of the second item cell.
- That Windows CI can prove the Unity hit-test on the user's client.
- Runtime success of v0.6.1.1 before live retest.

## EVID-001 — v0.6 user runtime resolver failure

- Type: USER_RUNTIME / LOG
- Date/version: 2026-08-20 / v0.6
- Character/PID: MộcBăngMi, RoleID 4024949, PID 12252
- Confidence: High for observed failure; insufficient to name the null dependency

### Observation

```text
[22:24:26] SESSION ROUTE RESET PASS: AutoPath OFF → route kế tiếp phải do tool StartPath mới để arm Confirm.
[22:24:31] LÂU LAN GATE XN NỘI BỘ FAIL: Không resolve đủ UIObject/UIButton/UIToggle/UIRect/Executor
[22:24:57] TÚI CHẠM NGƯỠNG → bắt đầu bán • Mã Kiêu Minh • M5 • ID 373
[22:25:05] BÁN NỀN FAIL khi mở chuỗi shop: Không resolve đủ UIObject/UIButton/UIToggle/UIRect/Executor • probe trước: Không resolve đủ UIObject/UIButton/UIToggle/UIRect/Executor • dừng fail-closed
```

### Supports

- Route/FSM progressed far enough to invoke both semantic UI flows.
- Both flows failed at their shared UI readiness gate before role scoring/callback.
- v0.6 cannot be called runtime-pass for Confirm or Auto Sell.

### Does not prove

- Which of the aggregated dependencies was null.
- Whether UIButton discovery itself is valid once optional dependencies are removed.
- Whether Revive or AUTO succeeds/fails.
- Whether the v0.6.1 hotfix works at runtime.

# EVIDENCE REGISTRY

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

## EVID-002 — v0.6.1 live action results and regressions

- Type: USER_RUNTIME / LOG + USER OBSERVATION + SCREENSHOTS
- Date/version: 2026-08-20 / v0.6.1
- Character/PID: MộcBăngMi, RoleID 4024949, PID 12252
- Confidence: High for logged transitions; medium for unlogged F4 delivery cause

### Runtime observations

```text
[23:07:45] ĐẦU THAI P2 PASS: Bridge xác minh IsDeath rồi gọi UIButton.HandleClickEvent nội bộ; chuỗi acc khác không mất index/repeat.
[23:08:00] LÂU LAN GATE WATCHDOG P1: tìm MessageBox + gọi callback nút đồng ý nội bộ • KHÔNG foreground/chuột • lần 1
[23:08:01] FREEZE ACTION: game đang chuyển map
[23:08:25] Đã tới bãi và ổn định.
[23:08:25] PRIORITY #3 AUTO NỘI BỘ FAIL: Không tìm thấy Lua UI theo tên
```

User additionally reported:

- F4 pause no longer reacts.
- Auto Sell reaches `ClickNPC → shop → Bán vật phẩm → Bán nhanh → Trang bị` and can close the UI, but the item-click portion is unreliable.
- In one run the bag remained full after two passes; in another, one sold item created a free slot and the tool returned to train too early.
- Screenshots show the visible fallback interaction is two actions: `AUTO` root, then `Đánh quái` to start or `Dừng` to stop.

### Supports

- BUG-001 resolver split works at runtime for Confirm/Revive and semantic shop-stage controls.
- Confirm and Revive are individual **RUNTIME PASS** results for v0.6.1.
- AUTO is an individual **RUNTIME FAIL** at the fixed-name lookup, after Lua capability resolution.
- Auto Sell has two independent problems: item-control execution/order and controller completion proof.
- F4 needs delivery hardening, but its exact root cause is not proven by the supplied log.

### Does not prove

- That `TopIcon` is absent from `UIObject.instances`; v0.6.1 did not inspect/report that fallback.
- That clicking visible `Đánh quái` alone is the canonical semantic start; client source identifies `StartAutoFight(Train)` as the authoritative action.
- Which exact startup hotkey registration result occurred.
- That v0.6.2 fixes work at runtime.

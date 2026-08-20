# BUG REGISTRY

## BUG-003 — v0.6.1.1 geometry resolver assumes one Unity assembly

- Status: FIXED-IN-SOURCE / RUNTIME UNTESTED
- Severity: High
- First observed: v0.6.1.1, user runtime 2026-08-20
- Related feature: Auto Sell fixed item-cell hit-test only
- Evidence: EVID-004

### Runtime evidence

`BÁN NỀN FAIL callback item: Thiếu RectTransform/Utility/Screen để hit-test ô cố định • dừng fail-closed`

The separate CoreModule-open error did not fire. Therefore CoreModule opened, but at least one of RectTransform, Transform, GameObject, RectTransformUtility or Screen was not found in that one image. The old aggregate detail cannot identify the exact class.

### Root cause

CONFIRMED: v0.6.1.1 resolves all five classes only from `UnityEngine.CoreModule`. Unity clients may split UI types into `UnityEngine.UIModule` or use legacy monolithic `UnityEngine.dll`.

### Fix in v0.6.1.2

- Open CoreModule, UIModule and legacy UnityEngine independently.
- Resolve each class through an explicit ordered fallback.
- Report the exact missing class list and assembly availability.
- Preserve row 5, 90/adaptive count and every non-resolver action.

## BUG-002 — v0.6.1 item-cell auto-enumeration does not complete the bag sale

- Status: FIXED-IN-SOURCE / RUNTIME UNTESTED
- Severity: High
- First observed: v0.6.1, user runtime 2026-08-20
- Last known-good behavior: v0.5 coordinate Step 5 (physical mouse)
- Related feature: Auto Sell item stage only
- Evidence: EVID-002

### Confirmed scope

The user reports that `ClickNPC → shop → Bán vật phẩm → Bán nhanh → Trang bị` succeeds. The failure begins only when v0.6.1 attempts to discover/callback bag-item controls. v0.6.1 source confirms that active Step 5 no longer consumes the recorded item coordinate or learned repeat.

### Fix in v0.6.1.1

Keep all working semantic stages. Replace only `CollectSafeBagItems` selection with a fresh coordinate-to-live-control hit-test and internal callback. Restore the v0.5 adaptive count lifetime: 90 first, then stable `FreeBagSpace`, capped at 90.

### Runtime verification required

1. Capture the center of equipment cell 2 in the active item row.
2. Confirm cursor and foreground never move during all callbacks.
3. Confirm the first session reaches 90 callbacks, closes UI and learns stable free space.
4. Confirm the next session logs and uses the learned count.

## BUG-001 — Aggregate UI readiness blocks every internal control action

- Status: FIXED-BUILD-PASS / RUNTIME UNTESTED
- Severity: High
- First observed: v0.6, user runtime 2026-08-20 22:24–22:25 local time
- Last known-good: none for this new internal-callback implementation
- First confirmed-bad: v0.6
- Related features: Lâu Lan Confirm, Auto Sell, Revive, AUTO
- Evidence: EVID-001

### Known evidence

Both Confirm and Auto Sell reached `EnsureUiRuntime` and returned the identical aggregate error:

`Không resolve đủ UIObject/UIButton/UIToggle/UIRect/Executor`

Source inspection proves that v0.6 required UIObject, UIButton, UIToggle, UIRectTransform, MonoBehaviourExecutor, LuaSystemAPI_GUI, System.Object and UIObject.instances as one indivisible gate.

### Root cause

- CONFIRMED: dependency scope was wrong. UIButton/UIToggle discovery was unnecessarily blocked by Lua/Executor dependencies.
- UNKNOWN: which exact pointer was null on the user's client, because v0.6 erased that information.
- LIKELY: MonoBehaviourExecutor, because its namespace was guessed while the donor only proved its native RVA.

### Fix in v0.6.1

- Basic discovery and Lua execution are separate capabilities.
- Button/Toggle do not resolve Executor.
- Executor resolution uses candidate namespaces and bounded metadata enumeration with method-surface validation.
- Error output names the exact missing component.

### Runtime verification required

1. Confirm at the Lâu Lan gate.
2. Mã Kiêu Minh semantic shop stages.
3. AUTO start/stop as a separate Lua-dependent test.

### Do not do

- Do not reintroduce coordinate fallback merely because a semantic dependency is absent.
- Do not mark fixed-runtime from CI or static tests.
- Do not conclude all UI class names are wrong from the old aggregate message.

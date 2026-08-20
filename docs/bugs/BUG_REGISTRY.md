# BUG REGISTRY

## BUG-001 — Aggregate UI readiness blocks every internal control action

- Status: FIXED-RUNTIME in v0.6.1 for Button/Toggle discovery
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

EVID-002 confirms XN, Revive and Mã Kiêu Minh semantic shop stages progressed beyond the old aggregate gate. AUTO reached a later name-lookup failure, tracked separately as BUG-002.

### Do not do

- Do not reintroduce coordinate fallback merely because a semantic dependency is absent.
- Do not mark fixed-runtime from CI or static tests.
- Do not conclude all UI class names are wrong from the old aggregate message.

## BUG-002 — AUTO direct Lua lookup cannot resolve TopIcon

- Status: FIXED-BUILD-PASS / RUNTIME UNTESTED in v0.6.2
- Severity: High
- First observed: v0.6.1, user runtime 2026-08-20 23:08 local time
- Evidence: EVID-002

### Observation and root cause boundary

`PRIORITY #3 AUTO NỘI BỘ FAIL: Không tìm thấy Lua UI theo tên` repeated after arriving at the train spot. The Bridge had already resolved its Lua/Executor capability; failure occurred in the `FindUI/MainFindUI("TopIcon")` result.

- CONFIRMED: the active v0.6.1 AUTO path depended on finding one fixed UI name.
- UNKNOWN: whether the exact client renames/does not register `TopIcon`, or whether its GUI lookup API returns null despite the object being present.
- The visible two-click UI and source semantics are both retained as independent fallback evidence; neither is declared runtime-pass before retest.

### Fix in v0.6.2

1. Try exact `FindUI/MainFindUI` action.
2. Find exact active `TopIcon` in `UIObject.instances` and invoke the same Lua action.
3. If unavailable, stage current `AUTO` root then freshly resolve current `Đánh quái/Dừng` control after 650 ms.
4. Continue to verify authoritative AutoFight state; callback OK alone is not success.

## BUG-003 — Auto Sell can skip item actions and finish after one free slot

- Status: FIXED-BUILD-PASS / RUNTIME UNTESTED in v0.6.2
- Severity: High
- First observed: v0.6.1 user runtime
- Evidence: EVID-002

### Confirmed defects

- Candidate order used raw managed pointer address, not logical/actionable bag-cell order.
- `callbacks >= 90` returned `NoCandidate`, conflating a safety ceiling with exhaustion.
- Controller phase 7 accepted any `FreeBagSpace > 0` as completion; phase 10 also escaped automatically after any manual/new free slot.
- Close failure could still advance into bag verification.

### Fix in v0.6.2

- Rank item-click handler controls and natural cell index before pointer tie-break.
- Separate `SafetyLimit`, `NoProgress` and `NoCandidate` protocol outcomes.
- Completion requires verified sold count, exhaustion signal and stable FreeBag increase relative to session start.
- Error phase and close failure stay fail-closed; one free slot cannot return to train.

### Remaining boundary

UI-callback exhaustion is not equivalent to the canonical fresh inventory-instance scan. A later packet-level seller must use current shop state and one-item mutation proof; v0.6.2 does not overclaim that implementation.

## BUG-004 — F4 hotkey user-reported nonresponsive

- Status: MITIGATED-BUILD-PASS / RUNTIME UNTESTED in v0.6.2
- Severity: Medium
- Evidence: EVID-002

Source diff proves `RegisterHotKey(F4)`, `WM_HOTKEY` and the exact v0.5 `ToggleGlobalPause` body were still present in v0.6.1. Therefore deletion/regression of pause semantics is not the confirmed root cause. Registration conflict or lost message delivery is UNKNOWN because startup warning output was not supplied.

v0.6.2 keeps the old path and adds latched 250 ms `GetAsyncKeyState` edge polling. Both paths share one latch so a single press cannot toggle twice.

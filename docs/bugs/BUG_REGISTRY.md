# BUG REGISTRY

## BUG-001 — Aggregate UI readiness blocks every internal control action

- Status: FIXED-BUILD-PENDING / RUNTIME UNTESTED
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

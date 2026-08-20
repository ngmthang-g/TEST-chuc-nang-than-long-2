# DECISION REGISTRY

## DEC-004 — Use the exact client EventSystem point dispatcher for Step 5

- Date/version: 2026-08-20 / v0.6.1.3
- Status: ACTIVE
- Decision: dispatch one fixed-cell left click as `InputSyncManager.TryClickUI(0, point)` followed by `EndUIDrag(point)` on the existing game window thread.

### Why

Runtime proves 149 custom UI controls expose geometry but none owns the bag-cell point. Exact client metadata and native call sites instead prove that `InputSyncManager` already maps a screen point through Unity EventSystem and performs separate press/release phases.

### Constraints

- Resolve by class/method/field metadata; do not hardcode native RVAs.
- Never call `TryClickUI` without the matching `EndUIDrag` for a successful press.
- Do not overlap an existing InputSync drag.
- Cancel only cleanup state created by this action when release fails.
- Keep the coordinate, row 5, 90/adaptive count and fresh `FreeBagSpace` proof.
- Do not change F4 or any non-item sell stage.

## DEC-003 — Resolve Unity geometry per class across assembly layouts

- Date/version: 2026-08-20 / v0.6.1.2
- Status: RETAINED FOR DIAGNOSTICS / SUPERSEDED FOR ACTIVE ITEM-CELL DISPATCH BY DEC-004
- Decision: search CoreModule, UIModule and legacy UnityEngine.dll with an explicit class-specific order.

### Why

Runtime proves CoreModule exists but the v0.6.1.1 all-in-one class gate fails. `RectTransformUtility` is UI-related and may be split from core classes; older clients may instead use monolithic UnityEngine.dll.

### Constraints

- Do not change the coordinate, row, count or callback transport.
- Missing classes remain fail-closed.
- Diagnostics must name each missing class and available assembly.
- Passing this gate is not yet proof that a live item control exposes RectTransform.

## DEC-002 — Scoped fixed-slot internal callback for v0.6.1.1

- Date/version: 2026-08-20 / v0.6.1.1
- Status: SUPERSEDED BY DEC-004
- Decision: keep semantic shop stages, but let one captured coordinate select the current live item-cell callback at every Step 5 tick.

### Why this exception exists

The client knowledge base recommends instance-ID sale packets and normally rejects blind coordinate fallback. The user explicitly requested a one-fix v0.6.1 hotfix that restores the already understood v0.5 collapsing-cell behavior, without introducing item policy or packet-sale changes. The coordinate therefore selects a managed UI callback; it never produces Windows mouse input.

### Constraints

- Resolve fresh on every click.
- One callback per Bridge request.
- 90 first, then learned stable free space capped at 90.
- Ambiguous or missing hit fails closed.
- Do not change F4 or any non-item sell stage.

## DEC-001 — Resolve UI capabilities lazily

- Date/version: 2026-08-20 / v0.6.1
- Status: ACTIVE
- Decision: Separate `UIObject.instances` discovery from UIRect/Lua execution dependencies.

### Context

v0.6 treated seven managed components as one readiness condition. Runtime showed that one absent optional component disabled both Confirm and Auto Sell before any control could be inspected.

### Consequences

- UIButton and UIToggle remain usable without MonoBehaviourExecutor.
- UIRect failures cannot disable Button/Toggle actions globally.
- AUTO remains independently dependent on LuaSystemAPI_GUI, System.Object and Executor.
- Diagnostics expose capability boundaries and preserve fail-closed behavior.

### Alternatives rejected

- Hardcode only the donor RVA: build-specific and contrary to semantic resolution.
- Restore physical clicks automatically: hides the defect and retakes the mouse.
- Replace the whole action dispatcher in the same hotfix: too broad without the required CTS MainThread live proof.

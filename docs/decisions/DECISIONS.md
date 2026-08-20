# DECISION REGISTRY

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

## DEC-002 — Layer AUTO lookup and restore F4 delivery without changing semantics

- Date/version: 2026-08-20 / v0.6.2
- Status: ACTIVE

### Decision

- Keep exact `TopIcon.AutoTrainClick/AutoStopClick` first.
- Add exact active-name registry lookup, then a freshly-resolved two-stage UI callback fallback.
- Preserve v0.5 `ToggleGlobalPause` and add a shared-latch keyboard-state fallback around the existing hotkey path.

### Why

The runtime failure proves fixed-name GUI lookup is insufficient, not that the Lua action is wrong. User screenshots prove a current two-action visible route. F4 source inspection proves its core pause function was not deleted, so replacing pause semantics would be unjustified.

### Rejected

- Directly write the AutoFight state flag: it does not establish built-in train setup/teardown.
- Re-enable physical AUTO coordinates: retakes the mouse and hides lookup defects.
- Remove RegisterHotKey and use polling only: unnecessary behavior change; both delivery paths can coexist safely with one latch.

## DEC-003 — Sell completion requires transaction proof, not “bag is no longer full”

- Date/version: 2026-08-20 / v0.6.2
- Status: ACTIVE

### Decision

Treat callback ceiling and zero progress as explicit failures. Only permit return-to-train after a verified sold count, current candidate exhaustion and stable FreeBag growth from the session baseline.

### Why

The live run demonstrated that `FreeBagSpace > 0` can mean only one item was sold. Client research also establishes the canonical loop is one live instance → one sell request → inventory proof → rescan; a blind callback limit cannot stand in for exhaustion.

### Boundary

This decision hardens the current callback implementation. It does not claim UI-control scanning is equivalent to fresh `GetItemsAtSite(Bag)` policy classification; that packet-level loop remains separate work.

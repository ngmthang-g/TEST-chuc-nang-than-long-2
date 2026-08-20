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

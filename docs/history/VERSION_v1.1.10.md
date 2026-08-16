# VERSION v1.1.10-test — LuaSystemManager / LuaEnv resolver

## Trigger
User runtime-tested delivered v1.1.9 on 2026-08-16. The screenshot/log shows NPC 339 / Map 5 opens the dialog, then every live-selection probe fails at exactly:

`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`

The status remains `DIALOG ĐÃ MỞ • CHỜ LIVE SELECTION TRỊ LIỆU`.

## What this proves
- route/NPC-open path is still working;
- v1.1.9 does not reach `LuaEnv.DoString`, `Selections`, Treatment ID, `MAINTHREAD_PROOF` action stage or packet send;
- the new failure is narrower than v1.1.8: UIRoot was already abandoned, and now the exact failing stage is the assumed `LuaSystemManager` singleton resolution;
- this runtime does **not** prove the Lua probe logic, packet action or server behavior is bad because none of those stages were reached.

## Source correction
v1.1.9 `ResolveLuaEnvV119()` first required `ResolveLuaSystemManagerV119()` even though metadata only verifies `get_LuaEnv/set_LuaEnv`, not a canonical `get_Instance` contract. It then tried only `get_Instance` plus four guessed static field names.

v1.1.10 changes only this boundary.

### New resolver order
1. Resolve `LuaSystemManager` class semantically.
2. Resolve `get_LuaEnv`.
3. **If `get_LuaEnv` is static, invoke it directly** without requiring a manager instance.
4. If an instance getter is required, try semantic `get_Instance` if present.
5. Enumerate static **reference** fields on the manager hierarchy and a bounded set of known Lua bridge classes; read them only through `il2cpp_field_static_get_value`; accept a candidate only when its runtime class is `LuaSystemManager` or derives from it.
6. If the manager is actually in the `UnityEngine.Object` hierarchy, try typed Unity lookups (`FindFirstObjectByType`, `FindAnyObjectByType`, legacy `FindObjectOfType`) using the real managed `System.Type`.
7. If still unresolved, fail closed with `LUA_MANAGER_V120` diagnostics including static reference count/candidates and whether Unity lookup was applicable.

No heap pointer guessing, broad scanning, UIRoot retry, hardcoded Treatment ID or NPC reopen was added.

## Active flow
`one NPC open -> current GameDialog presence -> ResolveLuaEnvV120 -> DoString -> current GameDialogData.Selections -> semantic text match -> re-read current selection at action time -> exact CMD_SHOW_GAMEDIALOG request -> fresh state proof`.

Diagnostics:
- `LUA_MANAGER_V120 ...`
- `LUA_DIALOG_V120 ... route=... T=... C=... K=...`
- `ACTION_V120 ...`

## Canonical facts used
- `LuaSystemManager` exposes `get_LuaEnv/set_LuaEnv`, but canonical KB does not claim a stable singleton getter/field name.
- IL2CPP snapshot exports field/class reflection helpers used for metadata-driven bounded resolution.
- GameDialog identity remains `Selections[selectionID]=visibleText`.
- selection request remains `CMD_SHOW_GAMEDIALOG=100007`, payload `selectionID:SelectedItemID`.

## Source/build
Relevant v1.1.10 code:
- `src/bridge_lua_manager_v1_1_10.inc`
- `src/bridge_action_v1_1_10.inc`
- `src/bridge.cpp`
- `src/protocol.h`
- controller title/start log + build/workflow versioning.

Source-bearing build commit: `d35517385266d4fa75011374966816a0e8d5ada1`.

GitHub Actions run `31933118883`:
- architecture audit PASS;
- Route FSM PASS;
- Heal FSM PASS;
- bridge DLL compile + PE verification PASS;
- controller EXE compile PASS;
- artifact upload PASS.

Artifact: `ThanLongTestAutoHeal-v1.1.10`, ID `9259895908`, ZIP digest `sha256:4b43c205bedd4177288b562ed7df20b08adb5f295b6b1601a11699e0bb80ef60`.

## Status
- BUILD/CI: **PASS** for source-bearing resolver build.
- RUNTIME v1.1.10: **UNTESTED**.
- BUG-001: **OPEN**.

## Next runtime proof
The next user log should identify the earliest reached stage:
1. `LUA_DIALOG_V120` with resolver route and `T=<id>`;
2. or exact `LUA_MANAGER_V120` failure details;
3. then, only if T resolves, `MAINTHREAD_PROOF` / `ACTION_V120` and the server-driven follow-up.

Do not redesign later stages until this resolver boundary is runtime-proven.
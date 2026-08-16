# PROJECT KNOWLEDGE

## Identity / mandatory startup
- Repo: `ngmthang-g/TEST-chuc-nang-than-long-2`; branch `agent/auto-tri-lieu-v1.1.0`.
- Current version: `v1.1.12-test`; full Auto trị liệu known-good: **NONE**.
- Every version: `AI_START_HERE.md` -> V2 protocol -> client-analysis TXT -> `AI_PROJECT_HANDOFF_FULL.md` -> this file -> `CHANGELOG.md` -> affected feature/BUG/DEC/EVID/history -> source/tests.
- If only artifact ZIP is available, read `AI_PROJECT_HANDOFF_FULL.md` first.
- Client facts: start from `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`; no broad reverse for solved facts.
- BUILD/CI PASS != RUNTIME PASS.

## Artifact handoff contract
From v1.1.11 onward every delivered CI artifact must contain EXE/DLL plus:
- `AI_PROJECT_HANDOFF_FULL.md`;
- `AI_START_HERE.md`;
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`;
- `AI_CLIENT_ANALYSIS_RULES.txt`;
- `PROJECT_KNOWLEDGE.md`;
- `CHANGELOG.md`;
- generated `BUILD_EVIDENCE.txt` with branch-head SHA, checkout SHA, run/build/runtime state.

## Runtime-confirmed protected path
- raw user-captured MapID/X/Y;
- mount / AutoPath / dismount;
- NPC 339 Đỗ Thanh Đằng on Map 5 can be opened semantically;
- v1.1.8 tested transaction removed the old WaitTreatment reopen behavior: one NPC interaction followed by fail-closed observation.

## Runtime layer history
### v1.1.8
GameDialog present but UIRoot observer repeatedly returned `clickable=0 • texts=0 • labels=<none>`. UIRoot/CoreChildren/UIButton is abandoned for this dynamic dialog.

### v1.1.9
GameDialog opened; probe failed at `LuaSystemManager instance unresolved`. LuaEnv/DoString/Selections/action were not reached.

### v1.1.10
User runtime reached the returned Lua environment object and then failed at old DoString lookup. Therefore `LuaSystemManager -> LuaEnv` is **RUNTIME PARTIAL PASS**.

### v1.1.11 — latest runtime evidence 2026-08-16
User tested the delivered v1.1.11 artifact. Repeated exact diagnostic:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

This is decisive new knowledge:
- `LuaSystemManager.get_LuaEnv` returns a live **`MoonSharp.Interpreter.Script`** object in this client/runtime;
- the relevant current method shape is **`DoString(System.String, MoonSharp.Interpreter.Table, System.String) -> MoonSharp.Interpreter.DynValue`**;
- v1.1.11 did enumerate the real DoString method, but its accepted-shape scorer incorrectly required parameter #2 to be `System.String`; MoonSharp parameter #2 is `Table`, so the real method was rejected;
- v1.1.11 did **not** execute the Lua probe; current Selections/Treatment ID/action/server remain NOT REACHED / UNKNOWN.

The earlier targeted xLua assumption is now **DISPROVEN for the returned runtime type**. Preserve it only as failed investigation history, not as current engine identity.

Official MoonSharp source independently matches the runtime signature:
`Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null)` and returns a `DynValue`; `DynValue.String` exposes a string result when the script returns a string.

## Canonical GameDialog contract
- `GameDialog.Selections[selectionID] = visibleText`.
- IDs are server/runtime data; never hardcode Treatment ID.
- built-in AutoFight dialog flows inspect current selections.
- `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`; ordinary function choice commonly uses `-1` item.
- MessageBox OK uses its callback path.

## v1.1.12 active design
Only the now-confirmed MoonSharp DoString call/result boundary changes. Keep v1.1.10 LuaEnv resolver, route/NPC/no-reopen, current Selections probe, and current-ID action semantics.

Active files:
- `src/bridge_lua_manager_v1_1_10.inc` — runtime-proven path to returned Script object;
- `src/bridge_lua_moonsharp_v1_1_12.inc` — exact MoonSharp DoString invocation + DynValue.String extraction + V122 dialog probe;
- `src/bridge_action_v1_1_12.inc` — V122 current-ID action;
- `AI_PROJECT_HANDOFF_FULL.md` — consolidated artifact handoff.

Exact flow:
1. `ResolveLuaEnvV120()` returns current object;
2. require runtime metadata matching `DoString(String, MoonSharp.Interpreter.Table, String) -> MoonSharp.Interpreter.DynValue`;
3. invoke as `DoString(code, null, "ThanLongAutoHealV122")` — **Table is argument #2, friendly name is argument #3**;
4. read returned `DynValue.get_String()`;
5. parse current T/C/K from existing bounded GameDialog/AutoFight_Main probe;
6. if `T>0`, re-read V122 current state immediately before action and submit exact semantic selection;
7. observe fresh server/UI/result state.

New diagnostics:
- `LUA_MOONSHARP_V122 ...` for exact method/invoke/result failures;
- `LUA_DIALOG_V122 • route=... • T=... • C=... • K=...` after successful MoonSharp execution;
- `ACTION_V122 ...` only after live choice is re-read.

## Build evidence
- v1.1.11 final build/packaging passed; runtime failed at its shape filter as above.
- v1.1.12 source-bearing build: **PENDING** at commit creation.
- v1.1.12 runtime: **UNTESTED**.

## Hard rules
- no inferred NPC X/Y as universal truth;
- no hardcoded/cached Treatment ID;
- no WaitTreatment NPC reopen;
- no UIRoot/UIButton tuning for this dialog;
- no fixed sleep as success proof;
- one mutable action per PID;
- runtime metadata validation before invoking dynamic managed methods;
- current runtime engine identity at this boundary is MoonSharp, not xLua;
- preserve build/runtime distinction and failed attempts;
- every artifact must include consolidated knowledge/handoff files.

## BUG-001 current status
OPEN.
- resolved component: controller-driven NPC reopen absent in tested v1.1.8 transaction;
- failed v1.1.8: UIRoot representation;
- failed v1.1.9: LuaSystemManager singleton resolver assumption;
- partial pass v1.1.10: LuaSystemManager -> returned Script object;
- failed v1.1.11: DoString accepted-shape model rejected the runtime-proven MoonSharp overload;
- current experiment v1.1.12: exact MoonSharp invocation + DynValue.String extraction;
- unknown: actual live Treatment selectionID and server/follow-up sequence until the V122 Lua probe executes.

## Next runtime evidence
Run v1.1.12 and preserve from first NPC open. Earliest useful marker:
- `LUA_MOONSHARP_V122 ...` — fix only exact method/invoke/result boundary;
- `LUA_DIALOG_V122 ... T=0 ...` — DoString passed; inspect only runtime table/path/text;
- `LUA_DIALOG_V122 ... T=<positive>` — then evaluate `MAINTHREAD_PROOF` / `ACTION_V122` and fresh result state.

See `AI_PROJECT_HANDOFF_FULL.md`, `docs/history/VERSION_v1.1.11.md`, `docs/history/VERSION_v1.1.12.md`, `docs/investigations/V121_MOONSHARP_RUNTIME_FINDING.md`, feature/BUG/EVID/DEC registries.

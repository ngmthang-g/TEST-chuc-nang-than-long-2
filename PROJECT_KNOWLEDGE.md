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
GameDialog opened; probe failed at `LuaSystemManager instance unresolved`. Returned script object/DoString/Selections/action were not reached.

### v1.1.10
Manager resolver advanced to the object returned by `get_LuaEnv`, then old DoString lookup failed. Therefore manager -> returned object is **RUNTIME PARTIAL PASS**.

### v1.1.11 — latest runtime evidence
User tested the delivered artifact and repeatedly got:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

Confirmed:
- current returned runtime object is `MoonSharp.Interpreter.Script`;
- live method is `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
- V121 enumerated the correct method but rejected it because its scorer required parameter index 1 to be `System.String`; the live parameter index 1 is `MoonSharp.Interpreter.Table`;
- V121 never invoked the chunk; Selections/Treatment/action/server remain NOT REACHED / UNKNOWN.

The earlier xLua engine assumption is **DISPROVEN for the current returned object** and is preserved only as failed-investigation history. Official MoonSharp source independently matches the runtime signature and `DynValue.String` result access.

## Canonical GameDialog contract
- `GameDialog.Selections[selectionID] = visibleText`.
- IDs are server/runtime data; never hardcode Treatment ID.
- built-in AutoFight dialog flows inspect current selections.
- `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`; ordinary function choice commonly uses `-1` item.
- MessageBox OK uses its callback path.

## v1.1.12 active design
Only the now-confirmed MoonSharp DoString call/result boundary changes. Keep v1.1.10 returned-object resolver, route/NPC/no-reopen, current Selections probe and current-ID action semantics.

Active files:
- `src/bridge_lua_manager_v1_1_10.inc` — runtime-proven path to current Script object;
- `src/bridge_lua_moonsharp_v1_1_12.inc` — exact MoonSharp invocation + `DynValue.String` extraction + V122 dialog probe;
- `src/bridge_action_v1_1_12.inc` — V122 current-ID action;
- `AI_PROJECT_HANDOFF_FULL.md` — consolidated artifact handoff.

Exact flow:
1. `ResolveLuaEnvV120()` returns current object;
2. require current metadata matching `DoString(String, MoonSharp.Interpreter.Table, String) -> MoonSharp.Interpreter.DynValue`;
3. invoke `DoString(code, null, "ThanLongAutoHealV122")` — Table is argument #2, friendly name is #3;
4. read returned `DynValue.get_String()`;
5. parse T/C/K from the existing bounded GameDialog/AutoFight_Main probe;
6. if `T>0`, re-read V122 immediately before action and submit exact current selection;
7. observe fresh server/UI/result state.

Diagnostics:
- `LUA_MOONSHARP_V122 ...` for exact method/invoke/result failure;
- `LUA_DIALOG_V122 • route=... • T=... • C=... • K=...` after MoonSharp execution;
- `ACTION_V122 ...` only after live choice is re-read.

## Build evidence — v1.1.12
Initial source commit `ad2f0403863251330b1aca80eb1eba9681b58c9a` triggered run `31937607280`, which **FAILED before compilation** in architecture audit because the audit incorrectly required the literal `MoonSharp.Interpreter.Script` inside the resolver source. The resolver intentionally validates the live method signature rather than hardcoding that class-name literal. No artifact was uploaded.

Correction commit `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1` removed only that false-positive audit token while keeping V122 structural checks.

Source-bearing run `31937703988`: **CI/BUILD PASS**.
- architecture audit PASS;
- Route FSM PASS;
- Heal FSM PASS;
- bridge DLL compile + PE verification PASS;
- controller EXE compile PASS;
- knowledge packaging PASS;
- artifact upload PASS.

Artifact `ThanLongTestAutoHeal-v1.1.12`, ID `9261162703`, ZIP digest `sha256:d25f999934be62152cf02fd0251f533743f3e9eaa0d4178f74c2fa1007d85c19`.
Verified exactly 9 artifact members.  
EXE SHA-256: `32a3977469a26f5807be192a620630fba9ea3242e265706522fd7d93bb27c823`.  
DLL SHA-256: `553c1d245c06c614561b9c2c1e6368daf30cb671b46602aec60b1938f326a037`.

Source-bearing `BUILD_EVIDENCE.txt`:
- `SOURCE_HEAD_SHA=33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1`
- `CHECKOUT_SHA=3855f781069c240cffe2ee8349bcdfb657535d85`
- `GITHUB_RUN_ID=31937703988`
- `BUILD=PASS`
- `RUNTIME=UNTESTED_FOR_V1.1.12`

v1.1.12 runtime remains **UNTESTED**.

## Hard rules
- no inferred NPC X/Y as universal truth;
- no hardcoded/cached Treatment ID;
- no WaitTreatment NPC reopen;
- no UIRoot/UIButton tuning for this dialog;
- no fixed sleep as success proof;
- one mutable action per PID;
- runtime metadata validation before dynamic managed invocation;
- current runtime engine identity at this boundary is MoonSharp, not xLua;
- preserve build/runtime distinction and failed attempts;
- every artifact includes consolidated knowledge/handoff files.

## BUG-001 current status
OPEN.
- resolved: controller-driven NPC reopen absent in tested v1.1.8 transaction;
- failed v1.1.8: UIRoot representation;
- failed v1.1.9: LuaSystemManager singleton resolver assumption;
- partial pass v1.1.10: manager -> current Script object;
- failed v1.1.11: DoString accepted-shape model rejected runtime-proven MoonSharp overload;
- current v1.1.12: exact MoonSharp invocation + DynValue.String extraction is BUILD PASS, RUNTIME UNTESTED;
- unknown: actual live Treatment selectionID and server/follow-up sequence until V122 probe executes.

## Next runtime evidence
Run v1.1.12 and preserve from first NPC open:
- `LUA_MOONSHARP_V122 ...` — diagnose only exact invocation/result boundary;
- `LUA_DIALOG_V122 ... T=0 ...` — invocation passed; inspect current table/path/text;
- `LUA_DIALOG_V122 ... T=<positive>` — then evaluate `MAINTHREAD_PROOF` / `ACTION_V122` and fresh result state.

See `AI_PROJECT_HANDOFF_FULL.md`, `docs/history/VERSION_v1.1.11.md`, `docs/history/VERSION_v1.1.12.md`, `docs/investigations/V121_MOONSHARP_RUNTIME_FINDING.md`, feature/BUG/EVID/DEC registries.

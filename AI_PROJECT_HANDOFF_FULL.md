# AI PROJECT HANDOFF FULL — THẦN LONG AUTO TRỊ LIỆU TEST LAB

> First-read file inside every artifact ZIP. It summarizes current engineering state but does not replace the repo's full knowledge base.

## 0. MANDATORY STARTUP
Project: `ngmthang-g/TEST-chuc-nang-than-long-2`  
Branch: `agent/auto-tri-lieu-v1.1.0`  
Current version: **v1.1.12-test**  
Feature: Auto trị liệu NPC  
Full known-good: **NONE**  
BUG-001: **OPEN**

Read in order: V2 protocol -> client-analysis rules -> this file -> `PROJECT_KNOWLEDGE.md` -> `CHANGELOG.md` -> affected feature/BUG/DEC/EVID/history -> source/tests.

Canonical client knowledge starts at `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`. Do not broad reverse solved client facts. Runtime evidence outranks assumptions. BUILD/CI PASS is not RUNTIME PASS.

## 1. GOAL / PROTECTED PATH
Goal:
`stable state -> route -> one NPC open -> current server GameDialog -> current Trị liệu selection -> one semantic action -> fresh follow-up -> result proof`.

Runtime-protected partial path:
- raw MapID/X/Y capture;
- mount / AutoPath / stop / dismount;
- NPC 339 Đỗ Thanh Đằng on Map 5;
- semantic `ClickNPC(339)` opens intended GameDialog;
- no WaitTreatment NPC reopen after successful initial open in tested v1.1.8 transaction.

## 2. CANONICAL GAMEDIALOG CONTRACT
- `GameDialogData.Selections[selectionID] = visibleText`;
- selection IDs are current server/runtime state, never a universal Treatment constant;
- `CMD_SHOW_GAMEDIALOG = 100007`;
- payload = `selectionID:SelectedItemID`, ordinary function choice commonly `<currentID>:-1`;
- built-in automation inspects active Selections;
- server-driven dialog processing may destroy/recreate UI, so flicker is not completion proof;
- MessageBox OK uses semantic callback.

## 3. BUG-001 RUNTIME LINEAGE
### v1.1.8
GameDialog opened but bridge-visible UIRoot observer returned `clickable=0 • texts=0 • labels=<none>`. UIRoot/UIButton abandoned. Anti-reopen passed for that transaction.

### v1.1.9
After GameDialog open: `LuaSystemManager instance unresolved`. Returned object/DoString/Selections/action not reached.

### v1.1.10
Metadata-driven manager resolver reached the object returned by `get_LuaEnv`, then old DoString lookup failed. Manager -> returned object = RUNTIME PARTIAL PASS.

### v1.1.11 — decisive current runtime finding
Repeated user diagnostic:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

Confirmed:
- returned runtime object is **`MoonSharp.Interpreter.Script`**;
- live method is **`DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`**;
- V121 found the method but rejected it because its scorer expected argument #2 to be `System.String`; actual #2 is `MoonSharp.Interpreter.Table`;
- V121 never invoked the method, so Lua chunk/Selections/Treatment/action/server remain UNKNOWN.

Earlier xLua identity is now **DISPROVEN for the current returned object** and retained only as failed-investigation history.

Targeted official MoonSharp source independently confirms `Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null) -> DynValue` and `DynValue.String`.

## 4. v1.1.12 ACTIVE IMPLEMENTATION
### Returned Script resolver
`src/bridge_lua_manager_v1_1_10.inc` retained; current runtime proves it reaches the Script object.

### MoonSharp observer
`src/bridge_lua_moonsharp_v1_1_12.inc`:
1. obtain current returned object through V120 resolver;
2. require metadata shape `DoString(String, MoonSharp.Interpreter.Table, String) -> MoonSharp.Interpreter.DynValue`;
3. invoke `DoString(code, null, "ThanLongAutoHealV122")` in the correct order;
4. capture managed exception diagnostics if invocation fails;
5. resolve `DynValue.get_String()` -> `System.String`;
6. parse T/C/K from the existing bounded GameDialog/AutoFight_Main probe;
7. emit `LUA_MOONSHARP_V122` on exact method/invoke/result failure or `LUA_DIALOG_V122` after execution.

### Action
`src/bridge_action_v1_1_12.inc`:
- re-runs V122 immediately before action;
- refuses absent/guessed/cached ID;
- sends exact `CMD_SHOW_GAMEDIALOG=100007` with `<currentID>:-1`;
- marker `ACTION_V122`;
- MessageBox confirmation stays semantic callback based.

## 5. BUILD / CI — v1.1.12
Initial source commit `ad2f0403863251330b1aca80eb1eba9681b58c9a`, run `31937607280`: **CI FAILED before compilation** because architecture audit incorrectly required literal `MoonSharp.Interpreter.Script` in the resolver source. No artifact was uploaded. This is preserved as a real failed build.

Correction commit `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1` changed only that false-positive audit requirement.

Source-bearing run `31937703988`: **CI/BUILD PASS**:
- architecture audit PASS;
- Route FSM PASS;
- Heal FSM PASS;
- bridge DLL compile + PE PASS;
- controller EXE compile PASS;
- knowledge packaging PASS;
- artifact upload PASS.

Source-bearing artifact:
- name `ThanLongTestAutoHeal-v1.1.12`;
- ID `9261162703`;
- ZIP SHA-256 `d25f999934be62152cf02fd0251f533743f3e9eaa0d4178f74c2fa1007d85c19`;
- verified 9 members;
- EXE SHA-256 `32a3977469a26f5807be192a620630fba9ea3242e265706522fd7d93bb27c823`;
- DLL SHA-256 `553c1d245c06c614561b9c2c1e6368daf30cb671b46602aec60b1938f326a037`.

`BUILD_EVIDENCE.txt` correctly records source head `33a3b56...`, checkout SHA, run `31937703988`, BUILD PASS and `RUNTIME=UNTESTED_FOR_V1.1.12`.

Final docs-only HEAD is rebuilt before delivery; binaries must be byte-compared with these source-bearing hashes.

## 6. CURRENT STATUS
- route/mount/AutoPath/dismount: protected runtime partial PASS;
- ClickNPC -> intended GameDialog: runtime PASS;
- anti-reopen: runtime PASS for tested v1.1.8 transaction;
- UIRoot representation: runtime FAIL / abandoned;
- v1.1.9 singleton assumption: runtime FAIL / superseded;
- v1.1.10 manager -> current Script object: runtime PARTIAL PASS;
- v1.1.11 MoonSharp identity/signature: CONFIRMED;
- v1.1.11 invocation: NOT REACHED;
- v1.1.12 source/CI: BUILD PASS;
- v1.1.12 runtime: **UNTESTED**;
- live Treatment ID: UNKNOWN;
- full Auto Heal: NO KNOWN-GOOD VERSION.

## 7. DO-NOT-DO
- no hardcoded Treatment ID;
- no stale UIButton pointer;
- no return to empty UIRoot observer;
- no WaitTreatment ClickNPC retry after successful open;
- no fixed Sleep as success proof;
- no broad reverse when canonical data answers a fact;
- do not revert V120 returned-object resolver without contrary runtime evidence;
- do not call current returned object xLua after MoonSharp runtime proof;
- do not blame packet/server before a live current ID is actually sent;
- keep one mutable action in flight per PID.

## 8. NEXT RUNTIME TEST
Capture from `Đã gọi ClickNPC` onward.

Earliest marker decides the next task:
1. `LUA_MOONSHARP_V122 ...` -> fix only exact method invocation/result boundary.
2. `LUA_DIALOG_V122 ... T=0 ... raw={...}` -> DoString/DynValue passed; inspect current table/path/text only.
3. `LUA_DIALOG_V122 ... T=<positive>` -> inspect action prerequisite + `ACTION_V122`.
4. `ACTION_V122 SENT ...` -> only then evaluate fresh GameDialog/MessageBox, HP/money and server result.

## 9. ARTIFACT CONTRACT
Every v1.1.11+ CI ZIP contains:
- versioned EXE;
- `ThanLongCleanRouteBridge.dll`;
- `AI_PROJECT_HANDOFF_FULL.md`;
- `AI_START_HERE.md`;
- V2 protocol;
- client-analysis rules;
- `PROJECT_KNOWLEDGE.md`;
- `CHANGELOG.md`;
- `BUILD_EVIDENCE.txt`.

## 10. IMPORTANT FILES
Active source: `bridge_lua_manager_v1_1_10.inc`, `bridge_lua_moonsharp_v1_1_12.inc`, `bridge_action_v1_1_12.inc`, `bridge.cpp`, `controller_part04.inc`, `protocol.h`, `build.cmd`, workflow.

Knowledge: `PROJECT_KNOWLEDGE.md`, `CHANGELOG.md`, feature/BUG/EVID/DEC registries, `VERSION_v1.1.11.md`, `VERSION_v1.1.12.md`, `V121_MOONSHARP_RUNTIME_FINDING.md`.

## 11. HANDOFF PRINCIPLE
Continue from the earliest runtime-proven failing layer:

`GameDialog open -> MoonSharp.Script resolved -> exact Script.DoString(String,Table,String) -> DynValue.String -> current Selections -> live Treatment ID -> one semantic action -> real result proof`.

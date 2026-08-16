# AI PROJECT HANDOFF FULL — THẦN LONG AUTO TRỊ LIỆU TEST LAB

> First-read file inside every artifact ZIP. It summarizes the current engineering state but does not replace the repo's full knowledge base.

## 0. MANDATORY STARTUP
Project: `ngmthang-g/TEST-chuc-nang-than-long-2`  
Branch: `agent/auto-tri-lieu-v1.1.0`  
Current version: **v1.1.12-test**  
Feature: Auto trị liệu NPC  
Full known-good: **NONE**  
BUG-001: **OPEN**

Read in order:
1. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
2. `AI_CLIENT_ANALYSIS_RULES.txt`
3. this file
4. `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. affected feature/BUG/DEC/EVID/history docs
7. current source/tests.

Canonical client knowledge starts at `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`. Do not broad reverse solved client facts. Runtime evidence outranks assumptions. BUILD/CI PASS is not RUNTIME PASS.

## 1. GOAL / PROTECTED ARCHITECTURE
Goal:
`stable state -> route -> one NPC open -> current server GameDialog -> current Trị liệu selection -> one semantic action -> fresh follow-up -> heal/result proof`.

Protected working path from user runtime:
- raw MapID/X/Y capture;
- mount / AutoPath / stop / dismount;
- NPC 339 Đỗ Thanh Đằng, Map 5;
- semantic `ClickNPC(339)` opens intended GameDialog;
- no WaitTreatment NPC reopen after the successful initial open in the tested v1.1.8 transaction.

Do not rewrite those layers while diagnosing a later boundary.

## 2. CANONICAL GAMEDIALOG CONTRACT
Verified client knowledge:
- `GameDialogData.Selections[selectionID] = visibleText`;
- selection IDs are current server/runtime state, never a universal Treatment constant;
- `CMD_SHOW_GAMEDIALOG = 100007`;
- payload = `selectionID:SelectedItemID`, ordinary function choice commonly `<currentID>:-1`;
- built-in automation inspects active Selections;
- server-driven dialog processing may destroy/recreate UI, so flicker is not completion proof;
- MessageBox OK uses its semantic callback.

## 3. BUG-001 RUNTIME LINEAGE
### v1.1.8
GameDialog opened, but active UIRoot/CoreChildren observer had `clickable=0 • texts=0 • labels=<none>` for ~15 s. UIRoot/UIButton representation is abandoned. Anti-reopen behavior passed for that transaction.

### v1.1.9
After GameDialog open: `LuaSystemManager instance unresolved`. Returned script object/DoString/Selections/action were not reached.

### v1.1.10
Manager resolver was corrected. Runtime reached a non-null object returned by `get_LuaEnv`, then failed at the old DoString method resolver. Therefore manager -> returned object is RUNTIME PARTIAL PASS.

### v1.1.11 — CRITICAL CURRENT DISCOVERY
User runtime repeatedly reported:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

This changes the model:
- the returned runtime object is **`MoonSharp.Interpreter.Script`**, not `XLua.LuaEnv`;
- live metadata exposes **`DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`**;
- V121 *found* this method but rejected it because its scorer expected argument #2 to be `System.String`; actual argument #2 is `MoonSharp.Interpreter.Table`;
- V121 never invoked the method, so Lua chunk/Selections/Treatment/action/server remain UNKNOWN.

The earlier xLua assumption is therefore **DISPROVEN as current runtime engine identity at this boundary**. Keep it only as failed investigation history.

Targeted official MoonSharp source independently confirms:
`Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null)` -> `DynValue`.
`DynValue.String` returns the string payload when the DynValue is String.

## 4. WHY v1.1.12 EXISTS
v1.1.12 corrects exactly the runtime-proven MoonSharp call/result contract. It does not redesign route, NPC, GameDialog semantics, LuaEnv/Script resolver, or the final current-ID request.

## 5. v1.1.12 ACTIVE IMPLEMENTATION
### `src/bridge_lua_manager_v1_1_10.inc`
Retained. It is the runtime-proven path that returns the current `MoonSharp.Interpreter.Script` object.

### `src/bridge_lua_moonsharp_v1_1_12.inc`
Active V122 observer:
1. call `ResolveLuaEnvV120()`;
2. require current metadata to match exactly:
   `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
3. invoke in the correct order:
   `DoString(code, null, "ThanLongAutoHealV122")`;
4. on managed exception, capture exception class/text when possible;
5. require returned object and resolve `DynValue.get_String()` -> `System.String`;
6. copy the returned probe text;
7. parse T/C/K from the existing bounded current GameDialog/AutoFight_Main probe;
8. emit `LUA_DIALOG_V122` on execution success or `LUA_MOONSHARP_V122` on exact method/invoke/result failure.

### `src/bridge_action_v1_1_12.inc`
- consumes only V122 live IDs;
- re-runs V122 immediately before action;
- refuses absent/guessed/cached ID;
- submits exact `CMD_SHOW_GAMEDIALOG=100007` with `<currentID>:-1`;
- marker `ACTION_V122`;
- MessageBox confirmation remains semantic callback based.

### Lineage
v1.1.11 code remains compiled under legacy names. Do not erase the failed shape-filter experiment because it produced the decisive MoonSharp runtime evidence.

## 6. CURRENT STATUS
- Route/mount/AutoPath: RUNTIME PASS protected partial path.
- Dismount: RUNTIME PASS protected partial path.
- `ClickNPC(339)` opens intended GameDialog: RUNTIME PASS.
- WaitTreatment anti-reopen: RUNTIME PASS for tested v1.1.8 transaction.
- UIRoot representation: RUNTIME FAIL / abandoned.
- v1.1.9 manager singleton assumption: RUNTIME FAIL / superseded.
- v1.1.10 manager -> returned Script object: RUNTIME PARTIAL PASS.
- v1.1.11 runtime engine identification: **CONFIRMED MoonSharp.Interpreter.Script**.
- v1.1.11 exact DoString overload enumeration: **CONFIRMED**.
- v1.1.11 invocation: NOT REACHED because shape filter rejected real method.
- v1.1.12 MoonSharp invocation/result extraction: RUNTIME UNTESTED.
- live Treatment ID: UNKNOWN.
- semantic Treatment send: UNKNOWN in current Lua/MoonSharp path.
- full Auto Heal: NO KNOWN-GOOD VERSION.

## 7. DO-NOT-DO
- no hardcoded Treatment ID;
- no stale UIButton pointer;
- no return to the empty UIRoot observer;
- no WaitTreatment ClickNPC retry after a successful open;
- no fixed Sleep as success proof;
- no broad reverse when canonical data answers a fact;
- do not revert the V120 returned-object resolver without contrary runtime evidence;
- do not call this path xLua now: runtime identifies MoonSharp;
- do not blame packet/server before a live current ID is actually sent;
- keep one mutable action in flight per PID.

## 8. NEXT RUNTIME TEST
Capture from `Đã gọi ClickNPC` onward.

Interpret earliest marker only:
1. `LUA_MOONSHARP_V122 ...` -> fix only exact method invocation/result boundary.
2. `LUA_DIALOG_V122 ... T=0 ... raw={...}` -> DoString/DynValue passed; inspect only current Lua table/path/text representation.
3. `LUA_DIALOG_V122 ... T=<positive>` -> then inspect action prerequisite + `ACTION_V122`.
4. `ACTION_V122 SENT ...` -> only then evaluate fresh GameDialog/MessageBox, HP/money and server result.

## 9. ARTIFACT CONTRACT
Every v1.1.11+ CI ZIP must contain exactly the self-describing bundle:
- `ThanLongTestAutoHeal_vX.Y.Z.exe`
- `ThanLongCleanRouteBridge.dll`
- `AI_PROJECT_HANDOFF_FULL.md`
- `AI_START_HERE.md`
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
- `AI_CLIENT_ANALYSIS_RULES.txt`
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
- `BUILD_EVIDENCE.txt`.

`BUILD_EVIDENCE.txt` records `SOURCE_HEAD_SHA`, `CHECKOUT_SHA`, run number, artifact, build state and explicit runtime state.

## 10. BUILD STATE
v1.1.12 source-bearing CI/build: **PENDING at commit creation**.  
v1.1.12 runtime: **UNTESTED**.

## 11. IMPORTANT FILES
Active source:
- `src/bridge_lua_manager_v1_1_10.inc`
- `src/bridge_lua_moonsharp_v1_1_12.inc`
- `src/bridge_action_v1_1_12.inc`
- `src/bridge.cpp`
- `src/controller_part04.inc`
- `src/protocol.h`
- `build.cmd`
- `.github/workflows/build.yml`.

Knowledge:
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
- `docs/features/AUTO_HEAL_NPC.md`
- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/decisions/DECISIONS.md`
- `docs/history/VERSION_v1.1.11.md`
- `docs/history/VERSION_v1.1.12.md`
- `docs/investigations/V121_MOONSHARP_RUNTIME_FINDING.md`.

## 12. HANDOFF PRINCIPLE
Continue from the earliest runtime-proven failing layer. Current continuation point:

`GameDialog open -> MoonSharp.Script resolved -> exact Script.DoString(String,Table,String) -> DynValue.String -> current Selections -> live Treatment ID -> one semantic action -> real result proof`.

If future runtime evidence contradicts this file, update this file and the knowledge base immediately; do not preserve a contradicted assumption as current truth.

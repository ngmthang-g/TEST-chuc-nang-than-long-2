# AI PROJECT HANDOFF FULL — THẦN LONG AUTO TRỊ LIỆU TEST LAB

> First-read file inside every artifact ZIP. Runtime evidence outranks assumptions. BUILD/CI PASS is not RUNTIME PASS.

## 0. MANDATORY STARTUP
Project: `ngmthang-g/TEST-chuc-nang-than-long-2`  
Branch: `agent/auto-tri-lieu-v1.1.0`  
Current version: **v1.1.13-test**  
Feature: Auto trị liệu NPC  
Full known-good: **NONE**  
BUG-001: **OPEN**

Read in order: `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md` -> `AI_CLIENT_ANALYSIS_RULES.txt` -> this file -> `PROJECT_KNOWLEDGE.md` -> `CHANGELOG.md` -> affected feature/BUG/DEC/EVID/history/investigation -> source/tests.

Canonical client knowledge starts at `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`. Do not broad reverse solved facts.

## 1. GOAL / PROTECTED PATH
Goal:
`stable state -> route -> one NPC open -> observe current server GameDialog -> identify current Trị liệu selection -> one semantic action -> fresh follow-up -> result proof`.

Runtime-protected partial path:
- raw MapID/X/Y capture;
- mount / AutoPath / stop / dismount;
- Map 5 healer candidate NPC 339 Đỗ Thanh Đằng;
- semantic `ClickNPC(339)` opens intended GameDialog;
- no WaitTreatment NPC reopen after successful initial open in tested v1.1.8 transaction.

## 2. CANONICAL GAMEDIALOG CONTRACT
Verified from canonical client knowledge:
- `GameDialogData.Selections[selectionID] = visibleText`;
- IDs are current server/runtime data; no universal Treatment ID;
- `CMD_SHOW_GAMEDIALOG = 100007`;
- submit payload = `selectionID:SelectedItemID`, ordinary no-item function choice commonly `<currentID>:-1`;
- built-in AutoFight/FuBen flows inspect current Selections;
- MessageBox OK follows its stored semantic callback;
- server-driven GameDialog may be destroyed/recreated, so visual flicker is not success proof.

## 3. RUNTIME LINEAGE / EARLIEST FAILING LAYER
### v1.1.8
GameDialog opened but UIRoot/CoreChildren observer reported `clickable=0 • texts=0 • labels=<none>`. UIRoot/UIButton observer abandoned. Anti-reopen passed for the tested transaction.

### v1.1.9
GameDialog opened then failed at `LuaSystemManager instance unresolved` before returned script object/DoString/Selections/action.

### v1.1.10
Metadata-driven manager resolver reached the object returned by `get_LuaEnv`; old DoString lookup failed. Manager -> returned object became runtime PARTIAL PASS.

### v1.1.11
Runtime identified the returned object as `MoonSharp.Interpreter.Script` and exposed the exact live overload:
`DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`.
V121 found that method but rejected it because its scorer expected parameter #2 to be `System.String`; invocation was not reached.

### v1.1.12 — NEW RUNTIME EVIDENCE, 2026-08-16
User tested delivered final-head v1.1.12. After one NPC open:

```text
Đã gọi ClickNPC npcID=339
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

The line repeats while the dialog remains open.

This proves:
- `ResolveLuaEnvV120()` -> current MoonSharp Script object: **RUNTIME PASS for this path**;
- exact `Script.DoString(String,Table,String)` invocation: **RUNTIME PASS**;
- returned `DynValue.String` extraction: **RUNTIME PASS** because the full probe string returned to C++;
- `GUI.FindUI("GameDialog")` result type = table and UI presence = present;
- `GUI.FindUI("AutoFight_Main")` result type = table;
- MessageBox absent at this stage;
- Treatment/Confirm/Know not found by the V122 observer.

### Critical correction: `N=4` is NOT four selections
In the V122 Lua source, `N` is `nodes`, incremented once per traversed table in `scan()`. Therefore `N=4` means four table nodes were traversed. It does **not** mean four GameDialog choices exist. Do not preserve or repeat the incorrect interpretation "4 selections".

### Confirmed V122 observer weakness
The V122 probe searches semantic fields with `rawget(t, "Selections")` and `rawget(t, priorityKey)`. Lua `rawget` deliberately bypasses `__index`/metatable lookup. Runtime now proves the execution engine is good enough to run the probe, so the earliest unresolved layer is **representation/access of current dialog data**, not MoonSharp invocation.

Canonical knowledge still says the desired semantic source is `GameDialogData.Selections`; there is no evidence that the server contract changed.

## 4. WHY v1.1.13 EXISTS
v1.1.13 changes only the read-only table observer. It does NOT redesign route, NPC interaction, MoonSharp invocation, packet semantics, or action state machine.

Hypothesis under test:
`GUI.FindUI` script objects may expose semantic fields through normal table indexing / `__index` / metatable or through nested current dialog data not visible to V122 raw-only access.

Root-cause classification before V123 runtime: **LIKELY observer representation/access issue, not yet CONFIRMED until V123 reveals Selections or diagnostics**.

## 5. v1.1.13 ACTIVE IMPLEMENTATION
### MoonSharp execution primitive — protected
`src/bridge_lua_manager_v1_1_10.inc` + `src/bridge_lua_moonsharp_v1_1_12.inc` remain the runtime-proven execution path. V122 is compiled under legacy names but `RunLuaChunkV122()` is reused by V123.

### Metatable-aware observer
`src/bridge_lua_dialog_v1_1_13.inc`:
1. runs through proven `RunLuaChunkV122()`;
2. uses `safeget(t,key)` = `pcall(function() return t[key] end)` for normal indexing instead of raw-only semantic access;
3. checks `Selections`, `GameDialogData`, `CurrentGameDialogData`, `DialogData` and case variants;
4. boundedly traverses table children;
5. inspects `getmetatable(t)` and a table-valued `__index` chain;
6. checks canonical global names if this client exposes them;
7. scans `Selections` mapping by numeric/current key and string visible text;
8. also records a bounded fallback text field when a selection value is unexpectedly a table, without promoting that fallback to a canonical fact;
9. emits diagnostics:
   - `NODE` = number of table nodes traversed;
   - `ST` = number of `Selections` tables observed;
   - `SV` = number of values enumerated inside those Selections tables;
   - `MT` = metatables observed;
   - `KS` = bounded key/type samples;
   - `S` = bounded selection samples.
10. output marker: `LUA_DIALOG_V123`.

Semantic matching remains fail-closed. Treatment ID must be a positive current key matched to visible text beginning with `Trị liệu` after bounded normalization.

### Action
`src/bridge_action_v1_1_13.inc`:
- re-runs V123 immediately before mutable action;
- refuses absent/guessed/cached ID;
- only after current `T>0` sends canonical `CMD_SHOW_GAMEDIALOG=100007` with `<currentID>:-1`;
- marker `ACTION_V123`;
- MessageBox still uses semantic `ButtonOKClicked()` when actually present.

## 6. STATUS BEFORE v1.1.13 BUILD
- route/mount/AutoPath/dismount: protected runtime partial PASS;
- NPC open: runtime PASS;
- anti-reopen: runtime PASS for tested v1.1.8 transaction;
- UIRoot representation: runtime FAIL / abandoned;
- returned MoonSharp Script resolution: runtime PASS in V122 transaction;
- exact MoonSharp DoString invocation: runtime PASS in V122 transaction;
- DynValue.String extraction: runtime PASS in V122 transaction;
- current GameDialog/AutoFight Lua object presence: runtime PASS;
- V122 rawget-only Selections observation: runtime FAIL to produce current ID;
- live Treatment ID: UNKNOWN;
- Treatment request/server follow-up: NOT REACHED in current MoonSharp path;
- v1.1.13 source/CI: **PENDING at source commit creation**;
- v1.1.13 runtime: **UNTESTED**.

## 7. DO-NOT-DO
- no hardcoded Treatment ID;
- no stale UIButton pointer;
- no return to UIRoot tree tuning;
- no WaitTreatment ClickNPC retry after successful open;
- no fixed sleep success proof;
- no broad reverse while canonical data answers the contract;
- do not touch the runtime-proven MoonSharp execution layer unless new evidence contradicts it;
- do not call `N` a selection count;
- do not blame packet/server before `ACTION_V123 SENT` with a live ID;
- one mutable action in flight per PID.

## 8. NEXT RUNTIME TEST
Capture from `Đã gọi ClickNPC` onward.

Interpret V123 in this order:
1. `LUA_DIALOG_V123 ... T=<positive>` -> observer found current Treatment; then evaluate `ACTION_V123` and fresh follow-up.
2. `T=0;ST>0;SV>0` -> Selections table reached; inspect `S=` value types/text and matching only.
3. `T=0;ST>0;SV=0` -> Selections object exists but is empty/unexpected at sample time; inspect lifecycle/state.
4. `T=0;ST=0;MT>0` -> metatable traversal happened but semantic field still not found; use `KS=` to identify exact current field names/types.
5. `T=0;ST=0;MT=0` -> current tables expose no traversable metatable; use `KS=` and only then consider a more targeted runtime source.
6. `ACTION_V123 SENT ...` -> only then investigate next GameDialog/MessageBox/HP/money/server result.

## 9. ARTIFACT CONTRACT
Every delivered CI ZIP from v1.1.11+ contains exactly the self-describing bundle:
- versioned EXE;
- `ThanLongCleanRouteBridge.dll`;
- `AI_PROJECT_HANDOFF_FULL.md`;
- `AI_START_HERE.md`;
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`;
- `AI_CLIENT_ANALYSIS_RULES.txt`;
- `PROJECT_KNOWLEDGE.md`;
- `CHANGELOG.md`;
- `BUILD_EVIDENCE.txt`.

## 10. IMPORTANT FILES
Active: `bridge_lua_manager_v1_1_10.inc`, `bridge_lua_moonsharp_v1_1_12.inc` (legacy/proven primitive), `bridge_lua_dialog_v1_1_13.inc`, `bridge_action_v1_1_13.inc`, `bridge.cpp`, `controller_part04.inc`, `protocol.h`, `build.cmd`, workflow.

Knowledge: `PROJECT_KNOWLEDGE.md`, `CHANGELOG.md`, feature/BUG/EVID/DEC registries, `VERSION_v1.1.13.md`, `V122_SELECTIONS_RUNTIME_FINDING.md`.

## 11. HANDOFF PRINCIPLE
Continue from the earliest runtime-proven failing layer:
`GameDialog open -> MoonSharp execution PASS -> DynValue.String PASS -> current Lua table representation/access -> current Selections -> live Treatment ID -> one semantic action -> real result proof`.

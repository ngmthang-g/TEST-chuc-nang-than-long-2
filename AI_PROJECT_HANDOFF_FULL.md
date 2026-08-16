# AI PROJECT HANDOFF FULL — THẦN LONG AUTO TRỊ LIỆU TEST LAB

> First-read file inside every artifact ZIP. Runtime evidence outranks assumptions. BUILD/CI PASS is not RUNTIME PASS.

## 0. MANDATORY STARTUP
Project: `ngmthang-g/TEST-chuc-nang-than-long-2`  
Branch: `agent/auto-tri-lieu-v1.1.0`  
Current version: **v1.1.13-test**  
Feature: Auto trị liệu NPC  
Full known-good: **NONE**  
BUG-001: **OPEN**

Read: V2 protocol -> client-analysis rules -> this file -> `PROJECT_KNOWLEDGE.md` -> `CHANGELOG.md` -> affected feature/BUG/DEC/EVID/history/investigation -> source/tests. Canonical client knowledge starts at `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`.

## 1. PROTECTED RUNTIME PATH
- raw MapID/X/Y capture;
- mount / AutoPath / stop / dismount;
- Map 5 NPC 339 Đỗ Thanh Đằng;
- semantic `ClickNPC(339)` opens intended GameDialog;
- no WaitTreatment NPC reopen in tested v1.1.8 transaction.

## 2. CANONICAL GAMEDIALOG CONTRACT
- `GameDialogData.Selections[selectionID] = visibleText`;
- IDs are current server/runtime state, never a universal Treatment constant;
- `CMD_SHOW_GAMEDIALOG = 100007`;
- payload = `selectionID:SelectedItemID`, common no-item choice `<currentID>:-1`;
- built-in AutoFight/FuBen logic inspects current Selections;
- MessageBox OK uses semantic callback.

## 3. LAYER HISTORY
- v1.1.8: UIRoot representation runtime FAIL; anti-reopen PASS for tested transaction.
- v1.1.9: manager singleton assumption runtime FAIL.
- v1.1.10: returned-object resolver progressed.
- v1.1.11: runtime identified `MoonSharp.Interpreter.Script` and exact `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`; V121 rejected the real overload before invocation.
- v1.1.12: exact MoonSharp invocation/result implemented and runtime-tested.

## 4. v1.1.12 RUNTIME — DECISIVE CURRENT BOUNDARY
After one NPC open, delivered v1.1.12 repeatedly returned:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

Confirmed PASS:
- current MoonSharp Script resolution;
- exact `Script.DoString(String,Table,String)` invocation;
- `DynValue.String` extraction;
- GameDialog and AutoFight_Main returned as Lua tables.

Not reached: live Treatment ID, mutable GameDialog request, follow-up/result.

**Critical correction:** V122 `N=4` is table-node count (`nodes`), **not four selections**.

V122 probe reads semantic fields through `rawget`, including `rawget(t,"Selections")`. `rawget` bypasses `__index`/metatable lookup. Therefore the earliest unresolved layer is current Lua representation/access, not MoonSharp execution.

## 5. v1.1.13 ACTIVE IMPLEMENTATION
Protected primitive: V120 returned-Script resolver + V122 MoonSharp `DoString` / `DynValue.String` execution.

`src/bridge_lua_dialog_v1_1_13.inc` changes only the read-only observer:
- normal indexing through `safeget(t,key)` under `pcall`;
- checks `Selections`, `GameDialogData`, `CurrentGameDialogData`, `DialogData` variants;
- bounded child-table traversal;
- bounded `getmetatable()` + table-valued `__index` traversal;
- checks canonical global GameDialogData names if exposed;
- diagnostics `NODE`, `ST`, `SV`, `MT`, `KS`, `S`;
- marker `LUA_DIALOG_V123`.

`src/bridge_action_v1_1_13.inc`:
- re-runs V123 immediately before action;
- refuses absent/guessed/cached ID;
- sends `CMD_SHOW_GAMEDIALOG=100007` with `<currentID>:-1` only after current semantic match;
- marker `ACTION_V123`;
- MessageBox callback path preserved.

Hypothesis that current data is hidden behind normal indexing/metatable/nested representation is **LIKELY, not CONFIRMED** until V123 runtime.

## 6. v1.1.13 BUILD EVIDENCE
Source-bearing commit: `79235f61ded9d393694be807c996128b082f67b4`  
GitHub Actions run: `31939000139`  
Result: **CI/BUILD PASS**.

Passed:
- architecture audit;
- Route FSM self-test;
- Heal FSM self-test;
- bridge DLL compile + PE verification;
- controller EXE compile;
- 9-file knowledge packaging;
- artifact upload.

Artifact: `ThanLongTestAutoHeal-v1.1.13`, ID `9261527342`  
ZIP SHA-256: `1cb886860b998b955efe68164bf5379642a2c2bde739acb425c7a9a650ecee5f`  
EXE SHA-256: `764fc10e98afd0fcd608bd9dd50c9fa86e51b467524cab4aebbe5001646d037a`  
DLL SHA-256: `8d8b9e3feeecf0be509610ad990bbe3852761fb750d1862aee09d5f51eadb7f7`

Verified artifact contains exactly 9 required files. `BUILD_EVIDENCE.txt` records source head `79235f61...`, PR checkout `f4f3bded...`, run `31939000139`, BUILD PASS, `RUNTIME=UNTESTED_FOR_V1.1.13`.

## 7. CURRENT STATUS
- MoonSharp execution/result: **RUNTIME PASS** from v1.1.12;
- V122 rawget-only observer: failed to expose T/C/K;
- V123 metatable-aware observer: **BUILD PASS / RUNTIME UNTESTED**;
- live Treatment ID: UNKNOWN;
- action/follow-up/result: NOT REACHED;
- BUG-001 OPEN; full Auto Heal known-good NONE.

## 8. NEXT RUNTIME TEST
Read first `LUA_DIALOG_V123` after one NPC open:
1. `T>0` -> observer found current Treatment; inspect `ACTION_V123` + fresh follow-up.
2. `T=0;ST>0;SV>0` -> Selections reached; inspect `S=` only.
3. `T=0;ST>0;SV=0` -> Selections exists but empty/unexpected at sample time.
4. `T=0;ST=0` -> inspect `MT` and `KS` representation diagnostics.
5. No packet/server conclusion before `ACTION_V123 SENT`.

## 9. HARD RULES
No hardcoded Treatment ID; no UIRoot tuning; no WaitTreatment NPC reopen; no fixed-sleep proof; no broad reverse while canonical facts suffice; do not modify the runtime-proven MoonSharp boundary without contrary evidence; do not interpret `N=4` as four selections.

## 10. ARTIFACT CONTRACT
Every v1.1.11+ ZIP contains exactly: versioned EXE, bridge DLL, this handoff, `AI_START_HERE.md`, V2 protocol, client rules, project knowledge, changelog, `BUILD_EVIDENCE.txt`.

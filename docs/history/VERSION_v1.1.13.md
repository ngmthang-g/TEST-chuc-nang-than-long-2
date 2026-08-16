# VERSION v1.1.13-test — metatable-aware GameDialog Selections observer

## A. Identity / lineage
- Version: v1.1.13-test
- Date: 2026-08-16
- Based on v1.1.12 final HEAD `03b33595e122217baa4c006a0fe8998af3395d44`
- Related BUG: BUG-001
- Related decision: DEC-015
- Complete Auto Heal known-good: NONE.

## B. User runtime trigger
Delivered v1.1.12 after one `ClickNPC npcID=339` repeatedly returned:
```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

## C. Facts established before modification
Runtime PASS in V122 transaction:
- current MoonSharp Script resolution;
- exact `DoString(String,Table,String)` invocation;
- DynValue.String extraction;
- GameDialog/AutoFight_Main Lua-table presence.

Not reached: live Treatment ID, GameDialog mutation, follow-up/result.

## D. Critical correction
V122 `N` is `nodes`, incremented once per traversed table. `N=4` is four table nodes, not four selections.

## E. Source investigation
V122 reuses a probe that calls `rawget(t,"Selections")` and rawget priority fields. `rawget` bypasses `__index`/metatable lookup. Canonical client knowledge still verifies `GameDialogData.Selections[selectionID]=visibleText`.

Hypothesis: the current service/UI tables expose needed state through normal indexing/metatable/nested representation not visible to raw-only V122. Status: LIKELY, not confirmed before V123 runtime.

## F. Changes
### `src/bridge_lua_dialog_v1_1_13.inc`
- reuses proven `RunLuaChunkV122()`;
- normal `t[key]` access inside `pcall`;
- checks `Selections`, `GameDialogData`, `CurrentGameDialogData`, `DialogData` variants;
- bounded child-table traversal;
- bounded metatable/table-`__index` traversal;
- checks canonical global names if exposed;
- current numeric key -> normalized visible text matching;
- diagnostics `NODE/ST/SV/MT/KS/S`;
- marker `LUA_DIALOG_V123`.

### `src/bridge_action_v1_1_13.inc`
- re-runs V123 immediately before mutation;
- requires current positive semantic ID;
- sends canonical `<currentID>:-1` only after current match;
- marker `ACTION_V123`;
- MessageBox callback preserved.

### Wiring/versioning
- V122 MoonSharp implementation preserved under legacy observer/action names and reused as execution primitive;
- protocol `0x00010113u`;
- controller title/log and artifact naming v1.1.13;
- 9-file handoff contract preserved.

## G. Source-bearing build
Commit `79235f61ded9d393694be807c996128b082f67b4`.  
Run `31939000139`: **CI/BUILD PASS**.

Passed:
- architecture audit;
- Route FSM self-test;
- Heal FSM self-test;
- bridge DLL compile + PE verification;
- controller EXE compile;
- knowledge packaging;
- artifact upload.

Artifact `ThanLongTestAutoHeal-v1.1.13`, ID `9261527342`.  
ZIP SHA-256 `1cb886860b998b955efe68164bf5379642a2c2bde739acb425c7a9a650ecee5f`.  
EXE SHA-256 `764fc10e98afd0fcd608bd9dd50c9fa86e51b467524cab4aebbe5001646d037a`.  
DLL SHA-256 `8d8b9e3feeecf0be509610ad990bbe3852761fb750d1862aee09d5f51eadb7f7`.  
Exactly 9 required files verified.

`BUILD_EVIDENCE.txt`:
- `SOURCE_HEAD_SHA=79235f61ded9d393694be807c996128b082f67b4`
- `CHECKOUT_SHA=f4f3bdedc5b7df5b0e11f82ebf783298e215b01b`
- `GITHUB_RUN_ID=31939000139`
- `GITHUB_RUN_NUMBER=266`
- `BUILD=PASS`
- `RUNTIME=UNTESTED_FOR_V1.1.13`

## H. Runtime state
v1.1.13: **UNTESTED**. BUG-001 remains OPEN.

## I. Protected behavior
Do not change route/NPC flow, add WaitTreatment reopen, alter proven MoonSharp signature/order/result extraction, return to UIRoot, hardcode Treatment ID or blame packet/server before a live ID is sent.

## J. Next runtime classification
From first `LUA_DIALOG_V123`:
- `T>0` -> current Treatment observed; inspect `ACTION_V123` and fresh follow-up;
- `T=0,ST>0,SV>0` -> Selections reached; inspect `S`;
- `T=0,ST>0,SV=0` -> Selections reached but empty/unexpected;
- `T=0,ST=0` -> inspect `MT` and `KS`;
- no server diagnosis before `ACTION_V123 SENT`.

## K. Handoff principle
Continue from:
`MoonSharp execution PASS -> current Lua representation/access -> Selections -> live ID -> semantic action -> result proof`.

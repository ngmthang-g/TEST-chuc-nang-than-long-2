# VERSION v1.1.13-test — metatable-aware GameDialog Selections observer

## A. Identity / lineage
- Version: v1.1.13-test
- Date: 2026-08-16
- Based on: v1.1.12-test final HEAD `03b33595e122217baa4c006a0fe8998af3395d44`
- Related BUG: BUG-001
- Related decision: DEC-015
- Complete Auto Heal known-good: NONE.

## B. User runtime trigger
Delivered v1.1.12 reached the intended NPC and after one `ClickNPC npcID=339` repeatedly returned:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

This is decisive progress because the probe string itself returned intact from MoonSharp.

## C. Runtime facts before modification
Confirmed PASS in the v1.1.12 transaction:
- current returned MoonSharp Script resolution;
- exact `DoString(String,Table,String)` invocation;
- `DynValue.String` extraction;
- GameDialog present and exposed as table;
- AutoFight_Main exposed as table.

Not reached:
- current Treatment ID;
- GameDialog mutation;
- server follow-up/completion proof.

## D. Correction of diagnostic interpretation
V122 `N` is the local variable `nodes` from the Lua scan, incremented once per traversed table. Therefore `N=4` means four table nodes were traversed. It does not mean four selections or four GameDialog buttons.

## E. Source investigation
The V122 observer reuses `kGameDialogProbeLuaV119`. It looks for semantic fields with:
- `rawget(t,"Selections")`;
- `rawget(t,key)` for priority child fields.

Lua `rawget` intentionally bypasses metatable `__index` processing. This is a concrete limitation now that execution itself is runtime-proven.

Canonical client knowledge still verifies:
`GameDialogData.Selections[selectionID]=visibleText`.

Therefore v1.1.13 tests a narrow hypothesis: the current UI/service tables may expose the needed semantic state through normal indexing / metatable / nested data not visible to V122 raw-only access.

Hypothesis confidence before V123 runtime: LIKELY, not CONFIRMED.

## F. Changes made
### `src/bridge_lua_dialog_v1_1_13.inc`
New read-only observer:
- reuses `RunLuaChunkV122()`;
- `safeget(t,key)` uses normal `t[key]` inside `pcall`;
- checks semantic keys `Selections`, `GameDialogData`, `CurrentGameDialogData`, `DialogData` and variants;
- boundedly scans child tables;
- boundedly scans metatables and table-valued `__index`;
- checks canonical global GameDialogData names if exposed;
- matches current numeric selection key to normalized semantic text;
- keeps fallback table-value text probing diagnostic-only;
- returns `LUA_DIALOG_V123` with `NODE/ST/SV/MT/KS/S`.

### `src/bridge_action_v1_1_13.inc`
- consumes only V123 current IDs;
- re-runs V123 at action time;
- refuses absent/cached/guessed ID;
- sends `<actualCurrentID>:-1` only after current semantic match;
- marker `ACTION_V123`;
- MessageBox callback path preserved.

### `src/bridge.cpp`
- V122 exact MoonSharp implementation preserved under legacy observer/action names;
- V123 observer/action become active;
- V123 reuses the runtime-proven V122 chunk execution primitive.

### Version/build surfaces
- protocol version -> `0x00010113u`;
- title/startup log -> v1.1.13;
- build audit verifies V122 primitive retention + V123 observer/action wiring;
- artifact/EXE naming -> v1.1.13;
- 9-file handoff packaging retained.

## G. Protected behavior
v1.1.13 intentionally does NOT:
- change route/NPC mapping;
- add WaitTreatment reopen;
- alter proven MoonSharp method signature/argument order/result getter;
- return to UIRoot/UIButton;
- hardcode Treatment ID;
- claim packet/server failure.

## H. Build state at source creation
**PENDING CI**. Do not promote to build PASS until GitHub Actions completes architecture audit, tests, bridge/controller compile and artifact upload.

## I. Runtime state
**UNTESTED** for v1.1.13.

## J. Next runtime classification
From first `LUA_DIALOG_V123`:
- `T>0` -> current Treatment found; inspect action/follow-up;
- `T=0,ST>0,SV>0` -> current Selections reached; inspect `S` matching/value shape;
- `T=0,ST>0,SV=0` -> Selections reached but empty at sample;
- `T=0,ST=0` -> inspect `MT` and `KS` representation evidence;
- `ACTION_V123 SENT` -> only then investigate packet/server/follow-up/result.

## K. Handoff principle
Do not restart from NPC coordinates, mouse clicking, UIRoot or MoonSharp method resolution. Continue from:
`MoonSharp execution PASS -> current Lua representation/access -> Selections -> live ID -> semantic action -> result proof`.

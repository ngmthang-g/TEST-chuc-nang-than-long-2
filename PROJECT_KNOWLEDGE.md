# PROJECT KNOWLEDGE

## Identity / mandatory startup
- Repo: `ngmthang-g/TEST-chuc-nang-than-long-2`; branch `agent/auto-tri-lieu-v1.1.0`.
- Current version: `v1.1.13-test`; full Auto trị liệu known-good: **NONE**.
- Read every version: `AI_START_HERE.md` -> V2 protocol -> client-analysis TXT -> `AI_PROJECT_HANDOFF_FULL.md` -> this file -> `CHANGELOG.md` -> affected feature/BUG/DEC/EVID/history/investigation -> source/tests.
- Client facts start at `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`; no broad reverse for solved facts.
- BUILD/CI PASS != RUNTIME PASS.

## Artifact contract
Every v1.1.11+ delivered artifact contains exactly 9 files: versioned EXE, bridge DLL, consolidated handoff, startup pointer, V2 protocol, client-analysis rules, project knowledge, changelog and `BUILD_EVIDENCE.txt`.

## Runtime-protected path
- runtime MapID/X/Y capture;
- mount / AutoPath / stop / dismount;
- Map 5 NPC 339 Đỗ Thanh Đằng;
- semantic NPC interaction opens intended GameDialog;
- no WaitTreatment NPC reopen after successful initial open in tested v1.1.8 transaction.

## Layer history
- v1.1.8: UIRoot representation runtime FAIL; anti-reopen PASS for tested transaction.
- v1.1.9: LuaSystemManager singleton assumption failed.
- v1.1.10: metadata-driven returned-object resolver progressed.
- v1.1.11: runtime identifies `MoonSharp.Interpreter.Script` and exact `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`; V121 rejects real method before invoke.
- v1.1.12: exact MoonSharp invocation/result implemented and runtime-tested.

## v1.1.12 runtime — current proof
After one `ClickNPC npcID=339`, repeated:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

Confirmed runtime PASS:
- current Script resolution;
- exact MoonSharp DoString invocation;
- DynValue.String extraction;
- GameDialog and AutoFight_Main Lua-table presence.

V122 did not expose current T/C/K. Live Treatment ID/action/follow-up were not reached.

**Correction:** V122 `N` equals `nodes`, the number of traversed table nodes. `N=4` is not a selection count.

V122 uses `rawget(t,"Selections")` and rawget priority fields, bypassing `__index`/metatable lookup. Canonical client knowledge still verifies `GameDialogData.Selections[selectionID]=visibleText`. Earliest unresolved layer is current Lua representation/access.

## v1.1.13 design
Execution primitive remains protected:
- V120 returned-object resolver;
- V122 exact MoonSharp DoString + DynValue.String path.

Active V123 observer:
- normal indexing in `pcall` via `safeget`;
- checks `Selections`, `GameDialogData`, `CurrentGameDialogData`, `DialogData` variants;
- bounded child-table scan;
- bounded metatable/table-`__index` scan;
- checks canonical globals if exposed;
- diagnostics `NODE/ST/SV/MT/KS/S`;
- marker `LUA_DIALOG_V123`.

Action:
- re-read V123 immediately before mutation;
- require positive current semantic ID;
- exact `CMD_SHOW_GAMEDIALOG=100007` with `<currentID>:-1`;
- marker `ACTION_V123`;
- no guessed/cached ID.

Hypothesis that V122 missed state because of raw-only representation/access is LIKELY, not confirmed before V123 runtime.

## v1.1.13 build evidence
Source-bearing commit `79235f61ded9d393694be807c996128b082f67b4`.  
Run `31939000139`: **CI/BUILD PASS** — architecture audit, Route FSM, Heal FSM, bridge DLL/PE, controller EXE, knowledge packaging and upload all PASS.  
Artifact `ThanLongTestAutoHeal-v1.1.13`, ID `9261527342`, ZIP SHA-256 `1cb886860b998b955efe68164bf5379642a2c2bde739acb425c7a9a650ecee5f`.  
EXE SHA-256 `764fc10e98afd0fcd608bd9dd50c9fa86e51b467524cab4aebbe5001646d037a`.  
DLL SHA-256 `8d8b9e3feeecf0be509610ad990bbe3852761fb750d1862aee09d5f51eadb7f7`.  
Exactly 9 artifact members verified. `BUILD_EVIDENCE.txt`: `SOURCE_HEAD_SHA=79235f61...`, `CHECKOUT_SHA=f4f3bded...`, `GITHUB_RUN_ID=31939000139`, BUILD PASS, RUNTIME UNTESTED.

## Hard rules
- no hardcoded/cached Treatment ID;
- no inferred NPC X/Y as universal truth;
- no WaitTreatment NPC reopen;
- no UIRoot tuning;
- no fixed-sleep completion proof;
- one mutable action per PID;
- do not modify runtime-proven MoonSharp execution while diagnosing representation unless contrary evidence appears;
- never treat V122 `N` as selection count;
- no packet/server blame before current live ID is sent;
- preserve failed attempts and BUILD/RUNTIME distinction.

## BUG-001 status
OPEN.
- MoonSharp execution/result: runtime PASS in v1.1.12;
- V122 rawget-only observer: failed to expose T/C/K;
- V123 observer: BUILD PASS, RUNTIME UNTESTED;
- live Treatment ID/action/follow-up: UNKNOWN.

## Next evidence
First `LUA_DIALOG_V123`:
- `T>0` -> evaluate `ACTION_V123` + fresh result;
- `T=0,ST>0,SV>0` -> inspect `S`;
- `T=0,ST=0` -> inspect `MT`/`KS`;
- no server diagnosis before `ACTION_V123 SENT`.

# BUG REGISTRY

## BUG-001 — GameDialog Trị liệu mở được nhưng chưa tiến sang service/result

**Status:** OPEN  
**Severity:** HIGH  
**Last Runtime Tested:** v1.1.12 delivered artifact, 2026-08-16  
**Last Known-Good complete flow:** NONE

## Runtime-confirmed working
- runtime NPC coordinate capture;
- route/mount/AutoPath/dismount;
- `ClickNPC(339)` opens intended GameDialog;
- tested anti-reopen behavior;
- returned object is MoonSharp Script;
- v1.1.12 exact `DoString(String,Table,String)` invocation executes;
- v1.1.12 DynValue.String returns full probe;
- GameDialog and AutoFight_Main are reachable as Lua tables.

## Failed / unresolved layers
- v1.1.8 UIRoot representation: runtime FAIL;
- v1.1.9 manager singleton assumption: runtime FAIL;
- v1.1.11 accepted DoString shape: runtime FAIL before invoke;
- v1.1.12 raw-only Selections observation: T/C/K=0, S empty.

Exact v1.1.12 line:
```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

**Correction:** V122 `N` is traversed table-node count, not number of selections.

## Current source correlation
V122 reads `Selections` and priority fields via `rawget`, bypassing normal `__index`/metatable lookup. Canonical client knowledge still verifies `GameDialogData.Selections[selectionID]=visibleText`.

Current root-cause hypothesis: V122 representation/access path is too raw/narrow. **LIKELY, not CONFIRMED** until V123 runtime.

## v1.1.13 strategy
Preserve runtime-proven MoonSharp execution. Replace only the read-only observer with normal indexing under `pcall`, bounded child traversal, bounded metatable/table-`__index` traversal and diagnostics `NODE/ST/SV/MT/KS/S`. Mutation remains fail-closed until current positive T is re-read immediately before action.

## Build evidence
Source commit `79235f61ded9d393694be807c996128b082f67b4`, run `31939000139`: **CI/BUILD PASS**. Artifact ID `9261527342`, ZIP SHA-256 `1cb886860b998b955efe68164bf5379642a2c2bde739acb425c7a9a650ecee5f`; EXE `764fc10e98afd0fcd608bd9dd50c9fa86e51b467524cab4aebbe5001646d037a`; DLL `8d8b9e3feeecf0be509610ad990bbe3852761fb750d1862aee09d5f51eadb7f7`; 9 required files verified.

v1.1.13 runtime: **UNTESTED**.

## Root-cause status
- anti-reopen: resolved component / runtime supported;
- UIRoot: confirmed failed representation;
- MoonSharp identity/signature: confirmed;
- MoonSharp invocation/result: runtime PASS in v1.1.12;
- V122 raw-only observer result: confirmed no T/C/K exposed;
- metatable/normal-index representation hypothesis: likely, awaiting V123;
- live Treatment ID/action/follow-up/result: UNKNOWN.

## Next diagnostic
- `LUA_DIALOG_V123 T>0` -> action stage;
- `T=0,ST>0,SV>0` -> inspect `S`;
- `T=0,ST=0` -> inspect `MT`/`KS`;
- no packet/server blame before `ACTION_V123 SENT`.

## Do-not-do
No broad reverse, no UIRoot tuning, no hardcoded ID, no WaitTreatment reopen, no fixed Sleep proof, no change to proven MoonSharp execution while diagnosing representation, and no interpretation of V122 N as selection count.

# FEATURE: AUTO HEAL NPC / TRỊ LIỆU

## Goal
`route -> one NPC open -> observe current server GameDialog data -> current Treatment selection -> one semantic action -> fresh follow-up -> result proof`.
Only port after repeated runtime PASS.

## Mandatory startup / artifact handoff
Read `AI_START_HERE.md`, V2 protocol, client-analysis TXT, `AI_PROJECT_HANDOFF_FULL.md`, project knowledge/changelog, this feature + BUG/DEC/EVID/history + current source. Use `clinent-game-than-long-DATA-2222/AI_INDEX.md` before targeted reverse work.

If only a build ZIP is available, `AI_PROJECT_HANDOFF_FULL.md` is the first-read consolidated state file.

## Protected working path
- raw user-captured MapID/X/Y;
- mount / route / StopPath / dismount;
- Map 5 -> NPC 339 Đỗ Thanh Đằng;
- semantic NPC interaction/open;
- no WaitTreatment NPC reopen after successful initial open in tested v1.1.8 transaction.

## Runtime layer findings
### v1.1.8
GameDialog opened, but bridge-visible UIRoot observer reported `clickable=0`, `texts=0`, `labels=<none>` until fail-closed. UIRoot/UIButton is not the active observer.

### v1.1.9
GameDialog opened then `LuaSystemManager instance unresolved`. Returned script object/DoString/Selections/action were not reached.

### v1.1.10
Manager resolver advanced to a non-null object returned by `get_LuaEnv`; old DoString lookup then failed. Manager -> returned object = RUNTIME PARTIAL PASS.

### v1.1.11 — MoonSharp discovery
User runtime repeatedly reports:

`LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}`

This proves:
- current returned object is `MoonSharp.Interpreter.Script`;
- live DoString shape is `String, Table, String -> DynValue`;
- V121 found the method but rejected it because its accepted-shape logic expected parameter #2 to be String;
- V121 never executed the Lua chunk.

Therefore current Selections/Treatment ID/action/server result are still UNKNOWN.

## Current implementation — v1.1.12
### Returned Script resolver — protected from v1.1.10
`src/bridge_lua_manager_v1_1_10.inc` remains the active prerequisite. Runtime evidence now identifies its returned object as MoonSharp Script. Do not redesign this layer merely because downstream invocation was wrong.

### MoonSharp DoString / dialog observer
`src/bridge_lua_moonsharp_v1_1_12.inc`:
- obtains current returned object through V120 resolver;
- enumerates/validates exact runtime method:
  `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
- calls `DoString(code, null, friendlyName)` in the verified parameter order;
- captures managed exception class/text when possible;
- resolves `DynValue.get_String()` and copies the returned diagnostic string;
- runs existing bounded current GameDialog/AutoFight_Main Selections probe;
- emits `LUA_MOONSHARP_V122` on method/invoke/result failures;
- emits `LUA_DIALOG_V122` with T/C/K/raw sample after execution success.

Official MoonSharp source matches the live signature: `Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null)` returns `DynValue`; `DynValue.String` exposes a string value.

### GameDialog action
`src/bridge_action_v1_1_12.inc`:
- requires safe action prerequisite;
- re-runs current V122 probe immediately before action;
- rejects absent/stale/guessed ID;
- submits canonical `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentID>:-1`;
- logs `ACTION_V122`;
- waits for fresh server/UI state.

### Confirmation
If a live MessageBox exists, use semantic `ButtonOKClicked()`. Otherwise current GameDialog `Xác nhận` uses its current selection ID.

## Canonical facts
- `Selections[selectionID]=visibleText`.
- no global Treatment ID.
- built-in AutoFight dialog logic inspects current selections.
- GameDialog request ID `100007`, payload `selectionID:SelectedItemID`.
- returned script object at current boundary is runtime-confirmed `MoonSharp.Interpreter.Script`.
- exact current DoString metadata shape is runtime-confirmed String/Table/String -> DynValue.

## Failed/superseded assumptions
- UIRoot/UIButton observer: runtime failed.
- named LuaSystemManager singleton: runtime failed.
- xLua identity/parameter model for current returned object: disproven by v1.1.11 runtime. Keep only as investigation history.

## Artifact contract — v1.1.11+
Every delivered ZIP contains EXE/DLL plus consolidated handoff/startup/protocol/rules/project knowledge/changelog/build evidence.

## Safety / isolation
v1.1.12 does NOT:
- return to UIRoot/button scanning;
- add NPC reopen retries;
- hardcode Treatment ID;
- raw-scan heap/pointers;
- change route/state-machine semantics;
- claim packet/server failure before action is reached.

## Build status
v1.1.11 build/packaging PASS; runtime identifies MoonSharp but fails before invocation due V121 shape filter.

v1.1.12 source/CI: **PENDING at source commit creation**. Runtime: **UNTESTED**.

## Required next log
After one NPC open, preserve:
- `LUA_MOONSHARP_V122 ...` if exact invocation/result fails;
- or `LUA_DIALOG_V122 • route=... • T=<id> ...` if MoonSharp probe executes.
If `T>0`, then capture `MAINTHREAD_PROOF`, `ACTION_V122`, next GameDialog/MessageBox and HP/money/result state.

## Do not break
- no guessed/cached ID;
- no UIRoot tuning;
- no WaitTreatment NPC reopen;
- no fixed sleep success proof;
- no broad reverse;
- do not revert V120 returned-object resolver without contrary runtime evidence;
- do not call current returned object xLua after MoonSharp runtime proof;
- BUILD PASS is not Runtime PASS;
- artifact handoff bundle is mandatory.

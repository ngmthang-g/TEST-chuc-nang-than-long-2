# FEATURE: AUTO HEAL NPC / TRỊ LIỆU

## Goal
`route -> one NPC open -> observe current server GameDialog data -> current Treatment selection -> one semantic action -> fresh follow-up -> result proof`.
Only port after repeated runtime PASS.

## Mandatory startup / artifact handoff
Read `AI_START_HERE.md`, V2 protocol, client-analysis TXT, `AI_PROJECT_HANDOFF_FULL.md`, project knowledge/changelog, this feature + BUG/DEC/EVID/history + current source. Use `clinent-game-than-long-DATA-2222/AI_INDEX.md` before targeted reverse work.

## Protected working path
- raw user-captured MapID/X/Y;
- mount / route / StopPath / dismount;
- Map 5 -> NPC 339 Đỗ Thanh Đằng;
- semantic NPC interaction/open;
- no WaitTreatment NPC reopen after successful initial open in tested v1.1.8 transaction.

## Runtime layer findings
### v1.1.8
GameDialog opened but UIRoot observer returned `clickable=0`, `texts=0`, `labels=<none>`. UIRoot/UIButton observer abandoned.

### v1.1.9
GameDialog opened then `LuaSystemManager instance unresolved`. Returned object/DoString/Selections/action not reached.

### v1.1.10
Manager resolver advanced to object returned by `get_LuaEnv`; old DoString lookup failed. Manager -> returned object = runtime PARTIAL PASS.

### v1.1.11 — MoonSharp discovery
Repeated runtime diagnostic:

`LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}`

This proves current returned object is MoonSharp Script and exact live method is String/Table/String -> DynValue. V121 found it but rejected it because its scorer expected parameter #2 String. Method invocation was not reached, so Selections/Treatment/action/server remain UNKNOWN.

## Current implementation — v1.1.12
### Returned Script resolver
`src/bridge_lua_manager_v1_1_10.inc` retained. Runtime proves it reaches current Script object.

### MoonSharp DoString / dialog observer
`src/bridge_lua_moonsharp_v1_1_12.inc`:
- obtains current object through V120 resolver;
- validates exact current method `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
- invokes `DoString(code, null, friendlyName)`;
- captures managed exception class/text when possible;
- resolves `DynValue.get_String()` and copies returned diagnostic string;
- runs existing bounded current GameDialog/AutoFight_Main Selections probe;
- emits `LUA_MOONSHARP_V122` on exact method/invoke/result failures;
- emits `LUA_DIALOG_V122` with T/C/K/raw after execution.

Official MoonSharp source matches the live signature and `DynValue.String` result model.

### GameDialog action
`src/bridge_action_v1_1_12.inc`:
- requires safe action prerequisite;
- re-runs V122 immediately before action;
- rejects absent/stale/guessed ID;
- submits `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentID>:-1`;
- logs `ACTION_V122`;
- waits for fresh server/UI state.

### Confirmation
If live MessageBox exists, use semantic `ButtonOKClicked()`. Otherwise current GameDialog `Xác nhận` uses current selection ID.

## Canonical facts
- `Selections[selectionID]=visibleText`;
- no global Treatment ID;
- built-in AutoFight dialog logic inspects current selections;
- GameDialog request ID `100007`, payload `selectionID:SelectedItemID`;
- current returned object = runtime-confirmed `MoonSharp.Interpreter.Script`;
- current DoString metadata shape = runtime-confirmed String/Table/String -> DynValue.

## Failed / superseded assumptions
- UIRoot/UIButton observer: runtime failed.
- named LuaSystemManager singleton: runtime failed.
- xLua identity/parameter model for current returned object: disproven by v1.1.11 runtime.

## Build status — v1.1.12
Initial commit `ad2f0403863251330b1aca80eb1eba9681b58c9a`, run `31937607280`: CI FAILED before compilation due false-positive audit requiring class-name literal in resolver source.

Correction commit `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1`, run `31937703988`: **CI/BUILD PASS**. Architecture audit, Route/Heal tests, bridge DLL/PE, controller EXE, knowledge packaging and artifact upload all PASS.

Artifact `9261162703`, ZIP SHA `d25f999934be62152cf02fd0251f533743f3e9eaa0d4178f74c2fa1007d85c19`; exactly 9 required files.  
EXE SHA `32a3977469a26f5807be192a620630fba9ea3242e265706522fd7d93bb27c823`.  
DLL SHA `553c1d245c06c614561b9c2c1e6368daf30cb671b46602aec60b1938f326a037`.

Runtime v1.1.12: **UNTESTED**.

## Artifact contract — v1.1.11+
Every delivered ZIP contains EXE/DLL plus consolidated handoff/startup/protocol/rules/project knowledge/changelog/build evidence.

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

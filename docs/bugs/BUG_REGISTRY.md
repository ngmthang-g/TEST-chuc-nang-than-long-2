# BUG REGISTRY

## BUG-001 — GameDialog Trị liệu mở được nhưng chưa tiến sang service/result

**Status:** OPEN  
**Severity:** HIGH for Auto Heal feature  
**First Observed:** v1.1.0-test  
**Last Runtime Tested:** v1.1.11-test delivered artifact, 2026-08-16  
**Last Known-Good:** NONE for complete Treatment flow  
**Related Feature:** Auto trị liệu NPC

### Runtime-confirmed working
- raw NPC coordinate capture;
- route/mount/AutoPath/dismount;
- `ClickNPC(339)` opens intended server-driven GameDialog;
- v1.1.8 transaction removed WaitTreatment NPC reopen;
- v1.1.10+ reaches object returned by `LuaSystemManager.get_LuaEnv`;
- v1.1.11 identifies that object as `MoonSharp.Interpreter.Script` and enumerates the real DoString overload.

### Runtime-confirmed failures by layer
#### v1.1.8 — UIRoot representation
GameDialog existed but UIRoot/CoreChildren walker repeatedly saw `clickable=0 • texts=0 • labels=<none>`. Abandoned.

#### v1.1.9 — manager singleton assumption
After GameDialog opened: `LuaSystemManager instance unresolved`. Returned object/DoString/Selections/action not reached.

#### v1.1.10 — old DoString lookup
Manager resolver advanced to returned object; old DoString lookup failed. Manager -> returned object became runtime PARTIAL PASS.

#### v1.1.11 — real MoonSharp method rejected by accepted-shape model
Exact repeated diagnostic:

`LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}`

Source correlation: V121 required parameter index 1 to be `System.String`; live index 1 is `MoonSharp.Interpreter.Table`. Correct method was enumerated but rejected before invocation. Lua chunk / Selections / live Treatment ID / action/server remain NOT REACHED / UNKNOWN.

### Current semantic source of truth
- `GameDialog.Selections[selectionID] = visibleText`;
- `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`;
- no universal Treatment selectionID;
- current returned script object is runtime-confirmed `MoonSharp.Interpreter.Script`;
- current live DoString shape is runtime-confirmed `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`.

### Attempts / lineage
1. v1.1.0–v1.1.2: initial UIRoot/button experiments; full runtime FAIL.
2. v1.1.3: GameDialog callback via ExecuteUIObject; shared discovery remained.
3. v1.1.4: live Tag + exact packet experiment; shared discovery still gated reachability.
4. v1.1.5: alternate NPC/map reproduced symptom.
5. v1.1.6: CTS/MainThread experiment; build PASS, no full Treatment runtime PASS.
6. v1.1.7: observer redesign; final CI failed and source retained gaps.
7. v1.1.8: anti-reopen runtime PASS; UIRoot representation runtime FAIL.
8. v1.1.9: manager singleton resolver runtime FAIL.
9. v1.1.10: metadata-driven returned-object resolver; runtime PARTIAL PASS, old DoString lookup fail.
10. v1.1.11: overload diagnostics + handoff packaging; runtime discovers MoonSharp exact method but V121 shape scorer rejects it.
11. v1.1.12: exact MoonSharp call/result contract. Initial CI audit false-positive failed before compile; corrected source-bearing build PASS; runtime pending.

### v1.1.12 strategy
- preserve `ResolveLuaEnvV120()` because runtime reaches current Script object;
- require exact metadata shape `DoString(String,MoonSharp.Interpreter.Table,String)->MoonSharp.Interpreter.DynValue`;
- invoke `DoString(code, null, friendlyName)`;
- read `DynValue.get_String()` because bounded probe returns one string;
- emit managed exception/result diagnostics if exact invocation/result fails;
- only after V122 probe returns live `T>0`, re-read and send current GameDialog selection.

### v1.1.12 build state
Initial run `31937607280`: CI FAILED before compilation due architecture-audit false-positive requiring a class-name literal in source.  
Correction commit `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1`.  
Run `31937703988`: **CI/BUILD PASS**; artifact ID `9261162703`; verified 9-file handoff bundle.  
Runtime: **UNTESTED**.

### Root cause status
**CONFIRMED resolved component:** WaitTreatment NPC reopen removed in tested v1.1.8 transaction.  
**CONFIRMED failed component:** v1.1.8 UIRoot representation.  
**CONFIRMED failed component:** v1.1.9 singleton resolver assumption.  
**RUNTIME PARTIAL PASS:** v1.1.10 manager -> returned Script object.  
**CONFIRMED failed component:** v1.1.11 accepted-shape scorer rejected actual MoonSharp overload.  
**UNKNOWN:** V122 execution, live Treatment ID, action/follow-up/result.

### Current workaround
None. Do not port Auto Heal to production.

### Fixed in
UNKNOWN until complete runtime PASS.

### Next diagnostic step
Run v1.1.12 and preserve earliest marker:
- `LUA_MOONSHARP_V122 ...`, or
- `LUA_DIALOG_V122 ... T=...`.
Only after `T>0` evaluate `MAINTHREAD_PROOF`, `ACTION_V122`, next GameDialog/MessageBox and HP/money state.

### Do-not-do
- no broad reverse;
- no UIRoot depth/label tuning;
- no hardcoded Treatment ID;
- no WaitTreatment ClickNPC retry;
- no fixed Sleep as success proof;
- no packet/server blame before current live selection is sent;
- do not revert runtime-proven returned Script resolver;
- do not keep treating current returned object as xLua after MoonSharp runtime proof.

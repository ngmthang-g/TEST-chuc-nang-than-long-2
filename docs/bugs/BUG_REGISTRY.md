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
- v1.1.10+ reaches the object returned by `LuaSystemManager.get_LuaEnv`;
- v1.1.11 identifies that object as `MoonSharp.Interpreter.Script` and enumerates the real DoString overload.

### Runtime-confirmed failures by layer
#### v1.1.8 — UIRoot representation
GameDialog existed but active UIRoot/CoreChildren walker repeatedly saw `clickable=0 • texts=0 • labels=<none>`. Abandoned.

#### v1.1.9 — manager singleton assumption
After GameDialog opened: `LuaSystemManager instance unresolved`. Returned script object/DoString/Selections/action were not reached.

#### v1.1.10 — old DoString lookup
Manager resolver advanced to the returned object; old `FindLuaDoStringV119` failed. Manager -> returned object became RUNTIME PARTIAL PASS.

#### v1.1.11 — real MoonSharp method rejected by wrong accepted-shape model
Exact user diagnostic repeats:

`LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}`

Source correlation:
- V121 did enumerate the real method;
- `ScoreDoStringV121` required parameter #2 (`index 1`) to be `System.String` whenever argc >= 2;
- actual MoonSharp parameter #2 is `MoonSharp.Interpreter.Table`;
- therefore V121 rejected the correct runtime method before invocation.

Result: Lua chunk / Selections / live Treatment ID / action/server remain NOT REACHED / UNKNOWN.

### Current semantic source of truth
- `GameDialog.Selections[selectionID] = visibleText`.
- `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`.
- no universal Treatment selectionID.
- current script engine object at this boundary is runtime-confirmed `MoonSharp.Interpreter.Script`.
- current live DoString signature is runtime-confirmed `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`.

### Attempts / lineage
1. v1.1.0–v1.1.2: initial UIRoot/button experiments.
2. v1.1.3: GameDialog callback through ExecuteUIObject; old button discovery remained.
3. v1.1.4: live Tag + exact packet; old discovery still gated reachability.
4. v1.1.5: alternate NPC/map reproduced symptom.
5. v1.1.6: MainThread/CTS proof experiment; build pass, no full Treatment runtime pass.
6. v1.1.7: observer redesign; final CI failed and source retained gaps.
7. v1.1.8: anti-reopen runtime pass; UIRoot representation runtime fail.
8. v1.1.9: runtime Lua path attempt; manager singleton resolver fail.
9. v1.1.10: metadata-driven returned-object resolver; partial runtime pass, old DoString lookup fail.
10. v1.1.11: overload diagnostics + handoff packaging; runtime discovers MoonSharp exact method but shape scorer rejects it.
11. v1.1.12: exact MoonSharp call/result contract; build/runtime pending.

### v1.1.12 strategy
- preserve `ResolveLuaEnvV120()` because runtime reaches the current Script object;
- require exact runtime metadata shape `DoString(String,MoonSharp.Interpreter.Table,String)->MoonSharp.Interpreter.DynValue`;
- invoke `DoString(code, null, friendlyName)` in that parameter order;
- read `DynValue.get_String()` because the bounded probe returns one string;
- emit detailed managed exception/result diagnostics if invocation fails;
- only after V122 probe returns a live `T>0`, re-read and send current GameDialog selection.

### Root cause status
**CONFIRMED resolved component:** WaitTreatment NPC reopen removed in tested v1.1.8 transaction.  
**CONFIRMED failed component:** v1.1.8 UIRoot representation.  
**CONFIRMED failed component:** v1.1.9 singleton resolver assumption.  
**RUNTIME PARTIAL PASS:** v1.1.10 manager -> returned Script object.  
**CONFIRMED failed component:** v1.1.11 accepted-shape scorer rejected the actual MoonSharp overload.  
**UNKNOWN:** V122 Lua execution, live Treatment ID, action/follow-up/result.

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
- do not revert the runtime-proven returned Script resolver;
- do not keep treating current returned object as xLua after MoonSharp runtime proof.

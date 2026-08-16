# BUG REGISTRY

## BUG-001 — GameDialog Trị liệu mở được nhưng không tiến sang service/result

**Status:** OPEN  
**Severity:** HIGH for Auto Heal feature  
**First Observed:** v1.1.0-test  
**Last Runtime Tested:** v1.1.10-test delivered artifact, 2026-08-16  
**Last Known-Good:** NONE for full Treatment flow  
**Related Feature:** Auto trị liệu NPC

### Runtime-confirmed working
- raw NPC coordinate capture;
- route/mount/AutoPath/dismount;
- `ClickNPC(339)` opens the intended server-driven GameDialog;
- v1.1.8 tested transaction removed the old WaitTreatment reopen behavior;
- v1.1.10 advances through the LuaSystemManager/LuaEnv resolver far enough to attempt DoString resolution.

### Runtime-confirmed failures by layer
#### v1.1.8 — UIRoot representation
After one NPC open, every probe returned `DIALOG_V118 NO MATCH ... clickable=0 • texts=0 • labels=<none>` until fail-closed. GameDialog presence existed. The active UIRoot/CoreChildren representation does not expose the needed dynamic content in that runtime.

#### v1.1.9 — LuaSystemManager singleton resolver
After GameDialog opened, repeated:
`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`.

This failed before LuaEnv/DoString/Selections/action.

#### v1.1.10 — old DoString resolver
Latest user log after route/dismount/one ClickNPC:
`LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)`.

Current source calls `ResolveLuaEnvV120()` before this lookup. Therefore:
- LuaSystemManager -> LuaEnv = **RUNTIME PARTIAL PASS**;
- old `FindLuaDoStringV119` = **RUNTIME FAIL**;
- chunk execution / Selections / Treatment ID / action / server = NOT REACHED / UNKNOWN.

### Canonical semantic source of truth
- `GameDialog.Selections[selectionID] = visibleText`.
- built-in AutoFight dialog logic stores/inspects current server selections.
- `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`.
- no universal static Treatment ID.
- canonical KB verifies LuaSystemManager `get_LuaEnv/set_LuaEnv`; exact client DoString overload visibility was not already documented.

### Attempts / lineage
1. v1.1.0–v1.1.2: initial UIRoot/button experiments; full runtime FAIL.
2. v1.1.3: GameDialog callback via ExecuteUIObject; shared discovery remained.
3. v1.1.4: live Tag + exact packet experiment; shared discovery still gated reachability.
4. v1.1.5: alternate NPC/map reproduced same overall symptom.
5. v1.1.6: CTS/MainThread queued UIButton experiment; build PASS, action-stage runtime evidence not preserved.
6. v1.1.7: observer redesign; final CI failed and source retained gaps.
7. v1.1.8: all-descendant scan + live Tag gate + no WaitTreatment reopen. Runtime anti-reopen PASS, UIRoot representation FAIL.
8. v1.1.9: moved observer to Lua runtime Selections; BUILD PASS, runtime FAIL at manager-instance resolver before LuaEnv.
9. v1.1.10: metadata-driven LuaEnv resolver. BUILD PASS; runtime reaches LuaEnv, then FAILS at old DoString resolver.
10. v1.1.11: retain V120 LuaEnv resolution; replace only DoString lookup with overload-aware runtime metadata resolver and self-describing diagnostics. Artifact also gains mandatory consolidated handoff package.

### v1.1.11 strategy
- actual runtime LuaEnv class + declared get_LuaEnv return class are both inspected;
- enumerate current DoString overloads and direct-lookup by arity as second route;
- prefer String chunk overload;
- support Byte[] chunk overload with managed UTF-8 byte array;
- only invoke arity/type combinations validated by runtime metadata;
- fail closed with `LUA_DOSTRING_V121` actual class/signature diagnostics;
- if probe succeeds, use V121 current Selections and re-read live ID before any action.

### Root cause status
**CONFIRMED resolved component:** WaitTreatment NPC reopen removed in tested v1.1.8 transaction.

**CONFIRMED failed component:** v1.1.8 UIRoot representation.

**CONFIRMED failed component:** v1.1.9 singleton resolver assumption/path.

**RUNTIME PARTIAL PASS:** v1.1.10 LuaSystemManager -> LuaEnv resolution.

**CONFIRMED failed component:** v1.1.10 old DoString resolver.

**UNKNOWN:** whether v1.1.11 resolves a usable DoString method; current Treatment ID and follow-up sequence remain unknown until probe success.

### Current workaround
None. Do not port Auto Heal to production.

### Fixed in
UNKNOWN until complete runtime PASS.

### Next diagnostic step
Run v1.1.11. Preserve earliest new line:
- `LUA_DOSTRING_V121 unresolved ...` if lookup still fails; or
- `LUA_DIALOG_V121 ... T=...` if the Lua chunk executes.
Only after T resolves should `MAINTHREAD_PROOF`, `ACTION_V121`, subsequent GameDialog/MessageBox and HP/money state be evaluated.

### Do-not-do
- no broad reverse;
- no UIRoot depth/label tuning;
- no hardcoded Treatment ID;
- no WaitTreatment ClickNPC retry;
- no fixed Sleep as success proof;
- no claim of packet/server failure before a live current selection is actually sent;
- no reverting the runtime-proven V120 LuaEnv resolver while diagnosing DoString.

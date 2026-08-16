# BUG REGISTRY

## BUG-001 — GameDialog Trị liệu mở được nhưng không tiến sang service/result

**Status:** OPEN  
**Severity:** HIGH for Auto Heal feature  
**First Observed:** v1.1.0-test  
**Last Runtime Tested:** v1.1.9-test delivered artifact, 2026-08-16  
**Last Known-Good:** NONE for full Treatment flow  
**Related Feature:** Auto trị liệu NPC

### Runtime-confirmed working
- raw NPC coordinate capture;
- route/mount/AutoPath/dismount;
- `ClickNPC(339)` opens the intended server-driven GameDialog;
- v1.1.8 tested transaction removed the old WaitTreatment reopen behavior.

### Runtime-confirmed failures by layer
#### v1.1.8 — UIRoot representation
After one NPC open, every probe returned `DIALOG_V118 NO MATCH ... clickable=0 • texts=0 • labels=<none>` until fail-closed. GameDialog presence existed. This proves the active UIRoot/CoreChildren representation does not expose the needed dynamic content in that runtime.

#### v1.1.9 — LuaSystemManager resolver
User screenshot/live log shows the dialog opens and the tool repeatedly reports:

`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`

Therefore v1.1.9 fails **before** `LuaEnv.DoString`, before current `Selections`, before Treatment ID and before `ACTION_V119`. Do not classify Lua probe/parser/packet/server as failed from this run.

### Canonical semantic source of truth
- `GameDialog.Selections[selectionID] = visibleText`.
- built-in AutoFight dialog logic stores/inspects current server selections.
- `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`.
- no universal static Treatment ID.
- canonical KB verifies `LuaSystemManager.get_LuaEnv/set_LuaEnv`, but does not verify a stable manager singleton getter/field name.

### Attempts / lineage
1. v1.1.0–v1.1.2: initial UIRoot/button experiments; full runtime FAIL.
2. v1.1.3: GameDialog callback via ExecuteUIObject; shared discovery remained.
3. v1.1.4: live Tag + exact packet experiment; shared discovery still gated reachability.
4. v1.1.5: alternate NPC/map reproduced same overall symptom.
5. v1.1.6: CTS/MainThread queued UIButton experiment; build PASS, action-stage runtime evidence not preserved.
6. v1.1.7: observer redesign; final CI failed and source retained gaps.
7. v1.1.8: all-descendant scan + live Tag gate + no WaitTreatment reopen. Runtime anti-reopen PASS, UIRoot representation FAIL.
8. v1.1.9: moved observer to Lua runtime `Selections`; BUILD PASS, runtime FAIL at `LuaSystemManager instance unresolved` before LuaEnv/DoString.
9. v1.1.10: correct only the manager/LuaEnv resolver boundary; source-bearing BUILD PASS, runtime pending.

### v1.1.10 fix strategy
- check whether `get_LuaEnv` is static and call it directly first;
- if an instance is genuinely required, try `get_Instance` only if metadata exposes it;
- enumerate bounded static reference fields with IL2CPP APIs instead of guessing four names;
- validate candidate runtime class as LuaSystemManager/derived before accepting it;
- use typed Unity object lookup only if manager hierarchy proves it is a Unity object;
- fail closed with `LUA_MANAGER_V120` diagnostics if unresolved;
- keep Lua `Selections` probe/action logic unchanged after the resolver.

### Root cause status
**CONFIRMED resolved component:** WaitTreatment NPC reopen removed in tested v1.1.8 transaction.

**CONFIRMED failed component:** v1.1.8 UIRoot representation.

**CONFIRMED failed component:** v1.1.9 singleton resolver assumption/path.

**UNKNOWN:** whether v1.1.10 resolves LuaEnv and what actual Treatment selectionID/follow-up sequence is returned.

### Current workaround
None. Do not port Auto Heal to production.

### Fixed in
UNKNOWN until complete runtime PASS.

### Next diagnostic step
Run v1.1.10. Preserve the earliest resolver/probe line:
- `LUA_DIALOG_V120 • route=... • T=...` if LuaEnv/DoString succeeds; or
- `LUA_MANAGER_V120 ...` exact fallback/candidate failure.
Only after T resolves should `MAINTHREAD_PROOF`, `ACTION_V120`, subsequent GameDialog/MessageBox and HP/money state be evaluated.

### Do-not-do
- no broad reverse;
- no UIRoot depth/label tuning;
- no hardcoded Treatment ID;
- no WaitTreatment ClickNPC retry;
- no fixed Sleep as success proof;
- no claim of packet/server failure before a live current selection is actually sent.
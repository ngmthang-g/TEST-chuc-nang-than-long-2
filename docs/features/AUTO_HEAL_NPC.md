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
- Map 3 -> NPC 463 alternate test candidate;
- semantic NPC interaction/open;
- no WaitTreatment NPC reopen after successful initial open.

## Runtime layer findings
### v1.1.8
GameDialog opens but UIRoot observer reports `clickable=0`, `texts=0`, `labels=<none>` until fail-closed. UIRoot/UIButton is not the active Treatment observer anymore.

### v1.1.9
GameDialog opens then repeatedly:
`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`.
This fails before LuaEnv/DoString/Selections/Treatment ID/action.

### v1.1.10
Latest user runtime:
```text
Đã gửi AutoPath tới map=5 x=9454 y=5477
Đã gửi lệnh xuống ngựa
Đã gọi ClickNPC npcID=339
LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)
```

Current source calls `ResolveLuaEnvV120()` before old `FindLuaDoStringV119()`. Therefore:
- manager/LuaEnv resolution = **RUNTIME PARTIAL PASS**;
- old DoString resolver = **RUNTIME FAIL**;
- Lua probe execution / Selections / live Treatment ID / action/server = NOT REACHED.

## Current implementation — v1.1.11
### LuaEnv resolver — protected from v1.1.10
`src/bridge_lua_manager_v1_1_10.inc` remains the active prerequisite. Do not redesign it merely because the next DoString layer failed.

### DoString resolver / dialog observer
`src/bridge_lua_dostring_v1_1_11.inc`:
- obtains actual LuaEnv runtime class;
- also resolves the declared return class of `LuaSystemManager.get_LuaEnv`;
- enumerates current non-static `DoString` overloads on class/parents;
- tries direct lookup by arity as a second metadata route;
- prefers `System.String` chunk overload;
- supports `System.Byte[]` chunk overload with managed UTF-8 byte-array construction;
- supports safe metadata-matched arity 3/2/1;
- on failure logs `LUA_DOSTRING_V121 unresolved` with actual/declared class and discovered signatures;
- on execution success runs the bounded current GameDialog/AutoFight_Main Selections probe and returns `LUA_DIALOG_V121` T/C/K.

Targeted research note: official Tencent xLua source exposes both String and Byte[] DoString overloads. This guides the resolver but does not prove the exact game client build; live metadata decides what is callable.

### GameDialog action
`src/bridge_action_v1_1_11.inc`:
- requires safe current managed game/thread prerequisite;
- re-reads current V121 selections immediately before action;
- rejects absent/stale/guessed ID;
- submits canonical `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentID>:-1`;
- logs `ACTION_V121`;
- waits for fresh server/UI state.

### Confirmation
If a live MessageBox exists, use semantic `ButtonOKClicked()`. Otherwise a current GameDialog `Xác nhận` selection uses its actual current ID.

## Canonical facts
- `Selections[selectionID]=visibleText`.
- no global Treatment ID.
- built-in AutoFight dialog logic inspects current selections.
- GameDialog request ID `100007`, payload `selectionID:SelectedItemID`.
- canonical KB verifies LuaSystemManager `get_LuaEnv/set_LuaEnv`.
- current exact DoString method availability must come from runtime metadata.

## Artifact contract — v1.1.11+
Every delivered ZIP must contain:
- EXE/DLL;
- `AI_PROJECT_HANDOFF_FULL.md`;
- `AI_START_HERE.md`;
- mandatory protocol/rules;
- `PROJECT_KNOWLEDGE.md`;
- `CHANGELOG.md`;
- generated `BUILD_EVIDENCE.txt`.

## Safety / isolation
v1.1.11 does NOT:
- return to UIRoot/button scanning;
- add NPC reopen retries;
- hardcode Treatment ID;
- raw-scan heap/pointers;
- change route/state machine semantics;
- declare downstream packet/server failure before those stages are reached.

## Build status
v1.1.10 BUILD PASS; runtime reaches LuaEnv then fails at DoString resolver.

v1.1.11 source/CI: **PENDING at source commit creation**. Runtime: **UNTESTED**.

## Required next log
After the single NPC open, capture one of:
- `LUA_DOSTRING_V121 unresolved ...` — inspect actual class/signatures only;
- `LUA_DIALOG_V121 • route=... • T=<id> ...` — Lua chunk executed.
If `T>0`, then capture `MAINTHREAD_PROOF`, `ACTION_V121`, next GameDialog/MessageBox and HP/money/result state.

## Do not break
- no guessed/cached ID;
- no UIRoot tuning;
- no WaitTreatment NPC reopen;
- no fixed sleep success proof;
- no broad reverse;
- do not revert V120 LuaEnv resolver without contrary runtime evidence;
- BUILD PASS is not Runtime PASS;
- artifact handoff bundle is mandatory.

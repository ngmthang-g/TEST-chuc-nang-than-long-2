# PROJECT KNOWLEDGE

## Identity / mandatory startup
- Repo: `ngmthang-g/TEST-chuc-nang-than-long-2`; branch `agent/auto-tri-lieu-v1.1.0`.
- Current version: `v1.1.11-test`; full Auto trị liệu known-good: **NONE**.
- Every version: `AI_START_HERE.md` -> V2 protocol -> client-analysis TXT -> `AI_PROJECT_HANDOFF_FULL.md` -> this file -> `CHANGELOG.md` -> affected feature/BUG/DEC/EVID/history -> source/tests.
- If only artifact ZIP is available, read `AI_PROJECT_HANDOFF_FULL.md` first.
- Client facts: start from `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`; no broad reverse for solved facts.
- BUILD/CI PASS != RUNTIME PASS.

## Artifact handoff contract
From v1.1.11 onward every delivered CI artifact must contain EXE/DLL plus:
- `AI_PROJECT_HANDOFF_FULL.md`;
- `AI_START_HERE.md`;
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`;
- `AI_CLIENT_ANALYSIS_RULES.txt`;
- `PROJECT_KNOWLEDGE.md`;
- `CHANGELOG.md`;
- generated `BUILD_EVIDENCE.txt` with branch-head SHA, checkout SHA, run/build/runtime state.

This was added after the user correctly reported that the v1.1.10 ZIP exposed only EXE/DLL and did not contain the requested consolidated project file.

## Runtime-confirmed protected path
- raw user-captured MapID/X/Y;
- mount / AutoPath / dismount;
- NPC 339 Đỗ Thanh Đằng on Map 5 can be opened semantically;
- v1.1.8 tested transaction removed the old WaitTreatment reopen behavior: one NPC interaction followed by fail-closed observation.

## Runtime layer history
### v1.1.8
Delivered v1.1.8 showed GameDialog present but every active UIRoot observer sample returned `clickable=0 • texts=0 • labels=<none>` until timeout. Therefore UIRoot/CoreChildren/UIButton is a confirmed unusable representation for this live dynamic GameDialog. Do not return to tree-depth/text tuning.

### v1.1.9
User tested delivered v1.1.9 on PID 4324 / Map 5 / NPC 339. GameDialog opened; every probe failed at:
`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`.

This is **RUNTIME FAIL at manager resolver**. v1.1.9 never reached LuaEnv/DoString/Selections/action.

### v1.1.10 — latest user runtime 2026-08-16
User log:
```text
Đã gửi AutoPath tới map=5 x=9454 y=5477
Đã gửi lệnh xuống ngựa
Đã gọi ClickNPC npcID=339
LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)
```
The DoString failure repeats while GameDialog is open.

Current source control flow is decisive: `RunLuaChunkV120()` first succeeds through `ResolveLuaEnvV120(...)`, then obtains the returned object's runtime class, then calls old `FindLuaDoStringV119(...)`. Therefore reaching the shown error establishes:
- `LuaSystemManager -> LuaEnv`: **RUNTIME PARTIAL PASS**;
- old narrow DoString resolver: **RUNTIME FAIL**;
- Lua chunk execution / current Selections / Treatment ID / action / server follow-up: **NOT REACHED / UNKNOWN**.

Do not classify this as generic Lua failure and do not blame packet/server yet.

## Canonical GameDialog contract
- `GameDialog.Selections[selectionID] = visibleText`.
- IDs are server/runtime data; never hardcode Treatment ID.
- built-in AutoFight dialog flows inspect current selections.
- `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`; ordinary function choice commonly uses `-1` item.
- MessageBox OK uses its callback path.

## v1.1.11 active design
Only the runtime-confirmed DoString lookup boundary changes. The v1.1.10 LuaEnv resolver, route/NPC/no-reopen, Lua selection probe concept and current-ID action semantics remain protected.

Active files:
- `src/bridge_lua_manager_v1_1_10.inc` — retained runtime-proven LuaEnv resolution;
- `src/bridge_lua_dostring_v1_1_11.inc` — active overload-aware DoString resolver and V121 dialog probe;
- `src/bridge_action_v1_1_11.inc` — active V121 live-ID action;
- `AI_PROJECT_HANDOFF_FULL.md` — consolidated artifact handoff.

DoString resolver:
1. inspect actual runtime class of the returned LuaEnv object;
2. also inspect declared return class of `LuaSystemManager.get_LuaEnv`;
3. enumerate current non-static `DoString` methods on class/parents;
4. prefer `System.String` chunk overload;
5. support `System.Byte[]` overload with managed byte[] UTF-8 allocation;
6. accept arity 3/2/1 only when runtime metadata matches safe parameter shapes;
7. additionally try direct method lookup by arity as a second metadata route;
8. if unresolved, emit `LUA_DOSTRING_V121` with actual/declared class and discovered signatures instead of generic failure.

Upstream Tencent xLua source is used only as research guidance: it contains both String and Byte[] DoString overloads. The live client metadata remains source of truth.

After DoString resolves:
`execute bounded Lua probe -> current GameDialogData.Selections -> T/C/K -> re-read current selection at action time -> exact semantic request -> fresh state proof`.

Expected diagnostics:
- `LUA_DOSTRING_V121 unresolved ...` on DoString resolution failure;
- `LUA_DIALOG_V121 • route=... • T=... • C=... • K=...` on probe success;
- `ACTION_V121 ...` only after current semantic selection is re-read.

## Build evidence
- v1.1.10 BUILD PASS; runtime reaches LuaEnv then fails at DoString as described.
- v1.1.11 source-bearing commit: `95f285f929a32c9748342a3480748a5b79d1a4d0`.
- source-bearing PR Actions run `31935080947`: architecture audit PASS; Route FSM PASS; Heal FSM PASS; bridge DLL/PE PASS; controller PASS; knowledge packaging PASS; artifact upload PASS.
- source-bearing artifact: `ThanLongTestAutoHeal-v1.1.11`, ID `9260424284`, ZIP digest `sha256:2e085f606c5df0fed5933c3eff13cec0b544d923ea18e94b1892e392cc2cb8ae`.
- verified source-bearing artifact content: 9 files including EXE/DLL, `AI_PROJECT_HANDOFF_FULL.md`, startup/protocol/rules/current knowledge/changelog and `BUILD_EVIDENCE.txt`.
- source-bearing EXE SHA-256: `ee7ff5aeb66c6e9715d7f73522834ced6d56484dc4933259b441af6d19959764`.
- source-bearing DLL SHA-256: `d30b30757442b9671606f83301509c539eb22ad8da6e9b93638cf24f2a9eff5f`.
- correction before final handoff: PR-event `GITHUB_SHA` is a merge/checkout SHA, so final packaging records both `SOURCE_HEAD_SHA` and `CHECKOUT_SHA` separately.
- v1.1.11 runtime: **UNTESTED**.

## Hard rules
- no inferred NPC X/Y as universal truth;
- no hardcoded/cached Treatment ID;
- no WaitTreatment NPC reopen;
- no UIRoot/UIButton tuning for this dialog;
- no fixed sleep as success proof;
- one mutable action per PID;
- metadata/runtime validation before invoking dynamic managed methods;
- preserve build/runtime distinction and failed attempts;
- every artifact must include consolidated knowledge/handoff files.

## BUG-001 current status
OPEN.
- resolved component: controller-driven NPC reopen absent in tested v1.1.8 transaction;
- failed component v1.1.8: UIRoot representation;
- failed component v1.1.9: LuaSystemManager singleton resolver assumption;
- partial-pass component v1.1.10: LuaSystemManager/LuaEnv resolution;
- failed component v1.1.10: old `FindLuaDoStringV119` resolver;
- current experiment v1.1.11: overload-aware DoString resolution;
- unknown: actual live Treatment selectionID and server/follow-up sequence until Lua probe succeeds.

## Next runtime evidence
Run v1.1.11 and preserve log from first NPC open. Earliest useful marker is either:
- `LUA_DOSTRING_V121 unresolved ...`, or
- `LUA_DIALOG_V121` with `route=` and T/C/K.
Only after `T>0` should `MAINTHREAD_PROOF` / `ACTION_V121` and subsequent dialog/HP/money state be interpreted.

See `AI_PROJECT_HANDOFF_FULL.md`, `docs/history/VERSION_v1.1.10.md`, `docs/history/VERSION_v1.1.11.md`, `docs/investigations/V120_DOSTRING_RUNTIME_FINDING.md`, feature/BUG/EVID/DEC registries.

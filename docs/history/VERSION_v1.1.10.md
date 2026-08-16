# VERSION v1.1.10-test — LuaSystemManager / LuaEnv resolver

## A. Identity / Lineage
- Version: v1.1.10-test
- Date: 2026-08-16
- Based On: v1.1.9-test
- Reason Created: v1.1.9 runtime failed before LuaEnv at `LuaSystemManager instance unresolved`.
- Last Known-Good full Auto Heal: NONE.
- Superseded By: v1.1.11-test for the next confirmed failing layer.
- Related BUG: BUG-001.

## B. User Request / Trigger
User runtime-tested delivered v1.1.9. NPC 339 / Map 5 opened GameDialog, then every live-selection probe failed at:
`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`.

v1.1.10 was created to correct only manager/LuaEnv resolution without touching route/NPC/no-reopen or the downstream current-Selections model.

## C. State Before Modification
Working/protected:
- route/mount/AutoPath/dismount;
- one semantic NPC open;
- v1.1.8 anti-reopen behavior.

Broken:
- v1.1.9 assumed it could obtain a LuaSystemManager instance via `get_Instance` or four guessed static field names before checking the LuaEnv getter.

Unknown before v1.1.10 runtime:
- whether LuaEnv could be obtained;
- whether DoString was present/callable;
- current Treatment selection ID/action/server response.

## D. Investigation / Root Cause
v1.1.9 source required manager instance first even though canonical client KB only verified `get_LuaEnv/set_LuaEnv`, not a stable singleton contract.

Root cause for v1.1.9 failure: **CONFIRMED overly narrow manager-instance resolution path**.

## E. Changes Made
`src/bridge_lua_manager_v1_1_10.inc`:
1. if `get_LuaEnv` is static, call it directly;
2. otherwise try semantic `get_Instance` if exposed;
3. enumerate bounded static reference fields using IL2CPP metadata and validate candidate runtime class;
4. typed Unity object lookup only if the manager class hierarchy proves UnityEngine.Object ancestry;
5. fail closed with `LUA_MANAGER_V120` diagnostics.

`src/bridge_action_v1_1_10.inc` retained live current-ID action semantics and used V120 markers.

No heap pointer guessing, UIRoot retry, hardcoded Treatment ID or WaitTreatment NPC reopen was added.

## F. Important Implementation Details
Active v1.1.10 flow:
`one NPC open -> ResolveLuaEnvV120 -> old FindLuaDoStringV119 -> bounded Lua Selections probe -> current ID -> exact semantic request -> fresh state proof`.

The DoString stage intentionally remained from v1.1.9 so v1.1.10 isolated the manager/LuaEnv variable.

## G. Files / Components Changed
Added:
- `src/bridge_lua_manager_v1_1_10.inc`
- `src/bridge_action_v1_1_10.inc`

Modified:
- `src/bridge.cpp`
- `src/protocol.h`
- controller version strings
- build/workflow versioning
- knowledge/history.

## H. Build / CI History
Source-bearing build commit: `d35517385266d4fa75011374966816a0e8d5ada1`.
Run `31933118883`: architecture audit PASS; Route FSM PASS; Heal FSM PASS; bridge DLL/PE PASS; controller EXE PASS; artifact upload PASS.

Final branch build around the delivered v1.1.10 lineage also passed. BUILD/CI did not establish runtime completion.

## I. Runtime Result — UPDATED AFTER USER TEST
**RUNTIME: PARTIAL PASS + FAIL AT NEXT LAYER**

User exact runtime log:
```text
Đã gửi AutoPath tới map=5 x=9454 y=5477
Đã gửi lệnh xuống ngựa
Đã gọi ClickNPC npcID=339
LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)
```
The final line repeats while waiting.

Confirmed working in this run:
- route to healer area;
- dismount;
- semantic NPC open path;
- LuaSystemManager/LuaEnv resolver progresses far enough to return a LuaEnv object.

Still failing:
- old DoString resolver `FindLuaDoStringV119`.

Not reached / still UNKNOWN:
- actual Lua chunk execution;
- current GameDialog Selections;
- Treatment selection ID;
- `ACTION_V120`/packet;
- server follow-up/HP/money completion.

Why LuaEnv is considered partial PASS:
`RunLuaChunkV120()` returns immediately if `ResolveLuaEnvV120()` fails. The logged `DoString unresolved` is emitted only after a LuaEnv object is available and its runtime class is queried.

## J. Regression / Failed Attempts
This runtime does not regress the V120 LuaEnv resolver; it narrows the next failure to method lookup. Do not revert the V120 resolver while debugging DoString without contrary evidence.

## K. Known-Good Established
No full Auto Heal known-good established. Only the LuaEnv boundary gained runtime partial-pass evidence.

## L. Remaining Bugs / New Knowledge / Decisions
- BUG-001 remains OPEN.
- EVID-016 records the v1.1.10 runtime boundary.
- DEC-012 in v1.1.11 replaces the narrow DoString assumption with live overload metadata resolution.
- User also reported the v1.1.10 ZIP lacked the requested consolidated knowledge handoff; DEC-013 corrects artifact packaging from v1.1.11.

## M. Handoff
Next version must start at:
`LuaEnv resolved -> DoString method resolution`.

Do NOT restart at NPC coordinates, UIRoot, button text, guessed selection ID or packet/server theory.

# PROJECT KNOWLEDGE

## Identity / mandatory startup
- Repo: `ngmthang-g/TEST-chuc-nang-than-long-2`; branch `agent/auto-tri-lieu-v1.1.0`.
- Current version: `v1.1.10-test`; full Auto trị liệu known-good: **NONE**.
- Every version: `AI_START_HERE.md` -> `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md` -> `AI_CLIENT_ANALYSIS_RULES.txt` -> this file -> `CHANGELOG.md` -> affected feature/BUG/DEC/EVID/history -> source/tests.
- Client facts: start from `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`; no broad reverse for solved facts.
- BUILD/CI PASS != RUNTIME PASS.

## Runtime-confirmed protected path
- raw user-captured MapID/X/Y;
- mount / AutoPath / dismount;
- NPC 339 Đỗ Thanh Đằng on Map 5 can be opened semantically;
- v1.1.8 tested transaction removed the old WaitTreatment reopen behavior: one NPC interaction followed by fail-closed observation.

## v1.1.8 runtime finding
Delivered v1.1.8 showed GameDialog present but every active UIRoot observer sample returned `clickable=0 • texts=0 • labels=<none>` until timeout. Therefore UIRoot/CoreChildren/UIButton is a confirmed unusable representation for this live dynamic GameDialog. Do not return to tree-depth/text tuning.

## v1.1.9 runtime finding — 2026-08-16
User tested delivered v1.1.9 on PID 4324 / Map 5 / NPC 339. Screenshot/log shows:
- Auto trị liệu starts and `ClickNPC(339)` opens the dialog;
- status: `DIALOG ĐÃ MỞ • CHỜ LIVE SELECTION TRỊ LIỆU`;
- every probe fails at `LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`.

This is **RUNTIME FAIL at the Lua manager resolver boundary**. v1.1.9 never reaches `LuaEnv.DoString`, `Selections`, live Treatment ID, action send or server follow-up. Therefore later stages remain UNKNOWN, not failed.

Source correction: v1.1.9 required `LuaSystemManager` instance before checking `get_LuaEnv`, and its instance resolver only tried `get_Instance` plus four guessed static field names. Canonical KB verifies `get_LuaEnv/set_LuaEnv` but does not verify a stable LuaSystemManager singleton contract.

## Canonical GameDialog contract
- `GameDialog.Selections[selectionID] = visibleText`.
- IDs are server/runtime data; never hardcode Treatment ID.
- built-in AutoFight dialog flows inspect current selections.
- `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`; ordinary function choice commonly uses `-1` item.
- MessageBox OK uses its callback path.

## v1.1.10 active design
Only the failing `LuaSystemManager -> LuaEnv` boundary changes. Route/NPC/no-reopen/Lua selection probe/action semantics remain protected.

Active files:
- `src/bridge_lua_manager_v1_1_10.inc`
- `src/bridge_action_v1_1_10.inc`

Resolver order:
1. resolve manager class + `get_LuaEnv` semantically;
2. if `get_LuaEnv` is static, invoke it directly — no manager singleton required;
3. otherwise try semantic static `get_Instance` if present;
4. enumerate static reference fields on manager hierarchy and bounded known Lua bridge classes using IL2CPP metadata APIs; accept only a value whose runtime class is LuaSystemManager/derived;
5. only if LuaSystemManager belongs to UnityEngine.Object hierarchy, try typed `FindFirstObjectByType` / `FindAnyObjectByType` / legacy `FindObjectOfType`;
6. fail closed with `LUA_MANAGER_V120` field/count/Unity-candidate diagnostics if unresolved.

No heap pointer guessing, raw address scan, UIRoot retry, fixed Treatment ID or NPC reopen was added.

After LuaEnv resolves:
`DoString -> current GameDialogData.Selections -> T/C/K -> re-read live selection at action time -> exact semantic request -> fresh state proof`.

Expected diagnostics:
- `LUA_MANAGER_V120 ...` on resolver failure;
- `LUA_DIALOG_V120 • route=... • T=... • C=... • K=...` on probe success;
- `ACTION_V120 ...` only after current semantic selection is re-read.

## Build evidence
- v1.1.9 build PASS but runtime fails at manager resolver as above.
- v1.1.10 source-bearing build commit `d35517385266d4fa75011374966816a0e8d5ada1`.
- Actions run `31933118883`: architecture audit PASS; Route/Heal self-tests PASS; bridge DLL/PE verification PASS; controller compile PASS; artifact upload PASS.
- Artifact `ThanLongTestAutoHeal-v1.1.10`, ID `9259895908`, ZIP SHA-256 `4b43c205bedd4177288b562ed7df20b08adb5f295b6b1601a11699e0bb80ef60`.
- v1.1.10 runtime: **UNTESTED**.

## Hard rules
- no inferred NPC X/Y;
- no hardcoded/cached Treatment ID;
- no WaitTreatment NPC reopen;
- no UIRoot/UIButton tuning for this dialog;
- no fixed sleep as success proof;
- one mutable action per PID;
- metadata/runtime validation before accepting object candidates;
- preserve build/runtime distinction and failed attempts.

## BUG-001 current status
OPEN.
- resolved component: controller-driven NPC reopen absent in tested v1.1.8 transaction;
- failed component v1.1.8: UIRoot representation;
- failed component v1.1.9: LuaSystemManager instance resolver;
- current experiment v1.1.10: robust LuaEnv resolution;
- unknown: actual live Treatment selectionID and server/follow-up sequence until Lua probe succeeds.

## Next runtime evidence
Run v1.1.10 and preserve from first NPC open. Earliest useful marker is either:
- `LUA_DIALOG_V120` with `route=` and T/C/K, or
- exact `LUA_MANAGER_V120` failure diagnostics.
Only after `T>0` should `MAINTHREAD_PROOF` / `ACTION_V120` and subsequent dialog/HP/money state be interpreted.

See `docs/history/VERSION_v1.1.10.md`, `docs/investigations/V119_LUA_MANAGER_RUNTIME_FINDING.md`, feature/BUG/EVID/DEC registries.
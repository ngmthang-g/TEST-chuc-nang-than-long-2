# Thần Long TEST — Auto trị liệu runtime lab

Repository này là **test lab độc lập**, không phải tool production. Mục tiêu hiện tại là chứng minh chuỗi Auto trị liệu theo state thật của client trước khi port sang bản chính.

## Bắt buộc đọc trước mỗi version
1. `AI_START_HERE.md`
2. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
3. `AI_CLIENT_ANALYSIS_RULES.txt`
4. `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. feature / BUG / DEC / EVID / version history liên quan
7. source/tests hiện tại.

Canonical client knowledge: `ngmthang-g/clinent-game-than-long-DATA-2222`. Không broad reverse lại client khi fact exact đã có trong VERIFIED/database/docs.

## Current version
`v1.1.10-test`

Runtime known-good partial path:
`tọa độ user capture -> route/mount -> stop/dismount -> ClickNPC -> GameDialog mở`.

Full `Trị liệu -> follow-up -> completion proof`: **chưa có runtime PASS**.

### Latest runtime evidence
Delivered v1.1.9 opens the NPC dialog but repeatedly fails at:

`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`

So the current confirmed failure is **before LuaEnv.DoString / Selections / action**.

### v1.1.10 experiment
Only fixes the Lua manager/LuaEnv resolver:
- static `get_LuaEnv` first;
- semantic `get_Instance` if present;
- bounded metadata-driven static reference scan with runtime class validation;
- typed Unity object lookup only when hierarchy proves it applies;
- fail-closed `LUA_MANAGER_V120` diagnostics.

Downstream GameDialog semantics remain current runtime `Selections[selectionID]=visibleText`, re-read at action time, no hardcoded ID and no UIRoot/UIButton dependency.

Source-bearing build run `31933118883`: PASS. Final verified run `31933561439`: PASS. Final artifact name: `ThanLongTestAutoHeal-v1.1.10`.

See `docs/history/VERSION_v1.1.10.md` and `docs/investigations/V119_LUA_MANAGER_RUNTIME_FINDING.md`.
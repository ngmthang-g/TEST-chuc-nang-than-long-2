# Thần Long TEST — Auto trị liệu runtime lab

Repository này là **test lab độc lập**, không phải tool production. Mục tiêu hiện tại là chứng minh chuỗi Auto trị liệu theo state thật của client trước khi port sang bản chính.

## Bắt buộc đọc trước mỗi version
1. `AI_START_HERE.md`
2. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
3. `AI_CLIENT_ANALYSIS_RULES.txt`
4. `AI_PROJECT_HANDOFF_FULL.md`
5. `PROJECT_KNOWLEDGE.md`
6. `CHANGELOG.md`
7. feature / BUG / DEC / EVID / version history liên quan
8. source/tests hiện tại.

Canonical client knowledge: `ngmthang-g/clinent-game-than-long-DATA-2222`. Không broad reverse lại client khi fact exact đã có trong VERIFIED/database/docs.

## Current version
`v1.1.11-test`

Runtime known-good partial path:
`tọa độ user capture -> route/mount -> stop/dismount -> ClickNPC -> GameDialog mở`.

Full `Trị liệu -> follow-up -> completion proof`: **chưa có runtime PASS**.

### Latest runtime evidence — v1.1.10
User log after one NPC open repeatedly reports:

`LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)`

Because current V120 control flow resolves LuaEnv before attempting old DoString lookup, the manager/LuaEnv boundary is now **RUNTIME PARTIAL PASS** and the earliest confirmed failure is the old DoString resolver. Selections/action/server remain not reached.

### v1.1.11 experiment
- keeps v1.1.10 LuaEnv resolver;
- reads actual/declared LuaEnv class metadata;
- resolves DoString overloads from current runtime methods;
- supports String and managed UTF-8 Byte[] chunk shapes when metadata validates them;
- logs `LUA_DOSTRING_V121` signatures on failure;
- continues to current runtime `Selections[selectionID]=visibleText` only after DoString is actually callable;
- action marker becomes `ACTION_V121`.

## Artifact handoff rule
From v1.1.11 onward the CI ZIP is self-describing. It must contain EXE/DLL plus:
- `AI_PROJECT_HANDOFF_FULL.md`;
- `AI_START_HERE.md`;
- mandatory protocol/rules;
- `PROJECT_KNOWLEDGE.md`;
- `CHANGELOG.md`;
- generated `BUILD_EVIDENCE.txt`.

If you only have the ZIP, read `AI_PROJECT_HANDOFF_FULL.md` first.

See `docs/history/VERSION_v1.1.11.md` and `docs/investigations/V120_DOSTRING_RUNTIME_FINDING.md`.

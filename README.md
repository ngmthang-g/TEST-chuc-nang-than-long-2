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
`v1.1.12-test`

Runtime known-good partial path:
`tọa độ user capture -> route/mount -> stop/dismount -> ClickNPC -> GameDialog mở`.

Full `Trị liệu -> follow-up -> completion proof`: **chưa có runtime PASS**.

### Latest runtime evidence — v1.1.11
Delivered v1.1.11 repeatedly reports:

`actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue ...}`

This is the current runtime source of truth. v1.1.11 successfully exposed the real method but rejected it because its accepted-shape scorer expected argument #2 to be `System.String`; the live method uses `MoonSharp.Interpreter.Table`. The Lua chunk was not invoked, so Selections/action/server remain not reached.

### v1.1.12 experiment
- keeps the runtime-proven v1.1.10 returned-object resolver;
- treats the current object as `MoonSharp.Interpreter.Script`;
- requires exact live method `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
- invokes `DoString(code, null, friendlyName)` in that order;
- extracts the returned diagnostic string through `DynValue.get_String()`;
- only then continues to current runtime `Selections[selectionID]=visibleText`;
- markers: `LUA_MOONSHARP_V122`, `LUA_DIALOG_V122`, `ACTION_V122`.

Official MoonSharp source independently matches this signature/result model. The earlier xLua assumption is preserved only as failed investigation history and is not current engine identity for this boundary.

## Artifact handoff rule
From v1.1.11 onward the CI ZIP is self-describing. It contains EXE/DLL plus:
- `AI_PROJECT_HANDOFF_FULL.md`;
- `AI_START_HERE.md`;
- mandatory protocol/rules;
- `PROJECT_KNOWLEDGE.md`;
- `CHANGELOG.md`;
- generated `BUILD_EVIDENCE.txt`.

If you only have the ZIP, read `AI_PROJECT_HANDOFF_FULL.md` first.

See `docs/history/VERSION_v1.1.12.md` and `docs/investigations/V121_MOONSHARP_RUNTIME_FINDING.md`.

# Thần Long TEST — Auto trị liệu runtime lab

Repository này là **test lab độc lập**, không phải tool production. Mục tiêu là chứng minh chuỗi Auto trị liệu bằng state thật của client trước khi port sang bản chính.

## Bắt buộc đọc trước mỗi version
1. `AI_START_HERE.md`
2. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
3. `AI_CLIENT_ANALYSIS_RULES.txt`
4. `AI_PROJECT_HANDOFF_FULL.md`
5. `PROJECT_KNOWLEDGE.md`
6. `CHANGELOG.md`
7. feature / BUG / DEC / EVID / version history / investigation liên quan
8. source/tests hiện tại.

Canonical client knowledge: `ngmthang-g/clinent-game-than-long-DATA-2222`. Không broad reverse khi exact fact đã có trong VERIFIED/database/docs.

## Current version
`v1.1.13-test`

Runtime known-good partial path:
`tọa độ runtime -> route/mount -> stop/dismount -> ClickNPC -> GameDialog mở -> MoonSharp Script.DoString chạy -> DynValue.String trả probe`.

Full `Trị liệu -> follow-up -> completion proof`: **chưa có runtime PASS**.

## Latest runtime evidence — v1.1.12
Delivered v1.1.12 repeatedly returned:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

This proves the exact MoonSharp execution/result path works and isolates the current problem to observing the live Lua dialog representation.

**Important correction:** V122 `N=4` is four traversed table nodes, **not four selections**.

## v1.1.13 experiment
V122 observer used `rawget` for `Selections`/priority fields, which bypasses `__index`/metatable lookup. v1.1.13 keeps the runtime-proven MoonSharp execution unchanged and replaces only the read-only observer:
- normal `t[key]` indexing under `pcall`;
- bounded child-table traversal;
- bounded metatable/table-`__index` traversal;
- canonical `Selections`/`GameDialogData` field checks;
- diagnostics `NODE`, `ST`, `SV`, `MT`, `KS`, `S`;
- current action only when `T>0`, with live ID re-read immediately before send;
- markers `LUA_DIALOG_V123`, `ACTION_V123`.

Canonical contract remains `GameDialogData.Selections[selectionID]=visibleText`; no static Treatment ID is assumed.

## Artifact handoff rule
Every v1.1.11+ CI ZIP contains exactly 9 files: EXE, bridge DLL, consolidated handoff, startup file, V2 protocol, client-analysis rules, project knowledge, changelog and `BUILD_EVIDENCE.txt`.

If only the ZIP is available, read `AI_PROJECT_HANDOFF_FULL.md` first.

See `docs/history/VERSION_v1.1.13.md` and `docs/investigations/V122_SELECTIONS_RUNTIME_FINDING.md`.

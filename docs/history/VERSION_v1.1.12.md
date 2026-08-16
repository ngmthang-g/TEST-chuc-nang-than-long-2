# VERSION v1.1.12-test — exact MoonSharp DoString + DynValue result

## A. Identity / Lineage
- Version: v1.1.12-test
- Date: 2026-08-16
- Based On: v1.1.11-test
- Reason Created: user runtime proved returned object is `MoonSharp.Interpreter.Script` and exposed exact `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`. v1.1.11 enumerated this method but rejected it before invocation.
- Last Known-Good full Auto Heal: NONE.
- Supersedes: v1.1.11 active DoString accepted-shape logic/action markers.
- Related BUG: BUG-001.
- Related Decisions: DEC-014.
- Related Evidence: EVID-019, EVID-020, EVID-021, EVID-022.

## B. User Runtime Report / Requirement
Delivered v1.1.11 repeatedly logged:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

Stopping/restarting Auto Heal reproduced the same boundary. Continue from exact returned-object/DoString boundary; do not revisit route/NPC/UIRoot.

## C. State Before Modification
Runtime-confirmed protected:
- raw captured target MapID/X/Y;
- route/mount/AutoPath/dismount;
- `ClickNPC(339)` opens intended GameDialog;
- no WaitTreatment controller reopen in tested v1.1.8 transaction;
- v1.1.10+ reaches object returned through LuaSystemManager path;
- v1.1.11 identifies it as MoonSharp Script and exposes real DoString through IL2CPP metadata.

Not reached before v1.1.12:
- actual DoString invocation;
- bounded probe execution;
- current GameDialog Selections/Treatment ID;
- current-ID semantic action;
- server/UI/HP/money completion state.

## D. Investigation / Root Cause
### Root cause: CONFIRMED
`ScoreDoStringV121()` required parameter index 1 to be `System.String` for multi-argument methods.

Live method:
`DoString(System.String, MoonSharp.Interpreter.Table, System.String) -> MoonSharp.Interpreter.DynValue`.

Thus correct method was enumerated but rejected because index 1 is `MoonSharp.Interpreter.Table`; V121 stopped before `runtime_invoke`.

### Engine identity correction
Earlier targeted xLua research was a diagnostic hypothesis only. v1.1.11 runtime disproves xLua as current returned type. Current evidence hierarchy is live runtime metadata -> current source -> targeted official MoonSharp source.

Official MoonSharp defines `Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null)` -> `DynValue`, and `DynValue.String` exposes string payload.

## E. Changes Made
### `src/bridge_lua_moonsharp_v1_1_12.inc`
- preserves `ResolveLuaEnvV120()`;
- requires exact current metadata shape `DoString(String,Table,String)->DynValue`;
- invokes `DoString(code, null, "ThanLongAutoHealV122")`;
- adds bounded managed-exception diagnostics;
- treats return as MoonSharp `DynValue`, not `object[]`;
- resolves `DynValue.get_String()` and requires `System.String`;
- copies returned probe text and parses T/C/K;
- markers `LUA_MOONSHARP_V122` and `LUA_DIALOG_V122`.

### `src/bridge_action_v1_1_12.inc`
- consumes V122 current IDs;
- re-runs V122 immediately before action;
- rejects absent/guessed/cached ID;
- submits `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentID>:-1`;
- emits `ACTION_V122`;
- MessageBox remains semantic callback.

### Wiring / versioning
- v1.1.11 observer/action retained under legacy names;
- protocol -> `0x00010112`;
- controller strings -> v1.1.12;
- artifact -> `ThanLongTestAutoHeal-v1.1.12`;
- 9-file self-contained artifact handoff retained.

## F. Important Implementation Details
- no hardcoded method RVA;
- no raw heap scan;
- no UIRoot/UIButton fallback;
- no hardcoded Treatment selection ID;
- parameter #2 is MoonSharp Table and is passed null; friendly name is #3;
- bounded probe is designed to return one string, so V122 intentionally reads `DynValue.String`; non-String result fails closed.

## G. Files / Components Changed
Added:
- `src/bridge_lua_moonsharp_v1_1_12.inc`
- `src/bridge_action_v1_1_12.inc`
- `docs/history/VERSION_v1.1.12.md`
- `docs/investigations/V121_MOONSHARP_RUNTIME_FINDING.md`

Modified bridge wiring, protocol/version strings, build/workflow, consolidated handoff/current knowledge/changelog, feature/BUG/EVID/DEC docs, v1.1.11 history and README.

## H. Build / CI History
### Initial source build
- Commit: `ad2f0403863251330b1aca80eb1eba9681b58c9a`
- Run: `31937607280`
- Result: **CI FAILED before compilation**.
- Cause: architecture audit required literal `MoonSharp.Interpreter.Script` inside V122 resolver source even though resolver validates current method metadata instead of hardcoding class-name literal.
- Artifact: none; upload skipped.

### Correction
- Commit: `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1`
- Change: remove only false-positive class-name literal requirement from audit; keep Table/DynValue/get_String/V122 structural checks.

### Source-bearing successful build
- Run: `31937703988`
- Architecture audit: PASS
- Route FSM: PASS
- Heal FSM: PASS
- Bridge DLL compile: PASS
- PE verification: PASS
- Controller EXE compile: PASS
- Knowledge packaging: PASS
- Artifact upload: PASS
- Artifact: `ThanLongTestAutoHeal-v1.1.12`, ID `9261162703`
- ZIP SHA-256: `d25f999934be62152cf02fd0251f533743f3e9eaa0d4178f74c2fa1007d85c19`
- Verified artifact members: exactly 9 required files
- EXE SHA-256: `32a3977469a26f5807be192a620630fba9ea3242e265706522fd7d93bb27c823`
- DLL SHA-256: `553c1d245c06c614561b9c2c1e6368daf30cb671b46602aec60b1938f326a037`
- `BUILD_EVIDENCE`: source head `33a3b56...`, checkout `3855f781...`, run `31937703988`, BUILD PASS, RUNTIME UNTESTED.

A docs-only final HEAD is rebuilt before handoff. Its EXE/DLL must match the source-bearing hashes byte-for-byte.

## I. Runtime Result
`RUNTIME UNTESTED` for v1.1.12.

Do not promote inherited working layers into full v1.1.12 runtime PASS until user runs delivered artifact.

## J. Regression / Failed Attempts
### v1.1.11 xLua-inspired accepted-shape model
- useful result: exposed actual runtime class/signatures;
- failure: rejected correct MoonSharp method before invocation;
- status: FAILED / SUPERSEDED;
- lesson: current live metadata outranks upstream-library assumptions.

### v1.1.12 initial CI audit
- failure mode: audit false-positive before compiler;
- correction: removed unnecessary class-name literal requirement while preserving structural source checks;
- gameplay logic unchanged by correction.

## K. Known-Good Established
NONE for complete Auto Heal.

## L. Remaining Bugs / New Knowledge / Decisions
BUG-001 remains OPEN. DEC-014 ACTIVE. Current stable runtime knowledge: returned object is MoonSharp Script; exact live DoString is String/Table/String -> DynValue; V121 never invoked it.

## M. Handoff / Next Test
Capture from first `Đã gọi ClickNPC` onward.

1. `LUA_MOONSHARP_V122 ...` -> diagnose only exact invocation/result boundary.
2. `LUA_DIALOG_V122 ... T=0 ... raw={...}` -> MoonSharp execution passed; inspect current table/path/text.
3. `LUA_DIALOG_V122 ... T=<positive>` -> evaluate MainThread/action prerequisite and `ACTION_V122`.
4. `ACTION_V122 SENT ...` -> only then evaluate fresh GameDialog/MessageBox, HP/money and server result.

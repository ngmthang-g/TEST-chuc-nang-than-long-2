# VERSION v1.1.12-test — exact MoonSharp DoString + DynValue result

## A. Identity / Lineage
- Version: v1.1.12-test
- Date: 2026-08-16
- Based On: v1.1.11-test
- Reason Created: user runtime proved the object returned by the LuaSystemManager path is `MoonSharp.Interpreter.Script` and exposed the exact live `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue` signature. v1.1.11 enumerated this method but rejected it before invocation.
- Last Known-Good full Auto Heal: NONE.
- Supersedes: v1.1.11 active DoString accepted-shape logic/action markers.
- Related BUG: BUG-001.
- Related Decisions: DEC-014.
- Related Evidence: EVID-019, EVID-020.

## B. User Runtime Report / Requirement
Delivered v1.1.11 repeatedly logged:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

User also stopped/restarted Auto Heal and reproduced the same boundary.

Required continuation: do not revisit route/NPC/UIRoot; use the exact runtime method now exposed and continue the semantic GameDialog probe.

## C. State Before Modification
Runtime-confirmed protected:
- raw captured target MapID/X/Y;
- route/mount/AutoPath/dismount;
- one semantic `ClickNPC(339)` opens the intended GameDialog;
- no WaitTreatment controller reopen in the tested v1.1.8 transaction;
- v1.1.10+ resolves a non-null object returned through LuaSystemManager path;
- v1.1.11 identifies that object as `MoonSharp.Interpreter.Script` and exposes the real DoString method through IL2CPP metadata.

Still not reached before v1.1.12:
- actual DoString invocation;
- bounded Lua probe execution;
- current GameDialog Selections/Treatment ID;
- current-ID semantic action;
- server/UI/HP/money completion state.

## D. Investigation / Root Cause
### Root cause: CONFIRMED
`ScoreDoStringV121()` in v1.1.11 accepted multi-argument methods only when parameter index 1 was `System.String`.

Live runtime metadata shows the real method is:

`DoString(System.String, MoonSharp.Interpreter.Table, System.String) -> MoonSharp.Interpreter.DynValue`

Therefore the correct method was successfully enumerated but rejected because index 1 is `MoonSharp.Interpreter.Table`, not String. V121 stopped before `runtime_invoke`.

### Engine identity correction
Earlier targeted xLua research was useful only as a diagnostic hypothesis. v1.1.11 runtime now disproves xLua as the current returned runtime type at this boundary.

Current evidence hierarchy:
1. user live runtime metadata: `MoonSharp.Interpreter.Script` + exact signature;
2. current source: confirms V121 rejection condition;
3. targeted official MoonSharp source: independently matches the live signature and result type.

Official MoonSharp source defines:
`Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null)` -> `DynValue`.
`DynValue.String` returns the string payload for a String DynValue.

## E. Changes Made
### `src/bridge_lua_moonsharp_v1_1_12.inc`
- preserves `ResolveLuaEnvV120()` as the runtime-proven returned-object resolver;
- requires the current metadata shape exactly:
  `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
- enumerates current DoString methods and retains direct 3-arg lookup only as a metadata route, never a guessed RVA;
- invokes the exact parameter order:
  `DoString(code, null, "ThanLongAutoHealV122")`;
- adds bounded managed-exception class/text diagnostics;
- treats the return as a MoonSharp `DynValue`, not `object[]`;
- resolves `DynValue.get_String()` and requires return type `System.String`;
- copies the returned probe string, then parses T/C/K from the existing bounded GameDialog/AutoFight_Main probe;
- failure marker: `LUA_MOONSHARP_V122`;
- success marker: `LUA_DIALOG_V122`.

### `src/bridge_action_v1_1_12.inc`
- consumes only V122 current IDs;
- re-runs the current V122 probe immediately before action;
- rejects absent/guessed/cached IDs;
- submits `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentID>:-1` only after a live semantic match;
- emits `ACTION_V122`;
- MessageBox confirmation remains semantic callback based.

### Wiring / versioning
- v1.1.11 observer/action retained under legacy names for lineage;
- protocol version -> `0x00010112`;
- controller title/startup log -> v1.1.12;
- build/workflow artifact -> `ThanLongTestAutoHeal-v1.1.12`;
- self-contained 9-file artifact handoff contract retained.

## F. Important Implementation Details
- No hardcoded MoonSharp method RVA.
- No raw heap scan.
- No fallback to UIRoot/UIButton.
- No hardcoded Treatment selection ID.
- Parameter #2 is explicitly the MoonSharp `Table` argument and is passed `null`; friendly name is parameter #3.
- The existing probe returns one string, so v1.1.12 intentionally reads `DynValue.String` rather than trying to model all possible DynValue types.
- If the returned DynValue is not String, the version fails closed with a precise result diagnostic instead of coercing it.

## G. Files / Components Changed
Added:
- `src/bridge_lua_moonsharp_v1_1_12.inc`
- `src/bridge_action_v1_1_12.inc`
- `docs/history/VERSION_v1.1.12.md`
- `docs/investigations/V121_MOONSHARP_RUNTIME_FINDING.md`

Modified:
- `src/bridge.cpp`
- `src/protocol.h`
- controller version strings
- `build.cmd`
- `.github/workflows/build.yml`
- `AI_PROJECT_HANDOFF_FULL.md`
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
- feature/BUG/EVID/DEC docs
- v1.1.11 history runtime result
- README.

## H. Build / CI History
Source-bearing v1.1.12 build: **PENDING at commit creation**.

Build must pass:
- architecture audit;
- Route FSM;
- Heal FSM;
- bridge DLL compile + PE verification;
- controller compile;
- knowledge packaging;
- artifact upload.

## I. Runtime Result
`RUNTIME UNTESTED` for v1.1.12.

Do not promote inherited working layers into a full v1.1.12 runtime PASS until user runs the delivered artifact.

## J. Regression / Revert / Failed Attempts
### v1.1.11 xLua-inspired accepted-shape model
- Goal: generalize DoString resolution.
- Useful result: exposed actual runtime class/signatures.
- Failure: rejected the correct MoonSharp method before invocation.
- Status: FAILED / SUPERSEDED.
- Lesson: current live metadata outranks upstream-library assumptions.
- Retry: not with the same scorer. v1.1.12 uses the exact MoonSharp contract.

## K. Known-Good Established
NONE for complete Auto Heal.

## L. Remaining Bugs / New Knowledge / Decisions
BUG-001 remains OPEN.

New stable runtime knowledge:
- current returned object = `MoonSharp.Interpreter.Script`;
- current DoString signature = `String, Table, String -> DynValue`;
- V121 invocation was never reached.

DEC-014 is ACTIVE.

## M. Handoff / Next Test
Capture from first `Đã gọi ClickNPC` onward.

Interpret earliest marker:
1. `LUA_MOONSHARP_V122 ...` -> diagnose only exact invocation/result boundary;
2. `LUA_DIALOG_V122 ... T=0 ... raw={...}` -> MoonSharp execution passed; inspect only current table/path/text representation;
3. `LUA_DIALOG_V122 ... T=<positive>` -> then evaluate MainThread/action prerequisite and `ACTION_V122`;
4. `ACTION_V122 SENT ...` -> only then evaluate fresh GameDialog/MessageBox, HP/money and server result.

# VERSION v1.1.11-test — Lua DoString overload diagnostics + self-contained artifact handoff

## A. Identity / Lineage
- Version: v1.1.11-test
- Date: 2026-08-16
- Based On: v1.1.10-test
- Reason Created: v1.1.10 reached the returned script object but old DoString lookup failed; user also required a consolidated knowledge file inside the ZIP.
- Last Known-Good full Auto Heal: NONE.
- Superseded By: v1.1.12-test.
- Related BUG: BUG-001.
- Related Decisions: DEC-012, DEC-013, later corrected by DEC-014.

## B. User Requests
1. Continue from exact v1.1.10 runtime failure, not restart route/UI work.
2. Deliver a ZIP that includes a detailed AI-readable project handoff, not only EXE/DLL.

## C. State Before Modification
Protected runtime path:
- route/mount/AutoPath/dismount;
- `ClickNPC(339)` opens GameDialog;
- no WaitTreatment reopen in tested transaction;
- v1.1.10 advances through manager -> returned object.

Earliest confirmed broken layer before V121: old `FindLuaDoStringV119`.

## D. Investigation / Original Hypothesis
V121 introduced live method/class diagnostics and an overload-aware scorer. The implementation was influenced by targeted official xLua research and accepted String-first/Byte[]-first shapes, with a rule that parameter #2 was String for multi-argument methods.

This was explicitly experimental because exact client DoString metadata had not yet been observed.

## E. Changes Made
### `src/bridge_lua_dostring_v1_1_11.inc`
- kept `ResolveLuaEnvV120()`;
- inspected actual/declared returned-object class;
- enumerated DoString methods and logged signatures;
- tried direct lookup by arity;
- supported String and managed UTF-8 Byte[] chunk candidates;
- emitted `LUA_DOSTRING_V121` diagnostics.

### `src/bridge_action_v1_1_11.inc`
- V121 current-ID re-read/action markers.

### Artifact handoff
- added `AI_PROJECT_HANDOFF_FULL.md`;
- build audit requires handoff/current knowledge files;
- CI artifact includes EXE/DLL plus handoff/startup/protocol/rules/knowledge/changelog/build evidence;
- build evidence separates `SOURCE_HEAD_SHA` from checkout/PR merge SHA.

## F. Important Implementation Detail / Later Correction
`ScoreDoStringV121` required parameter index 1 to be `System.String` whenever argc >= 2. This was the critical accepted-shape assumption later disproven by runtime.

## G. Files / Components Changed
Added:
- `src/bridge_lua_dostring_v1_1_11.inc`
- `src/bridge_action_v1_1_11.inc`
- `AI_PROJECT_HANDOFF_FULL.md`
- version/investigation docs.

Modified bridge wiring, protocol/version strings, build/workflow and knowledge files.

## H. Build / CI History
### Source-bearing build
- Commit: `95f285f929a32c9748342a3480748a5b79d1a4d0`
- Run: `31935080947`
- Result: CI/BUILD PASS
- Artifact ID: `9260424284`

### Final pre-v1.1.12 build
- HEAD: `4c9b0d5dceaa3828e3452aee339b3a03ceb0ef3b`
- Run: `31935342682`
- Result: CI/BUILD PASS
- Artifact: `ThanLongTestAutoHeal-v1.1.11`, ID `9260507355`
- Artifact knowledge bundle verified present.

## I. Runtime Result — UPDATED 2026-08-16
**RUNTIME FAIL at V121 accepted-shape resolution; valuable diagnostic success.**

User log repeatedly reports:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

What this proves:
- actual and declared returned object = `MoonSharp.Interpreter.Script`;
- live DoString method exists and is visible to metadata enumeration;
- exact shape = `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
- V121 did not invoke it because parameter index 1 is `MoonSharp.Interpreter.Table`, conflicting with the V121 String requirement.

What this does NOT prove:
- Lua chunk execution;
- current Selections/Treatment ID;
- semantic action/packet acceptance;
- server follow-up/result.

## J. Regression / Failed Attempt
### Failed approach: xLua-inspired accepted DoString shapes
- Goal: generalize old DoString lookup.
- Result: diagnostics succeeded, invocation did not.
- Failure mode: real engine/type is MoonSharp and real parameter order differs from V121 accepted model.
- Lesson: live runtime metadata must outrank upstream-library assumptions.
- Can retry: not the same scorer. Use the exact MoonSharp shape observed at runtime.

## K. Known-Good Established
No complete Auto Heal known-good established.

## L. New Knowledge / Decisions
- `MoonSharp.Interpreter.Script` is current returned runtime type at this boundary.
- exact live DoString signature is now known.
- DEC-014 supersedes DEC-012's engine-specific shape while preserving metadata-first principle.

## M. Handoff
v1.1.12 should:
1. retain V120 returned-object resolver;
2. invoke exact MoonSharp `DoString(code, null Table, friendlyName)`;
3. read returned `DynValue.String`;
4. only then interpret current GameDialog probe and proceed to current-ID action.

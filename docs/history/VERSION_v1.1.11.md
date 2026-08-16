# VERSION v1.1.11-test — Lua DoString overload resolver + self-contained artifact handoff

## A. Identity / Lineage
- Version: v1.1.11-test
- Date: 2026-08-16
- Based On: v1.1.10-test
- Reason Created: user runtime proved v1.1.10 reaches LuaEnv then fails specifically at old DoString lookup; user also reported the ZIP lacked the requested consolidated project knowledge file.
- Last Known-Good full Auto Heal: NONE.
- Supersedes: v1.1.10 active DoString lookup/action markers.
- Related BUG: BUG-001.
- Related Decisions: DEC-012, DEC-013.

## B. User Requests
1. Continue from exact v1.1.10 runtime failure rather than restarting previous investigation layers.
2. Ensure delivered ZIP contains a detailed one-file project handoff for AI, not only EXE/DLL.

Acceptance for this version:
- preserve runtime-proven LuaEnv resolver;
- replace narrow DoString resolver with metadata/overload-aware logic and useful diagnostics;
- package `AI_PROJECT_HANDOFF_FULL.md` and current knowledge files inside the artifact;
- BUILD/CI and RUNTIME remain explicitly separate.

## C. State Before Modification
Runtime-confirmed protected:
- route/mount/AutoPath/dismount;
- semantic `ClickNPC(339)` opens GameDialog;
- no WaitTreatment reopen in tested transaction;
- v1.1.10 progresses through LuaSystemManager/LuaEnv.

Runtime-confirmed broken:
`FindLuaDoStringV119` used by v1.1.10 returns unresolved.

Exact v1.1.10 log:
```text
Đã gửi AutoPath tới map=5 x=9454 y=5477
Đã gửi lệnh xuống ngựa
Đã gọi ClickNPC npcID=339
LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)
```

Artifact gap:
workflow uploaded only EXE/DLL, so the requested consolidated knowledge file was absent from v1.1.10 ZIP.

## D. Investigation / Root Cause
### DoString
Current source showed v1.1.10 still called old `FindLuaDoStringV119`, which:
- used a narrow exact String-first signature model;
- depended on one method-enumeration route;
- had no Byte[] overload support;
- did not report the actual returned LuaEnv runtime class and current DoString signatures.

Canonical client KB did not already contain exact client DoString overload metadata. Targeted research of official Tencent xLua source found both `DoString(byte[], string, LuaTable)` and `DoString(string, string, LuaTable)`. This does not prove the game uses the same exact version, but it disproves the safety of assuming one upstream method shape.

Root cause of v1.1.10 DoString failure: **CONFIRMED old resolver is insufficient for the live client; exact reason (stripping/type-name/iterator/overload difference) remains UNKNOWN until V121 diagnostics/runtime**.

### Artifact
Root cause: **CONFIRMED workflow uploaded only two binary paths**.

## E. Changes Made
### `src/bridge_lua_dostring_v1_1_11.inc`
- reuses `ResolveLuaEnvV120()` unchanged;
- captures actual returned LuaEnv runtime class;
- captures declared return class of `LuaSystemManager.get_LuaEnv`;
- enumerates current DoString methods across class/parents;
- independently tries direct lookup by arity 3/2/1;
- scores safe runtime shapes;
- prefers first parameter `System.String`;
- supports first parameter `System.Byte[]`;
- requires second parameter String when present and third parameter reference type when present;
- allocates managed `System.Byte[]` and encodes Lua chunk UTF-8 when byte overload is chosen;
- executes existing bounded GameDialog/AutoFight_Main Selections probe only after verified DoString method resolution;
- emits `LUA_DOSTRING_V121` diagnostics or successful `LUA_DIALOG_V121` T/C/K data.

### `src/bridge_action_v1_1_11.inc`
- consumes only V121 live IDs;
- re-reads current selection immediately before action;
- preserves exact semantic GameDialog request and MessageBox callback behavior;
- logs `ACTION_V121`.

### Artifact / knowledge packaging
- added root `AI_PROJECT_HANDOFF_FULL.md`;
- build audit requires handoff + mandatory current knowledge files;
- successful build copies handoff/startup/protocol/rules/project knowledge/changelog to `dist`;
- successful build generates `dist/BUILD_EVIDENCE.txt`;
- workflow artifact uploads all of those beside EXE/DLL;
- after inspecting the source-bearing PR artifact, packaging was corrected so `BUILD_EVIDENCE.txt` records branch `SOURCE_HEAD_SHA` separately from PR merge/check-out `CHECKOUT_SHA`.

## F. Important Implementation Details
No method RVA or raw function pointer is guessed. V121 only invokes a DoString method whose current IL2CPP metadata matches an accepted shape.

Byte[] path uses `il2cpp_get_corlib`, `il2cpp_array_new`, `System.Byte`, and the existing IL2CPP array header model already used by the bridge for managed arrays.

The existing Lua probe still looks for current semantic text in runtime Selections. This version intentionally does not change the selection algorithm until DoString execution is proven.

## G. Files / Components Changed
Added:
- `src/bridge_lua_dostring_v1_1_11.inc`
- `src/bridge_action_v1_1_11.inc`
- `AI_PROJECT_HANDOFF_FULL.md`
- `docs/history/VERSION_v1.1.11.md`
- `docs/investigations/V120_DOSTRING_RUNTIME_FINDING.md`

Modified:
- `src/bridge.cpp`
- `src/protocol.h`
- controller version strings
- `build.cmd`
- `.github/workflows/build.yml`
- `AI_START_HERE.md`
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
- feature/BUG/EVID/DEC docs
- v1.1.10 history runtime result
- README.

## H. Build / CI History
### Source-bearing build
- Commit: `95f285f929a32c9748342a3480748a5b79d1a4d0`
- PR Actions run: `31935080947`
- Result: **CI/BUILD PASS**
- Architecture audit PASS
- Route FSM PASS
- Heal FSM PASS
- Bridge DLL compile + PE verification PASS
- Controller EXE compile PASS
- Knowledge packaging PASS
- Artifact upload PASS
- Artifact: `ThanLongTestAutoHeal-v1.1.11`, ID `9260424284`
- ZIP digest: `sha256:2e085f606c5df0fed5933c3eff13cec0b544d923ea18e94b1892e392cc2cb8ae`
- EXE SHA-256: `ee7ff5aeb66c6e9715d7f73522834ced6d56484dc4933259b441af6d19959764`
- DLL SHA-256: `d30b30757442b9671606f83301509c539eb22ad8da6e9b93638cf24f2a9eff5f`
- Verified artifact members: 9 files including consolidated handoff and mandatory knowledge bundle.

### Build-evidence correction before final handoff
Inspection showed `GITHUB_SHA` inside a pull_request checkout is the merge/check-out SHA, not the branch HEAD. Final packaging therefore emits both `SOURCE_HEAD_SHA` and `CHECKOUT_SHA`. This is a packaging/evidence correction, not gameplay logic change. Final HEAD is rebuilt before delivery.

## I. Runtime Result
`RUNTIME UNTESTED` for v1.1.11.

Confirmed inherited working behavior is not promoted to full v1.1.11 PASS until user runs the delivered build.

## J. Regression / Revert / Failed Attempts
Protected failed history:
- v1.1.8 empty UIRoot representation;
- v1.1.9 singleton resolver failure;
- v1.1.10 old DoString resolver failure.

Do not retry those exact mechanisms unless new evidence changes their classification.

## K. Known-Good Established
NONE for complete Auto Heal.

## L. Remaining Bugs / New Knowledge / Decisions
BUG-001 OPEN.

New knowledge:
- v1.1.10 manager/LuaEnv resolution is partial runtime PASS;
- DoString lookup is the earliest confirmed failing layer before v1.1.11;
- artifact knowledge must be shipped with binaries.

## M. Handoff / Next Test
Capture from `Đã gọi ClickNPC` onward.

Earliest markers:
1. `LUA_DOSTRING_V121 unresolved ...` -> inspect only actual class/signatures;
2. `LUA_DIALOG_V121 ... T=0 ...` -> DoString passed; inspect only live Lua table/semantic text path;
3. `LUA_DIALOG_V121 ... T=<positive>` -> then evaluate action prerequisite and `ACTION_V121`;
4. action sent -> only then evaluate server/UI/HP/money follow-up.

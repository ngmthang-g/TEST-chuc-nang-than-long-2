# CHANGELOG

## [v1.1.11-test] - 2026-08-16

### Trigger / runtime evidence
- User tested delivered v1.1.10 on Map 5 / NPC 339.
- Route reaches the saved healer area, dismount succeeds, and one `ClickNPC npcID=339` opens the dialog path.
- Repeated exact failure after the dialog opens:
  `LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)`.
- Because `RunLuaChunkV120()` calls `ResolveLuaEnvV120()` before `FindLuaDoStringV119()`, this establishes **RUNTIME PARTIAL PASS for LuaSystemManager -> LuaEnv** and **RUNTIME FAIL specifically at the old DoString resolver**.
- Lua chunk execution, current `Selections`, live Treatment ID, action send and server follow-up are still NOT REACHED / UNKNOWN.

### User handoff requirement correction
- User correctly reported that the v1.1.10 ZIP did not contain the requested consolidated project knowledge file.
- From v1.1.11 onward artifacts are required to include `AI_PROJECT_HANDOFF_FULL.md`, mandatory startup/protocol/rules files, current knowledge/changelog and generated `BUILD_EVIDENCE.txt` beside EXE/DLL.

### Investigation
- v1.1.10 reused old `FindLuaDoStringV119`, which narrowly accepted a String-first DoString shape and had weak runtime-class/signature diagnostics.
- Canonical client KB verifies `LuaSystemManager.get_LuaEnv` but does not document exact DoString overloads for this client.
- Targeted external primary-source research of Tencent xLua shows upstream `LuaEnv` exposes both `DoString(byte[], string, LuaTable)` and `DoString(string, string, LuaTable)`. This is guidance only; live client metadata remains authoritative.

### Added / changed
- Added `src/bridge_lua_dostring_v1_1_11.inc`:
  - reuses runtime-proven `ResolveLuaEnvV120`;
  - inspects actual LuaEnv runtime class and declared `get_LuaEnv` return class;
  - enumerates current DoString overloads and also uses direct lookup by arity as a fallback metadata route;
  - prefers `System.String` chunk overload;
  - supports `System.Byte[]` chunk overload by allocating a managed byte array and UTF-8 encoding the Lua chunk;
  - supports safe runtime-matched arity 3/2/1;
  - emits `LUA_DOSTRING_V121` with actual/declared class + discovered signatures if unresolved;
  - emits `LUA_DIALOG_V121` when the current dialog probe executes.
- Added `src/bridge_action_v1_1_11.inc` using V121 live selection IDs and `ACTION_V121` markers.
- Preserved v1.1.10 observer/action under legacy names for lineage.
- Protocol/title/startup log/artifact naming bumped to v1.1.11.
- Added root `AI_PROJECT_HANDOFF_FULL.md` as one-file artifact handoff.
- `build.cmd` enforces handoff presence, copies knowledge files into `dist`, and generates `BUILD_EVIDENCE.txt` after successful compilation.
- CI artifact uploads EXE/DLL plus the handoff/knowledge bundle.
- Final packaging correction records branch `SOURCE_HEAD_SHA` separately from PR merge/check-out `CHECKOUT_SHA`.

### Build
- Source-bearing commit `95f285f929a32c9748342a3480748a5b79d1a4d0`.
- PR Actions run `31935080947`: **CI/BUILD PASS**.
- Architecture audit PASS; Route FSM PASS; Heal FSM PASS; bridge DLL + PE verification PASS; controller compile PASS; knowledge packaging PASS; artifact upload PASS.
- Source-bearing artifact `ThanLongTestAutoHeal-v1.1.11`, ID `9260424284`, ZIP digest `sha256:2e085f606c5df0fed5933c3eff13cec0b544d923ea18e94b1892e392cc2cb8ae`.
- Verified artifact contains 9 files: EXE, DLL, consolidated handoff, startup, protocol, rules, project knowledge, changelog and build evidence.
- EXE SHA-256 `ee7ff5aeb66c6e9715d7f73522834ced6d56484dc4933259b441af6d19959764`.
- DLL SHA-256 `d30b30757442b9671606f83301509c539eb22ad8da6e9b93638cf24f2a9eff5f`.
- Final docs/packaging HEAD is rebuilt after the `SOURCE_HEAD_SHA` evidence correction before handoff.

### Runtime
- v1.1.11: **RUNTIME UNTESTED**.
- BUG-001 remains OPEN.

### Next evidence
`one NPC open -> LUA_DOSTRING_V121 failure diagnostics OR LUA_DIALOG_V121 T/C/K -> if T>0 then MAINTHREAD_PROOF/ACTION_V121 -> fresh GameDialog/MessageBox/HP/money result`.

## [v1.1.10-test] - 2026-08-16
- Created after v1.1.9 failed at `LuaSystemManager instance unresolved` before LuaEnv/DoString.
- Replaced singleton-name assumption with static get_LuaEnv first, metadata-driven static reference scanning and typed Unity fallback when applicable.
- BUILD PASS.
- Runtime update: **LuaSystemManager/LuaEnv RUNTIME PARTIAL PASS**, then old `FindLuaDoStringV119` **RUNTIME FAIL** with repeated `DoString unresolved`; Selections/action/server NOT REACHED.
- Superseded by v1.1.11 DoString-resolution experiment.

## [v1.1.9-test] - 2026-08-16
- Moved observer from empty UIRoot representation to intended Lua runtime Selections path.
- BUILD PASS.
- Runtime: GameDialog opens, but resolver fails at `LuaSystemManager instance unresolved` before LuaEnv/DoString/Selections/action.

## [v1.1.8-test] - 2026-08-16
- Added all-descendant UIRoot scan, live Tag gate and removed WaitTreatment NPC reopen.
- BUILD PASS.
- Runtime: route/open PASS; anti-reopen PASS for tested transaction; UIRoot representation FAIL with `clickable=0/texts=0` despite GameDialog presence.

## [v1.1.7-test] - 2026-08-16
- Observer redesign; later audit found first-text-only matching and transient-absence retry.
- final CI `31925922772` FAILED; no valid handoff artifact.

## [v1.1.6-test] - 2026-08-16
- Added mandatory project-memory files/startup pointer and CTS/MainThread proof experiment.
- CI/BUILD PASS.

## [v1.1.5-test] - 2026-08-16
- Alternate NPC 463 / Lạc Dương reproduced overall Treatment symptom; BUILD PASS.

## [v1.1.4-test] - 2026-08-16
- Tried live Tag/selectionID + exact GameDialog request; later correction: shared button discovery still gated reachability.

## [v1.1.3-test] - 2026-08-16
- Tried `GameDialog.FunctionButtonClicked(liveButton)` through ExecuteUIObject; shared discovery remained.

## [v1.1.2-test] - 2026-08-16
- Changed UI-root discovery order; route/NPC open partial PASS, Treatment failed.

## [v1.1.1-test] - 2026-08-16
- Removed inferred X/Y; added runtime coordinate capture.

## [v1.1.0-test] - 2026-08-16
- Initial Auto Heal route/state-machine test.

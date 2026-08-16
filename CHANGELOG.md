# CHANGELOG

## [v1.1.12-test] - 2026-08-16

### Trigger / runtime evidence
- User tested delivered v1.1.11 on Map 5 / NPC 339.
- GameDialog opens; repeated V121 diagnostic reports:
  `actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script`.
- Runtime enumerates the real overload:
  `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`.
- Therefore v1.1.11 did reach the returned Script object and enumerate DoString, but rejected the real method because its scorer expected parameter #2 to be `System.String` rather than `MoonSharp.Interpreter.Table`.
- Lua probe execution / current Selections / live Treatment ID / action / server follow-up remain NOT REACHED / UNKNOWN.

### Correction of prior assumption
- The active returned runtime type is **MoonSharp.Interpreter.Script**, not XLua.LuaEnv.
- Earlier xLua research remains historical evidence explaining why v1.1.11 accepted the wrong parameter model, but xLua is DISPROVEN as the current returned Script type for this boundary.
- Targeted official MoonSharp source matches runtime metadata: `Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null)` returns `DynValue`; `DynValue.String` exposes a string result.

### Added / changed
- Added `src/bridge_lua_moonsharp_v1_1_12.inc`:
  - retains runtime-proven `ResolveLuaEnvV120()`;
  - resolves only the runtime-proven MoonSharp signature `DoString(String,Table,String)->DynValue`;
  - invokes with argument order `{code, null Table, friendlyName}`;
  - captures managed exception text when possible;
  - extracts the returned probe string through `DynValue.get_String()`;
  - emits `LUA_MOONSHARP_V122` failures and `LUA_DIALOG_V122` success diagnostics.
- Added `src/bridge_action_v1_1_12.inc` using V122 current live selection IDs and `ACTION_V122` markers.
- Preserved v1.1.11 implementation under legacy names for lineage.
- Protocol/title/startup log/artifact naming bumped to v1.1.12.
- Self-contained artifact handoff contract from v1.1.11 remains mandatory and unchanged.

### Build
- v1.1.12 source-bearing build: **PENDING** at commit creation.

### Runtime
- v1.1.12: **RUNTIME UNTESTED**.
- BUG-001 remains OPEN.

### Next evidence
`one NPC open -> LUA_MOONSHARP_V122 failure OR LUA_DIALOG_V122 T/C/K -> if T>0 then MAINTHREAD_PROOF/ACTION_V122 -> fresh GameDialog/MessageBox/HP/money result`.

## [v1.1.11-test] - 2026-08-16

### Purpose
- Created after v1.1.10 reached a returned Lua environment object but old `FindLuaDoStringV119` could not resolve DoString.
- Added overload-aware diagnostics and mandatory self-contained AI handoff packaging.

### Build
- Source commit `95f285f929a32c9748342a3480748a5b79d1a4d0`, run `31935080947`: CI/BUILD PASS.
- Final HEAD before v1.1.12: `4c9b0d5dceaa3828e3452aee339b3a03ceb0ef3b`, run `31935342682`: CI/BUILD PASS.
- Final artifact `ThanLongTestAutoHeal-v1.1.11`, ID `9260507355`; ZIP contained EXE/DLL + handoff/startup/protocol/rules/knowledge/changelog/build evidence.

### Runtime update
- **RUNTIME FAIL at V121 DoString shape acceptance**, with major new discovery:
  - actual/declared object = `MoonSharp.Interpreter.Script`;
  - live overload = `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
  - v1.1.11 incorrectly required parameter #2 String and never invoked the real method.
- Superseded by v1.1.12 exact MoonSharp path.

## [v1.1.10-test] - 2026-08-16
- Replaced singleton-name assumption with metadata-driven LuaEnv resolver.
- BUILD PASS.
- Runtime: manager -> returned Script object = PARTIAL PASS; then old DoString lookup failed.

## [v1.1.9-test] - 2026-08-16
- Moved observer from empty UIRoot representation to intended Lua runtime data path.
- BUILD PASS.
- Runtime failed at `LuaSystemManager instance unresolved` before returned script object/DoString/Selections/action.

## [v1.1.8-test] - 2026-08-16
- Added all-descendant UIRoot scan, live Tag gate and removed WaitTreatment NPC reopen.
- BUILD PASS.
- Runtime: route/open PASS; anti-reopen PASS for tested transaction; UIRoot representation FAIL with `clickable=0/texts=0` despite GameDialog presence.

## [v1.1.7-test] - 2026-08-16
- Observer redesign; final CI failed and source retained gaps.

## [v1.1.6-test] - 2026-08-16
- Added mandatory project-memory files/startup pointer and CTS/MainThread proof experiment. CI/BUILD PASS.

## [v1.1.5-test] - 2026-08-16
- Alternate NPC 463 / Lạc Dương reproduced overall symptom.

## [v1.1.4-test] - 2026-08-16
- Tried live Tag/selectionID + exact GameDialog request; shared button discovery still gated reachability.

## [v1.1.3-test] - 2026-08-16
- Tried `GameDialog.FunctionButtonClicked(liveButton)` through ExecuteUIObject; shared discovery remained.

## [v1.1.2-test] - 2026-08-16
- Changed UI-root discovery order; route/NPC open partial PASS, Treatment failed.

## [v1.1.1-test] - 2026-08-16
- Removed inferred X/Y; added runtime coordinate capture.

## [v1.1.0-test] - 2026-08-16
- Initial Auto Heal route/state-machine test.

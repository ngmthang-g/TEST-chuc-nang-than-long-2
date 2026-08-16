# CHANGELOG

## [v1.1.12-test] - 2026-08-16

### Trigger / runtime evidence
- User tested delivered v1.1.11 on Map 5 / NPC 339.
- Repeated V121 diagnostic reports `actual=MoonSharp.Interpreter.Script` and `declared=MoonSharp.Interpreter.Script`.
- Runtime enumerates `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`.
- v1.1.11 reached method enumeration but rejected the real method because its scorer expected parameter #2 to be `System.String` rather than `MoonSharp.Interpreter.Table`.
- Lua probe execution / current Selections / live Treatment ID / action / server follow-up remain NOT REACHED / UNKNOWN.

### Correction of prior assumption
- Current returned runtime type is **MoonSharp.Interpreter.Script**, not XLua.LuaEnv.
- Earlier xLua research remains failed-investigation history, not current engine identity.
- Official MoonSharp source matches runtime metadata: `Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null)` returns `DynValue`; `DynValue.String` exposes string result.

### Added / changed
- Added `src/bridge_lua_moonsharp_v1_1_12.inc`:
  - retains runtime-proven `ResolveLuaEnvV120()`;
  - requires exact MoonSharp `DoString(String,Table,String)->DynValue` metadata shape;
  - invokes `{code, null Table, friendlyName}`;
  - captures managed exception diagnostics;
  - extracts probe text through `DynValue.get_String()`;
  - emits `LUA_MOONSHARP_V122` failures and `LUA_DIALOG_V122` success diagnostics.
- Added `src/bridge_action_v1_1_12.inc` using current V122 IDs and `ACTION_V122` markers.
- Preserved v1.1.11 implementation under legacy names for lineage.
- Protocol/title/startup log/artifact naming bumped to v1.1.12.
- Self-contained 9-file artifact handoff contract remains mandatory.

### Build / CI
- Initial source commit `ad2f0403863251330b1aca80eb1eba9681b58c9a` -> run `31937607280`: **CI FAILED** before compilation. Architecture audit incorrectly required literal `MoonSharp.Interpreter.Script` inside the resolver source; artifact upload skipped.
- Correction commit `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1` removes only that false-positive audit token.
- Run `31937703988`: **CI/BUILD PASS** — architecture audit, Route FSM, Heal FSM, bridge DLL/PE, controller EXE, knowledge packaging and artifact upload all PASS.
- Artifact `ThanLongTestAutoHeal-v1.1.12`, ID `9261162703`.
- ZIP digest `sha256:d25f999934be62152cf02fd0251f533743f3e9eaa0d4178f74c2fa1007d85c19`.
- Verified 9 artifact files.
- EXE SHA-256 `32a3977469a26f5807be192a620630fba9ea3242e265706522fd7d93bb27c823`.
- DLL SHA-256 `553c1d245c06c614561b9c2c1e6368daf30cb671b46602aec60b1938f326a037`.

### Runtime
- v1.1.12: **RUNTIME UNTESTED**.
- BUG-001 remains OPEN.

### Next evidence
`one NPC open -> LUA_MOONSHARP_V122 failure OR LUA_DIALOG_V122 T/C/K -> if T>0 then MAINTHREAD_PROOF/ACTION_V122 -> fresh GameDialog/MessageBox/HP/money result`.

## [v1.1.11-test] - 2026-08-16
- Added live DoString class/signature diagnostics and mandatory self-contained AI handoff packaging.
- Source/final CI builds PASS.
- Runtime FAIL at V121 accepted-shape resolution but produced decisive finding: actual/declared object `MoonSharp.Interpreter.Script`; live method `DoString(String,Table,String)->DynValue`; correct method was enumerated but rejected before invocation.
- Superseded by v1.1.12 exact MoonSharp path.

## [v1.1.10-test] - 2026-08-16
- Replaced manager singleton-name assumption with metadata-driven returned-object resolver.
- BUILD PASS.
- Runtime: manager -> returned Script object = PARTIAL PASS; then old DoString lookup failed.

## [v1.1.9-test] - 2026-08-16
- Moved observer from empty UIRoot representation toward runtime script data.
- BUILD PASS.
- Runtime failed at `LuaSystemManager instance unresolved` before returned object/DoString/Selections/action.

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

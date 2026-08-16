# CHANGELOG

## [v1.1.13-test] - 2026-08-16

### Trigger / runtime evidence
- User tested delivered v1.1.12 final-head on Map 5 / NPC 339.
- After one semantic `ClickNPC npcID=339`, V122 repeatedly returned:
  `LUA_DIALOG_V122 ... GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}`.
- This is the first runtime proof that the exact MoonSharp invocation path actually executes the probe and returns its String result.

### Runtime classification
Confirmed PASS for this transaction:
- `ResolveLuaEnvV120()` -> current MoonSharp Script object;
- exact `Script.DoString(String,Table,String)` invocation;
- `DynValue.String` extraction;
- GameDialog Lua object presence (`GD=table` + bridge UI presence);
- AutoFight_Main Lua object presence (`AF=table`).

Not reached / unresolved:
- current Treatment selectionID;
- mutable GameDialog request;
- follow-up MessageBox/GameDialog;
- HP/money completion proof.

### Important correction
- V122 field `N` is the variable `nodes`, incremented per traversed table.
- `N=4` means four table nodes scanned, **not four selections**.
- Previous conversational interpretation that N=4 meant four choices is corrected and must not be propagated.

### Source analysis / root-cause hypothesis
- V122 uses `rawget(t,"Selections")` plus rawget for priority semantic fields.
- `rawget` bypasses Lua `__index` / metatable lookup.
- Canonical client knowledge still verifies `GameDialogData.Selections[selectionID]=visibleText` and built-in automation uses current Selections.
- Therefore the next targeted hypothesis is a representation/access mismatch in the read-only observer, not a MoonSharp invocation failure and not packet/server failure.
- Hypothesis status before V123 runtime: **LIKELY, not CONFIRMED**.

### Added / changed
- Added `src/bridge_lua_dialog_v1_1_13.inc`:
  - reuses runtime-proven `RunLuaChunkV122()`;
  - replaces raw-only semantic access with `safeget(t,key)` using normal indexing in `pcall`;
  - checks `Selections`, `GameDialogData`, `CurrentGameDialogData`, `DialogData` and case variants;
  - boundedly traverses table children;
  - boundedly inspects metatable + table-valued `__index`;
  - checks canonical global GameDialogData names if exposed;
  - emits `NODE`, `ST`, `SV`, `MT`, `KS`, `S` diagnostics;
  - marker `LUA_DIALOG_V123`.
- Added `src/bridge_action_v1_1_13.inc`:
  - re-reads V123 immediately before action;
  - requires current positive semantic ID;
  - submits canonical `CMD_SHOW_GAMEDIALOG=100007` payload `<currentID>:-1` only after current match;
  - marker `ACTION_V123`.
- Preserved V122 exact MoonSharp implementation under legacy `InspectHealDialog` / action names and reuse its execution primitive.
- Protocol/title/startup log/artifact naming bumped to v1.1.13.
- Artifact handoff contract remains exactly 9 files.

### Build / CI
- v1.1.13 source-bearing CI: **PENDING at source commit creation**.
- Runtime: **UNTESTED**.
- BUG-001 remains OPEN.

### Next evidence
`one NPC open -> LUA_DIALOG_V123`:
- `T>0` -> inspect `ACTION_V123` and fresh result;
- `T=0, ST>0, SV>0` -> inspect `S=` only;
- `T=0, ST=0` -> inspect `MT`/`KS` only;
- no packet/server conclusion before a live ID is sent.

## [v1.1.12-test] - 2026-08-16
- Triggered by v1.1.11 runtime proving current returned object is `MoonSharp.Interpreter.Script` and exact method is `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`.
- Added exact MoonSharp invocation + `DynValue.String` extraction and V122 action.
- Initial source commit `ad2f0403863251330b1aca80eb1eba9681b58c9a`, run `31937607280`: CI failed before compiler due architecture-audit false-positive; no artifact.
- Correction commit `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1`, run `31937703988`: CI/BUILD PASS; artifact ID `9261162703`; source ZIP SHA `d25f999934be62152cf02fd0251f533743f3e9eaa0d4178f74c2fa1007d85c19`; EXE SHA `32a3977469a26f5807be192a620630fba9ea3242e265706522fd7d93bb27c823`; DLL SHA `553c1d245c06c614561b9c2c1e6368daf30cb671b46602aec60b1938f326a037`.
- Final HEAD `03b33595e122217baa4c006a0fe8998af3395d44`, run `31938051044`: CI/BUILD PASS; final artifact ID `9261253049`.
- Runtime update: exact MoonSharp execution + DynValue.String PASS; V122 rawget-only observer returns T/C/K=0; N=4 is table-node count, not selection count. Superseded by v1.1.13 observer experiment.

## [v1.1.11-test] - 2026-08-16
- Added live DoString class/signature diagnostics and mandatory self-contained AI handoff packaging.
- Build PASS.
- Runtime identified MoonSharp exact method but V121 shape scorer rejected it before invocation.

## [v1.1.10-test] - 2026-08-16
- Replaced manager singleton-name assumption with metadata-driven returned-object resolver.
- Build PASS; runtime progressed to returned Script object, then old DoString lookup failed.

## [v1.1.9-test] - 2026-08-16
- Moved observer away from empty UIRoot representation toward runtime script data.
- Build PASS; runtime failed at `LuaSystemManager instance unresolved`.

## [v1.1.8-test] - 2026-08-16
- Added all-descendant UIRoot scan, live Tag gate and removed WaitTreatment NPC reopen.
- Build PASS; runtime route/open + anti-reopen PASS, UIRoot representation FAIL.

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

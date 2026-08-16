# CHANGELOG

## [v1.1.13-test] - 2026-08-16

### Trigger / runtime evidence
Delivered v1.1.12 repeatedly returned after one NPC open:
`LUA_DIALOG_V122 ... GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}`.

Confirmed runtime PASS: current MoonSharp Script resolution, exact `DoString(String,Table,String)` invocation, `DynValue.String` extraction, GameDialog/AutoFight_Main Lua-table presence. Current Treatment ID/action/server follow-up remain not reached.

### Important correction
V122 `N` is the `nodes` table-traversal counter, not a selection count. `N=4` does not mean four choices.

### Investigation
V122 reuses a probe that reads `Selections`/priority fields via `rawget`, bypassing `__index`/metatable lookup. Canonical client knowledge still verifies `GameDialogData.Selections[selectionID]=visibleText`. Current representation/access mismatch is LIKELY, not confirmed before V123 runtime.

### Added / changed
- `src/bridge_lua_dialog_v1_1_13.inc`: reuse proven V122 MoonSharp execution; normal indexing under `pcall`; bounded child + metatable/table-`__index` scan; semantic field variants; diagnostics `NODE/ST/SV/MT/KS/S`; marker `LUA_DIALOG_V123`.
- `src/bridge_action_v1_1_13.inc`: re-read current V123 ID; fail closed on T=0; canonical `<currentID>:-1`; marker `ACTION_V123`; MessageBox callback preserved.
- V122 exact MoonSharp implementation preserved under legacy observer/action names and reused as the execution primitive.
- Protocol/title/log/artifact bumped to v1.1.13.
- 9-file self-contained artifact contract preserved.
- Knowledge/history corrects the earlier conversational misread of `N=4`.

### Build / CI
Source-bearing commit `79235f61ded9d393694be807c996128b082f67b4`.  
Run `31939000139`: **CI/BUILD PASS**.
- architecture audit PASS;
- Route FSM PASS;
- Heal FSM PASS;
- bridge DLL compile + PE verification PASS;
- controller EXE compile PASS;
- knowledge packaging PASS;
- artifact upload PASS.

Artifact `ThanLongTestAutoHeal-v1.1.13`, ID `9261527342`.  
ZIP SHA-256 `1cb886860b998b955efe68164bf5379642a2c2bde739acb425c7a9a650ecee5f`.  
EXE SHA-256 `764fc10e98afd0fcd608bd9dd50c9fa86e51b467524cab4aebbe5001646d037a`.  
DLL SHA-256 `8d8b9e3feeecf0be509610ad990bbe3852761fb750d1862aee09d5f51eadb7f7`.  
Exactly 9 required files verified. `BUILD_EVIDENCE.txt` records source HEAD `79235f61...`, run `31939000139`, BUILD PASS and `RUNTIME=UNTESTED_FOR_V1.1.13`.

### Runtime
v1.1.13: **RUNTIME UNTESTED**. BUG-001 OPEN. Full known-good NONE.

### Next evidence
- `LUA_DIALOG_V123 T>0` -> evaluate `ACTION_V123` and fresh follow-up;
- `T=0,ST>0,SV>0` -> inspect `S` only;
- `T=0,ST=0` -> inspect `MT`/`KS`;
- no packet/server conclusion before a live ID is sent.

## [v1.1.12-test] - 2026-08-16
- Exact MoonSharp `DoString(String,Table,String)->DynValue` + `DynValue.String` implementation.
- Initial run `31937607280` failed before compiler due audit false-positive; corrected source run `31937703988` PASS; final HEAD `03b33595e122217baa4c006a0fe8998af3395d44`, run `31938051044` PASS.
- Runtime update: MoonSharp execution/result PASS; V122 rawget-only observer T/C/K=0. Superseded by V123 observer experiment.

## [v1.1.11-test] - 2026-08-16
- Added live DoString metadata diagnostics and self-contained handoff artifact.
- Runtime identified MoonSharp exact method but V121 shape scorer rejected it before invoke.

## [v1.1.10-test] - 2026-08-16
- Metadata-driven returned-object resolver progressed beyond manager singleton failure.

## [v1.1.9-test] - 2026-08-16
- Runtime failed at LuaSystemManager instance resolution.

## [v1.1.8-test] - 2026-08-16
- Route/open + tested anti-reopen PASS; UIRoot representation FAIL.

## [v1.1.7-test] - 2026-08-16
- Final CI failed; source retained observer/retry gaps.

## [v1.1.6-test] - 2026-08-16
- CTS/MainThread proof + mandatory startup knowledge; CI/BUILD PASS.

## [v1.1.5-test] - 2026-08-16
- Alternate NPC/map reproduced symptom.

## [v1.1.4-test] - 2026-08-16
- live Tag/exact packet experiment remained gated by shared discovery.

## [v1.1.3-test] - 2026-08-16
- GameDialog callback experiment remained discovery-gated.

## [v1.1.2-test] - 2026-08-16
- UI-root discovery change; full Treatment failed.

## [v1.1.1-test] - 2026-08-16
- Runtime NPC coordinate capture.

## [v1.1.0-test] - 2026-08-16
- Initial Auto Heal runtime lab.

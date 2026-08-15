# CHANGELOG

## v1.1.3-test

### Changed
- Replaced Treatment/Ack `UIButton.HandleClickEvent()` with exact Lua `GameDialog.FunctionButtonClicked(liveButton)`.
- Confirmation now calls exact Lua `MessageBox.ButtonOKClicked()` when MessageBox is present.
- Added managed `System.Object[]` bridge for `MonoBehaviourExecutor.ExecuteUIObject`.
- Kept GameDialog-confirm fallback through the same exact Lua callback.

### Build Result
- Architecture audit PASS.
- Route FSM 8/8 PASS.
- Heal FSM 7/7 PASS.
- Windows EXE + bridge DLL PASS.

### Runtime Status
- NEEDS USER TEST.

## v1.1.2-test

### Fixed
- Changed visible GameDialog discovery order to `FindUI -> MainFindUI -> executor fallback`.

### Runtime Result
- PARTIAL PASS: route + `ClickNPC(339)` opens correct NPC.
- FAIL: Treatment choice still does not advance.
- New observation: dialog/button visually flickers during attempted Treatment.

### Known Issues
- `UIButton.HandleClickEvent()` is not confirmed to reach the required Lua/server business action in this flow.

## v1.1.1-test

### Changed
- Removed hardcoded/inferred NPC X/Y.
- Added `TỰ LẤY TỌA ĐỘ NPC` and persistence in `ThanLongAutoHeal.target.tsv`.

## v1.1.0-test

### Added
- Auto-heal route/state-machine test.
- NPC/ResID 339 for Đỗ Thanh Đằng.
- GameDialog / MessageBox inspection.

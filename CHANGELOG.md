# CHANGELOG

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

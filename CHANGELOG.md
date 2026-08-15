# CHANGELOG

## v1.1.4-test

### Changed
- `Trị liệu` và `Ta biết rồi` không còn đi qua UIButton click hoặc `ExecuteUIObject` GameDialog callback.
- Tool tìm đúng button sống theo text, đọc `UIButton.Tag` để lấy runtime `selectionID` do server cấp.
- Gửi semantic request thật qua `LuaSystemAPI_Network.SendPacket`:
  - packet `CMD_SHOW_GAMEDIALOG = 100007`
  - payload `selectionID:selectedItemID`
  - với lựa chọn thường dùng `selectedItemID = -1` theo Lua source.
- Nếu `Xác nhận` là một dynamic GameDialog thì dùng cùng cơ chế packet; nếu là MessageBox thì giữ `ButtonOKClicked()` semantic callback.

### Why
- v1.1.3 vẫn chỉ làm UI nháy và không tiến qua `Trị liệu`.
- Canonical client KB xác nhận GameDialog button chỉ là presentation; action semantic là selectionID + CMD_SHOW_GAMEDIALOG.

### Build Result
- GitHub Actions run `31906609147`: PASS.
- Architecture audit PASS.
- Route FSM 8/8 PASS.
- Heal FSM 7/7 PASS.
- Windows bridge DLL + controller EXE PASS.
- Artifact `ThanLongTestAutoHeal-v1.1.4` created successfully.

### Runtime Status
- NEEDS USER TEST.

## v1.1.3-test

### Changed
- Replaced Treatment/Ack `UIButton.HandleClickEvent()` with exact Lua `GameDialog.FunctionButtonClicked(liveButton)`.
- Confirmation calls `MessageBox.ButtonOKClicked()` when MessageBox is present.
- Added managed `System.Object[]` bridge for `MonoBehaviourExecutor.ExecuteUIObject`.

### Build Result
- Architecture audit PASS.
- Route FSM 8/8 PASS.
- Heal FSM 7/7 PASS.
- Windows EXE + bridge DLL PASS.

### Runtime Result
- FAIL at Treatment: user reports same visual flicker as older versions and no transition to confirmation.

## v1.1.2-test

### Fixed
- Changed visible GameDialog discovery order to `FindUI -> MainFindUI -> executor fallback`.

### Runtime Result
- PARTIAL PASS: route + `ClickNPC(339)` opens correct NPC.
- FAIL: Treatment choice still does not advance.
- Dialog/button visually flickers during attempted Treatment.

## v1.1.1-test

### Changed
- Removed hardcoded/inferred NPC X/Y.
- Added `TỰ LẤY TỌA ĐỘ NPC` and persistence in `ThanLongAutoHeal.target.tsv`.

## v1.1.0-test

### Added
- Auto-heal route/state-machine test.
- NPC/ResID 339 for Đỗ Thanh Đằng.
- GameDialog / MessageBox inspection.

# CHANGELOG

## v1.1.5-test

### Changed
- Active Auto trị liệu test NPC switched from Đỗ Thanh Đằng `339` / Lâu Lan to Long Phá Thiên `463` / Lạc Dương.
- Exact static source: canonical `database/npcs/NPCS_0401_0600.csv`.
- Existing v1.1.4 semantic GameDialog packet path is intentionally unchanged so this is a one-variable NPC/map isolation test.
- Previous saved heal target is not auto-loaded in this version; user must press `TỰ LẤY TỌA ĐỘ NPC` again beside Long Phá Thiên.
- No NPC X/Y is hardcoded.

### Confidence
- CONFIRMED STATIC: NPC 463 = Long Phá Thiên, ResName `PuTongXiaShi2`, MapID 3 = Lạc Dương.
- HYPOTHESIS: Long Phá Thiên exposes the desired `Trị liệu` service; canonical healer-family database does not currently classify 463 as LangZhong/MingYi.

### Build Result
- GitHub Actions run `31907554441`: PASS.
- Build/self-test job: PASS.
- Artifact upload: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.5`.
- Artifact SHA256: `d81461b081dddb03941a5e93c3d4b1c01dd9f238fc12a97bc00993b92dbbf134`.

### Runtime Status
- NEEDS USER TEST.

## v1.1.4-test

### Changed
- `Trị liệu` và `Ta biết rồi` không còn đi qua UIButton click hoặc `ExecuteUIObject` GameDialog callback.
- Tool tìm đúng button sống theo text, đọc `UIButton.Tag` để lấy runtime `selectionID` do server cấp.
- Gửi semantic request thật qua `LuaSystemAPI_Network.SendPacket`: packet `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:selectedItemID`, default no-award `selectedItemID = -1` theo Lua source.

### Build Result
- GitHub Actions run `31906609147`: PASS.
- Route FSM 8/8 PASS; Heal FSM 7/7 PASS; Windows bridge DLL + controller EXE PASS.

### Runtime Result
- FAIL on Đỗ Thanh Đằng `339` / Lâu Lan: user reports the same visible dialog/screen flicker and no transition through `Trị liệu`.
- Failure cause remains unproven; this result motivates v1.1.5 NPC/map isolation rather than another action-layer rewrite.

## v1.1.3-test

### Changed
- Replaced Treatment/Ack `UIButton.HandleClickEvent()` with exact Lua `GameDialog.FunctionButtonClicked(liveButton)`.
- Confirmation calls `MessageBox.ButtonOKClicked()` when MessageBox is present.
- Added managed `System.Object[]` bridge for `MonoBehaviourExecutor.ExecuteUIObject`.

### Build Result
- Architecture audit PASS; Route FSM 8/8 PASS; Heal FSM 7/7 PASS; Windows EXE + bridge DLL PASS.

### Runtime Result
- FAIL at Treatment: same visual flicker and no transition to confirmation.

## v1.1.2-test

### Fixed
- Changed visible GameDialog discovery order to `FindUI -> MainFindUI -> executor fallback`.

### Runtime Result
- PARTIAL PASS: route + `ClickNPC(339)` opens correct NPC.
- FAIL: Treatment choice still does not advance; dialog/button visibly flickers.

## v1.1.1-test

### Changed
- Removed hardcoded/inferred NPC X/Y.
- Added `TỰ LẤY TỌA ĐỘ NPC` and persistence in `ThanLongAutoHeal.target.tsv`.

## v1.1.0-test

### Added
- Auto-heal route/state-machine test.
- NPC/ResID 339 for Đỗ Thanh Đằng.
- GameDialog / MessageBox inspection.

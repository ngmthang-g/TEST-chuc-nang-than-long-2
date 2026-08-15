# CHANGELOG

## v1.1.5-test

### Changed
- Added Long Phá Thiên `463` / Lạc Dương `MapID 3` as a second healer-NPC runtime test target.
- Existing Đỗ Thanh Đằng `339` / Lâu Lan `MapID 5` remains available.
- NPC is selected from the **captured target map**: Map 3 -> 463, Map 5 -> 339.
- Coordinates remain raw runtime values captured by `TỰ LẤY TỌA ĐỘ NPC`; no NPC X/Y is hardcoded.
- Removed an intermediate macro-override implementation from `controller.cpp`; mapping now lives cleanly in the heal controller logic.
- v1.1.4 semantic GameDialog packet action is intentionally unchanged for A/B isolation.

### Confidence
- CONFIRMED STATIC: NPC 463 = Long Phá Thiên, `ResName=PuTongXiaShi2`, MapID 3 = Lạc Dương.
- CONFIRMED STATIC: NPC 339 = Đỗ Thanh Đằng, MapID 5 = Lâu Lan.
- HYPOTHESIS: NPC 463 exposes the same `Trị liệu` service. Canonical healer-family database does not classify 463 as LangZhong/MingYi, so runtime must prove it.

### Build Result
- Clean source GitHub Actions run `31909204317`: PASS.
- Architecture audit, Route FSM, Heal FSM, bridge DLL, PE verification, controller EXE and artifact upload: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.5`.
- SHA256: `652a12e2f454d1d8bee6d5025512825d98262e38a567d69b6597b1143daedf93`.

### Runtime Status
- v1.1.4 on NPC 339/Lâu Lan: FAIL at Treatment; same screen/dialog flicker.
- v1.1.5: NEEDS USER TEST on NPC 463/Lạc Dương.

## v1.1.4-test

### Changed
- `Trị liệu` and `Ta biết rồi` use live `UIButton.Tag` to obtain runtime `selectionID`.
- Sends source-verified `CMD_SHOW_GAMEDIALOG = 100007` payload `selectionID:selectedItemID`; default no-award `selectedItemID=-1`.

### Build Result
- GitHub Actions run `31906609147`: PASS.
- Route FSM 8/8 PASS; Heal FSM 7/7 PASS; Windows bridge DLL + controller EXE PASS.

### Runtime Result
- FAIL on Đỗ Thanh Đằng `339` / Lâu Lan: same visible flicker and no Treatment transition.

## v1.1.3-test

### Changed
- Replaced Treatment/Ack `UIButton.HandleClickEvent()` with `GameDialog.FunctionButtonClicked(liveButton)` via `ExecuteUIObject`.
- Confirmation calls `MessageBox.ButtonOKClicked()` when MessageBox is present.

### Runtime Result
- FAIL at Treatment: same visual flicker and no transition.

## v1.1.2-test

### Fixed
- Changed visible GameDialog discovery order to `FindUI -> MainFindUI -> executor fallback`.

### Runtime Result
- PARTIAL PASS: route + `ClickNPC(339)` opens correct NPC.
- FAIL: Treatment choice does not advance.

## v1.1.1-test

### Changed
- Removed hardcoded/inferred NPC X/Y.
- Added `TỰ LẤY TỌA ĐỘ NPC` and persistence in `ThanLongAutoHeal.target.tsv`.

## v1.1.0-test

### Added
- Auto-heal route/state-machine test.
- NPC/ResID 339 for Đỗ Thanh Đằng.
- GameDialog / MessageBox inspection.

# v1.1.3-test — Auto trị liệu Lua callback

## User runtime evidence
- Route + `ClickNPC(339)` mở đúng NPC.
- v1.1.2 vẫn không tiến qua `Trị liệu`.
- Người test quan sát bảng/nút trị liệu có nháy khi tool thao tác.

## Root-cause direction
`UIButton.HandleClickEvent()` có thể tạo phản ứng UI nhưng không được coi là bằng chứng đã chạy business callback Lua/server.

## Changed
- `Trị liệu`: resolve button sống rồi gọi `GameDialog.FunctionButtonClicked(button)`.
- `Xác nhận`: ưu tiên gọi `MessageBox.ButtonOKClicked()`; fallback GameDialog confirm cũng dùng `GameDialog.FunctionButtonClicked(button)`.
- `Ta biết rồi`: resolve button sống mới rồi gọi `GameDialog.FunctionButtonClicked(button)`.
- Không hardcode selectionID/packet payload.
- Không giữ button pointer qua transition.
- Không thêm `Sleep` trên game thread.

## Timing note
Controller đang poll 500 ms. Vì vậy giả thuyết “quá nhanh” không được coi là root cause đã xác nhận. v1.1.3 thay action layer bằng callback Lua thật thay vì chỉ tăng delay.

## Build Test
GitHub Actions run `31905470360`:
- Architecture audit: PASS.
- Route FSM: 8/8 PASS.
- Heal FSM: 7/7 PASS.
- Bridge DLL compile + PE verification: PASS.
- Controller EXE compile: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.3`.

## Status
- BUILD: PASS.
- RUNTIME: NEEDS USER TEST.

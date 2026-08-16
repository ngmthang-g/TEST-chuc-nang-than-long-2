# HANDOFF v1.1.3-test — Auto trị liệu bằng callback Lua thật

## Trạng thái kế thừa
- Route tới tọa độ NPC tự lấy: runtime đã đủ để mở đúng NPC.
- `ClickNPC(339)`: runtime CONFIRMED mở đúng GameDialog Đỗ Thanh Đằng.
- v1.1.0–v1.1.2 `UIButton.HandleClickEvent()` ở lựa chọn Trị liệu: FAILED runtime; không port lại cơ chế đó.

## Root cause direction
User thấy UI trị liệu nháy nhưng flow không tiến. Đây là bằng chứng rằng phản ứng visual của UIButton không đồng nghĩa business callback Lua/server đã chạy.

Asset/historical implementation xác nhận:
- `GameDialog:FunctionButtonClicked(uiButton)` đọc `uiButton.Tag`, gửi `CMD_SHOW_GAMEDIALOG`, rồi destroy GameDialog.
- `MessageBox:ButtonOKClicked()` là callback xác nhận.
- AutoTrain v0.8.9 từng dùng chính hai callback trên cho chuỗi trị liệu.

## v1.1.3 implementation

### Treatment
`FindUI/MainFindUI(GameDialog) -> resolve live button Text="Trị liệu" -> MonoBehaviourExecutor.ExecuteUIObject(GameDialog, "FunctionButtonClicked", object[]{liveButton})`

### Confirm
Ưu tiên:
`FindUI/MainFindUI(MessageBox) -> ExecuteUIObject(MessageBox, "ButtonOKClicked", object[]{})`

Fallback nếu server dùng GameDialog confirm:
`resolve live "Xác nhận" button -> GameDialog.FunctionButtonClicked(button)`.

### Ack
`resolve live GameDialog button Text="Ta biết rồi" -> GameDialog.FunctionButtonClicked(button)`.

## Managed Object[] bridge
`src/bridge_lua_array.inc` tạo `System.Object[]` trong chính process game qua `il2cpp_get_corlib + il2cpp_array_new`; với một argument nó đặt live UI object vào array và dùng write barrier khi export có sẵn.

Không dùng `CreateRemoteThread`, `WriteProcessMemory`, remote worker hay click màn hình.

## Timing
Controller poll 500 ms. v1.1.3 không chèn `Sleep` trên callback/game thread vì việc khóa game thread để “đợi UI” là sai kiến trúc. Nếu cần stability gate sau runtime test, phải làm ở controller/state machine qua nhiều tick, không Sleep trong bridge action.

## Safety
- resolve lại button sống ở từng dialog;
- không cache pointer qua transition;
- không hardcode selectionID;
- không hardcode packet payload;
- không hardcode X/Y;
- action vẫn chạy ở target window callback thread;
- fail closed khi UI/action không resolve.

## Files
- `src/bridge_lua_array.inc`
- `src/bridge.cpp`
- `src/bridge_part04.inc`
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
- `VERSION_v1.1.3.md`

## Test status
BUILD và FSM phải đọc từ GitHub Actions của đúng commit v1.1.3.
Runtime: NEEDS USER TEST cho đến khi log đi đủ:
`ClickNPC(339) -> GameDialog.FunctionButtonClicked(Trị liệu) -> MessageBox.ButtonOKClicked -> GameDialog.FunctionButtonClicked(Ta biết rồi)`.

## Port rule
Chỉ port sang tool chính sau runtime PASS nhiều vòng. Không port cơ chế `HandleClickEvent()` của handoff v1.1.0 cho business choices; phần đó đã bị runtime evidence thay thế.

# Runtime finding v1.1.2 — NPC mở được nhưng không chọn Trị liệu

## Quan sát runtime

Người test xác nhận:

- AutoPath tới đúng tọa độ tự lấy.
- `LuaSystemAPI_Game.ClickNPC(339)` mở đúng dialog Đỗ Thanh Đằng.
- Trên màn hình có button `Trị liệu` và `Ta chỉ đi ngang qua`.
- Tool vẫn đứng ở `AUTO TRỊ LIỆU: CHỜ DIALOG TRỊ LIỆU` và sau timeout gọi lại `ClickNPC(339)`.

Kết luận: route và NPC/ResID đã PASS runtime. Lỗi nằm ở resolver của Lua UI, trước bước click button.

## Root cause trong v1.1.0

`FindUiRootViaPublicApi()` làm:

1. resolve method `MainFindUI`;
2. chỉ khi method đó KHÔNG tồn tại mới resolve `FindUI`;
3. nếu `MainFindUI` tồn tại nhưng trả `null`, hàm kết thúc với `root=null` và không thử `FindUI`.

Đây là lỗi điều kiện fallback. Nó giải thích đúng hiện tượng: dialog thật đang hiển thị nhưng `InspectHealDialog` trả mask không có `HealDialogTreatment`.

Tài liệu phân tích client và code test cũ cũng cho thấy registry Lua UI có cả `LuaSystemAPI_GUI.FindUI(uiName)` và `MainFindUI(uiName)`; code từng hoạt động ưu tiên `FindUI` rồi mới fallback `MainFindUI`.

## Sửa ở v1.1.2

File mới:

- `src/bridge_ui_root_fix_v1_1_2.inc`

`src/bridge.cpp` giữ implementation cũ dưới tên audit (`FindUiRoot_v110`) rồi thay resolver dùng cho phần heal bằng flow:

`FindUI -> MainFindUI -> MonoBehaviourExecutor.FindScriptUIRoots`

Không thay:

- NPC ID 339;
- tọa độ runtime đã tự lấy;
- route FSM;
- cooldown/retry ClickNPC;
- Heal FSM;
- cơ chế fail-closed.

Mục tiêu của bản này là sửa đúng một biến số để test runtime rõ ràng. Nếu v1.1.2 nhìn thấy `Trị liệu` nhưng action click vẫn không chuyển sang MessageBox, bước kế tiếp sẽ thay riêng final dispatch bằng Lua action đã bóc từ `Interface.unity3d`: `GameDialog.FunctionButtonClicked(uiButton)`, thay vì sửa route hoặc quay lại click tọa độ màn hình.

## Kỳ vọng log khi test

Sau khi NPC mở, trạng thái phải rời `CHỜ DIALOG TRỊ LIỆU` và chuyển sang `ĐANG CHỌN TRỊ LIỆU` / `CHỜ XÁC NHẬN`.

Nếu vẫn đứng ở `CHỜ DIALOG TRỊ LIỆU`, không thay NPC/tọa độ. Khi đó cần log/probe thêm root của `GameDialog` vì lỗi vẫn nằm ở UI discovery.

# Runtime finding v1.1.2 — NPC mở được nhưng không chọn Trị liệu

## Quan sát runtime

Người test xác nhận:

- AutoPath tới đúng tọa độ tự lấy.
- `LuaSystemAPI_Game.ClickNPC(339)` mở đúng dialog Đỗ Thanh Đằng.
- Trên màn hình có button `Trị liệu` và `Ta chỉ đi ngang qua`.
- Tool v1.1.0 vẫn đứng ở `AUTO TRỊ LIỆU: CHỜ DIALOG TRỊ LIỆU` và sau timeout gọi lại `ClickNPC(339)`.

Kết luận: route và NPC/ResID đã PASS runtime. Lỗi nằm ở resolver của Lua UI, trước bước click button.

## Root cause trong v1.1.0

`FindUiRootViaPublicApi()` làm:

1. resolve method `MainFindUI`;
2. chỉ khi method đó KHÔNG tồn tại mới resolve `FindUI`;
3. nếu `MainFindUI` tồn tại nhưng trả `null`, hàm kết thúc với `root=null` và không thử `FindUI`.

Đây là lỗi điều kiện fallback. Nó giải thích đúng hiện tượng: dialog thật đang hiển thị nhưng `InspectHealDialog` trả mask không có `HealDialogTreatment`.

Tài liệu phân tích client và code test cũ cũng cho thấy registry Lua UI có cả `LuaSystemAPI_GUI.FindUI(uiName)` và `MainFindUI(uiName)`; code từng hoạt động ưu tiên `FindUI` rồi mới fallback `MainFindUI`.

## Sửa ở v1.1.2

Resolver được sửa trực tiếp trong `src/bridge_part03.inc`, không chèn hotfix giữa các file `.inc`.

Flow mới:

`FindUI -> MainFindUI -> MonoBehaviourExecutor.FindScriptUIRoots`

Không thay:

- NPC ID 339;
- tọa độ runtime đã tự lấy;
- route FSM;
- cooldown/retry ClickNPC;
- Heal FSM;
- cơ chế fail-closed.

Mục tiêu của bản này là sửa đúng một biến số để test runtime rõ ràng. Nếu v1.1.2 nhìn thấy `Trị liệu` nhưng action click vẫn không chuyển sang MessageBox, bước kế tiếp sẽ thay riêng final dispatch bằng Lua action đã bóc từ `Interface.unity3d`: `GameDialog.FunctionButtonClicked(uiButton)`, thay vì sửa route hoặc quay lại click tọa độ màn hình.

## CI

Commit build được: `be03f263b883db630074b45dcf8c5de494b04290`.

GitHub Actions:

- Architecture audit: PASS.
- Route FSM: 8/8 PASS.
- Heal FSM: 7/7 PASS.
- Bridge DLL: build PASS.
- Controller v1.1.2: build PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.2`.

CI PASS không đồng nghĩa runtime trị liệu PASS; vẫn cần test client thật.

## Kỳ vọng log khi test

Sau khi NPC mở, trạng thái phải rời `CHỜ DIALOG TRỊ LIỆU` và chuyển sang `ĐANG CHỌN TRỊ LIỆU` / `CHỜ XÁC NHẬN`.

Nếu vẫn đứng ở `CHỜ DIALOG TRỊ LIỆU`, không thay NPC/tọa độ. Khi đó cần log/probe thêm root của `GameDialog` vì lỗi vẫn nằm ở UI discovery.

Nếu đã sang `ĐANG CHỌN TRỊ LIỆU` nhưng dialog không đổi, UI discovery đã PASS và lỗi kế tiếp nằm ở final dispatch; khi đó đổi riêng action sang `GameDialog.FunctionButtonClicked(liveButton)`.

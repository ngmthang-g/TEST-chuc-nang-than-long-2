# PROJECT KNOWLEDGE — v0.6 CURRENT

## Quy tắc dự án

- Nền phát triển trực tiếp là v0.5.0; v0.8.4 chỉ là donor nghiên cứu.
- Không thay logic ngoài phạm vi nếu không cần để tích hợp.
- Callback trả OK không phải bằng chứng nghiệp vụ; luôn verify bằng snapshot tương ứng.
- Mỗi version cập nhật README, changelog, source provenance, bảng logic và verifier.

## Kiến trúc action v0.6

Controller vẫn là scheduler/FSM. `ThanLongCleanRouteBridge.dll` vẫn được nạp bằng `WH_GETMESSAGE` vào đúng game window thread. Protocol v0.6 bổ sung: ConfirmMap, Revive, Start/StopAutoFight, Begin/Advance/SellNext/CloseBackgroundSell.

Không có `CreateRemoteThread` trong controller/Bridge. Một mapping chỉ có một request in-flight; timeout không được ghi đè.

## Ưu tiên toàn cục

1. P1 XN Lâu Lan: điều kiện route ownership/stall của v0.5 giữ nguyên; action đổi sang MessageBox callback.
2. P2 Đầu thai: giữ death lifecycle/World Flow FIFO; action đổi sang death-guarded UIButton callback.
3. P3 AUTO: Attack/Stop request đổi sang `TopIcon.AutoTrainClick/AutoStopClick`.

Ba action không dùng mouse guard vì không tạo mouse input. Mouse guard vẫn áp dụng cho chuỗi giao dịch tọa độ.

## Auto-sell

- Quyết định khi nào bán, đi NPC, quay bãi, retry hai pass và ngưỡng MAIN/CON giữ từ v0.5.
- Recorded sell macro không còn nằm trên active path.
- Bridge nhận dạng từng semantic stage; controller delay giữa các stage để không block game thread.
- Item phải thuộc cây bag/inventory và không thuộc product/shop list.
- `FreeBagSpace` tăng mới tính sold; 3 lần không tiến triển thì skip; tối đa 90 callback.

## Logic được bảo vệ

World Flow observation/recovery, route ownership, Travel Guard authoritative, mount recovery, rotation, F4/F8, trade FIFO/relock/pass/bag stabilization và trade macro giữ nguyên trừ nơi phải đổi từ AUTO physical sang AUTO nội bộ.

## Rủi ro còn cần test thật

- Mã Kiêu Minh ID 373 là donor đã chứng minh. Dược Đại Phu ID 279 có thể dùng cây shop khác.
- UI scorer phụ thuộc metadata/text của đúng client.
- Game minimized có thể tự dừng update dù action không chiếm chuột.
- Giao dịch MAIN/CON vẫn chiếm chuột vì chưa nằm trong phạm vi v0.6.

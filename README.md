# Thần Long Item Consolidator v0.6

Nền phát triển trực tiếp: source v0.5.0 do người dùng cung cấp. Source v0.8.4 chỉ là donor để nghiên cứu cơ chế callback UI nội bộ; không ghép nguyên remote worker của donor.

## Tải bản Windows x64

- [ThanLongItemConsolidator-v0.6-win-x64.zip](release/ThanLongItemConsolidator-v0.6-win-x64.zip)
- Giải nén và giữ `ThanLongItemConsolidator_v0.6.exe` cùng thư mục với `ThanLongCleanRouteBridge.dll`.
- Hash bản build và CI nguồn nằm tại [release/SHA256SUMS_v0.6.txt](release/SHA256SUMS_v0.6.txt).

Gói trên đã qua MSVC Windows x64 build và toàn bộ self-test. Trạng thái đó không thay thế test thực chiến trên đúng client game; hãy thử từng action bằng nút `TEST` trước khi bật chu trình nhiều tài khoản.

## Thay đổi chính

- `Xác nhận ra map`: vẫn giữ nguyên điều kiện watchdog Lâu Lan của v0.5, nhưng Bridge tìm nút đồng ý duy nhất bên trong `MessageBox` rồi gọi callback nội bộ. Không foreground cửa sổ, không di chuyển chuột.
- `Đầu thai`: Bridge đọc lại `IsDeath=true`, tìm đúng control `Đầu thai` và gọi `UIButton.HandleClickEvent()`.
- `AUTO → Đánh quái`: gọi trực tiếp Lua action `TopIcon.AutoTrainClick`; `Dừng AUTO` gọi `TopIcon.AutoStopClick`. Controller vẫn xác minh kết quả bằng snapshot AutoFight authoritative.
- `Tự bán đồ`: bỏ macro tọa độ khỏi active auto-sell. Chuỗi mới tiến từng action nội bộ: `ClickNPC` → shop → Bán vật phẩm → Bán nhanh → Trang bị → từng item an toàn → đóng UI. Mỗi item được xác minh bằng `GetFreeBagSpace`; lỗi 3 lần thì bỏ qua, tối đa 90 callback.
- Chuỗi giao dịch MAIN/CON vẫn dùng tọa độ và chuột thật. Đây là phần ngoài phạm vi thay đổi v0.6.

## Kiến trúc tích hợp

v0.6 mở rộng Bridge DLL `WH_GETMESSAGE` vốn có của v0.5. Mỗi request chạy trên đúng message thread của cửa sổ game; controller không được ghi đè request đang bận. Không thêm `CreateRemoteThread` hoặc worker IL2CPP thứ hai.

Các điểm click XN/Đầu thai/AUTO cũ vẫn được đọc và lưu để tương thích file cấu hình, nhưng active runtime v0.6 không dùng chúng. Nút `TEST` tương ứng gọi action nội bộ; `LEGACY F8` chỉ giữ khả năng truy vết cấu hình cũ. Macro bán cũ cũng chỉ còn ở editor/INI, không điều khiển auto-sell.

## Build và kiểm tra

Workflow Windows x64 chạy:

1. `tools/verify_v06_logic.py`.
2. Build controller + Bridge bằng MSVC.
3. Route, rotation, trade coordinator self-tests.
4. Background UI scoring self-test.

Artifact: `ThanLongItemConsolidator-v0.6-win-x64` gồm EXE, Bridge DLL và tài liệu.

Đọc [DONOR_0.8.4_BACKGROUND_ACTION_ANALYSIS.md](DONOR_0.8.4_BACKGROUND_ACTION_ANALYSIS.md) để xem diễn giải chi tiết donor 0.8.4 và quyết định chuyển đổi.

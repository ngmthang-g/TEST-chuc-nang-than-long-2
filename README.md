# Thần Long Item Consolidator v0.6.1.1

Nền phát triển trực tiếp: source v0.5.0 do người dùng cung cấp. Source v0.8.4 chỉ là donor để nghiên cứu cơ chế callback UI nội bộ; không ghép nguyên remote worker của donor.

## Tải bản Windows x64

- v0.6.1.1 là hotfix hẹp trên đúng source v0.6.1: chỉ thay bước callback item tay nải đang lỗi.
- [ThanLongItemConsolidator-v0.6.1.1-win-x64.zip](release/ThanLongItemConsolidator-v0.6.1.1-win-x64.zip)
- Hash EXE/DLL/ZIP và nguồn CI: [release/SHA256SUMS_v0.6.1.1.txt](release/SHA256SUMS_v0.6.1.1.txt).
- Bản v0.6.1 gốc vẫn nằm tại [release/ThanLongItemConsolidator-v0.6.1-win-x64.zip](release/ThanLongItemConsolidator-v0.6.1-win-x64.zip) để đối chiếu.
- v0.6 cũ được giữ tại [release/ThanLongItemConsolidator-v0.6-win-x64.zip](release/ThanLongItemConsolidator-v0.6-win-x64.zip) để bảo toàn lịch sử, nhưng **không nên dùng để test callback UI**.
- Luôn giữ EXE cùng thư mục với `ThanLongCleanRouteBridge.dll`; protocol v0.6.1.1 cố ý từ chối DLL cũ.

CI/build không thay thế test thực chiến trên đúng client game. Hãy thử một PID và kiểm tra log đủ 90 callback trước khi bật chu trình nhiều tài khoản.

## Hotfix ô trang bị cố định

- Chuỗi đang tốt `ClickNPC → shop → Bán vật phẩm → Bán nhanh → Trang bị` giữ nguyên.
- Chỉ bước item tay nải đổi: Bridge dùng tọa độ đã lấy để tìm control live nằm dưới điểm đó rồi gọi callback nội bộ. Không foreground, không `SendInput`, không di chuyển chuột.
- Profile v0.5 đủ 5 dòng dùng dòng 5. Nếu profile chỉ còn một dòng thì dùng chính dòng đó; với 2–4 dòng dùng dòng cuối.
- Vòng bán đầu gọi ô cố định 90 lần. Sau khi đóng UI và `FreeBagSpace` ổn định 1,5 giây, vòng bán sau dùng đúng số ô trống đã học, tối đa 90. Giá trị học chỉ sống trong phiên chạy tool như v0.5.
- Delay của dòng tọa độ vẫn là delay giữa từng callback. Cột `Lặp` cũ không quyết định vòng đầu trong hotfix này vì yêu cầu đã cố định là 90.
- Nên lấy F8 tại tâm ô trang bị thứ 2 sau khi đã mở đúng tab Trang bị. `TEST DÒNG` cũ vẫn là test chuột legacy; chuỗi AUTO BÁN mới mới là đường callback nội bộ.

## Sửa lỗi resolver v0.6.1

- Basic discovery chỉ cần `UIObject.instances` và ít nhất một control class. `UIButton`/`UIToggle` không còn bị chặn vì thiếu Lua Executor.
- Namespace cũ vẫn là đường nhanh; nếu client đặt lớp ở namespace khác, Bridge quét metadata có giới hạn và chỉ nhận UIObject/Button/Toggle/Rect có đúng field/method cần dùng.
- `UIRectTransform` mới tải `MonoBehaviourExecutor` khi chính nó được chọn.
- AUTO Lua mới tải thêm `LuaSystemAPI_GUI`, `System.Object` và Executor.
- Executor được thử nhiều namespace, sau đó quét metadata theo tên lớp và bắt buộc có đúng `get_Instance()` + `ExecuteScriptFunction(3)`.
- Nếu vẫn lỗi, log trả chính xác component thiếu thay vì gộp bảy dependency thành một câu.

## Thay đổi chính

- `Xác nhận ra map`: vẫn giữ nguyên điều kiện watchdog Lâu Lan của v0.5, nhưng Bridge tìm nút đồng ý duy nhất bên trong `MessageBox` rồi gọi callback nội bộ. Không foreground cửa sổ, không di chuyển chuột.
- `Đầu thai`: Bridge đọc lại `IsDeath=true`, tìm đúng control `Đầu thai` và gọi `UIButton.HandleClickEvent()`.
- `AUTO → Đánh quái`: gọi trực tiếp Lua action `TopIcon.AutoTrainClick`; `Dừng AUTO` gọi `TopIcon.AutoStopClick`. Controller vẫn xác minh kết quả bằng snapshot AutoFight authoritative.
- `Tự bán đồ`: các stage semantic qua Trang bị giữ nguyên; v0.6.1.1 gọi lặp một ô trang bị do tọa độ cấu hình chọn, rồi đóng UI và xác minh `FreeBagSpace` ổn định.
- Chuỗi giao dịch MAIN/CON vẫn dùng tọa độ và chuột thật. Đây là phần ngoài phạm vi thay đổi v0.6.1.

## Kiến trúc tích hợp

v0.6.1.1 tiếp tục dùng Bridge DLL `WH_GETMESSAGE` vốn có của v0.5. Mỗi request chỉ resolve và gọi một callback trên đúng message thread của cửa sổ game; controller không được ghi đè request đang bận. Không thêm `CreateRemoteThread` hoặc worker IL2CPP thứ hai.

Các điểm click XN/Đầu thai/AUTO cũ vẫn chỉ để tương thích. Riêng editor macro bán được dùng lại đúng cho tọa độ/delay của ô item; bốn click mở shop cũ không quay lại active runtime.

## Build và kiểm tra

Workflow Windows x64 chạy:

1. `tools/verify_v0611_logic.py`.
2. Build controller + Bridge bằng MSVC.
3. Route, rotation, trade coordinator self-tests.
4. Background UI scoring self-test.
5. Fixed-slot selection/adaptive-count self-test.

Artifact: `ThanLongItemConsolidator-v0.6.1.1-win-x64` gồm EXE, Bridge DLL và tài liệu.

Đọc [DONOR_0.8.4_BACKGROUND_ACTION_ANALYSIS.md](DONOR_0.8.4_BACKGROUND_ACTION_ANALYSIS.md) để xem diễn giải chi tiết donor 0.8.4 và quyết định chuyển đổi.

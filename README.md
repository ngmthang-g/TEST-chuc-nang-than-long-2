# Thần Long Item Consolidator v0.6.1.3

Nền phát triển trực tiếp: source v0.5.0 do người dùng cung cấp. Source v0.8.4 chỉ là donor để nghiên cứu cơ chế callback UI nội bộ; không ghép nguyên remote worker của donor.

## Tải bản Windows x64

- v0.6.1.3 là hotfix hẹp trên đúng source v0.6.1.2: chỉ thay cách phát click nội bộ ở bước item tay nải.
- Gói runtime: `release/ThanLongItemConsolidator-v0.6.1.3-win-x64.zip` (được thêm sau khi Windows CI PASS).
- Gói source: `release/ThanLongItemConsolidator_Source_v0.6.1.3.zip` (được thêm cùng release).
- Hash EXE/DLL/ZIP và nguồn CI sẽ nằm tại `release/SHA256SUMS_v0.6.1.3.txt`.
- Bản v0.6.1.2 được giữ nguyên để đối chiếu lỗi `geometry=149`.
- Bản v0.6.1.1 được giữ nguyên để đối chiếu lỗi runtime.
- Bản v0.6.1 gốc vẫn nằm tại [release/ThanLongItemConsolidator-v0.6.1-win-x64.zip](release/ThanLongItemConsolidator-v0.6.1-win-x64.zip) để đối chiếu.
- v0.6 cũ được giữ tại [release/ThanLongItemConsolidator-v0.6-win-x64.zip](release/ThanLongItemConsolidator-v0.6-win-x64.zip) để bảo toàn lịch sử, nhưng **không nên dùng để test callback UI**.
- Luôn giữ EXE cùng thư mục với `ThanLongCleanRouteBridge.dll`; protocol v0.6.1.3 cố ý từ chối DLL cũ.

Windows CI v0.6.1.3 đang là cổng build bắt buộc. Dù CI PASS, bước item vẫn là **RUNTIME UNTESTED** cho tới khi chạy đúng cặp EXE/DLL trên một PID thật.

## Hotfix click item v0.6.1.3

- Log v0.6.1.2 `geometry=149` chứng minh resolver đã hoạt động, nhưng ô item không phải một `UIButton/UIRect` callback nằm dưới tọa độ; vì vậy cách dò 149 control không thể bấm ô đó.
- Repo dữ liệu client hiện tại chứng minh `InputSyncManager` có đúng chuỗi input UI của game. Một click hoàn chỉnh là `TryClickUI(0, point)` (nhấn trái) rồi `EndUIDrag(point)` (nhả/click), không phải gọi riêng `TryClickUI`.
- Bước item mới phát đúng cặp nhấn/nhả qua Unity EventSystem trên game thread. Nó không foreground, không `SetCursorPos`, không `SendInput` và không dùng chuột Windows.
- Trước nhấn phải không có `_uiDragging`; sau nhấn phải raycast trúng và tạo drag state; sau nhả state phải sạch. Nếu pha nhả lỗi, Bridge chỉ hủy drag do chính lượt đó tạo rồi dừng fail-closed.
- Tọa độ vẫn lấy từ dòng click đã gán; nên đặt ở tâm ô trang bị thứ 2 sau khi đã mở tab Trang bị.

## Hotfix resolver Unity geometry v0.6.1.2

- Log v0.6.1.1 chứng minh Bridge chưa hề dùng tọa độ hoặc callback item: nó dừng khi tìm class geometry.
- v0.6.1.1 sai ở giả định mọi class đều thuộc `UnityEngine.CoreModule`.
- v0.6.1.2 tìm riêng từng class qua `CoreModule`, `UIModule` và `UnityEngine.dll`. `RectTransformUtility` ưu tiên UIModule.
- Nếu vẫn thiếu, log mới nêu chính xác class nào thiếu và assembly nào thực sự mở được.
- Không đổi cách chọn dòng: chuỗi 6 click vẫn lấy dòng số 5; `6/6` trong log cũ là sáu lần retry callback, không phải dòng số 6.

## Logic ô trang bị cố định được giữ từ v0.5

- Chuỗi đang tốt `ClickNPC → shop → Bán vật phẩm → Bán nhanh → Trang bị` giữ nguyên.
- Chỉ bước item tay nải đổi: Bridge dùng tọa độ đã lấy để phát một click UI nội bộ của chính client. Không foreground, không `SendInput`, không di chuyển chuột.
- Profile v0.5 đủ 5 dòng dùng dòng 5. Nếu profile chỉ còn một dòng thì dùng chính dòng đó; với 2–4 dòng dùng dòng cuối.
- Vòng bán đầu gọi ô cố định 90 lần. Sau khi đóng UI và `FreeBagSpace` ổn định 1,5 giây, vòng bán sau dùng đúng số ô trống đã học, tối đa 90. Giá trị học chỉ sống trong phiên chạy tool như v0.5.
- Delay của dòng tọa độ vẫn là delay giữa từng callback. Cột `Lặp` cũ không quyết định vòng đầu trong hotfix này vì yêu cầu đã cố định là 90.
- Nên lấy F8 tại tâm ô trang bị thứ 2 sau khi đã mở đúng tab Trang bị. `TEST DÒNG` cũ vẫn là test chuột legacy; chuỗi AUTO BÁN mới là đường callback nội bộ.

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
- `Tự bán đồ`: các stage semantic qua Trang bị giữ nguyên; v0.6.1.3 gọi lặp một ô trang bị do tọa độ cấu hình chọn, rồi đóng UI và xác minh `FreeBagSpace` ổn định.
- Chuỗi giao dịch MAIN/CON vẫn dùng tọa độ và chuột thật. Đây là phần ngoài phạm vi thay đổi v0.6.1.3.

## Kiến trúc tích hợp

v0.6.1.3 tiếp tục dùng Bridge DLL `WH_GETMESSAGE` vốn có của v0.5. Mỗi request chỉ phát một click item hoàn chỉnh trên đúng message thread của cửa sổ game; controller không được ghi đè request đang bận. Không thêm `CreateRemoteThread` hoặc worker IL2CPP thứ hai.

Các điểm click XN/Đầu thai/AUTO cũ vẫn chỉ để tương thích. Riêng editor macro bán được dùng lại đúng cho tọa độ/delay của ô item; bốn click mở shop cũ không quay lại active runtime.

## Build và kiểm tra

Workflow Windows x64 chạy:

1. `tools/verify_v0613_logic.py`.
2. Build controller + Bridge bằng MSVC.
3. Route, rotation, trade coordinator self-tests.
4. Background UI scoring self-test.
5. Fixed-slot selection/adaptive-count self-test.
6. Unity geometry resolver search-order self-test.
7. Internal UI press/release dispatch-plan self-test.

Kết quả CI và mã artifact v0.6.1.3 sẽ được ghi vào đây sau khi workflow Windows hoàn tất; không được suy từ kiểm thử tĩnh thành runtime pass.

Đọc [DONOR_0.8.4_BACKGROUND_ACTION_ANALYSIS.md](DONOR_0.8.4_BACKGROUND_ACTION_ANALYSIS.md) để xem diễn giải chi tiết donor 0.8.4 và quyết định chuyển đổi.

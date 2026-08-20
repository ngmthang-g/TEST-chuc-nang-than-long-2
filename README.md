# Thần Long Item Consolidator v0.6.2

Nền phát triển trực tiếp là source v0.5.0 do người dùng cung cấp. Source v0.8.4 chỉ là donor nghiên cứu callback nội bộ; không ghép nguyên remote worker của donor.

## Tải bản Windows x64

- [ThanLongItemConsolidator-v0.6.2-win-x64.zip](release/ThanLongItemConsolidator-v0.6.2-win-x64.zip)
- Hash EXE/DLL/ZIP và nguồn CI: [release/SHA256SUMS_v0.6.2.txt](release/SHA256SUMS_v0.6.2.txt).
- Giữ `ThanLongItemConsolidator_v0.6.2.exe` và `ThanLongCleanRouteBridge.dll` cùng thư mục. Protocol `0x00010602` cố ý từ chối DLL khác version.
- Các ZIP v0.6/v0.6.1 vẫn được giữ trong `release/` để truy vết, không phải bản nên test tiếp.

## Bằng chứng runtime dẫn tới v0.6.2

Trên đúng client của người dùng, v0.6.1 đã chứng minh:

- `Đầu thai`: **RUNTIME PASS**; Bridge kiểm tra `IsDeath` rồi gọi callback, chuỗi acc khác không mất index/repeat.
- `Xác nhận ra map`: **RUNTIME PASS**; watchdog gọi callback MessageBox, không foreground/chuột, sau đó client chuyển map.
- `AUTO`: **RUNTIME FAIL** ở lookup `TopIcon` (`Không tìm thấy Lua UI theo tên`).
- Bán nền: các stage `ClickNPC → shop → Bán vật phẩm → Bán nhanh → Trang bị` và đóng UI chạy được, nhưng chọn item không ổn định. Logic cũ còn có thể coi một ô vừa trống là bán xong rồi quay bãi.
- F4: người dùng báo không phản hồi dù phần `ToggleGlobalPause` trong source v0.6.1 vẫn giống v0.5; điểm hỏng cụ thể của `RegisterHotKey`/message delivery chưa được log cũ chứng minh.

## Sửa trong v0.6.2

- AUTO vẫn thử đúng Lua `TopIcon.AutoTrainClick/AutoStopClick` trước. Nếu `FindUI/MainFindUI` trả null, Bridge tìm `TopIcon` trong `UIObject.instances`. Nếu vẫn không có, controller chạy hai nhịp không block: callback `AUTO` root → đợi 650 ms → resolve mới và callback `Đánh quái` hoặc `Dừng`. Snapshot AutoFight vẫn là bằng chứng ON/OFF cuối cùng.
- F4 giữ nguyên thân hàm pause/resume của v0.5 và `RegisterHotKey`; bổ sung edge polling `GetAsyncKeyState(VK_F4)` có latch để cứu trường hợp đăng ký/message bị mất mà không toggle hai lần.
- Item bán được xếp theo control có handler item thật và thứ tự ô tự nhiên, thay vì địa chỉ pointer ngẫu nhiên.
- Chạm giới hạn 90 callback không còn đồng nghĩa “hết item”. Không bán xác minh được món nào cũng là lỗi riêng; hai trường hợp đều giữ UI và dừng fail-closed.
- Một ô trống không còn đủ để tự quay bãi. Chỉ kết thúc khi Bridge đã xác minh có item rời túi, đã cạn candidate hiện hành và `FreeBagSpace` tăng ổn định so với đầu phiên.
- XN, Đầu thai, route ownership, Travel Guard, trade/FIFO/rotation và các logic v0.5 ngoài phạm vi được giữ nguyên.

## Kiến trúc và giới hạn

Bridge tiếp tục dùng `WH_GETMESSAGE` vốn có của v0.5; mỗi request chạy trên đúng window message thread, một request in-flight cho mỗi PID. Không thêm `CreateRemoteThread`, không fallback sang tọa độ cho XN/Đầu thai/AUTO/bán nền. Chuỗi giao dịch MAIN/CON vẫn dùng chuột thật.

Build/CI chỉ chứng minh source biên dịch và self-test pass. v0.6.2 phải được test thực chiến từng action trên một PID trước khi bật nhiều account. Đặc biệt, đường bán hiện vẫn dựa vào callback control UI + `FreeBagSpace`; bước nâng cấp chuẩn sau là quét item instance và gửi từng sell request có shop state, không phải khôi phục click mù 90 ô.

Source commit `17d5aa7` đã **BUILD PASS** trên Windows CI run 280: MSVC x64 và toàn bộ verifier/self-test đều pass. Trạng thái runtime v0.6.2 vẫn là **RUNTIME UNTESTED**.

## Build và kiểm tra

Windows workflow chạy `tools/verify_v062_logic.py`, MSVC x64 build, route/rotation/trade tests và background UI scoring tests. Artifact có tên `ThanLongItemConsolidator-v0.6.2-win-x64`.

Đọc [DONOR_0.8.4_BACKGROUND_ACTION_ANALYSIS.md](DONOR_0.8.4_BACKGROUND_ACTION_ANALYSIS.md) và [docs/history/VERSION_v0.6.2.md](docs/history/VERSION_v0.6.2.md) để xem ranh giới donor, bằng chứng và các quyết định fail-closed.

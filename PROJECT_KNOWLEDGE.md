# PROJECT KNOWLEDGE — v0.6.1.2 CURRENT

## Quy tắc dự án

- Nền phát triển trực tiếp là v0.5.0; v0.8.4 chỉ là donor nghiên cứu.
- Không thay logic ngoài phạm vi nếu không cần để tích hợp.
- Callback trả OK không phải bằng chứng nghiệp vụ; luôn verify bằng snapshot tương ứng.
- Mỗi version cập nhật README, changelog, source provenance, bảng logic, verifier và hồ sơ runtime/bug.

## Trạng thái hiện tại

- v0.6: **RUNTIME FAIL** đối với XN Lâu Lan và mở chuỗi bán nền. Cùng lỗi: `Không resolve đủ UIObject/UIButton/UIToggle/UIRect/Executor`.
- Điều runtime đã chứng minh: Bridge attach, snapshot, route và `ClickNPC` trước chuỗi UI vẫn hoạt động đủ để FSM đi tới đúng điểm lỗi. Log không chứng minh component cụ thể nào null.
- BUG-001 root cause ở mức **CONFIRMED**: basic UI enumeration bị khóa bởi một readiness gate gộp cả dependency Lua/Executor không cần cho UIButton. `MonoBehaviourExecutor` là component **LIKELY** bị thiếu vì namespace của nó chỉ là giả định từ donor RVA, chưa có metadata proof trong v0.6.
- v0.6.1: source audit + Windows MSVC x64 CI 274 **BUILD PASS**, nhưng vẫn **RUNTIME UNTESTED**. Hotfix chưa được gọi là runtime pass cho tới khi người dùng test lại.
- User runtime sau đó xác nhận chuỗi bán v0.6.1 đi đúng tới tab Trang bị nhưng bước item không giữ hành vi một ô lặp/dồn đồ của v0.5. Đây là BUG-002.
- v0.6.1.1: Windows MSVC x64 CI run 295 **BUILD PASS**, gồm scope audit và đủ năm nhóm self-test. Live runtime vẫn **RUNTIME UNTESTED** cho tới khi người dùng chạy đúng cặp EXE/DLL trên client thật.
- User test v0.6.1.1 xác nhận item stage fail `6/6` với chi tiết `Thiếu RectTransform/Utility/Screen để hit-test ô cố định`. Đây là BUG-003: resolver ép năm class vào riêng CoreModule; lỗi xảy ra trước hit-test/callback nên không liên quan dòng 5.
- v0.6.1.2: Windows MSVC x64 CI run 302 **BUILD PASS**, gồm scope audit và đủ sáu nhóm self-test. Live runtime vẫn **RUNTIME UNTESTED** cho tới khi người dùng chạy đúng cặp EXE/DLL.

## Kiến trúc action v0.6.1.2

Controller vẫn là scheduler/FSM. `ThanLongCleanRouteBridge.dll` vẫn được nạp bằng `WH_GETMESSAGE` vào game window thread. Protocol `0x00010612` từ chối ghép EXE/DLL khác version.

Không có `CreateRemoteThread` trong controller/Bridge. Một mapping chỉ có một request in-flight; timeout không được ghi đè.

Resolver UI chia hai tầng:

1. `EnsureUiDiscovery`: `UIObject.instances` + các class control hiện có. Dùng namespace đã biết trước, rồi fallback quét metadata có giới hạn và kiểm tra method/field surface; dùng cho scan và UIButton/UIToggle.
2. `EnsureUiLua`: chỉ gọi khi UIRect/Lua action thực sự cần `MonoBehaviourExecutor`, `System.Object`, `LuaSystemAPI_GUI`.

Executor được thử namespace ứng viên rồi quét toàn bộ class của Assembly-CSharp theo simple name; chỉ nhận class có `get_Instance()` và `ExecuteScriptFunction(3)`.

Geometry resolver mở ba layout Unity: `UnityEngine.CoreModule`, `UnityEngine.UIModule` và monolithic `UnityEngine.dll`. Mỗi class có search order riêng; `RectTransformUtility` ưu tiên UIModule. Lỗi ghi danh sách class thiếu và trạng thái từng assembly.

## Ưu tiên toàn cục

1. P1 XN Lâu Lan: điều kiện route ownership/stall của v0.5 giữ nguyên; action đổi sang MessageBox callback.
2. P2 Đầu thai: giữ death lifecycle/World Flow FIFO; action đổi sang death-guarded UIButton callback.
3. P3 AUTO: Attack/Stop request đổi sang `TopIcon.AutoTrainClick/AutoStopClick`.

Ba action không dùng mouse guard vì không tạo mouse input. Mouse guard vẫn áp dụng cho chuỗi giao dịch tọa độ.

## Auto-sell

- Quyết định khi nào bán, đi NPC, quay bãi, retry hai pass và ngưỡng MAIN/CON giữ từ v0.5.
- Bridge vẫn nhận dạng các semantic stage qua `Trang bị`; controller delay giữa các stage để không block game thread.
- Riêng item stage dùng tọa độ dòng 5 (hoặc dòng cuối/duy nhất) để hit-test control live rồi gọi một callback nội bộ mỗi tick.
- Vòng đầu 90 callback; sau sale hoàn tất, `FreeBagSpace` ổn định được lưu vào `sellStep5LearnedRepeat` cho vòng sau, cap 90.
- Không cache control/RectTransform giữa hai click; không tạo physical mouse input.
- Completion vẫn chỉ được kết luận sau `FreeBagSpace > 0` ổn định 1,5 giây.

## Logic được bảo vệ

World Flow observation/recovery, route ownership, Travel Guard authoritative, mount recovery, rotation, F4/F8, trade FIFO/relock/pass/bag stabilization và trade macro giữ nguyên trừ nơi phải đổi từ AUTO physical sang AUTO nội bộ.

## Rủi ro còn cần test thật

- Mã Kiêu Minh ID 373 là donor đã chứng minh. Dược Đại Phu ID 279 có thể dùng cây shop khác.
- UI scorer phụ thuộc metadata/text của đúng client.
- Game minimized có thể tự dừng update dù action không chiếm chuột.
- Giao dịch MAIN/CON vẫn chiếm chuột vì chưa nằm trong phạm vi v0.6.1.2.
- `WH_GETMESSAGE` hiện vẫn gọi mutation trực tiếp trong hook. Knowledge base client khuyến nghị bước sau phải proof `System.Action -> MainThread.Execute` bất đồng bộ trước khi coi đây là boundary production ổn định; v0.6.1.2 vẫn không mở rộng sang thay kiến trúc đó khi chưa có CTS live proof.

## Test tiếp theo bắt buộc

1. Dùng đúng cặp EXE/DLL v0.6.1.2.
2. Test XN Lâu Lan trước. Nếu fail, log mới phải nêu `UIObject`, `instances`, control type hoặc candidate ambiguity cụ thể.
3. Mở tab Trang bị, lấy dòng item F8 tại tâm ô trang bị thứ 2; chạy một PID và ghi log callback `1/90` tới `90/90`.
4. Xác nhận chuột/foreground không đổi, UI đóng, `FreeBagSpace` ổn định và log lần sau dùng learned count.
5. Test AUTO start/stop riêng; sell pass không tự động chứng minh AUTO pass.
6. Chưa bật hàng loạt account trước khi một PID qua đủ các bước trên.

## Knowledge index

- `docs/bugs/BUG_REGISTRY.md` — BUG-001.
- `docs/evidence/EVIDENCE_REGISTRY.md` — EVID-001.
- `docs/decisions/DECISIONS.md` — DEC-001.
- `docs/features/BACKGROUND_UI_ACTIONS.md`.
- `docs/history/VERSION_v0.6.1.md`.
- `docs/history/VERSION_v0.6.1.1.md`.
- `docs/history/VERSION_v0.6.1.2.md`.

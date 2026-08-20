# PROJECT KNOWLEDGE — v0.6.1 CURRENT

## Quy tắc dự án

- Nền phát triển trực tiếp là v0.5.0; v0.8.4 chỉ là donor nghiên cứu.
- Không thay logic ngoài phạm vi nếu không cần để tích hợp.
- Callback trả OK không phải bằng chứng nghiệp vụ; luôn verify bằng snapshot tương ứng.
- Mỗi version cập nhật README, changelog, source provenance, bảng logic, verifier và hồ sơ runtime/bug.

## Trạng thái hiện tại

- v0.6: **RUNTIME FAIL** đối với XN Lâu Lan và mở chuỗi bán nền. Cùng lỗi: `Không resolve đủ UIObject/UIButton/UIToggle/UIRect/Executor`.
- Điều runtime đã chứng minh: Bridge attach, snapshot, route và `ClickNPC` trước chuỗi UI vẫn hoạt động đủ để FSM đi tới đúng điểm lỗi. Log không chứng minh component cụ thể nào null.
- BUG-001 root cause ở mức **CONFIRMED**: basic UI enumeration bị khóa bởi một readiness gate gộp cả dependency Lua/Executor không cần cho UIButton. `MonoBehaviourExecutor` là component **LIKELY** bị thiếu vì namespace của nó chỉ là giả định từ donor RVA, chưa có metadata proof trong v0.6.
- v0.6.1: **RUNTIME UNTESTED**. Hotfix tách capability và thêm chẩn đoán chính xác; chưa được gọi là runtime pass cho tới khi người dùng test lại.

## Kiến trúc action v0.6.1

Controller vẫn là scheduler/FSM. `ThanLongCleanRouteBridge.dll` vẫn được nạp bằng `WH_GETMESSAGE` vào game window thread. Protocol `0x00010601` từ chối ghép EXE/DLL khác version.

Không có `CreateRemoteThread` trong controller/Bridge. Một mapping chỉ có một request in-flight; timeout không được ghi đè.

Resolver UI chia hai tầng:

1. `EnsureUiDiscovery`: `UIObject.instances` + các class control hiện có. Dùng namespace đã biết trước, rồi fallback quét metadata có giới hạn và kiểm tra method/field surface; dùng cho scan và UIButton/UIToggle.
2. `EnsureUiLua`: chỉ gọi khi UIRect/Lua action thực sự cần `MonoBehaviourExecutor`, `System.Object`, `LuaSystemAPI_GUI`.

Executor được thử namespace ứng viên rồi quét toàn bộ class của Assembly-CSharp theo simple name; chỉ nhận class có `get_Instance()` và `ExecuteScriptFunction(3)`.

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
- Giao dịch MAIN/CON vẫn chiếm chuột vì chưa nằm trong phạm vi v0.6.1.
- `WH_GETMESSAGE` hiện vẫn gọi mutation trực tiếp trong hook. Knowledge base client khuyến nghị bước sau phải proof `System.Action -> MainThread.Execute` bất đồng bộ trước khi coi đây là boundary production ổn định; v0.6.1 không mở rộng sang thay kiến trúc đó khi chưa có CTS live proof.

## Test tiếp theo bắt buộc

1. Dùng đúng cặp EXE/DLL v0.6.1.
2. Test XN Lâu Lan trước. Nếu fail, log mới phải nêu `UIObject`, `instances`, control type hoặc candidate ambiguity cụ thể.
3. Test Mã Kiêu Minh ID 373. Ghi lại từng semantic stage và `FreeBagSpace`.
4. Test AUTO start/stop riêng; đây là nhánh còn cần Lua Executor nên XN/Sell Button pass không tự động chứng minh AUTO pass.
5. Chưa bật hàng loạt account trước khi một PID qua đủ các bước trên.

## Knowledge index

- `docs/bugs/BUG_REGISTRY.md` — BUG-001.
- `docs/evidence/EVIDENCE_REGISTRY.md` — EVID-001.
- `docs/decisions/DECISIONS.md` — DEC-001.
- `docs/features/BACKGROUND_UI_ACTIONS.md`.
- `docs/history/VERSION_v0.6.1.md`.

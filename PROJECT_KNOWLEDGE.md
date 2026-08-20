# PROJECT KNOWLEDGE — v0.6.2 CURRENT

## Quy tắc dự án

- Nền phát triển trực tiếp là v0.5.0; v0.8.4 chỉ là donor nghiên cứu.
- Không thay logic ngoài phạm vi nếu không cần tích hợp.
- Callback trả OK không phải bằng chứng nghiệp vụ; phải verify bằng snapshot/state tương ứng.
- BUILD PASS không được gọi là RUNTIME PASS.
- Mỗi version cập nhật README, changelog, provenance, bảng logic, verifier và hồ sơ evidence/bug/decision.

## Sự thật runtime đã biết

| Version/action | Trạng thái | Bằng chứng |
|---|---|---|
| v0.6 XN + mở bán | RUNTIME FAIL | readiness gate gộp, EVID-001 |
| v0.6.1 XN Lâu Lan | RUNTIME PASS | callback nội bộ rồi map transition, EVID-002 |
| v0.6.1 Đầu thai | RUNTIME PASS | `IsDeath` + UIButton callback; sequence khác giữ index/repeat, EVID-002 |
| v0.6.1 AUTO | RUNTIME FAIL | `TopIcon` lookup trả `Không tìm thấy Lua UI theo tên`, EVID-002 |
| v0.6.1 Auto Sell | RUNTIME PARTIAL/FAIL | mở stage và đóng UI được; item callback không bán ổn định; có đường kết thúc sai sau một món, EVID-002 |
| v0.6.1 F4 | USER-REPORTED FAIL | thân hàm v0.5 vẫn còn; lỗi delivery cụ thể UNKNOWN, EVID-002 |
| v0.6.2 | BUILD PASS / RUNTIME UNTESTED | gói từ Windows CI run 282, source `a1f81d8`, artifact `9416247108`; chưa có live test |

## Kiến trúc action v0.6.2

Controller vẫn giữ scheduler/FSM v0.5. `ThanLongCleanRouteBridge.dll` được nạp bằng `WH_GETMESSAGE` vào đúng message thread của từng game window. Protocol `0x00010602`; một request in-flight cho mỗi PID; timeout không được ghi đè.

Không có `CreateRemoteThread` trong controller/Bridge.

Resolver UI:

1. `EnsureUiDiscovery`: `UIObject.instances` + control classes hiện có, không phụ thuộc Executor.
2. `EnsureUiLua`: chỉ tải Executor/System.Object/GUI API khi UIRect hoặc Lua action cần.
3. AUTO lookup thử `FindUI/MainFindUI`, rồi exact active `UIObject.instances`, rồi mới fallback hai callback UI hiện hành.

## Ưu tiên toàn cục

1. P1 XN Lâu Lan — runtime pass, giữ nguyên route/stall gate.
2. P2 Đầu thai — runtime pass, giữ nguyên death lifecycle/World Flow FIFO.
3. P3 AUTO — path 1 gọi `TopIcon.AutoTrainClick/AutoStopClick`; path 2 không block: `AUTO` root → 650 ms → resolve mới `Đánh quái/Dừng`. Snapshot `AutoFight` quyết định thành/bại.

## F4

Thân `ToggleGlobalPause` v0.6.2 giữ nguyên hành vi v0.5: toggle global pause, gửi StopPath cho acc RUN khi có thể, không tự đổi combat. `RegisterHotKey(F4)` vẫn là đường chính. Timer 250 ms đọc cạnh F4 bằng `GetAsyncKeyState`, dùng latch chung với `WM_HOTKEY` để phục hồi delivery và tránh toggle hai lần.

## Auto Sell

- Trigger/mode/role và hành trình NPC vẫn thuộc FSM v0.5.
- Active item path hiện vẫn là callback UI, không dùng macro tọa độ.
- Candidate ưu tiên control có item-click handler và cell index tự nhiên; raw pointer order bị loại.
- `FreeBagSpace` tăng mới tính là sold verified.
- `SafetyLimit` (90 callback) và `NoProgress` là lỗi fail-closed, không phải hết đồ.
- Hoàn tất cần đồng thời: sold verified > 0, Bridge báo exhausted sau scan hiện hành, FreeBag tăng so với đầu phiên và ổn định 1.5 giây.
- Phase lỗi không tự thoát chỉ vì người dùng tạo một ô trống; Stop/Start mới reset runtime.

Ranh giới: dữ liệu client đã chứng minh production semantic sell nên dùng fresh `GetItemsAtSite(Bag)` → item instance ID → current shop IDs → một sell request → RemoveItem/UpdateItemsList proof → rescan. v0.6.2 chưa tự nhận là đã triển khai packet loop đó; nó sửa lỗi live của callback bridge và completion gate trước.

## Logic được bảo vệ

XN/Revive đang pass, World Flow, route ownership, Travel Guard, mount recovery, trade/FIFO/relock/pass, rotation, F8, profile/INI và MAIN/CON coordinate trade không được đổi ngoài những điểm ghi rõ.

## Test tiếp theo bắt buộc

1. Dùng đúng cặp EXE/DLL v0.6.2 trên một PID.
2. Nhấn F4 hai lần khi tool không foreground; log phải lần lượt có `TẠM DỪNG` và `TIẾP TỤC` đúng một lần mỗi phím.
3. Test AUTO bật và dừng. Nếu path 1 không resolve, log phải có P1 AUTO-root rồi PASS `AUTO → Đánh quái/Dừng`; snapshot phải đổi đúng ON/OFF.
4. Test bán Mã Kiêu Minh. Ghi `callback`, `FreeBag`, `sold verified`, và kết quả `NoProgress/SafetyLimit/exhausted`; không được quay bãi sau chỉ một ô trống nếu chưa có exhaustion proof.
5. Regression XN và Đầu thai; không bật nhiều account trước khi một PID qua đủ bốn nhóm.

## Knowledge index

- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/decisions/DECISIONS.md`
- `docs/features/BACKGROUND_UI_ACTIONS.md`
- `docs/history/VERSION_v0.6.2.md`

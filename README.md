# TEST chức năng Thần Long 2

Repo này là **phòng thử nghiệm tính năng**, không phải tool chính thức.

Mục tiêu:

- thử riêng từng chức năng trên nền source sạch;
- đo độ ổn định và hành vi thực tế trên client;
- không lấy lại mù quáng code/action chain từ các bản cũ đã lỗi;
- tính năng nào test PASS mới mang sang phiên bản chính;
- mỗi version/tính năng phải có file `docs/HANDOFF_*.md` ghi rõ code nằm đâu, lấy dữ liệu từ đâu, state machine hoạt động thế nào, giả định nào đã/chưa được xác minh và cách ghép sang tool chính.

## v1.1.0-test — Auto trị liệu

Mục tiêu test:

`Auto trị liệu -> đi Lâu Lan -> Đỗ Thanh Đằng -> Trị liệu -> Xác nhận -> Ta biết rồi`

Dữ liệu đã bóc trực tiếp từ client được cung cấp:

- Lâu Lan: `MapID = 5`.
- NPC Đỗ Thanh Đằng: `NPC ID / ResID = 339`, `ResName = LangZhong1`.
- **Không hardcode tọa độ NPC.** Người test đứng đúng vị trí cạnh NPC rồi bấm `TỰ LẤY TỌA ĐỘ NPC`; tool lưu nguyên `MapID/X/Y` đang đọc từ client, không nhân/chia hay đổi scale.
- mở NPC bằng `FGStudio.LuaSystem.API.LuaSystemAPI_Game.ClickNPC(Int32)` được resolve từ metadata lúc runtime, không hardcode RVA.
- `GameDialog` tạo button lựa chọn động; text nằm ở `UIButton.Text`, selection ID nằm ở `Tag`.
- button dialog thật chạy handler Lua `FunctionButtonClicked` và gửi `CMD_SHOW_GAMEDIALOG`.
- `MessageBox_Layout` có `ButtonOK`, text `Xác nhận`, handler `ButtonOKClicked`.

### Nguyên tắc của bản test

- Không `CreateRemoteThread`, không `WriteProcessMemory`.
- Action chạy trên thread cửa sổ game qua `WH_GETMESSAGE` bridge hiện có.
- Không lưu pointer UIButton qua frame/UI transition.
- Mỗi lần cần bấm, bridge tìm lại **button đang sống** trong UI hiện tại rồi mới gọi event của chính instance đó.
- Không dùng chuỗi `Sleep(500) -> click tiếp` để đoán trạng thái UI.
- Chỉ chuyển state khi nhìn thấy đúng dialog/button tương ứng.
- Nếu không resolve được UI/button, **fail-closed** và dừng thay vì click nhầm.

Xem chi tiết kỹ thuật và hướng ghép sang bản chính tại:

- `docs/HANDOFF_v1.1.0_AUTO_TRI_LIEU.md`
- `docs/CLIENT_EVIDENCE_v1.1.0.md`

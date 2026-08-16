# TEST chức năng Thần Long 2

Repo này là **phòng thử nghiệm tính năng**, không phải tool chính thức. Tính năng chỉ được mang sang tool chính sau khi runtime test ổn định và tri thức kỹ thuật đã được bàn giao đầy đủ.

## BẮT BUỘC — AI phải đọc trước mỗi version

Bắt đầu từ:

1. `AI_START_HERE.md`
2. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
3. `AI_CLIENT_ANALYSIS_RULES.txt`
4. `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. docs/source liên quan trực tiếp tới task.

Khi cần dữ liệu client, dùng canonical repo:

`ngmthang-g/clinent-game-than-long-DATA-2222`

Route đọc chuẩn:

`AI_INDEX.md -> AI_BOOTSTRAP.md -> AUTO_TOOL_SCOPE.md -> AI_ROUTER.md -> đúng 1 contexts/BUILD_*.md -> REQUIRED docs -> exact VERIFIED/database lookup`.

**Không broad reverse-engineer lại client. Chỉ xuống binary/native khi exact fact cần cho task thực sự chưa có trong VERIFIED/database/canonical docs.**

## Mục tiêu repo test

- thử riêng từng chức năng trên nền source sạch;
- đo hành vi thật trên client, không đồng nhất BUILD PASS với RUNTIME PASS;
- không lặp lại giải pháp đã được runtime chứng minh là FAILED nếu không có bằng chứng mới;
- giữ đầy đủ lineage: yêu cầu -> code -> build/CI -> runtime -> bug -> fix/revert -> known-good;
- mỗi feature/version có tài liệu đủ để AI khác port sang tool chính mà không nghiên cứu lại từ đầu.

# Current: v1.1.6-test — Auto trị liệu / MainThread queued UI action

## Runtime đã biết trước v1.1.6

- Tự lấy tọa độ raw MapID/X/Y: hoạt động.
- Route/AutoPath + xuống ngựa + mở NPC: PARTIAL PASS.
- v1.1.0–v1.1.2 direct `UIButton.HandleClickEvent`: Treatment FAIL, UI nháy.
- v1.1.3 `GameDialog.FunctionButtonClicked` qua ExecuteUIObject: FAIL, cùng kiểu nháy.
- v1.1.4 live selectionID + source-verified `CMD_SHOW_GAMEDIALOG 100007`: FAIL trên Đỗ Thanh Đằng/Lâu Lan.
- v1.1.5 cùng action layer tại Long Phá Thiên/Lạc Dương: user runtime vẫn thấy **cùng kiểu nháy và không tiến Treatment**.

Kết quả v1.1.5 làm giả thuyết “chỉ NPC Lâu Lan bị lỗi” không còn đủ sức giải thích bug. Điều tra chuyển sang execution boundary chung của external bridge.

## v1.1.6 thay đổi gì

Canonical client KB xác nhận `WH_GETMESSAGE` có thể là producer context hợp lệ, nhưng mutable gameplay/UI callback nên đi qua:

`construct legitimate System.Action -> MainThread.Execute(Action) -> return from hook -> Unity Update -> Action.Invoke`.

Vì vậy v1.1.6:

1. Không coi direct mutation bên trong hook là authoritative Treatment action.
2. Trước tiên enqueue một Action vô hại `CancellationTokenSource.Cancel()`.
3. Hook trả về; tick sau poll `IsCancellationRequested`.
4. Chỉ khi proof thấy `false -> true`, bridge mới cho phép Treatment.
5. Tìm **UIButton đang sống** có text `Trị liệu`.
6. Tạo `System.Action(target=button, callback=HandleClickEvent)` hợp lệ.
7. Enqueue vào `FGStudio.Engine.Utilities.MainThread.Execute(Action)`.
8. Để Unity Update của game tự gọi event ở frame sau.
9. State machine chỉ tiến khi thấy UI/state thực sự đổi; flicker không được coi là success.

Đây **không phải** retry y hệt v1.1.0: callback có thể giống, nhưng execution boundary đã thay đổi từ direct/re-entrant hook invocation sang game-owned MainThread Update queue.

## NPC test

- captured Map 5 -> NPC 339 Đỗ Thanh Đằng.
- captured Map 3 -> NPC 463 Long Phá Thiên.
- X/Y không hardcode; do người dùng bấm `TỰ LẤY TỌA ĐỘ NPC`.
- map khác -> fail-closed.

## Tài liệu cần xem

- `PROJECT_KNOWLEDGE.md`
- `docs/features/AUTO_HEAL_NPC.md`
- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/decisions/DECISIONS.md`
- `docs/history/VERSION_v1.1.6.md`

Không merge/port sang tool chính cho tới khi full chain `NPC -> Trị liệu -> follow-up -> completion proof` đạt RUNTIME PASS lặp lại mà không crash/disconnect.

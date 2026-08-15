# v1.1.5-test — Long Phá Thiên / Lạc Dương isolation test

## User Request
v1.1.4 vẫn chỉ làm màn hình/dialog nháy. Người test nghi NPC Đỗ Thanh Đằng ở Lâu Lan có thể có vấn đề riêng và yêu cầu thêm Long Phá Thiên tại Lạc Dương; tọa độ vẫn do người dùng tự lấy.

## Mandatory Knowledge Protocol
Đã đọc lại AI_PROJECT_KNOWLEDGE_PROTOCOL trước khi sửa, đọc `PROJECT_KNOWLEDGE.md` hiện tại và canonical client `AI_INDEX.md`/database trước khi tra NPC. Không broad reverse binary.

## Canonical Data Source
`ngmthang-g/clinent-game-than-long-DATA-2222/database/npcs/NPCS_0401_0600.csv` xác nhận:
- NPC/ResID `463`
- Name `Long Phá Thiên`
- ResName `PuTongXiaShi2`
- AutoPath MapID `3`
- Map `Lạc Dương`

Đỗ Thanh Đằng vẫn là NPC/ResID `339`, MapID `5` Lâu Lan.

## Confidence
- CONFIRMED STATIC: identity/map của 463 và 339.
- HYPOTHESIS: Long Phá Thiên 463 có cùng dịch vụ `Trị liệu`. Canonical healer-family database không xếp 463 vào LangZhong/MingYi, nên chỉ runtime mới được phép promote mapping này.

## Code Change
Không thay action layer v1.1.4. Chỉ thay cách chọn NPC test:

- target do user bấm `TỰ LẤY TỌA ĐỘ NPC` vẫn lưu raw MapID/X/Y;
- captured MapID `3` -> runtime `ClickNPC(463)` Long Phá Thiên;
- captured MapID `5` -> runtime `ClickNPC(339)` Đỗ Thanh Đằng;
- map khác -> fail-closed, không đoán NPC;
- không hardcode X/Y.

Một hotfix macro tạm trong `src/controller.cpp` đã bị loại bỏ. Mapping hiện nằm trực tiếp trong `src/controller_part04.inc` qua `HealNpcIdForCapturedMap()` / `HealNpcNameForCapturedMap()`.

## Test Design
A/B test một biến:

`same route + same semantic GameDialog action + different NPC/map`

- Nếu 463 works còn 339 fails -> có bằng chứng mạnh cần điều tra khác biệt NPC/server-dialog của Lâu Lan.
- Nếu 463 vẫn nháy/fail giống 339 -> giả thuyết lỗi riêng NPC Lâu Lan yếu đi; quay lại targeted trace action/state chung, không đổi NPC liên tục.

## Files Changed
- `src/controller.cpp`
- `src/controller_part04.inc`
- `src/controller_part05.inc`
- `build.cmd`
- `.github/workflows/build.yml`
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
- `VERSION_v1.1.5.md`

## Runtime Status
NEEDS USER TEST.

## Test Procedure
1. Tới Lạc Dương và đứng cạnh Long Phá Thiên.
2. Bấm `TỰ LẤY TỌA ĐỘ NPC` để target lưu MapID 3 + raw X/Y.
3. Bấm `AUTO TRỊ LIỆU`.
4. Xác nhận log phải ghi `NPC 463 Long Phá Thiên` trước khi mở dialog.
5. Gửi log từ `AUTO TRỊ LIỆU START` đến khi fail/complete.

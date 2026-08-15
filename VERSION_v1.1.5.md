# v1.1.5-test — Long Phá Thiên / Lạc Dương isolation test

## User Request
v1.1.4 vẫn chỉ làm màn hình/dialog nháy. Người test nghi NPC Đỗ Thanh Đằng ở Lâu Lan có thể có vấn đề riêng và yêu cầu thử NPC Long Phá Thiên tại Lạc Dương; tọa độ vẫn do người dùng tự lấy.

## Mandatory Knowledge Protocol
Đã đọc lại AI_PROJECT_KNOWLEDGE_PROTOCOL trước khi sửa. Đã đọc `PROJECT_KNOWLEDGE.md` hiện tại và canonical client `AI_INDEX.md` trước khi tra NPC.

## Canonical Data Source
Không broad reverse binary.

Tra `ngmthang-g/clinent-game-than-long-DATA-2222/database/npcs/NPCS_0401_0600.csv`:
- NPC/ResID `463`
- Name `Long Phá Thiên`
- ResName `PuTongXiaShi2`
- AutoPath MapID `3`
- Map name `Lạc Dương`

## Confidence
- CONFIRMED STATIC: identity 463 / Long Phá Thiên / MapID 3 Lạc Dương.
- HYPOTHESIS: NPC 463 có thể cung cấp cùng dialog/dịch vụ `Trị liệu`. Canonical `NPC_SERVICE_CANDIDATES.md` không xếp 463 vào LangZhong/MingYi healer family.
- Therefore this version is an isolation test, not a newly VERIFIED healer mapping.

## Code Change
The v1.1.4 semantic GameDialog action layer is intentionally unchanged.

`src/controller.cpp` overrides only the active runtime NPC used by the heal FSM:
- legacy source constant remains documented as 339;
- runtime `kHealNpcID` token for the heal FSM is overridden to `463`;
- old saved target is deliberately not auto-loaded in v1.1.5, forcing a fresh `TỰ LẤY TỌA ĐỘ NPC` capture in Lạc Dương;
- X/Y remain raw runtime user-captured coordinates; no X/Y is hardcoded.

## Test Design
This version tests one variable:

`same route + same GameDialog semantic action + different NPC/map`

If 463 works while 339 fails, investigate NPC/server-dialog difference.
If 463 produces the same flicker/failure, the hypothesis that Lâu Lan NPC 339 alone is the root cause becomes much weaker; next work should return to targeted action/state tracing rather than changing NPCs repeatedly.

## Build Status
PENDING GitHub Actions at document creation time.

## Runtime Status
NEEDS USER TEST.

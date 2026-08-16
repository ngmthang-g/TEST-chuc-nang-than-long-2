# v1.1.5-test — Long Phá Thiên / Lạc Dương isolation test

## User Request
v1.1.4 vẫn chỉ làm màn hình/dialog nháy. Người test nghi NPC Đỗ Thanh Đằng ở Lâu Lan có thể có vấn đề riêng và yêu cầu thêm Long Phá Thiên tại Lạc Dương; tọa độ vẫn do người dùng tự lấy.

## Mandatory Knowledge Protocol
Đã đọc project Knowledge Protocol, `PROJECT_KNOWLEDGE.md` và canonical client `AI_INDEX.md`/database trước khi tra NPC. Không broad reverse binary.

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
- UNKNOWN as a canonical healer classification: Long Phá Thiên 463 không thuộc LangZhong/MingYi family trong static healer candidate list; version này dùng 463 làm A/B runtime target theo yêu cầu người dùng.

## Code Change
Không thay action layer v1.1.4. Chỉ thay cách chọn NPC test:
- target do user bấm `TỰ LẤY TỌA ĐỘ NPC` lưu raw MapID/X/Y;
- captured MapID `3` -> `ClickNPC(463)` Long Phá Thiên;
- captured MapID `5` -> `ClickNPC(339)` Đỗ Thanh Đằng;
- map khác -> fail-closed;
- không hardcode X/Y.

## Test Design
A/B test một biến:
`same route + same semantic GameDialog action + different NPC/map`.

Acceptance logic before test:
- nếu 463 works còn 339 fails -> investigate NPC/server-dialog difference;
- nếu 463 vẫn nháy/fail giống 339 -> hypothesis lỗi riêng NPC Lâu Lan bị mạnh mẽ bác bỏ/giảm độ tin cậy; quay lại common action bridge/state boundary.

## Build Status
GitHub Actions run `31909204317` on clean source:
- Architecture audit: PASS.
- Route FSM: PASS.
- Heal FSM: PASS.
- Bridge DLL build/PE check: PASS.
- Controller EXE: PASS.
- Artifact upload: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.5`.
- Artifact SHA256: `652a12e2f454d1d8bee6d5025512825d98262e38a567d69b6597b1143daedf93`.

## Runtime Result — USER CONFIRMED
**RUNTIME FAIL at Treatment.**

User report after testing Long Phá Thiên/Lạc Dương:
- NPC/dialog opens;
- tool action causes the Treatment screen/dialog to **flicker**;
- no Treatment progression;
- symptom is effectively the same as the previous Đỗ Thanh Đằng/Lâu Lan flow.

## Evidence / Conclusion
- CONFIRMED: changing from NPC 339/Map5 to NPC 463/Map3 while keeping the action layer unchanged did **not** change the failure symptom.
- DISPROVEN/strongly weakened: “NPC 339 or Lâu Lan alone is the root cause.”
- NOT PROVEN: exact common root cause.
- Next correct investigation: shared action execution boundary / state lifecycle; do not continue rotating NPCs as the primary diagnostic.

## Superseded By
`v1.1.6-test` — MainThread queued UI action experiment with CTS bridge proof.

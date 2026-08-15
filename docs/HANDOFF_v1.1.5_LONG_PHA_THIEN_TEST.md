# HANDOFF v1.1.5 — Long Phá Thiên / Lạc Dương isolation test

## Purpose
Test whether the repeated Auto-heal failure is specific to Đỗ Thanh Đằng (NPC 339, Lâu Lan) by running the same v1.1.4 semantic GameDialog action path against another NPC requested by the user.

## Exact Static Data
Canonical client database source: `database/npcs/NPCS_0401_0600.csv`.

- NPC/ResID: `463`
- Name: `Long Phá Thiên`
- ResName: `PuTongXiaShi2`
- MapID: `3`
- Map: `Lạc Dương`

Do not change these based on memory or guesswork without new canonical/runtime evidence.

## Important Confidence Boundary
NPC identity/map are CONFIRMED STATIC.

Long Phá Thiên is NOT listed in the canonical LangZhong/MingYi healer candidate families. Treat his ability to expose `Trị liệu` as a HYPOTHESIS until the live dialog proves it.

## Coordinate Rule
No NPC X/Y is hardcoded.

For v1.1.5 the user must:
1. go to Lạc Dương;
2. stand at the desired interaction position near Long Phá Thiên;
3. press `TỰ LẤY TỌA ĐỘ NPC`;
4. then run `AUTO TRỊ LIỆU`.

The controller deliberately ignores the previously persisted target at startup so an old Lâu Lan coordinate cannot be reused accidentally.

## Code Location
- `src/controller.cpp`: v1.1.5 test override; active heal FSM NPC becomes 463 and old target autoload is suppressed.
- `src/bridge_heal_packet_v1_1_4.inc`: unchanged semantic GameDialog selection mechanism.
- `src/controller_part04.inc` / `controller_part05.inc`: existing route + heal FSM donor.

## Action Layer Preserved
Do NOT alter the semantic action path in this isolation version:

`ClickNPC(463) -> wait live GameDialog -> identify "Trị liệu" -> obtain active selectionID -> CMD_SHOW_GAMEDIALOG 100007 -> wait next state`

The point of v1.1.5 is to change NPC/map only. Changing both NPC and action mechanism would destroy the diagnostic value of the test.

## Interpretation of Runtime Result
### If Long Phá Thiên works
The difference between NPC 339/Lâu Lan and 463/Lạc Dương becomes high-value runtime evidence. Capture the exact GameDialog text/selection/state differences before changing production code.

### If Long Phá Thiên shows the same flicker/failure
Mark the NPC-specific hypothesis as weakened/failed. Return to targeted tracing of the semantic request/state lifecycle; do not keep trying random NPCs.

## Status
BUILD: pending CI at document creation.
RUNTIME: NEEDS USER TEST.

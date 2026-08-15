# CURRENT PROJECT KNOWLEDGE

## Project Purpose
Repo test độc lập cho các cơ chế Thần Long Mobile. Chức năng chỉ được mang sang tool chính sau khi runtime test ổn định. Mỗi version phải kế thừa cả source lẫn tri thức kỹ thuật.

## Current Version
v1.1.5-test — dual-map healer NPC isolation test. BUILD đang chờ CI cho source sạch hiện tại; RUNTIME NEEDS USER TEST.

## Current Status
- RUNTIME PARTIAL PASS: runtime coordinate capture + AutoPath + xuống ngựa + `ClickNPC(339)` mở đúng Đỗ Thanh Đằng/GameDialog ở Lâu Lan.
- RUNTIME FAIL: v1.1.0–v1.1.2 `UIButton.HandleClickEvent()` không qua `Trị liệu`; UI nháy.
- RUNTIME FAIL: v1.1.3 `ExecuteUIObject(GameDialog.FunctionButtonClicked)` vẫn nháy, không qua Treatment.
- RUNTIME FAIL: v1.1.4 semantic `selectionID` + `CMD_SHOW_GAMEDIALOG=100007` trên NPC 339/Lâu Lan theo user report vẫn chỉ nháy, chưa qua Treatment.
- v1.1.5 giữ nguyên action layer v1.1.4 và đổi biến test NPC/map: captured Map 3 -> Long Phá Thiên 463; captured Map 5 -> Đỗ Thanh Đằng 339.

## Canonical Knowledge Source
Bắt buộc đọc `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`, `research/VERIFIED.md` và database/feature đúng task trước khi nghiên cứu. Với UI/auto flow ưu tiên:
`Lua source -> Config/Layout semantic data -> exact handler/payload -> targeted native reverse only if exact fact still missing`.
Không broad reverse lại client.

## Architecture
`Controller -> shared command -> WH_GETMESSAGE callback đúng target game thread -> metadata-driven IL2CPP bridge -> semantic game action -> observer/state machine`.

v1.1.5 auto-heal flow:
`capture raw MapID/X/Y -> choose NPC by captured map -> route -> ClickNPC(npcID) -> inspect live GameDialog -> read live selectionID -> send verified semantic request -> wait next real UI/state`.

## Confirmed Technical Facts
- CONFIRMED STATIC: Đỗ Thanh Đằng = NPC/ResID `339`, MapID `5` Lâu Lan.
- CONFIRMED STATIC: Long Phá Thiên = NPC/ResID `463`, `ResName=PuTongXiaShi2`, MapID `3` Lạc Dương.
- CONFIRMED RUNTIME: `ClickNPC(339)` opens intended Lâu Lan NPC dialog.
- CONFIRMED: NPC X/Y must not be inferred/hardcoded; user captures raw runtime MapID/X/Y into `ThanLongAutoHeal.target.tsv`.
- CONFIRMED canonical KB: dynamic `GameDialog` has `Selections[selectionID] = visibleText`; generated button stores selectionID in `Tag`.
- CONFIRMED Lua source: GameDialog selection sends `CMD_SHOW_GAMEDIALOG = 100007` payload `selectionID:SelectedItemID`; ordinary no-award choice defaults `SelectedItemID=-1`.
- CONFIRMED: `LuaSystemAPI_Network.SendPacket(packetID,data)` is outbound Lua network bridge.
- CONFIRMED asset: `MessageBox` stores semantic OK/Cancel callbacks and has `ButtonOKClicked()`.
- CONFIRMED RULE: BUILD PASS != RUNTIME PASS.

## Important IDs / Maps
- `339` = Đỗ Thanh Đằng = Map `5` Lâu Lan.
- `463` = Long Phá Thiên = Map `3` Lạc Dương.
- `CMD_SHOW_GAMEDIALOG = 100007`.

## Confidence / Hypotheses
- CONFIRMED STATIC: identity/map of NPC 463.
- HYPOTHESIS: Long Phá Thiên 463 provides the same `Trị liệu` service. Canonical `NPC_SERVICE_CANDIDATES.md` does not classify 463 in LangZhong/MingYi healer families. Do not call it a verified healer until runtime proves it.
- HYPOTHESIS under test: failure may be specific to NPC 339/Lâu Lan rather than shared action layer.

## Important Files
- `src/controller_part04.inc` — v1.1.5 map->NPC selection and heal runtime FSM.
- `src/controller_part05.inc` — continuation of runtime FSM/controller.
- `src/controller.cpp` — clean includes only; legacy macro NPC override removed.
- `src/bridge_heal_packet_v1_1_4.inc` — semantic selectionID packet implementation retained unchanged for isolation test.
- `src/bridge_part03.inc` / `src/bridge_part04.inc` — UI discovery and historical callback path.
- `src/heal_logic.h` — pure heal-dialog FSM.
- `src/protocol.h` — shared command/snapshot protocol.
- `VERSION_v1.1.5.md` — current version test design.
- `CHANGELOG.md` — short history.

## Working Mechanisms
- Runtime coordinate capture/persistence.
- Clean Route AutoPath.
- StopPath / dismount.
- `ClickNPC(339)` runtime opening in Lâu Lan.
- Live GameDialog discovery by text.
- Source/build path for semantic `selectionID` request.

## Failed / Unsafe Mechanisms

### FAILED-001 — inferred coordinate scale
**Attempt:** infer displayed 294:172 as 29400,17200.
**Result:** wrong coordinate.
**Lesson:** raw runtime capture only.

### FAILED-002 — `UIButton.HandleClickEvent()` as Treatment business action
**Versions:** v1.1.0–v1.1.2.
**Result:** UI flicker, no Treatment transition.
**Lesson:** visual click is not semantic success.
**Retry Conditions:** only after trace proves it reaches same business request.

### FAILED-003 — UI resolver-only fix
**Version:** v1.1.2.
**Result:** UI discovery improved/build PASS, Treatment still FAIL.
**Lesson:** discovery alone was not root cause.

### FAILED-004 — `ExecuteUIObject(GameDialog.FunctionButtonClicked(liveButton))`
**Version:** v1.1.3.
**Result:** same flicker, no Treatment transition.
**Lesson:** callback source is useful documentation of semantic request; current executor invocation is not authoritative runtime success.

### FAILED-005 — v1.1.4 semantic packet on NPC 339/Lâu Lan
**Attempt:** resolve live treatment button, read `Tag=selectionID`, send source-verified `CMD_SHOW_GAMEDIALOG 100007` payload `selectionID:-1`.
**Result:** user reports same screen/dialog flicker and no progression.
**Failure Cause:** NOT CONFIRMED. Could be NPC/server-dialog-specific, wrong runtime data exposure, lifecycle issue, or action path despite source-correct payload.
**Lesson:** do not immediately rewrite action layer again; isolate NPC/map variable first.
**Retry Conditions:** after A/B test with NPC 463 or targeted trace of actual manual Treatment request.

### DEPRECATED IMPLEMENTATION — macro NPC override hotfix
An intermediate v1.1.5 draft modified `src/controller.cpp` with preprocessor overrides for `kHealNpcID`/target loading. It was removed before current handoff because it was brittle and risked recursive macro behavior. Current mapping is explicit in `controller_part04.inc`.

## Architectural Rules
- Read uploaded Knowledge Protocol + project KB before code changes.
- Read canonical client `AI_INDEX.md` + VERIFIED/database before reverse.
- No broad binary reverse when exact fact exists in docs/database.
- Only targeted binary/metadata analysis for exact missing facts.
- No hardcoded NPC X/Y.
- No fixed/guessed Treatment selectionID.
- No guessed packet payload.
- No stale UI pointer across UI transition.
- No `Sleep` on game callback thread as readiness proof.
- One mutable action at a time; advance on concrete state evidence.
- BUILD PASS != RUNTIME PASS.
- Preserve failed attempts/corrections.

## Known Bugs / Limitations
- Full auto-heal chain has not achieved runtime PASS.
- Long Phá Thiên 463 healer service semantics are not yet runtime verified.
- Confirmation branch after successful Treatment has not been runtime reached in current test series.
- Existing saved target file does not store NPC ID; v1.1.5 intentionally derives NPC from captured MapID (3 or 5). Map other than 3/5 fails closed.

## Open Research Questions
1. Does NPC 463/Map 3 expose a `Trị liệu` GameDialog and advance with the unchanged v1.1.4 semantic action?
2. If 463 also fails identically, what exact outbound request occurs when user manually clicks `Trị liệu`?
3. Does local GameDialog lifecycle need a separate semantic close/transition after direct request?
4. What exact confirmation UI follows a successful Treatment on each NPC/server dialog?

## Next Development Priorities
1. Build clean v1.1.5 source in GitHub Actions.
2. User goes to Long Phá Thiên in Lạc Dương, presses `TỰ LẤY TỌA ĐỘ NPC`, confirms saved target MapID is 3.
3. Run Auto trị liệu and capture log beginning at `AUTO TRỊ LIỆU START • NPC 463 Long Phá Thiên`.
4. Compare runtime behavior with NPC 339/Lâu Lan.
5. If both fail identically, stop changing NPCs and perform targeted trace of one legitimate manual Treatment action only.

---

# VERSION HISTORY

## VERSION 1.1.0-test — 2026-08-16
Initial route/NPC/GameDialog auto-heal test using NPC 339.

## VERSION 1.1.1-test — 2026-08-16
Removed inferred X/Y; added user runtime coordinate capture/persistence.

## VERSION 1.1.2-test — 2026-08-16
UI resolver adjustment. Route/NPC open PARTIAL PASS; Treatment FAIL.

## VERSION 1.1.3-test — 2026-08-16
Exact Lua callback through ExecuteUIObject. Build PASS; runtime Treatment FAIL with same flicker.

## VERSION 1.1.4-test — 2026-08-16
Semantic dynamic GameDialog request using live `selectionID` + verified packet/payload. Build PASS. Runtime on NPC 339/Lâu Lan reported FAIL with same flicker.

## VERSION 1.1.5-test — 2026-08-16
### User Requests
Force-read MD/knowledge; suspect Lâu Lan NPC; add Long Phá Thiên Lạc Dương while keeping user-captured coordinates.
### Initial State
v1.1.4 NPC 339 still flickers/fails at Treatment.
### Investigation
Canonical NPC database directly confirms Long Phá Thiên = 463, MapID 3 Lạc Dương; no binary reverse required. NPC_SERVICE_CANDIDATES does not verify 463 as healer.
### Solution
Keep action layer unchanged. Select NPC from captured target map: Map3->463, Map5->339; other maps fail closed.
### Files Changed
`src/controller.cpp`, `src/controller_part04.inc`, `src/controller_part05.inc`, `build.cmd`, `.github/workflows/build.yml`, `VERSION_v1.1.5.md`, `CHANGELOG.md`, `PROJECT_KNOWLEDGE.md`.
### Data Sources
Uploaded AI Project Knowledge Protocol; project KB; canonical `AI_INDEX.md`; `database/npcs/NPCS_0401_0600.csv`; `NPC_SERVICE_CANDIDATES.md`.
### Test Results
BUILD pending for current clean source; RUNTIME NEEDS USER TEST.
### Recommended Next Step
Fresh coordinate capture next to Long Phá Thiên in Lạc Dương, then runtime A/B test.

---

# DECISION LOG

## DECISION-001
**Status:** ACTIVE
**Decision:** NPC target position is captured from runtime, never inferred from displayed coordinate scaling.

## DECISION-002
**Status:** SUPERSEDED by DECISION-004
**Decision:** Prefer exact GameDialog Lua callback over generic UIButton click.

## DECISION-003
**Status:** ACTIVE
**Decision:** Never solve UI readiness by sleeping on game callback thread; gate across observations/ticks.

## DECISION-004
**Status:** ACTIVE
**Decision:** For dynamic GameDialog, use live server-provided selection semantics and source-verified packet/payload rather than guessed button behavior.

## DECISION-005
**Status:** ACTIVE
**Decision:** Canonical client repo is read-first; native reverse is targeted-only for exact missing facts.

## DECISION-006
**Date:** 2026-08-16
**Status:** ACTIVE
**Decision:** v1.1.5 isolates NPC/map without changing v1.1.4 action layer. Captured MapID selects the test NPC (3->463, 5->339).
**Context:** v1.1.4 still fails on 339/Lâu Lan and user suspects NPC-specific issue.
**Alternatives Considered:** rewrite packet/action again; hardcode Long Phá Thiên coordinates; replace 339 globally.
**Why Rejected:** those alter multiple variables or violate runtime-coordinate rule.
**Consequences:** v1.1.5 is an A/B test. Runtime results determine whether NPC-specific investigation is justified.

---

# CORRECTION LOG

## CORRECTION-001
**Old Knowledge:** live UIButton `HandleClickEvent()` could be sufficient for Treatment.
**New Finding:** runtime disproved it.
**Status:** DEPRECATED/FAILED.

## CORRECTION-002
**Old Knowledge:** source-correct `FunctionButtonClicked` through current ExecuteUIObject bridge was likely complete fix.
**New Finding:** runtime v1.1.3 still fails.
**Status:** DEPRECATED as authoritative action path for this feature.

## CORRECTION-003
**Old Knowledge:** v1.1.4 semantic packet path was awaiting runtime test.
**New Finding:** user reports v1.1.4 on NPC 339/Lâu Lan still only flickers and does not advance.
**Evidence:** user runtime report immediately before v1.1.5 request.
**Affected Version:** v1.1.4.
**Impact:** mark runtime FAIL on 339/Lâu Lan; retain action layer only for controlled A/B test with 463/Lạc Dương.

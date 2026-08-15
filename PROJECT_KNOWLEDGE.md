# CURRENT PROJECT KNOWLEDGE

## Project Purpose
Repo test độc lập cho các cơ chế Thần Long Mobile. Chức năng chỉ được mang sang tool chính sau khi runtime test ổn định. Mỗi version phải lưu cả code lẫn tri thức kỹ thuật.

## Current Version
v1.1.2-test (runtime FAIL ở bước chọn Trị liệu). Version kế tiếp đang nghiên cứu: v1.1.3-test.

## Current Status
- BUILD PASS: v1.1.2 GitHub Actions.
- Route FSM: 8/8 PASS.
- Heal FSM pure logic: 7/7 PASS.
- RUNTIME PARTIAL PASS: AutoPath tới tọa độ tự lấy + ClickNPC(339) mở đúng GameDialog.
- RUNTIME FAIL: chưa thực hiện được lựa chọn `Trị liệu`.

## Architecture
Controller -> shared command -> WH_GETMESSAGE callback trên target window thread -> bridge IL2CPP runtime resolve -> action nội bộ game.

Flow Auto trị liệu:
`Snapshot -> Route FSM -> AutoPath -> StopPath -> Dismount -> ClickNPC(339) -> GameDialog -> Treatment -> MessageBox confirm -> GameDialog ack`.

## Confirmed Technical Facts
- CONFIRMED: Đỗ Thanh Đằng dùng NPC/ResID 339 từ Config client.
- CONFIRMED: MapID Lâu Lan = 5 trong Config client.
- CONFIRMED: tọa độ không được hardcode; user tự lấy MapID/X/Y runtime và lưu `ThanLongAutoHeal.target.tsv`.
- CONFIRMED RUNTIME: `ClickNPC(339)` mở đúng NPC/GameDialog.
- CONFIRMED ASSET: `GameDialog:FunctionButtonClicked(uiButton)` lấy `uiButton.Tag`, gửi `CMD_SHOW_GAMEDIALOG`, rồi destroy GameDialog.
- CONFIRMED ASSET: MessageBox có callback `ButtonOKClicked()` cho xác nhận.
- CONFIRMED HISTORICAL SOURCE: bản AutoTrain v0.8.9 đã dùng `GameDialog.FunctionButtonClicked(button)` và `MessageBox.ButtonOKClicked()` thay cho click pointer-event ở chuỗi trị liệu.

## Important Files
- `src/bridge_part03.inc`: Lua UI discovery / UI tree.
- `src/bridge_part04.inc`: heal dialog inspect + current heal action.
- `src/controller_part04.inc`: heal runtime FSM orchestration.
- `src/protocol.h`: bridge commands.
- `docs/CLIENT_EVIDENCE_v1.1.0.md`: evidence client.
- `docs/HANDOFF_v1.1.0_AUTO_TRI_LIEU.md`: handoff feature.

## Important Methods / Functions
- `LuaSystemAPI_Game.ClickNPC(Int32)`
- `LuaSystemAPI_GUI.FindUI(String)`
- `LuaSystemAPI_GUI.MainFindUI(String)`
- `MonoBehaviourExecutor.ExecuteUIObject(...)` / historical native bridge equivalent
- `GameDialog.FunctionButtonClicked(uiButton)`
- `MessageBox.ButtonOKClicked()`
- `UIButton.HandleClickEvent()` — do not assume this equals the Lua business action.

## Important IDs
- Heal NPC ResID: 339.
- MapID Lâu Lan: 5.

## Runtime Flow
Current intended flow:
`ReadState -> route to captured target -> ClickNPC(339) -> wait live GameDialog -> resolve live Treatment button -> invoke GameDialog.FunctionButtonClicked(button) -> wait MessageBox -> invoke ButtonOKClicked -> wait GameDialog "Ta biết rồi" -> FunctionButtonClicked(button)`.

## Working Mechanisms
- Runtime coordinate capture.
- AutoPath route.
- StopPath / dismount.
- ClickNPC(339).
- GameDialog visibly opens.

## Failed / Unsafe Mechanisms
### FAILED-001 — Hardcoded NPC coordinate scaling
Attempt: infer UI coordinate 294:172 as 29400,17200.
Result: user reported coordinate wrong.
Lesson: capture raw runtime MapID/X/Y; never infer scale.

### FAILED-002 — v1.1.0/v1.1.2 `UIButton.HandleClickEvent()` for Treatment
Attempt: find live button by text then call `HandleClickEvent()` on that instance.
Reason: avoid stale pointer while still using original UIButton event.
Result: runtime opens NPC but does not advance through Treatment. User observes the treatment dialog/button visually flicker, suggesting UI-level reaction without confirmed Lua/server business action.
Status: FAILED for this feature until contrary evidence.
Lesson: use the exact Lua action `GameDialog.FunctionButtonClicked(uiButton)` instead of treating UIButton visual click as authoritative business action.
Retry Conditions: only reconsider HandleClickEvent if runtime trace proves it reaches the same Lua callback reliably on this client.

### FAILED-003 — v1.1.2 UI resolver alone
Attempt: change GameDialog lookup order to FindUI -> MainFindUI -> executor.
Result: BUILD PASS; runtime still cannot complete Treatment.
Lesson: discovery was not the only problem; action layer remains wrong/incomplete.

## Architectural Rules
- No hardcoded RVA for new implementation; resolve metadata/runtime by class/method where practical.
- No screen-coordinate mouse click.
- No stale UI pointer across UI transition.
- No packet payload guessing when selectionID/payload is server-dialog data.
- Build PASS != Runtime PASS.
- Failed attempts remain documented.

## Known Bugs
- Auto trị liệu stops at/around Treatment selection after NPC dialog opens.

## Open Research Questions
- Exact managed signature/name of `MonoBehaviourExecutor.ExecuteUIObject` on this build when resolved via IL2CPP metadata.
- Whether MessageBox is always used after Treatment or any server variant returns a GameDialog confirm.

## Next Development Priorities
1. Replace Treatment/Ack `UIButton.HandleClickEvent` with exact Lua `GameDialog.FunctionButtonClicked(liveButton)`.
2. Replace MessageBox confirm button click with exact Lua `MessageBox.ButtonOKClicked()`.
3. Keep live-button re-resolution each state and fail closed.
4. Runtime test full chain repeatedly before porting to main.

---

# VERSION HISTORY

# VERSION 1.1.0-test — 2026-08-16
## User Requests
Auto trị liệu Đỗ Thanh Đằng, test repo only.
## Technical Findings
NPC 339; MapID 5; dynamic GameDialog buttons.
## Test Results
Build/FSM PASS; runtime initially not verified.

# VERSION 1.1.1-test — 2026-08-16
## User Requests
Do not hardcode NPC coordinate; add user runtime capture.
## Solution
Persist raw MapID/X/Y to `ThanLongAutoHeal.target.tsv`.
## Test Results
Runtime route later confirmed sufficient to open NPC.

# VERSION 1.1.2-test — 2026-08-16
## Problems Reported
NPC opens but Treatment does not proceed.
## Investigation
Adjusted visible GameDialog resolver to `FindUI -> MainFindUI -> executor`.
## Test Results
BUILD PASS, Route FSM 8/8, Heal FSM 7/7. Runtime still FAIL at Treatment.
## New Knowledge Learned
User reports visible dialog/button flicker when tool attempts the Treatment phase. This is evidence that visual UI reaction must not be equated with business-action success.
## Recommended Next Step
Use exact Lua callbacks rather than UIButton.HandleClickEvent.

---

# DECISION LOG

## DECISION-001
**Date:** 2026-08-16
**Status:** ACTIVE
**Decision:** NPC coordinates are captured from runtime and never inferred from displayed coordinates.
**Reason:** prior inferred scale was wrong.

## DECISION-002
**Date:** 2026-08-16
**Status:** ACTIVE
**Decision:** For GameDialog business choices, prefer exact Lua callback `FunctionButtonClicked(liveButton)` over generic `UIButton.HandleClickEvent()` once the callback is known from asset evidence.
**Context:** runtime v1.1.0–v1.1.2 opens NPC but does not advance Treatment; user sees UI flicker.
**Consequences:** future code must resolve the live button each dialog and pass it to the GameDialog Lua action.

---

# CORRECTION LOG

## CORRECTION-001
**Old Knowledge:** Finding a live UIButton and calling `HandleClickEvent()` was expected to be sufficient for Treatment.
**New Finding:** Runtime v1.1.2 shows it is not sufficient on this client/flow; exact Lua callback is required or at least the next evidence-backed path.
**Evidence:** NPC opens correctly, Treatment does not advance, user observes visual flicker; historical v0.8.9 implementation used GameDialog.FunctionButtonClicked.
**Affected Versions:** v1.1.0–v1.1.2.
**Impact:** mark HandleClickEvent path FAILED for Auto trị liệu and replace in v1.1.3.

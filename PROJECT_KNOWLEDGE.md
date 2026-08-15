# CURRENT PROJECT KNOWLEDGE

## Project Purpose
Repo test độc lập cho các cơ chế Thần Long Mobile. Chức năng chỉ được mang sang tool chính sau khi runtime test ổn định. Mỗi version phải lưu cả code lẫn tri thức kỹ thuật.

## Current Version
v1.1.3-test — exact Lua callback implementation. BUILD PASS, RUNTIME NEEDS USER TEST.

## Current Status
- BUILD PASS: v1.1.3 GitHub Actions run 31905470360.
- Route FSM: 8/8 PASS.
- Heal FSM pure logic: 7/7 PASS.
- RUNTIME PARTIAL PASS inherited from v1.1.2: AutoPath tới tọa độ tự lấy + ClickNPC(339) mở đúng GameDialog.
- RUNTIME NEEDS USER TEST: v1.1.3 Lua callback chain chưa được test thực tế.

## Architecture
Controller -> shared command -> WH_GETMESSAGE callback trên target window thread -> bridge IL2CPP runtime resolve -> action nội bộ game.

Flow Auto trị liệu:
`Snapshot -> Route FSM -> AutoPath -> StopPath -> Dismount -> ClickNPC(339) -> GameDialog.FunctionButtonClicked(Treatment) -> MessageBox.ButtonOKClicked -> GameDialog.FunctionButtonClicked(Ack)`.

## Confirmed Technical Facts
- CONFIRMED: Đỗ Thanh Đằng dùng NPC/ResID 339 từ Config client.
- CONFIRMED: MapID Lâu Lan = 5 trong Config client.
- CONFIRMED: tọa độ không được hardcode; user tự lấy MapID/X/Y runtime và lưu `ThanLongAutoHeal.target.tsv`.
- CONFIRMED RUNTIME: `ClickNPC(339)` mở đúng NPC/GameDialog.
- CONFIRMED ASSET: `GameDialog:FunctionButtonClicked(uiButton)` lấy `uiButton.Tag`, gửi `CMD_SHOW_GAMEDIALOG`, rồi destroy GameDialog.
- CONFIRMED ASSET: MessageBox có callback `ButtonOKClicked()` cho xác nhận.
- CONFIRMED HISTORICAL SOURCE: bản AutoTrain v0.8.9 đã dùng `GameDialog.FunctionButtonClicked(button)` và `MessageBox.ButtonOKClicked()` thay cho click pointer-event ở chuỗi trị liệu.
- CONFIRMED BUILD: v1.1.3 same-process bridge compiles with managed Object[] support and exact Lua callback path.

## Important Files
- `src/bridge_part03.inc`: Lua UI discovery / UI tree.
- `src/bridge_part04.inc`: heal dialog inspect + v1.1.3 exact Lua heal actions.
- `src/bridge_lua_array.inc`: managed `System.Object[]` allocation for ExecuteUIObject args.
- `src/controller_part04.inc`: heal runtime FSM orchestration.
- `src/protocol.h`: bridge commands.
- `docs/CLIENT_EVIDENCE_v1.1.0.md`: evidence client.
- `docs/HANDOFF_v1.1.0_AUTO_TRI_LIEU.md`: historical handoff; its HandleClickEvent business-action guidance is superseded.
- `docs/HANDOFF_v1.1.3_AUTO_TRI_LIEU.md`: current handoff.

## Important Methods / Functions
- `LuaSystemAPI_Game.ClickNPC(Int32)`
- `LuaSystemAPI_GUI.FindUI(String)`
- `LuaSystemAPI_GUI.MainFindUI(String)`
- `MonoBehaviourExecutor.ExecuteUIObject(...)` / `ExecuteUiObject(...)` runtime-name fallback
- `GameDialog.FunctionButtonClicked(uiButton)`
- `MessageBox.ButtonOKClicked()`
- `UIButton.HandleClickEvent()` — FAILED as authoritative business action for this heal flow.

## Important IDs
- Heal NPC ResID: 339.
- MapID Lâu Lan: 5.

## Runtime Flow
Current v1.1.3 intended flow:
`ReadState -> route to captured target -> ClickNPC(339) -> wait live GameDialog -> resolve live Treatment button -> ExecuteUIObject(GameDialog, FunctionButtonClicked, [button]) -> wait MessageBox -> ExecuteUIObject(MessageBox, ButtonOKClicked, []) -> wait live GameDialog "Ta biết rồi" -> ExecuteUIObject(GameDialog, FunctionButtonClicked, [button])`.

## Working Mechanisms
- Runtime coordinate capture.
- AutoPath route.
- StopPath / dismount.
- ClickNPC(339).
- GameDialog visibly opens.
- v1.1.3 code/build path for exact Lua callbacks.

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
Lesson: use exact Lua action `GameDialog.FunctionButtonClicked(uiButton)` instead of treating UIButton visual click as authoritative business action.
Retry Conditions: only reconsider HandleClickEvent if runtime trace proves it reaches the same Lua callback reliably on this client.

### FAILED-003 — v1.1.2 UI resolver alone
Attempt: change GameDialog lookup order to FindUI -> MainFindUI -> executor.
Result: BUILD PASS; runtime still cannot complete Treatment.
Lesson: discovery was not the only problem; action layer remained wrong/incomplete.

## Architectural Rules
- No hardcoded RVA for new implementation; resolve metadata/runtime by class/method where practical.
- No screen-coordinate mouse click.
- No stale UI pointer across UI transition.
- No packet payload guessing when selectionID/payload is server-dialog data.
- Do not Sleep on callback/game thread to wait for UI. If timing stabilization is needed, gate across controller ticks.
- Build PASS != Runtime PASS.
- Failed attempts remain documented.

## Known Bugs
- v1.1.3 runtime behavior is not yet verified; previous versions stop at/around Treatment selection.

## Open Research Questions
- Whether runtime resolves managed name as `ExecuteUIObject` or fallback `ExecuteUiObject`; code supports both.
- Whether MessageBox is always used after Treatment or any server variant returns a GameDialog confirm.
- Whether a multi-tick stability gate is needed after exact Lua callbacks are runtime-tested.

## Next Development Priorities
1. Runtime test v1.1.3 exact Lua Treatment callback.
2. If Treatment succeeds, verify MessageBox confirm and `Ta biết rồi` sequentially.
3. If runtime still fails, use returned bridge detail to determine whether failure is UI lookup, executor method resolve, managed Object[] creation, or Lua callback execution.
4. Do not change route/NPC path unless runtime evidence says it regressed.
5. Only port to main after many successful cycles without crash/disconnect.

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

# VERSION 1.1.3-test — 2026-08-16
## User Requests
Investigate whether Treatment failure could be because the tool clicks too fast; fix the flow.
## Initial State
Route + NPC opening work. Treatment does not advance. UI appears to flicker during attempt.
## Investigation
Controller timer is 500 ms, so “too fast” is not strong enough to call root cause. Historical v0.8.9 source and asset evidence show exact Lua callbacks for the business flow.
## Root Cause
LIKELY: wrong action layer in v1.1.0–v1.1.2 (`UIButton.HandleClickEvent`) rather than raw speed. Runtime confirmation still pending.
## Solution
Replace business choices with `MonoBehaviourExecutor.ExecuteUIObject` calls to exact Lua callbacks.
## Implementation Details
- Treatment/Ack: `GameDialog.FunctionButtonClicked(liveButton)`.
- Confirm: `MessageBox.ButtonOKClicked()`; GameDialog fallback supported.
- Added `System.Object[]` argument creation in `bridge_lua_array.inc`.
- No Sleep added on callback/game thread.
## Files Changed
- `src/bridge.cpp`
- `src/bridge_lua_array.inc`
- `src/bridge_part04.inc`
- `build.cmd`
- `.github/workflows/build.yml`
- `VERSION_v1.1.3.md`
- `docs/HANDOFF_v1.1.3_AUTO_TRI_LIEU.md`
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
## Data Sources
Config client, Interface.unity3d evidence, previous v0.8.9 patch, current runtime user observation.
## Test Results
BUILD PASS; Route FSM 8/8; Heal FSM 7/7; runtime NEEDS USER TEST.
## Bugs Remaining
Exact Lua callback behavior on current running client not yet confirmed.
## Recommended Next Step
Test artifact `ThanLongTestAutoHeal-v1.1.3` and capture full log.

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

## DECISION-003
**Date:** 2026-08-16
**Status:** ACTIVE
**Decision:** Do not solve UI readiness by sleeping on the game callback thread. If timing stabilization is needed, require stable UI across controller ticks.
**Reason:** sleeping the game/UI thread can itself freeze or destabilize UI processing.

---

# CORRECTION LOG

## CORRECTION-001
**Old Knowledge:** Finding a live UIButton and calling `HandleClickEvent()` was expected to be sufficient for Treatment.
**New Finding:** Runtime v1.1.2 shows it is not sufficient on this client/flow; exact Lua callback is the next evidence-backed path.
**Evidence:** NPC opens correctly, Treatment does not advance, user observes visual flicker; historical v0.8.9 implementation used GameDialog.FunctionButtonClicked.
**Affected Versions:** v1.1.0–v1.1.2.
**Impact:** mark HandleClickEvent path FAILED for Auto trị liệu and replace in v1.1.3.

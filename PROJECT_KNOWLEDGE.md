# CURRENT PROJECT KNOWLEDGE

## Project Purpose
Repo test độc lập cho các cơ chế Thần Long Mobile. Chức năng chỉ được mang sang tool chính sau khi runtime test ổn định. Mỗi version phải kế thừa cả source lẫn tri thức kỹ thuật.

## Current Version
v1.1.4-test — semantic dynamic-GameDialog selection path. BUILD PASS; RUNTIME NEEDS USER TEST.

## Current Status
- BUILD PASS: GitHub Actions run `31906609147`.
- Route FSM: 8/8 PASS.
- Heal FSM: 7/7 PASS.
- Bridge DLL compile + PE verification: PASS.
- Controller EXE compile: PASS.
- RUNTIME PARTIAL PASS: AutoPath tới target runtime + xuống ngựa + `ClickNPC(339)` mở đúng NPC/GameDialog.
- RUNTIME FAIL: v1.1.0–v1.1.2 không tiến qua `Trị liệu` bằng `UIButton.HandleClickEvent()`.
- RUNTIME FAIL: v1.1.3 dùng `GameDialog.FunctionButtonClicked(liveButton)` qua `MonoBehaviourExecutor.ExecuteUIObject` nhưng user vẫn chỉ thấy UI nháy và không sang bước xác nhận.
- v1.1.4 chuyển sang semantic request đã VERIFIED từ canonical client KB: đọc runtime `selectionID` từ `UIButton.Tag`, gửi `CMD_SHOW_GAMEDIALOG=100007` với `selectionID:selectedItemID`.

## Canonical Knowledge Source
Trước khi sửa feature phải đọc repo `ngmthang-g/clinent-game-than-long-DATA-2222` theo `AI_INDEX.md`. Với UI/auto flow ưu tiên:
`Lua source -> Config/Layout semantic data -> exact handler/payload -> native reverse only if exact information is still missing`.
Không broad reverse lại client.

## Architecture
`Controller -> shared command -> WH_GETMESSAGE callback đúng target game thread -> metadata-driven IL2CPP bridge -> semantic game action -> observer/state machine`.

Auto-heal intended flow v1.1.4:
`ReadState -> route target -> ClickNPC(339) -> inspect live GameDialog -> match text Trị liệu -> read live button.Tag(selectionID) -> Network.SendPacket(100007, "selectionID:-1") -> wait real confirmation -> confirm -> wait Ta biết rồi -> read new Tag -> SendPacket(100007, "selectionID:-1") -> DONE`.

## Confirmed Technical Facts
- CONFIRMED Config: Đỗ Thanh Đằng uses NPC/ResID `339`; Lâu Lan MapID `5`.
- CONFIRMED RUNTIME: `ClickNPC(339)` opens the intended NPC dialog.
- CONFIRMED: NPC coordinates must not be inferred/hardcoded; user captures raw runtime MapID/X/Y into `ThanLongAutoHeal.target.tsv`.
- CONFIRMED canonical KB: dynamic `GameDialog` receives `Selections[selectionID] = visibleText`.
- CONFIRMED canonical KB: generated GameDialog button stores `selectionID` in `Tag`.
- CONFIRMED Lua source: GameDialog business selection sends `CMD_SHOW_GAMEDIALOG = 100007` payload `selectionID:SelectedItemID`.
- CONFIRMED Lua source: ordinary function choice has default `SelectedItemID = -1` when no award-item selection is required.
- CONFIRMED: `LuaSystemAPI_Network.SendPacket(packetID,data)` is outbound Lua network bridge to `LuaSystemManager.SendPacketToServer`.
- CONFIRMED asset: `MessageBox` has `ButtonOKClicked()` and stores semantic OK/Cancel callbacks.
- CONFIRMED BUILD: v1.1.4 architecture audit, Route FSM, Heal FSM, DLL, PE verification and EXE all pass.
- CONFIRMED RULE: build success is not runtime success.

## Important Files
- `src/bridge_part03.inc` — Lua UI discovery/tree traversal.
- `src/bridge_part04.inc` — historical v1.1.3 GameDialog/MessageBox callback path and UI inspect.
- `src/bridge_heal_packet_v1_1_4.inc` — current semantic selectionID + packet implementation.
- `src/bridge.cpp` — selects v1.1.4 implementation while retaining v1.1.3 under legacy name for MessageBox fallback.
- `src/controller_part04.inc` — runtime heal FSM orchestration.
- `src/heal_logic.h` — pure dialog FSM.
- `src/protocol.h` — shared commands/snapshot.
- `CHANGELOG.md` — short version history.
- `VERSION_v1.1.4.md` — current version test record.
- `docs/HANDOFF_v1.1.4_AUTO_TRI_LIEU.md` — current handoff.

## Important Methods / APIs
- `LuaSystemAPI_Game.ClickNPC(Int32)` — working runtime NPC interaction.
- `LuaSystemAPI_GUI.FindUI/MainFindUI` — current UI discovery.
- `UIButton.Tag` / `get_Tag` — v1.1.4 source for server-provided selectionID.
- `LuaSystemAPI_Network.SendPacket(packetID,data)` — v1.1.4 semantic action.
- `GameDialog.FunctionButtonClicked(uiButton)` — source evidence for payload semantics, but v1.1.3 runtime invocation path FAILED.
- `MessageBox.ButtonOKClicked()` — semantic MessageBox confirmation path, runtime of this heal branch not yet confirmed.

## Important IDs / Payloads
- Heal NPC ResID: `339`.
- MapID Lâu Lan: `5`.
- `CMD_SHOW_GAMEDIALOG = 100007`.
- GameDialog payload: `selectionID:SelectedItemID`.
- Default no-award selected item: `-1`.

## Working Mechanisms
- Runtime coordinate capture/persistence.
- Clean Route AutoPath.
- StopPath / dismount.
- `ClickNPC(339)`.
- Visible live GameDialog discovery by text.
- v1.1.4 source/build path for reading live Tag and sending semantic packet.

## Failed / Unsafe Mechanisms

### FAILED-001 — hardcoded/inferred coordinate scale
**Attempt:** infer displayed 294:172 as 29400,17200.
**Result:** wrong coordinate per user runtime test.
**Lesson:** capture raw runtime MapID/X/Y; never infer display scale.

### FAILED-002 — `UIButton.HandleClickEvent()` as Treatment business action
**Versions:** v1.1.0–v1.1.2.
**Attempt:** resolve live button by text and call generic UIButton click.
**Result:** NPC opens but treatment does not advance; UI visibly flickers.
**Lesson:** visual click reaction is not proof the semantic Lua/server action ran.
**Retry Conditions:** only if a trace proves this exact call reaches same semantic request reliably.

### FAILED-003 — UI resolver-only fix
**Version:** v1.1.2.
**Attempt:** `FindUI -> MainFindUI -> executor` fallback.
**Result:** build PASS, runtime still fails Treatment.
**Lesson:** discovery alone was not root cause.

### FAILED-004 — `ExecuteUIObject(GameDialog.FunctionButtonClicked(liveButton))`
**Version:** v1.1.3.
**Reason tried:** exact Lua source showed this callback constructs correct GameDialog request.
**Result:** BUILD PASS, runtime user test still only produces same UI flicker and no transition to confirmation.
**Failure Cause:** not fully proven. The `ExecuteUIObject` invocation/action bridge is not trusted as semantic success on this runtime even when UI reacts.
**Lesson:** use the callback source as documentation of the real packet, not necessarily as the action entry point.
**Retry Conditions:** only after targeted runtime trace proves the executor call signature/receiver/args and callback execution end-to-end.

## Architectural Rules
- Read canonical `AI_INDEX.md` + VERIFIED/database before investigation.
- Do not broad reverse binary if exact information already exists in KB.
- Only perform targeted binary/metadata analysis when exact needed fact is absent from VERIFIED/database.
- No screen-coordinate mouse click for internal feature.
- No stale UI pointer across transition.
- Do not invent fixed treatment selectionID; always obtain active runtime selectionID.
- Do not invent packet payloads; only use Lua-source VERIFIED payloads.
- Do not Sleep on game callback thread to prove readiness.
- One mutable action at a time; advance FSM only after concrete state evidence.
- BUILD PASS != RUNTIME PASS.
- Preserve failed attempts and corrections.

## Known Bugs / Limitations
- Full auto-heal chain has not yet achieved runtime PASS.
- v1.1.4 treatment packet path needs runtime validation.
- Confirmation type for this server instance is not yet runtime-proven; may be MessageBox or dynamic GameDialog.
- Direct semantic packet does not fabricate/guess selectionID; if Tag cannot be read the feature fails closed.

## Open Research Questions
1. Runtime concrete type/value exposure of `UIButton.Tag` on current build (`Int32`, backing field, etc.). v1.1.4 supports metadata-driven getter/field variants and logs failure.
2. Exact runtime signature of `LuaSystemAPI_Network.SendPacket`; v1.1.4 accepts exact `(Int32,String)` or `(UInt32,String)` and fails closed otherwise.
3. After treatment packet, which UI/state is actual confirmation on this NPC/server?
4. Whether local GameDialog lifecycle needs explicit close after direct semantic packet; only change this after runtime evidence.

## Next Development Priorities
1. Runtime test v1.1.4 and capture log beginning at `ClickNPC(339)`.
2. Expect log to expose either `selectionID`, exact SendPacket failure, or successful packet followed by next UI.
3. If packet sends but no server transition, targeted trace only `LuaSystemAPI_Network.SendPacket`/outbound packet and active GameDialog data; do not broad reverse.
4. Verify MessageBox/GameDialog confirmation branch only after Treatment transitions.
5. Only port to main after repeated successful complete cycles without crash/disconnect.

---

# VERSION HISTORY

## VERSION 1.1.0-test — 2026-08-16
Initial auto-heal test using NPC 339, route, dynamic GameDialog inspect. Build/FSM passed; runtime not initially proven.

## VERSION 1.1.1-test — 2026-08-16
Removed inferred NPC coordinates. Added `TỰ LẤY TỌA ĐỘ NPC` and persistence. Runtime later proved route sufficient to reach/open NPC.

## VERSION 1.1.2-test — 2026-08-16
User reported NPC opens but Treatment does not. Changed UI root discovery order. Build PASS; runtime still FAIL at Treatment.

## VERSION 1.1.3-test — 2026-08-16
Changed GameDialog action from generic UIButton click to `GameDialog.FunctionButtonClicked(liveButton)` via `MonoBehaviourExecutor.ExecuteUIObject`; MessageBox uses `ButtonOKClicked`. Build PASS (Route 8/8, Heal 7/7). User runtime test: still same flicker, no Treatment transition. Mark executor GameDialog path FAILED.

## VERSION 1.1.4-test — 2026-08-16
### User Request / Runtime Evidence
v1.1.3 still only flickers; user explicitly requires reading canonical `AI_INDEX.md` + knowledge base and forbids broad re-reverse.
### Investigation
Canonical `features/AUTO_HEAL_NPC.md` and `analysis/11_EXACT_INTERNAL_ACTION_FLOWS.md` state the stable semantic unit is active GameDialog `selectionID`, not transient click behavior.
### Solution
Resolve live button by visible text, read `Tag=selectionID`, send `CMD_SHOW_GAMEDIALOG 100007` payload `selectionID:-1` through `LuaSystemAPI_Network.SendPacket`.
### Data Sources
Canonical client `AI_INDEX.md`, Phase2 decrypted Lua docs, VERIFIED findings, `AUTO_HEAL_NPC.md`, exact action flow, packet/API databases, current runtime user test.
### Test Results
BUILD PASS in GitHub Actions run `31906609147`; Route 8/8; Heal 7/7; DLL/PE/EXE PASS; artifact generated. RUNTIME NEEDS USER TEST.

---

# DECISION LOG

## DECISION-001
**Status:** ACTIVE
**Decision:** NPC target position is captured from runtime, never inferred from displayed coordinate scaling.

## DECISION-002
**Status:** SUPERSEDED by DECISION-004
**Decision:** Prefer `GameDialog.FunctionButtonClicked(liveButton)` over generic UIButton click.
**Reason superseded:** v1.1.3 runtime still fails through executor invocation despite source-correct callback semantics.

## DECISION-003
**Status:** ACTIVE
**Decision:** Never solve UI readiness by sleeping on the game callback thread; gate state across observations/ticks.

## DECISION-004
**Date:** 2026-08-16
**Status:** ACTIVE
**Decision:** For dynamic GameDialog choices, treat live `selectionID` + VERIFIED `CMD_SHOW_GAMEDIALOG` payload as the authoritative semantic action. UI button is used only to locate/read the active selectionID.
**Context:** generic UIButton click and ExecuteUIObject callback both produce visual flicker without runtime business transition.
**Alternatives Considered:** more delays; retry HandleClickEvent; retry same ExecuteUIObject callback.
**Why Rejected:** they repeat failed action layers and do not use the canonical source-verified semantic request.
**Consequences:** future code must never hardcode healer selectionID and must fail closed if active selectionID cannot be obtained.

## DECISION-005
**Date:** 2026-08-16
**Status:** ACTIVE
**Decision:** Canonical client repo is read-first; binary analysis is targeted-only when exact required information is absent from VERIFIED/database.

---

# CORRECTION LOG

## CORRECTION-001
**Old Knowledge:** live UIButton `HandleClickEvent()` could be sufficient for Treatment.
**New Finding:** runtime v1.1.2 proves it is not sufficient for this flow.
**Status:** DEPRECATED/FAILED.

## CORRECTION-002
**Old Knowledge:** source-correct `GameDialog.FunctionButtonClicked(liveButton)` invoked through current `ExecuteUIObject` bridge was the likely complete fix.
**New Finding:** runtime v1.1.3 still only flickers and does not advance.
**Evidence:** user runtime observation after v1.1.3 artifact.
**Affected Versions:** v1.1.3.
**Impact:** callback source remains valuable to derive exact packet semantics, but executor invocation is no longer treated as authoritative action success. v1.1.4 sends the verified semantic request directly.

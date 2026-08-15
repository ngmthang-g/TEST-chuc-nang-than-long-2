# v1.1.4-test — Semantic GameDialog selection packet

## Runtime evidence leading to this version
- Route + `ClickNPC(339)` remain working.
- v1.1.3 still only makes the visible Treatment UI flicker and does not transition to confirmation.
- Therefore v1.1.3 `ExecuteUIObject(GameDialog.FunctionButtonClicked)` is marked runtime FAILED for this feature.

## Canonical client knowledge used
Read-first sources from `ngmthang-g/clinent-game-than-long-DATA-2222`:
1. `AI_INDEX.md`
2. `analysis/00_MASTER_RESEARCH_MAP.md`
3. `analysis/09_PHASE2_DECRYPTED_DATA_LUA.md`
4. `research/VERIFIED.md`
5. `features/AUTO_HEAL_NPC.md`
6. `analysis/11_EXACT_INTERNAL_ACTION_FLOWS.md`
7. `database/API_QUICK_REFERENCE.md`
8. `database/PACKET_CATALOG.md`

No broad binary reverse was performed.

## Verified semantic flow used
Lua source establishes:
- dynamic GameDialog has `Selections[selectionID] = visibleText`;
- generated button stores `selectionID` in `Tag`;
- function selection sends `CMD_SHOW_GAMEDIALOG = 100007`;
- payload is `selectionID:SelectedItemID`;
- normal function choice defaults `SelectedItemID = -1` when no award item is selected.

## Implementation
For `Trị liệu` and `Ta biết rồi`:
1. resolve current live GameDialog;
2. find button by exact visible text;
3. read its live `Tag` as runtime `selectionID`;
4. resolve `FGStudio.LuaSystem.API.LuaSystemAPI_Network.SendPacket` by metadata;
5. send packet `100007` with payload `<selectionID>:-1`;
6. advance FSM only after the bridge action returns and then wait for the next real UI/state.

For `Xác nhận`:
- if it is another dynamic GameDialog choice, use the same selectionID packet path;
- if it is `MessageBox`, keep the semantic `ButtonOKClicked()` callback path until runtime evidence says otherwise.

## Safety
- no fixed Treatment selectionID;
- no guessed packet payload;
- no screen-coordinate mouse click;
- no stale button pointer across UI transitions;
- no sleep on game callback thread;
- fail closed if Tag or exact SendPacket signature cannot be resolved.

## Status
- BUILD: pending CI at document creation time.
- RUNTIME: NEEDS USER TEST.

# HANDOFF v1.1.4-test — Auto trị liệu semantic GameDialog packet

## Purpose
This document is for a future AI/developer only after v1.1.4 is runtime-tested. Do not port the feature to the main tool merely because it builds.

## Read-first rule
Before changing this feature, read:
- project `PROJECT_KNOWLEDGE.md`;
- canonical client `ngmthang-g/clinent-game-than-long-DATA-2222/AI_INDEX.md`;
- canonical `features/AUTO_HEAL_NPC.md` and `analysis/11_EXACT_INTERNAL_ACTION_FLOWS.md`.
Do not broad reverse the client. Binary work is targeted-only if an exact required fact is absent from VERIFIED/database.

## Working runtime facts inherited
- user captures NPC target MapID/X/Y at runtime; no inferred coordinate scaling;
- `ClickNPC(339)` successfully opens Đỗ Thanh Đằng's GameDialog;
- visible `Trị liệu` button can be located in the live UI tree.

## Failed action layers that must not be repeated
1. v1.1.0–v1.1.2: `UIButton.HandleClickEvent()` on the live Treatment button → visual flicker, no Treatment transition.
2. v1.1.3: `MonoBehaviourExecutor.ExecuteUIObject(GameDialog.FunctionButtonClicked, [liveButton])` → build PASS but runtime still same flicker/no transition.

These failures do not invalidate the Lua source semantics; they invalidate those invocation paths as trusted business-action entry points for this runtime.

## Canonical VERIFIED GameDialog semantics
Recovered Lua source states:
- `GameDialogData.Selections[selectionID] = visibleText`;
- dynamic button `Tag = selectionID`;
- selecting a function sends `CMD_SHOW_GAMEDIALOG = 100007`;
- payload: `selectionID:SelectedItemID`;
- normal choice defaults `SelectedItemID = -1` when no award-item selection is required.

## v1.1.4 implementation

### `src/bridge_heal_packet_v1_1_4.inc`
Main functions:
- `ReadButtonSelectionIdV114()`
  - preferred: `get_Tag()`;
  - supports numeric/string/object getter forms;
  - metadata field fallback: `Tag`, `tag`, `<Tag>k__BackingField`;
  - rejects invalid/nonpositive selection IDs.
- `SendGameDialogSelectionV114()`
  - runtime resolves class `FGStudio.LuaSystem.API.LuaSystemAPI_Network`;
  - exact method candidates: `SendPacket(Int32,String)` or `SendPacket(UInt32,String)`;
  - sends command 100007 and `<selectionID>:<selectedItemID>`.
- `SendLiveGameDialogChoiceV114()`
  - resolves current button by visible text;
  - reads its current Tag;
  - sends `<selectionID>:-1`.
- current `ClickHealDialogChoice()`
  - Treatment → direct semantic packet;
  - Know → direct semantic packet;
  - GameDialog Confirm → direct semantic packet;
  - MessageBox Confirm → legacy v1.1.3 semantic `ButtonOKClicked()` path only for this non-GameDialog case.

### `src/bridge.cpp`
The original v1.1.3 implementation is compiled as `ClickHealDialogChoiceV113`; the protocol-facing function is the v1.1.4 implementation. This preserves a narrow MessageBox fallback without using the old GameDialog callback for Treatment/Ack.

## Execution flow
`route -> ClickNPC(339) -> wait live Treatment -> read button.Tag -> SendPacket(100007,"selectionID:-1") -> wait confirmation -> confirm -> wait new GameDialog Ta biết rồi -> read new Tag -> SendPacket(100007,"selectionID:-1") -> DONE`.

## Runtime logs expected
A successful GameDialog semantic action should log something like:
`Đã gửi CMD_SHOW_GAMEDIALOG 100007 • selectionID=<n> • selectedItemID=-1 • label=Trị liệu`.

If it fails before send, the log should identify:
- missing live button;
- missing/unsupported `UIButton.Tag`;
- invalid Tag value;
- missing exact `LuaSystemAPI_Network.SendPacket` signature;
- managed exception during SendPacket.

## What must be tested before porting
- [ ] route to captured NPC target works;
- [ ] ClickNPC 339 opens correct dialog;
- [ ] Treatment log shows real selectionID and SendPacket success;
- [ ] confirmation appears and is completed;
- [ ] Ta biết rồi uses a newly resolved live selectionID and completes;
- [ ] HP/service result is actually applied;
- [ ] repeat at least 20 cycles without crash/disconnect;
- [ ] no mouse/foreground dependency;
- [ ] no duplicate packet/action caused by retries.

## If v1.1.4 still fails
Do not return to click timing experiments. Use the exact failure log.
- If Tag read fails: targeted metadata/runtime inspection of UIButton Tag type only.
- If SendPacket exact signature fails: targeted runtime metadata inspection of `LuaSystemAPI_Network.SendPacket` signature only.
- If SendPacket reports success but server does not transition: targeted outbound observation of `LuaSystemAPI_Network.SendPacket` and active GameDialog selection data for one manual Treatment action. Do not broad reverse GameAssembly.

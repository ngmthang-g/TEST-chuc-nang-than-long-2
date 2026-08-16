# Runtime finding — v1.1.9 Lua manager boundary

Date: 2026-08-16  
Source: user screenshot/live test of delivered v1.1.9.

Observed transaction:
- tool title identifies v1.1.9;
- PID 4324, Map 5;
- Auto trị liệu starts for NPC 339 Đỗ Thanh Đằng;
- `Đã gọi ClickNPC npcID=339` appears;
- status says the dialog is open and the tool is waiting for live Treatment selection;
- repeated exact failure: `LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`.

Interpretation:
- GameDialog/open path is still reached;
- failure occurs before `LuaEnv.DoString`, before scanning `Selections`, before resolving a Treatment selectionID and before `ACTION_V119`;
- therefore this runtime does not establish a DoString/selection/parser/packet/server failure;
- the v1.1.9 singleton resolver assumption is the exact next boundary.

Source audit of v1.1.9 confirms `ResolveLuaEnvV119` called `ResolveLuaSystemManagerV119` first. That manager resolver only tried a semantic static `get_Instance` and four guessed static field names (`Instance`, `instance`, `_instance`, `<Instance>k__BackingField`). Canonical client KB verifies `get_LuaEnv/set_LuaEnv` members but does not verify a LuaSystemManager singleton getter/field contract.

v1.1.10 therefore tests static `get_LuaEnv` directly before requiring an instance and adds bounded metadata-driven fallback discovery. This finding must remain preserved even if v1.1.10 succeeds.
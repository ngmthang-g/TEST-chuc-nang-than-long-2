# v1.1.0-test architecture audit

Repo vẫn giữ nền Clean Route, nhưng scope v1.1.0-test chủ động mở thêm đúng một feature: Auto trị liệu.

## Không được quay lại kiến trúc lỗi cũ

- Không `remote_worker.S`, `RemoteExecutor`, `CreateRemoteThread`, `WriteProcessMemory`.
- Không hardcode RVA action.
- Không giữ UIButton/UIObject pointer qua frame hoặc qua lần chuyển dialog.
- Không dùng delay cố định làm điều kiện state transition.
- Không gọi response handler server như action.

## Mutation surface v1.1.0-test

Route:

1. `LuaSystemAPI_Game.SendToggleRideState(Int32)`
2. `AutoPathManager.StartAutoPath(Int32,Int32,Int32)`
3. `LuaSystemAPI_Game.StopAutoPath()`

Auto trị liệu:

4. `LuaSystemAPI_Game.ClickNPC(Int32)`
5. `UIButton.HandleClickEvent()` **chỉ trên instance button đang sống vừa resolve trong cùng game-thread request**.

`HandleClickEvent` ở đây không phải global click API và không được cache pointer. Nó chỉ được dùng như final dispatch vào handler gốc sau khi state/UI/text đã được xác minh.

## Test tách riêng

- `route_logic_test.cpp`: 8 case.
- `heal_logic_test.cpp`: 7 case, gồm kiểm tra không click sai dialog khi state không khớp.

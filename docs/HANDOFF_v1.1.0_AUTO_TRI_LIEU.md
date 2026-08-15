# HANDOFF v1.1.0-test — Auto trị liệu

Tài liệu này dành cho AI/dev mang tính năng đã test PASS sang tool chính. Không copy cả repo test một cách mù quáng.

## Mục tiêu nghiệp vụ

Khi người dùng bấm `AUTO TRỊ LIỆU`:

1. đi tới Lâu Lan (`MapID=5`);
2. tới tọa độ NPC Đỗ Thanh Đằng (`294:172` -> raw test `29400,17200`);
3. xuống ngựa;
4. gọi NPC `339` bằng API game;
5. chờ `GameDialog` có lựa chọn `Trị liệu`;
6. bấm đúng button sống `Trị liệu`;
7. chờ `Xác nhận`;
8. bấm đúng button sống `Xác nhận`;
9. chờ `Ta biết rồi`;
10. bấm đúng button sống `Ta biết rồi`;
11. DONE.

## Bố cục source trên repo test

Do giới hạn kênh ghi file vào GitHub trong phiên làm việc này, hai translation unit lớn được chia thành các include-chunk dễ kiểm tra:

- `src/bridge.cpp` chỉ include tuần tự `bridge_part*.inc`;
- `src/controller.cpp` chỉ include tuần tự `controller_part*.inc`.

Khi compiler preprocess, nội dung tương đương file `.cpp` liền mạch. Khi port sang main, AI/dev nên **ghép nội dung các part theo thứ tự số** hoặc port theo các hàm nêu trong tài liệu này; không cần giữ cách chia chunk.

## File cần mang sang

### `src/protocol.h`

Thêm command:

- `ClickNpc = 5`
- `InspectHealDialog = 6`
- `ClickHealDialogChoice = 7`

Thêm `HealDialogMask` và `HealDialogChoice`.

Khi ghép vào tool chính phải **đổi ID command nếu protocol chính đã dùng các số này**; không ép giữ 5/6/7.

### `src/bridge.cpp`

Các phần mới cần port:

- resolve `il2cpp_string_new_utf16`;
- tạo managed string `NewString()`;
- lấy UI hiện hành qua `LuaSystemAPI_GUI` và fallback `MonoBehaviourExecutor`;
- duyệt cây `UIObject.CoreChildren`;
- đọc `UIButton.Text`;
- `InspectHealDialog()`;
- `ClickNpc()`;
- `ClickHealDialogChoice()`.

Điểm quan trọng nhất: `ClickHealDialogChoice()` **không giữ pointer button**. Nó tìm lại UI + button trong cùng request đang chạy trên game thread, rồi mới invoke `HandleClickEvent()` trên instance vừa tìm thấy.

Không port theo kiểu:

```text
buttonTreatment = pointer từ frame A
...
frame B/UI đổi
HandleClickEvent(buttonTreatment)
```

### `src/heal_logic.h`

Pure FSM cho ba bước dialog. Không phụ thuộc Win32, có thể unit-test độc lập.

### `src/controller.cpp`

Constants test:

```cpp
kHealMapID = 5
kHealNpcID = 339
kHealX = 29400
kHealY = 17200
kHealTolerance = 160
```

`TickAutoHeal()` dùng Route FSM cũ để đi NPC, sau đó chuyển sang dialog FSM.

## State machine

```text
Travel
  -> route tới Map 5 / 29400,17200
  -> StopPath
  -> Dismount
OpenNpc
  -> ClickNPC(339)
WaitTreatment
  -> Inspect UI mỗi tick
  -> chỉ khi thấy "Trị liệu" mới click
WaitConfirm
  -> ưu tiên MessageBox "Xác nhận"
  -> fallback GameDialog "Xác nhận"
WaitKnow
  -> chỉ khi thấy GameDialog "Ta biết rồi" mới click
Done
```

Timeout chỉ là **đường fail/retry**, không phải điều kiện để chuyển bước.

## Vì sao không dùng packet ID trực tiếp trong v1.1.0

Interface cho thấy `GameDialog:FunctionButtonClicked` tự lấy `Tag=selectionID` rồi gửi `CMD_SHOW_GAMEDIALOG`.

Selection ID của `Trị liệu`/flow NPC là dữ liệu dialog do server gửi, không có bằng chứng rằng nó cố định giữa mọi NPC/build/server. Vì vậy v1.1.0 để handler gốc gửi packet thay vì hardcode packet payload chưa chứng minh.

Nếu trace runtime sau này chứng minh selection ID/payload ổn định và muốn bỏ hẳn UI event, hãy làm **version mới**, không âm thầm thay cơ chế này.

## Safety / ổn định

- action chạy qua WH_GETMESSAGE callback đúng thread game;
- guard `IsMapReady` + `WaitingChangeMap` trước action;
- sau map change cần 2 snapshot MapID liên tiếp;
- không hardcode RVA;
- không click tọa độ màn hình;
- không di chuyển chuột Windows;
- không stale UIButton pointer;
- không click button khác text/state;
- không gọi response handler server như action;
- không retry packet/action vô hạn.

## Điểm CHƯA được xác minh bằng runtime

1. `294:172 -> 29400,17200` cần test client thật.
2. `MainFindUI/MonoBehaviourExecutor -> UIRoot` đã có hai fallback nhưng cần client thật chứng minh đường nào đang dùng.
3. Sau khi bấm `Trị liệu`, xác nhận có thể là `MessageBox` hoặc `GameDialog`; code hỗ trợ cả hai, nhưng log test phải ghi loại UI thực tế.
4. Không được gọi version này là PASS để ghép main cho đến khi log thực tế đi đủ 4 action: `ClickNPC -> Trị liệu -> Xác nhận -> Ta biết rồi` ít nhất nhiều vòng liên tiếp.

## Checklist trước khi ghép sang main

- [ ] AutoPath tới đúng NPC từ cùng map.
- [ ] AutoPath tới đúng NPC từ map khác.
- [ ] Tới nơi xuống ngựa rồi mới ClickNPC.
- [ ] Không giữ pointer button sau UI transition.
- [ ] Trị liệu được chọn đúng.
- [ ] Xác nhận được bấm đúng UI.
- [ ] Ta biết rồi được bấm đúng.
- [ ] Không chiếm chuột/màn hình foreground.
- [ ] Không crash/diss sau ít nhất 20 vòng test.
- [ ] Khi UI không đúng, tool fail-closed, không click bừa.

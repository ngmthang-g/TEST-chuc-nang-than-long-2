# Client evidence — v1.1.0 Auto trị liệu

File này chỉ ghi các bằng chứng đã lấy từ bộ client người dùng cung cấp. Không coi suy đoán là dữ liệu chắc chắn.

## 1. NPC / map

Sau khi giải mã `Config.unity3d` về UnityFS và đọc TextAsset cấu hình:

```xml
<NPC ID="337" Name="Ðỗ Bất Ðằng" ResName="LangZhong1" ... />
<NPC ID="338" Name="Ðỗ Hoàng Ðằng" ResName="LangZhong1" ... />
<NPC ID="339" Name="Ðỗ Thanh Ðằng" ResName="LangZhong1" ... />
```

Bảng map/NPC:

```xml
<NPCData ID="337" MapID="5" Name="Ðỗ Bất Ðằng" />
<NPCData ID="338" MapID="5" Name="Ðỗ Hoàng Ðằng" />
<NPCData ID="339" MapID="5" Name="Ðỗ Thanh Ðằng" />
```

Bảng location xác nhận `MapID=5` là Lâu Lan.

Kết luận dùng cho bản test:

- `NPC ID / ResID = 339`
- `MapID = 5`

Lưu ý encoding TextAsset ghi ký tự `Ð`; về nghĩa/nội dung đây là tên người dùng gọi **Đỗ Thanh Đằng**.

## 2. Tọa độ

Người dùng cung cấp tọa độ giao diện `294:172`.

Source sạch hiện đọc `get_PosX/get_PosY` và truyền raw Int32 trực tiếp cho `AutoPathManager.StartAutoPath(map,x,y)`. Dữ liệu runtime trước đó có dạng `9400,5400` tương ứng cách hiển thị `94,54`. Vì vậy bản test dùng:

- `X = 29400`
- `Y = 17200`

Đây là **suy luận có căn cứ nhưng vẫn cần xác nhận runtime**. Nếu client thực tế route lệch 100 lần, sửa duy nhất constants `kHealX/kHealY` ở `src/controller.cpp`; không sửa FSM/UI logic.

## 3. API mở NPC

Metadata/GameAssembly xác nhận method:

```text
FGStudio.LuaSystem.API.LuaSystemAPI_Game.ClickNPC(System.Int32)
```

Phân tích native trước đó cho thấy nó đi theo flow game: stop auto path -> tìm NPC object -> quay mặt -> select target -> gửi click object. Bản test resolve theo namespace/class/method runtime, không nhét RVA chết.

## 4. GameDialog

`Interface.unity3d` sau giải mã chứa script `GameDialog`:

```lua
if gameDialogData.Selections ~= nil then
    for selectionID, selectionName in pairs(gameDialogData.Selections) do
        local uiButton = self.ButtonPrefab:Clone()
        uiButton.Active = true
        uiButton.Parent = self.ButtonList
        uiButton.Text = selectionName
        uiButton.Tag = selectionID
    end
end
```

Handler thật:

```lua
function GameDialog:FunctionButtonClicked(uiButton)
    local selectionID = uiButton.Tag
    local strCmd = String.Format("{0}:{1}", selectionID, SelectedItemID)
    Network.SendPacket(G_TCPPacketDefine.CMD_SHOW_GAMEDIALOG, strCmd)
    self:Destroy()
end
```

Vì button được clone mới mỗi dialog, **không được giữ UIButton pointer từ dialog trước**.

## 5. “Ta biết rồi”

Script `AutoFight_Main:ProcessGameDialog` cũng xác nhận game nhận biết lựa chọn:

```lua
if selectionID == 99999 or selectionName == "Ta biết rồi" then
    Network.SendPacket(
        G_TCPPacketDefine.CMD_SHOW_GAMEDIALOG,
        String.Format("{0}:{1}", selectionID, -1)
    )
end
```

Bản test không hardcode `99999`; nó tìm đúng button text đang sống rồi để handler gốc của game gửi selection ID tương ứng.

## 6. Xác nhận

`MessageBox_Layout` có:

```xml
<Button Name="ButtonOK">
    <Property Name="Text" Value="Xác nhận" />
    <Property Name="ClickHandler" Value="ButtonOKClicked" />
</Button>
```

Bản test ưu tiên tìm `Xác nhận` trong `MessageBox`; fallback `GameDialog` chỉ để chịu được trường hợp server dùng GameDialog thay vì MessageBox ở flow trị liệu.

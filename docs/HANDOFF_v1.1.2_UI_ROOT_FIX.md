# v1.1.2 handoff note — UI root resolver fix

Port only after runtime PASS.

Change scope:

- `ClickNPC(339)` and navigation are already runtime-confirmed.
- The failure was UI discovery: `MainFindUI` existed but could return null, while the v1.1.0 resolver never tried `FindUI` afterward.
- v1.1.2 changes Lua UI root lookup to `FindUI -> MainFindUI -> executor fallback` directly in `src/bridge_part03.inc`.
- No coordinate hardcode was reintroduced.
- No mouse/foreground click was introduced.
- No RVA was added.

Build validation for code commit `be03f263b883db630074b45dcf8c5de494b04290`:

- Architecture audit PASS.
- Route FSM 8/8 PASS.
- Heal FSM 7/7 PASS.
- Bridge DLL PASS.
- Controller v1.1.2 PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.2`.

Runtime decision tree:

1. If dialog opens and state changes from `CHỜ DIALOG TRỊ LIỆU` to `ĐANG CHỌN TRỊ LIỆU`, UI discovery fix is PASS.
2. If the dialog then changes to confirmation, current live-button dispatch is also PASS; continue testing MessageBox and `Ta biết rồi`.
3. If state reaches `ĐANG CHỌN TRỊ LIỆU` but the dialog does not change, do not touch route/NPC/coordinates. Replace final dispatch only with the exact Lua action recovered from `Interface.unity3d`: `GameDialog.FunctionButtonClicked(liveButton)`.
4. For confirmation, the exact Lua callback is `MessageBox.ButtonOKClicked()`; for final acknowledgement use a freshly resolved `Ta biết rồi` button with `GameDialog.FunctionButtonClicked(liveButton)`.

Do not port this feature into the main tool until the whole runtime chain `ClickNPC -> Trị liệu -> Xác nhận -> Ta biết rồi` passes repeatedly.

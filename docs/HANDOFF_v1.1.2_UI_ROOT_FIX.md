# v1.1.2 handoff note — UI root resolver fix

Port only after runtime PASS.

Change scope:

- `ClickNPC(339)` and navigation are already runtime-confirmed.
- The failure was UI discovery: `MainFindUI` existed but returned null, while old resolver never tried `FindUI`.
- v1.1.2 changes Lua UI root lookup to `FindUI -> MainFindUI -> executor fallback`.
- No coordinate hardcode was reintroduced.
- No mouse/foreground click was introduced.
- No RVA was added.

If this version reaches the visible `Trị liệu` button but does not advance, the next isolated change should be final dispatch only: invoke `GameDialog.FunctionButtonClicked(liveButton)` and `MessageBox.ButtonOKClicked()` using the live Lua UI object, based on recovered `Interface.unity3d` semantics. Do not rewrite route/NPC logic.

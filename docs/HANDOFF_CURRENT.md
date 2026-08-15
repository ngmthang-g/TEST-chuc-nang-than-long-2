# CURRENT HANDOFF

Current Auto trị liệu handoff: `docs/HANDOFF_v1.1.3_AUTO_TRI_LIEU.md`.

`docs/HANDOFF_v1.1.0_AUTO_TRI_LIEU.md` is retained for history, but its recommendation to use `UIButton.HandleClickEvent()` for GameDialog business choices is **DEPRECATED/FAILED by runtime evidence from v1.1.2**.

Current rule:
- resolve live GameDialog button;
- call `GameDialog.FunctionButtonClicked(liveButton)`;
- confirm through `MessageBox.ButtonOKClicked()`;
- never treat a visual button flicker as proof that server/business action succeeded.

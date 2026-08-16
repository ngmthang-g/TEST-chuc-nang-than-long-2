# FEATURE: AUTO HEAL NPC / TRỊ LIỆU

## Purpose
Build a reliable internal flow:
`route -> NPC interaction -> observe current server GameDialog -> choose current Treatment action -> real follow-up -> state proof`.
Only port after repeated runtime PASS.

## Mandatory Startup
Before every version read:
1. `AI_START_HERE.md`
2. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
3. `AI_CLIENT_ANALYSIS_RULES.txt`
4. `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. this feature + BUG/DEC/EVID docs + current source.

Canonical client research route starts from `clinent-game-than-long-DATA-2222/AI_INDEX.md`; do not broad reverse when exact facts exist in VERIFIED/database.

## Current Implementation — v1.1.7
Protected working path:
- user captures raw MapID/X/Y;
- route / stop path / dismount;
- Map 5 -> NPC 339 Đỗ Thanh Đằng;
- Map 3 -> NPC 463 Long Phá Thiên;
- semantic NPC interaction/open.

Current Treatment observer/action path:

`ClickNPC -> Lua GameDialog presence -> robust dynamic button discovery -> MainThread queued live UIButton event -> wait next real UI/state`.

### v1.1.7 discovery correction
Old discovery required the identical object to satisfy both:
- `get_Text() == "Trị liệu"`;
- `HandleClickEvent()`.

v1.1.7 instead:
- identifies clickable object by `HandleClickEvent()`;
- searches its descendant subtree for visible label text;
- normalizes whitespace/NBSP/simple markup;
- records candidate labels/count for diagnostics;
- distinguishes Lua UI existence from UIRoot readiness;
- once GameDialog exists, does not reopen NPC just because exact Treatment button discovery is incomplete on that tick.

This fixes a previously unisolated observer/retry variable while retaining the v1.1.6 MainThread action boundary.

## Current Runtime / Build Status
- Route/NPC open: RUNTIME PARTIAL PASS.
- Full Treatment: no known-good version.
- v1.1.5: RUNTIME FAIL overall at Treatment/flicker on second NPC/map.
- v1.1.6: CI/BUILD PASS; runtime artifact-version result remains unconfirmed in preserved logs.
- v1.1.7: BUILD/CI pending; RUNTIME UNTESTED.

## Critical Evidence Correction
Earlier history described v1.1.0–v1.1.5 action paths as if each business action definitely executed and failed. That is too strong.

Confirmed:
- visible dialog opened;
- no progression;
- repeated `ClickNPC` appeared in original logs while status remained `CHỜ DIALOG TRỊ LIỆU`.

Not proven for every old version:
- that the Treatment action stage itself was actually reached.

Because all versions shared the same discovery primitive, observer failure can explain repeated NPC reopen/flicker before action execution.

## Canonical Client Facts
- Dynamic `GameDialog` is server/runtime-driven.
- `Selections[selectionID] = visibleText`.
- selection action submits `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`.
- inbound GameDialog lifecycle may destroy/recreate the existing dialog.
- canonical Auto Heal flow is wait/observe current dialog -> current semantic choice -> state proof.
- visual flicker is not success proof.

## Current Known-Good
No full Auto Heal known-good.
Protected partial behavior:
- coordinate capture;
- route;
- StopPath/dismount;
- NPC interaction/open.

## Version Timeline
### v1.1.0
Initial route + NPC + dialog experiment. Full Treatment runtime FAIL.

### v1.1.1
Removed inferred X/Y; added runtime capture.

### v1.1.2
Changed UI root discovery order. NPC opening confirmed; Treatment still not progressed.

### v1.1.3
Tried `GameDialog.FunctionButtonClicked(liveButton)` through ExecuteUIObject. Overall runtime still failed; shared button discovery remained.

### v1.1.4
Tried runtime Tag/selectionID + source-verified GameDialog packet. Overall runtime failed on NPC 339; shared discovery remained.

### v1.1.5
A/B NPC/map test using 463/Lạc Dương. Same overall symptom; NPC-339-only theory weakened.

### v1.1.6
Introduced CTS proof + `MainThread.Execute(System.Action)` queued UIButton action. CI/BUILD PASS. Runtime proof not preserved as confirmed in current evidence.

### v1.1.7
Re-audited symptom and original log. New likely cause: observer fails to see current dynamic button, controller repeatedly reopens NPC, creating flicker. Adds robust descendant-label discovery and GameDialog-presence anti-reopen guard. Action boundary otherwise retained.

## Failed / Unsafe Mechanisms
- inferred/hardcoded coordinate scaling — FAILED.
- treating fixed delay as UI readiness — unsafe.
- repeated `ClickNPC` when a server GameDialog already exists — now considered a likely bug path.
- claiming an action mechanism failed without proof that the action stage was reached — DEPRECATED reasoning.

Historical action experiments remain preserved; their full-chain runtime outcome was FAIL, but action-specific causality is not automatically confirmed.

## Important APIs / Constants
- `LuaSystemAPI_Game.ClickNPC(Int32)`.
- `GameDialog.Selections[selectionID] = visibleText`.
- `CMD_SHOW_GAMEDIALOG = 100007`.
- `FGStudio.Engine.Utilities.MainThread.Execute(System.Action)`.
- v1.1.7 diagnostic strings: `DIALOG_V117 MATCH`, `DIALOG_V117 NO MATCH`, `Lua UI tồn tại nhưng UIRoot chưa sẵn sàng`.

## Do-Not-Break Rules
- mandatory startup files every version;
- canonical KB first; targeted binary only for an exact remaining gap;
- no guessed fixed Treatment selection ID;
- no hardcoded/inferred NPC X/Y;
- no stale UI pointer across transition;
- no fixed Sleep as success proof;
- one mutable action at a time;
- do not reopen NPC when current GameDialog is already present;
- BUILD/CI PASS != RUNTIME PASS.

## Open Questions
1. Does v1.1.7 report the visible Treatment button via descendant-label discovery?
2. Was historical flicker primarily repeated NPC reopen?
3. After discovery succeeds, does current MainThread queued event advance Treatment?
4. If action is definitely enqueued and still no transition, what differs between manual and tool semantic action/request?
5. What exact follow-up confirmation/state proves successful Treatment?

## Next Diagnostic Step
Run v1.1.7 and preserve log from first `ClickNPC` through the first `DIALOG_V117` and `MAINTHREAD_*` lines. Do not rotate NPCs or broad reverse before this evidence.

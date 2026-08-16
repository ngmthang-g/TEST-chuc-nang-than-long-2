# PROJECT KNOWLEDGE

## Project Identity
- Repository: `ngmthang-g/TEST-chuc-nang-than-long-2`
- Purpose: independent runtime test lab for Thần Long automation features; not the production tool.
- Development branch: `agent/auto-tri-lieu-v1.1.0`
- Current version: `v1.1.6-test`
- Last known-good full Auto trị liệu: **NONE / UNKNOWN** — no version has completed the full Treatment chain at runtime.
- Runtime-known-good partial subsystem: user-captured target -> route -> dismount -> NPC interaction/open.

## Mandatory Startup / Memory Contract
Every version must read, in order:
1. `AI_START_HERE.md`
2. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
3. `AI_CLIENT_ANALYSIS_RULES.txt`
4. this `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. affected feature / bug / decision / evidence docs and current source/tests.

When client internals are needed, use canonical repository `ngmthang-g/clinent-game-than-long-DATA-2222` and route narrowly:
`AI_INDEX.md -> AI_BOOTSTRAP.md -> AUTO_TOOL_SCOPE.md -> AI_ROUTER.md -> one matching contexts/BUILD_*.md -> REQUIRED docs -> exact VERIFIED/database lookup`.

No broad client reverse. Binary/native work is targeted-only when an exact fact required by the current task is absent from canonical VERIFIED/database/docs.

## Current State
### Runtime-confirmed working
- raw MapID/X/Y capture and persistence;
- CleanRoute movement to captured target;
- StopPath / dismount behavior sufficient for NPC interaction;
- `ClickNPC(339)` opens Đỗ Thanh Đằng's dialog in Lâu Lan;
- v1.1.5 reaches/opens the Long Phá Thiên/Lạc Dương test flow sufficiently to reproduce the same Treatment symptom.

### Runtime-confirmed failing
- v1.1.0–v1.1.2: direct `UIButton.HandleClickEvent()` in hook path -> visible flicker, no Treatment progression.
- v1.1.3: `GameDialog.FunctionButtonClicked(liveButton)` through ExecuteUIObject -> same flicker/no progression.
- v1.1.4: live selectionID + source-verified `CMD_SHOW_GAMEDIALOG=100007`, payload `selectionID:-1`, NPC 339/Lâu Lan -> same flicker/no progression.
- v1.1.5: same v1.1.4 action layer, NPC 463 Long Phá Thiên/Lạc Dương -> user reports **same flicker/no Treatment progression**.

### v1.1.6 current implementation
- Changes only the execution boundary for the broken Treatment UI action.
- Constructs a legitimate `System.Action(target=live UIButton, callback=HandleClickEvent)` and enqueues it through `FGStudio.Engine.Utilities.MainThread.Execute(Action)` so callback execution happens later in normal Unity `Update()`, not re-entrantly inside the Windows message hook.
- Before real button mutation is allowed, runs a harmless `CancellationTokenSource.Cancel()` Action through the same queue and later polls `IsCancellationRequested false -> true`.
- v1.1.6 code BUILD/CI: **PASS**.
- Final source commit tested: `e54d537d8870c6e7e131816f3b7e78b60f10a62f`.
- GitHub Actions run: `31924151093`.
- Route FSM: **8/8 PASS**.
- Heal FSM: **7/7 PASS**.
- Bridge DLL compile/PE verify: PASS; PE characteristics `0x2022`.
- Controller EXE: PASS.
- Artifact: `ThanLongTestAutoHeal-v1.1.6`, ID `9257330034`.
- Artifact ZIP SHA256: `452e0093090b4ce3b7c00f7e4fc815b384d880554ebdfc4ff6b01bbee20cbd34`.
- v1.1.6 runtime: **RUNTIME UNTESTED**.

## Affected Feature
Auto trị liệu NPC / dynamic GameDialog action execution.

## Architecture
Read/decision side:
`Controller -> per-PID shared memory -> WH_GETMESSAGE producer hook -> state read/inspect -> state machine`.

Canonical mutable-action boundary under test:
`producer hook -> construct/root legitimate System.Action -> MainThread.Execute(Action) -> return hook -> Unity Update -> DoExecuteWorks -> Action.Invoke -> observer/state proof`.

The hook may be a valid managed producer context, but direct/re-entrant gameplay/UI mutation is not treated as authoritative merely because it runs on that thread.

## Confirmed Technical Facts
- Đỗ Thanh Đằng = NPC/ResID `339`, MapID `5` Lâu Lan.
- Long Phá Thiên = NPC/ResID `463`, `ResName=PuTongXiaShi2`, MapID `3` Lạc Dương.
- NPC X/Y must not be inferred/hardcoded in this test; use raw user-captured MapID/X/Y.
- Dynamic `GameDialog`: `Selections[selectionID] = visibleText`; generated button stores selectionID in `Tag`.
- Source-verified GameDialog function request: `CMD_SHOW_GAMEDIALOG=100007`, payload `selectionID:SelectedItemID`; ordinary no-award choice commonly uses `SelectedItemID=-1`.
- Inbound GameDialog lifecycle may destroy/recreate the dialog, so **flicker is not success proof**.
- `FGStudio.Engine.Utilities.MainThread.Execute(System.Action)` enqueues; Unity `Update()` / `DoExecuteWorks()` invokes queued Actions.
- Frozen client donor for legitimate `System.Action` constructor: `GameAssembly + 0x49F810`; build-specific locator, not universal identity.
- Canonical MainThread rule: enqueue and return; never synchronously wait for queued callback in the same hook request.
- BUILD/CI PASS does not establish runtime PASS.

## Root-Cause Assessment
### DISPROVEN / strongly weakened
**NPC 339/Lâu Lan as the sole root cause.**
EVID-002: NPC 463/Lạc Dương reproduces the same Treatment flicker/no-progress.

### LIKELY, not CONFIRMED
A shared external mutation/execution-boundary problem. v1.1.6 isolates this by moving the authentic live UIButton event across the game-owned MainThread Update queue. Runtime proof is still required before promoting this to confirmed root cause.

## Important Files
- `AI_START_HERE.md` — mandatory per-version startup pointer.
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md` — engineering-lineage protocol.
- `AI_CLIENT_ANALYSIS_RULES.txt` — canonical-client analysis rule.
- `src/bridge_mainthread_v1_1_6.inc` — CTS proof + queued live UIButton event.
- `src/bridge.cpp` — active v1.1.6 bridge wiring.
- `src/bridge_part04.inc` — current shared UI discovery/state/NPC code plus historical v1.1.3 helper.
- `src/bridge_heal_packet_v1_1_4.inc` — historical failed direct packet experiment; retained in source/history but not compiled into active v1.1.6 bridge.
- `src/controller_part04.inc` / `src/controller_part05.inc` — heal FSM and captured MapID->NPC mapping.
- `docs/features/AUTO_HEAL_NPC.md` — feature lineage.
- `docs/bugs/BUG_REGISTRY.md` — BUG-001.
- `docs/evidence/EVIDENCE_REGISTRY.md` — EVID records.
- `docs/decisions/DECISIONS.md` — active decisions.
- `docs/history/VERSION_v1.1.6.md` — full v1.1.6 lineage/build history.

## Important IDs / APIs / Data Sources
- `339` -> Đỗ Thanh Đằng -> Map 5.
- `463` -> Long Phá Thiên -> Map 3.
- `CMD_SHOW_GAMEDIALOG = 100007`.
- `FGStudio.Engine.Utilities.MainThread.Execute(System.Action)`.
- CTS proof: `CancellationTokenSource.Cancel()` -> `IsCancellationRequested false -> true`.
- Canonical client repo: `ngmthang-g/clinent-game-than-long-DATA-2222`.

## Working Mechanisms
- coordinate capture/persistence;
- route / AutoPath;
- StopPath / dismount;
- semantic NPC opening;
- live GameDialog/button discovery by text.

## Failed / Unsafe Mechanisms Summary
- FAILED-001: inferred coordinate scaling.
- FAILED-002: direct UIButton HandleClickEvent in hook path.
- FAILED-003: resolver-only adjustment.
- FAILED-004: ExecuteUIObject(GameDialog.FunctionButtonClicked).
- FAILED-005: direct source-verified GameDialog packet on NPC 339.
- FAILED-006: switching NPC/map to 463/Map3 with same action layer did not change symptom.

Detailed attempts are preserved in `docs/features/AUTO_HEAL_NPC.md`, BUG-001, `CHANGELOG.md`, and version history files.

## Build History — v1.1.6
- Initial run `31923691864` / commit `be99b7d...`: **BUILD FAILED** because historical `ClickHealDialogChoiceV114` became unused and `-Werror` rejected it.
- Correction: v1.1.4 helper removed from active compilation while source/history retained; v1.1.3 historical helper kept with explicit `[[maybe_unused]]` reference because its compilation unit supplies shared current functions.
- Final run `31924151093` / source commit `e54d537...`: **CI PASS / BUILD PASS**.

## Hard Architectural Rules
- read mandatory startup docs every version;
- canonical client KB first, targeted binary only for exact missing fact;
- no hardcoded/inferred NPC X/Y;
- no guessed fixed Treatment selection ID;
- no stale UI pointer across dialog transitions;
- no fixed Sleep as readiness/success proof;
- one mutable action in flight per PID;
- queued Action must return from hook before later proof polling;
- visual flicker is not success proof;
- BUILD PASS != RUNTIME PASS.

## Known Bugs
### BUG-001 — Treatment GameDialog flickers but does not advance
Status: OPEN.  
Observed bad runtime: v1.1.0 through v1.1.5.  
Current experiment: v1.1.6 MainThread queued live UIButton event after CTS proof.

## Current Limitations
- no full Auto Heal runtime PASS yet;
- confirmation/`Ta biết rồi` after a genuine Treatment transition has not been reached in this test series;
- v1.1.6 Action ctor uses frozen build locator after semantic checks and therefore requires live verification on this client build;
- route/ClickNPC intentionally remain unchanged in v1.1.6 to isolate the broken UI action boundary.

## Open Questions
1. Does CTS MainThread proof pass repeatedly on live client without crash/GC corruption?
2. After proof PASS, does queued `UIButton.HandleClickEvent` cause real Treatment progression?
3. If proof PASS but same flicker remains, what exact GameDialog/request/state difference exists between one manual human click and tool queued event?
4. What real confirmation UI/state follows an accepted Treatment?

## Next Development Priorities
1. User runtime-test v1.1.6 and preserve log from `MAINTHREAD_PROOF` through Treatment.
2. If proof fails, fix only exact delegate/MainThread stage reported.
3. If proof passes but Treatment still fails, do one targeted manual-vs-tool dialog/request/state trace.
4. Do not rotate more NPCs or broad-reverse the client as the next step.
5. Do not port to production before repeated full runtime PASS without crash/disconnect.

## Knowledge Index
- `docs/features/AUTO_HEAL_NPC.md`
- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/decisions/DECISIONS.md`
- `docs/history/VERSION_v1.1.6.md`
- older `VERSION_v1.1.x.md` and `docs/HANDOFF_*` files preserve previous detailed experiments.

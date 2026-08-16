# PROJECT KNOWLEDGE

## Project Identity
- Repository: `ngmthang-g/TEST-chuc-nang-than-long-2`
- Purpose: runtime test lab for Thần Long automation features; not the production tool.
- Development branch: `agent/auto-tri-lieu-v1.1.0`
- Current version: `v1.1.6-test`
- Last known-good full Auto trị liệu: **NONE / UNKNOWN** — no version has completed the full treatment chain at runtime.
- Runtime-known-good subsystem: route to captured target + dismount + NPC open on the tested healer flow.

## Mandatory Startup / Memory Contract
Every version must read, in order:
1. `AI_START_HERE.md`
2. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
3. `AI_CLIENT_ANALYSIS_RULES.txt`
4. this `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. affected feature/bug/decision/evidence docs and current source/tests.

When client internals are needed, use canonical repository `ngmthang-g/clinent-game-than-long-DATA-2222` and route narrowly:
`AI_INDEX.md -> AI_BOOTSTRAP.md -> AUTO_TOOL_SCOPE.md -> AI_ROUTER.md -> one matching contexts/BUILD_*.md -> REQUIRED docs -> exact VERIFIED/database lookup`.
No broad client reverse. Binary/native work is targeted-only when an exact required fact is absent from canonical VERIFIED/database/docs.

## Current State
### Runtime-confirmed working
- User-captured raw MapID/X/Y persistence.
- CleanRoute movement to the captured position.
- Stop path / dismount behavior sufficient to interact with NPC.
- `ClickNPC(339)` opens Đỗ Thanh Đằng's GameDialog at Lâu Lan.
- v1.1.5 also reached/opened the Long Phá Thiên test flow sufficiently for the user to observe the same Treatment-dialog flicker after the tool action.

### Runtime-confirmed failing
- `v1.1.0–v1.1.2`: direct `UIButton.HandleClickEvent()` inside the current hook action path -> visible UI flicker, no Treatment progression.
- `v1.1.3`: `GameDialog.FunctionButtonClicked(liveButton)` via `MonoBehaviourExecutor.ExecuteUIObject` -> same flicker, no progression.
- `v1.1.4`: live `selectionID` + source-verified `CMD_SHOW_GAMEDIALOG=100007` payload `selectionID:-1` on NPC 339/Lâu Lan -> same flicker, no progression.
- `v1.1.5`: same v1.1.4 action layer on NPC 463/Long Phá Thiên/Lạc Dương -> user reports the **same flicker and no Treatment progression**.

### Built / under verification
- `v1.1.6-test`: changes only the execution boundary for the Treatment UI action. It constructs a legitimate `System.Action(target=live UIButton, callback=HandleClickEvent)` and enqueues it through `FGStudio.Engine.Utilities.MainThread.Execute(Action)` so the actual callback executes later from Unity `Update()`, not re-entrantly inside the Windows message hook.
- Before a real button action is allowed, v1.1.6 runs a harmless `CancellationTokenSource.Cancel()` Action proof through the same queue and requires later false->true observation.
- v1.1.6 BUILD/CI status: **PENDING at this snapshot**.
- v1.1.6 runtime status: **RUNTIME UNTESTED**.

## Affected Feature
Auto trị liệu NPC / dynamic GameDialog action execution.

## Architecture
Read-only/decision side:
`Controller -> per-PID shared memory -> WH_GETMESSAGE producer hook -> state read/inspect -> state machine`.

Canonical mutable-action target architecture:
`validated producer hook -> construct/root legitimate System.Action -> MainThread.Execute(Action) -> return from hook -> Unity Update -> DoExecuteWorks -> Action.Invoke -> observe state proof`.

The hook may be a valid managed Unity-main-thread producer context, but gameplay/UI mutation must not be treated as safe merely because it is invoked directly from that hook. Queueing restores the normal game-owned Update boundary and avoids re-entrant mutation from the message hook.

## Confirmed Technical Facts
- CONFIRMED STATIC: Đỗ Thanh Đằng = NPC/ResID `339`, MapID `5` Lâu Lan.
- CONFIRMED STATIC: Long Phá Thiên = NPC/ResID `463`, `ResName=PuTongXiaShi2`, MapID `3` Lạc Dương.
- CONFIRMED: NPC X/Y must not be inferred/hardcoded; current test target uses raw user-captured MapID/X/Y.
- CONFIRMED canonical Lua/KB: dynamic `GameDialog` has `Selections[selectionID] = visibleText`; generated button stores selectionID in `Tag`.
- CONFIRMED canonical Lua/KB: a GameDialog function choice sends `CMD_SHOW_GAMEDIALOG = 100007`, payload `selectionID:SelectedItemID`; ordinary no-award choice commonly uses `SelectedItemID=-1`.
- CONFIRMED canonical UI lifecycle: inbound `CMD_SHOW_GAMEDIALOG` destroys the previous GameDialog and may create a new one. Therefore a visible dialog flicker can mean reconstruction and is **not** proof of Treatment success.
- CONFIRMED canonical MainThread: `MainThread.Execute(Action)` enqueues; Unity `Update()` calls `DoExecuteWorks()`, which dequeues and invokes Actions.
- CONFIRMED frozen-client Action donor: legitimate `System.Action` construction accepts managed target + callback `MethodInfo*`; frozen direct constructor locator `GameAssembly + 0x49F810` is diagnostic/build-specific evidence, not universal identity.
- CONFIRMED protocol rule: BUILD/CI PASS does not establish runtime PASS.

## Current Root-Cause Assessment
### DISPROVEN / strongly weakened
**“NPC 339/Lâu Lan alone is the root cause.”**
Reason: the user reports the same flicker/non-progression using the v1.1.5 A/B target NPC 463 in Lạc Dương.

### LIKELY, not yet CONFIRMED
A shared mutation/execution-boundary issue exists in the external bridge. Earlier versions performed the decisive action directly/re-entrantly from the `WH_GETMESSAGE` hook or via an invocation path rooted in that producer request. Canonical client knowledge says mutable actions should be queued through `MainThread.Execute(Action)` and execute later in normal Unity Update.

This is the hypothesis v1.1.6 isolates. It must not be called root cause until runtime proof succeeds and Treatment behavior changes accordingly.

## Important Files
- `AI_START_HERE.md` — mandatory startup pointer for every version.
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md` — mandatory engineering-lineage protocol.
- `AI_CLIENT_ANALYSIS_RULES.txt` — mandatory canonical-client analysis rule.
- `src/bridge_mainthread_v1_1_6.inc` — CTS proof + queued live UIButton event implementation.
- `src/bridge.cpp` — selects v1.1.6 as active heal-choice implementation while retaining old versions as historical helpers.
- `src/bridge_part04.inc` — UI discovery and v1.1.3 historical callback implementation.
- `src/bridge_heal_packet_v1_1_4.inc` — historical v1.1.4 direct semantic packet implementation; no longer active for v1.1.6 Treatment choice.
- `src/controller_part04.inc` / `src/controller_part05.inc` — heal state machine and MapID->NPC test mapping.
- `docs/features/AUTO_HEAL_NPC.md` — feature lineage/current contract.
- `docs/bugs/BUG_REGISTRY.md` — stable bug record.
- `docs/evidence/EVIDENCE_REGISTRY.md` — runtime/source/CI evidence.
- `docs/decisions/DECISIONS.md` — active architecture decisions.
- `docs/history/VERSION_v1.1.6.md` — detailed current version record.

## Important IDs / APIs / Locators
- NPC 339 = Đỗ Thanh Đằng, Map 5 Lâu Lan.
- NPC 463 = Long Phá Thiên, Map 3 Lạc Dương.
- `CMD_SHOW_GAMEDIALOG = 100007`.
- `FGStudio.Engine.Utilities.MainThread.Execute(System.Action)`.
- Frozen Action ctor locator: `GameAssembly + 0x49F810` — build-specific diagnostic/donor only.
- Proof target: `System.Threading.CancellationTokenSource.Cancel()`; proof observable: `IsCancellationRequested false -> true`.

## Working Mechanisms
- Runtime coordinate capture/persistence.
- Route/AutoPath to captured target.
- StopPath/dismount.
- Semantic `ClickNPC` opening NPC interaction.
- Live GameDialog/button discovery by text.

## Failed / Unsafe Mechanisms
### FAILED-001 — inferred coordinate scale
Attempt: infer displayed 294:172 as 29400,17200.
Result: wrong target coordinates.
Lesson: raw runtime capture only.

### FAILED-002 — direct `UIButton.HandleClickEvent()` in hook path
Versions: v1.1.0–v1.1.2.
Result: visual flicker, no Treatment transition.
Lesson: direct/re-entrant invocation from the hook is not accepted as equivalent to a normal Unity UI event. Do not retry this exact execution boundary.
Retry condition: only through a materially different execution boundary with new evidence — v1.1.6 queues the callback through MainThread and is therefore a distinct experiment.

### FAILED-003 — resolver-only fix
Version: v1.1.2.
Result: Treatment still failed.
Lesson: UI discovery alone was not root cause.

### FAILED-004 — `ExecuteUIObject(GameDialog.FunctionButtonClicked(liveButton))`
Version: v1.1.3.
Result: same flicker/no progression.
Lesson: source-correct callback semantics do not prove the external invocation boundary is correct.

### FAILED-005 — direct source-verified GameDialog packet on NPC 339
Version: v1.1.4.
Result: same flicker/no progression.
Lesson: correct source-level payload alone did not establish end-to-end runtime action success in this bridge.

### FAILED-006 — NPC/map isolation did not change symptom
Version: v1.1.5.
Attempt: same v1.1.4 action layer on Long Phá Thiên 463/Lạc Dương.
Result: user reports the same flicker/no progression.
Lesson: NPC 339/Lâu Lan-specific fault is not a sufficient explanation; investigate common action bridge/state boundary.

## Hard Architectural Rules
- Read mandatory startup docs before every version.
- Read canonical client KB narrowly before binary work.
- No broad reverse when exact fact already exists.
- No hardcoded NPC X/Y.
- No guessed universal Treatment selection ID.
- No stale UI pointer across UI transition.
- Do not use visible flicker as success proof.
- Do not sleep on the Unity/main-thread hook as readiness/success proof.
- Max one mutable action in flight per PID.
- Queue mutable UI/game actions through verified MainThread action boundary once that bridge proof passes.
- Never enqueue then synchronously wait for the Action callback in the same WH_GETMESSAGE invocation.
- BUILD PASS != RUNTIME PASS.

## Known Bugs
### BUG-001 — Auto Treatment GameDialog action flickers but does not advance
Status: OPEN.
Versions observed bad: v1.1.0 through v1.1.5.
Next experiment: v1.1.6 MainThread queued UIButton event after CTS proof.

## Current Limitations
- Full Treatment chain has never achieved runtime PASS.
- Confirmation/`Ta biết rồi` path has not been reached after a successful Treatment transition in this series.
- v1.1.6 Action construction uses a frozen-snapshot direct Action constructor locator after semantic class/method checks; it must be treated as build-specific and runtime-tested.
- The current route/ClickNPC actions are intentionally not migrated in v1.1.6 because they are already runtime-working; this version isolates only the broken UI mutation boundary.

## Open Questions
1. Does the CTS MainThread proof pass repeatedly on the live client without crash/corruption?
2. After proof PASS, does queued `UIButton.HandleClickEvent` perform a real Treatment transition instead of mere flicker?
3. If it still flickers with proof PASS, what exact GameDialog data/selection changes before vs after a manual human Treatment click compared with the queued action?
4. What real follow-up UI occurs after successful Treatment: MessageBox, dynamic GameDialog, or server-side state change?

## Next Development Priorities
1. Finish CI/build for v1.1.6.
2. Runtime test and preserve log from `MAINTHREAD_PROOF` through queued Treatment action.
3. If CTS proof fails: fix only the delegate/MainThread bridge stage indicated by log.
4. If CTS proof passes but Treatment still fails: do one targeted manual-vs-tool dialog/request/state trace; do not change NPC again and do not broad-reverse the client.
5. Only port to production after repeated complete runtime PASS without crash/disconnect.

## Knowledge Index
- Feature: `docs/features/AUTO_HEAL_NPC.md`
- Bug: `docs/bugs/BUG_REGISTRY.md#bug-001`
- Decisions: `docs/decisions/DECISIONS.md`
- Evidence: `docs/evidence/EVIDENCE_REGISTRY.md`
- Current version: `docs/history/VERSION_v1.1.6.md`
- Older detailed handoffs remain under `docs/` and root `VERSION_v1.1.x.md` files.

---

# VERSION LINEAGE SUMMARY

## v1.1.0-test — 2026-08-16
Initial Auto Heal test using NPC 339; direct UIButton event path. BUILD PASS; runtime Treatment FAIL.

## v1.1.1-test — 2026-08-16
Removed inferred X/Y; added user runtime coordinate capture. Route behavior became usable.

## v1.1.2-test — 2026-08-16
UI resolver adjustment. NPC open PARTIAL PASS; Treatment FAIL.

## v1.1.3-test — 2026-08-16
Tried exact GameDialog Lua callback through ExecuteUIObject. BUILD PASS; runtime Treatment FAIL.

## v1.1.4-test — 2026-08-16
Tried live selectionID + verified packet/payload. BUILD PASS; runtime FAIL on NPC 339/Lâu Lan.

## v1.1.5-test — 2026-08-16
A/B NPC/map isolation: Map3->463, Map5->339, coordinates still user-captured. BUILD PASS run `31909204317`. User runtime report: NPC 463/Lạc Dương shows same flicker/no progression. RUNTIME FAIL for Treatment.

## v1.1.6-test — 2026-08-16
Based on v1.1.5. Reason: identical failure across two NPC/maps points to common execution boundary rather than a sole NPC fault. Added mandatory V2 protocol/TXT/startup contract; added CTS proof and MainThread-queued live UIButton event. BUILD/CI PENDING; RUNTIME UNTESTED.

---

# DECISION SUMMARY
- DEC-001 ACTIVE: user captures raw NPC target coordinates; no inferred X/Y.
- DEC-002 ACTIVE: canonical client repo is read-first; binary reverse targeted-only.
- DEC-003 ACTIVE: state-driven progression; no fixed Sleep as proof.
- DEC-004 SUPERSEDED for current experiment: direct GameDialog packet is source-correct but failed end-to-end in v1.1.4/v1.1.5.
- DEC-005 ACTIVE: for v1.1.6, broken heal UI mutation is queued through `MainThread.Execute(Action)` after an isolated CTS proof; no direct hook mutation is considered authoritative.

# CORRECTION SUMMARY
- CORRECTION-001: direct HandleClickEvent inside hook is DEPRECATED/FAILED.
- CORRECTION-002: ExecuteUIObject callback path is DEPRECATED as authoritative action path.
- CORRECTION-003: v1.1.4 packet path is source-correct but runtime FAILED on 339.
- CORRECTION-004: hypothesis “Lâu Lan NPC alone causes the failure” is DISPROVEN as a sole explanation by identical v1.1.5 behavior at Long Phá Thiên/Lạc Dương.

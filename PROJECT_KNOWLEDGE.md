# PROJECT KNOWLEDGE

## Project Identity
- Repository: `ngmthang-g/TEST-chuc-nang-than-long-2`
- Purpose: independent runtime test lab for Thần Long automation features; not production.
- Development branch: `agent/auto-tri-lieu-v1.1.0`
- Current version: `v1.1.8-test`
- Last known-good full Auto trị liệu: **NONE / UNKNOWN**.
- Runtime-known-good partial subsystem: raw target capture -> route -> dismount -> NPC interaction/open.

## Mandatory Startup / Memory Contract
Before every analysis, code change, build or version, read in order:
1. `AI_START_HERE.md`
2. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
3. `AI_CLIENT_ANALYSIS_RULES.txt`
4. this `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. affected feature / BUG / DEC / EVID / version docs
7. current source/tests.

The two user-supplied protocol/rules files are mandatory inherited project memory and must not be silently removed or bypassed.

When client internals are needed use canonical repo `ngmthang-g/clinent-game-than-long-DATA-2222`:
`AI_INDEX.md -> AI_BOOTSTRAP.md -> AUTO_TOOL_SCOPE.md -> AI_ROUTER.md -> one matching contexts/BUILD_*.md -> REQUIRED docs -> exact VERIFIED/database lookup`.

No broad reverse. Binary/native work is targeted-only when an exact fact required by the task is absent from canonical VERIFIED/database/docs.

## Current State
### Runtime-confirmed working
- raw MapID/X/Y capture/persistence;
- route/AutoPath to target;
- StopPath/dismount sufficient for NPC interaction;
- NPC interaction opens visible server-driven GameDialog;
- NPC 339/Lâu Lan and NPC 463/Lạc Dương can reproduce the overall Treatment symptom.

### Runtime-confirmed failing / user report
- no version has completed full `Trị liệu -> follow-up -> completion proof`.
- latest user report on 2026-08-16: tool still visibly flickers the Treatment dialog when opening the NPC; exact tested artifact/version was not stated, therefore do not falsely assign this report to a specific binary.

### Build state
- v1.1.6: CI/BUILD PASS, runtime artifact-version result not preserved as confirmed evidence.
- v1.1.7 final CI run `31925922772`: **CI FAILED** at controller compile due broken `kTitle` macro/scope wiring. Therefore v1.1.7 was never a valid build handoff.
- v1.1.8 source commit `1da643b8384dfa64a2523938dffb4ddd9885b181`: **CI/BUILD PASS** in run `31926671467`.
- v1.1.8 artifact: `ThanLongTestAutoHeal-v1.1.8`, artifact ID `9258076757`, digest `sha256:390ae27248eebee99db7b1a6a463e291a797150a23de770aeed6d29aed8cd9d5`.
- v1.1.8 runtime remains **RUNTIME UNTESTED**.

## Critical v1.1.8 Correction
v1.1.7 did not fully implement its stated observer guarantee:
- `FirstTextInSubtreeV117()` returned after the **first** non-empty descendant text, so a later `Trị liệu` label in the same clickable subtree could still be missed;
- `WaitTreatment` still retried `ClickNPC` after 4 seconds whenever Lua `GameDialog` presence was transiently absent, so server-driven destroy/recreate timing could still trigger another NPC open;
- therefore `HealDialogPresent` alone did not make the transaction single-open.

The stronger v1.1.8 model is:
`one NPC open -> observe current GameDialog -> scan all descendant text -> read live Tag/selectionID -> one MainThread action -> wait for new state`.

Root cause of BUG-001 remains **LIKELY**, not CONFIRMED, until runtime proves that eliminating the reopen loop removes flicker.

## Architecture
Read/decision:
`Controller -> per-PID shared memory -> WH_GETMESSAGE producer -> immutable observation -> state machine`.

Mutable boundary:
`producer -> valid System.Action -> MainThread.Execute(Action) -> return -> Unity Update -> callback -> fresh state proof`.

v1.1.8 dialog/action path:
`NPC interaction -> current GameDialog presence -> all-descendant semantic match -> read current button.Tag selectionID -> fail-closed semantic gate -> MainThread queued live action -> observe next dialog/result`.

## Confirmed Canonical Facts
- Đỗ Thanh Đằng = NPC/ResID `339`, MapID `5` Lâu Lan.
- Long Phá Thiên = NPC/ResID `463`, MapID `3` Lạc Dương; healer role is not canonical-static proven.
- NPC X/Y in this lab are user-captured raw runtime values; never infer display scale.
- `GameDialog.Selections[selectionID] = visibleText`.
- generated dynamic button stores `selectionID` in `Tag`.
- GameDialog selection semantics use `CMD_SHOW_GAMEDIALOG=100007`, payload `selectionID:SelectedItemID`.
- inbound GameDialog lifecycle destroys/recreates current dialog when new server dialog data is handled.
- `MainThread.Execute(System.Action)` queues to Unity Update.
- BUILD/CI PASS != RUNTIME PASS.

## Current v1.1.8 Implementation
New files:
- `src/bridge_dialog_v1_1_8.inc`
- `src/bridge_action_v1_1_8.inc`

Behavior:
- searches every reachable text descendant under a clickable candidate instead of only the first label;
- normalizes whitespace/NBSP/simple markup;
- records observed labels/count;
- reads the matched current GameDialog button `Tag` and requires `selectionID > 0` before GameDialog action enqueue;
- retains v1.1.6 CTS/MainThread dispatcher proof;
- uses the freshly resolved button for the queued action and never caches it across a UI transition;
- removes the WaitTreatment `ClickNPC` retry loop; absence/unresolved dialog now times out fail-closed instead of reopening the NPC.

Expected diagnostics:
- `DIALOG_V118 MATCH`
- `DIALOG_V118 NO MATCH`
- `selectionID=<live id>` or explicit `selectionID=UNRESOLVED(...)`
- `MAINTHREAD_PROOF PASS`
- `ACTION_V118 ENQUEUED verified GameDialog choice`

## Important Files
- `AI_START_HERE.md`
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
- `AI_CLIENT_ANALYSIS_RULES.txt`
- `src/bridge_dialog_v1_1_8.inc`
- `src/bridge_action_v1_1_8.inc`
- `src/bridge_mainthread_v1_1_6.inc`
- `src/bridge.cpp`
- `src/controller_part04.inc` / `src/controller_part05.inc`
- `docs/features/AUTO_HEAL_NPC.md`
- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/history/VERSION_v1.1.8.md`

## Failed / Unsafe Mechanisms Summary
- inferred coordinate scale — FAILED.
- fixed Sleep as UI-ready proof — unsafe.
- repeated `ClickNPC` while waiting for a current server GameDialog — removed from v1.1.8 WaitTreatment.
- treating visible flicker as proof Treatment action ran — DEPRECATED reasoning.
- claiming an action layer failed without action-stage evidence — DEPRECATED reasoning.
- v1.1.7 first-text-only descendant matcher — superseded by all-descendant match.

Historical v1.1.0–v1.1.7 experiments remain preserved in CHANGELOG/version docs.

## Hard Rules
- mandatory startup files every version;
- canonical client KB first;
- no broad binary reverse for solved facts;
- no hardcoded/inferred NPC X/Y;
- no guessed fixed Treatment selectionID;
- no stale UI pointer across transition;
- no fixed Sleep as success proof;
- one mutable action in flight per PID;
- do not reopen NPC merely because current dialog discovery is temporarily absent/unresolved;
- distinguish implementation/build/runtime evidence;
- preserve failed attempts and corrections.

## Known Bugs
### BUG-001 — Treatment GameDialog flicker/no progression
Status: OPEN.
Current v1.1.8 theory under test: transient/missed dialog observation plus retry-driven NPC reopen contributes directly to flicker; exact action-stage success after removal remains UNKNOWN.

## Open Questions
1. Does v1.1.8 show exactly one successful `ClickNPC` per Treatment transaction after the initial NPC open?
2. Which exact live `selectionID` is reported for visible `Trị liệu`?
3. Does `ACTION_V118` advance the server dialog/state after the selection gate passes?
4. What actual follow-up sequence appears: MessageBox confirm, second GameDialog, acknowledgement, or immediate result?
5. What HP/money/dialog evidence should establish runtime Treatment PASS?

## Next Development Priority
Runtime-test the build-valid v1.1.8 artifact and preserve log from the initial NPC call through `DIALOG_V118`, `selectionID`, `MAINTHREAD_PROOF`, `ACTION_V118` and the next state. Do not broad-reverse or rotate more NPCs before that evidence.

## Knowledge Index
- `docs/features/AUTO_HEAL_NPC.md`
- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/decisions/DECISIONS.md`
- `docs/history/VERSION_v1.1.8.md`
- older `docs/history/VERSION_v1.1.x.md` / root version/handoff docs preserve prior lineage.

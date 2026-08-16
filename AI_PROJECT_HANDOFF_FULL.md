# AI PROJECT HANDOFF FULL — THẦN LONG AUTO TRỊ LIỆU TEST LAB

> Đây là file handoff một-file dành cho AI/người phát triển mở trực tiếp từ artifact ZIP.  
> Nó KHÔNG thay thế knowledge base đầy đủ trong repo, nhưng phải đủ để một AI mới hiểu trạng thái hiện tại trước khi sửa code.

## 0. MANDATORY RULES

Project: `ngmthang-g/TEST-chuc-nang-than-long-2`  
Development branch: `agent/auto-tri-lieu-v1.1.0`  
Current version in this handoff: **v1.1.11-test**  
Feature: **Auto trị liệu NPC**  
Full feature known-good: **NONE**  
BUG: **BUG-001 OPEN**

Trước khi sửa version mới, đọc theo thứ tự:
1. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
2. `AI_CLIENT_ANALYSIS_RULES.txt`
3. file này `AI_PROJECT_HANDOFF_FULL.md`
4. `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. affected feature / BUG / DEC / EVID / history trong repo
7. source/tests hiện tại.

Canonical client knowledge repo:
`ngmthang-g/clinent-game-than-long-DATA-2222`

Mandatory client route:
`AI_INDEX.md -> AI_BOOTSTRAP.md -> AUTO_TOOL_SCOPE.md -> AI_ROUTER.md -> contexts/BUILD_AUTO_HEAL.md -> REQUIRED docs -> exact VERIFIED/database fact`.

Do not broad reverse-engineer the client again. Binary/native inspection is only allowed when the exact needed fact is absent from canonical VERIFIED/database knowledge.

**BUILD/CI PASS != RUNTIME PASS.** Never silently promote untested behavior.

---

## 1. PROJECT PURPOSE

This repo is a runtime **test lab**, not production tool. Goal is to prove a clean Auto Heal transaction against the real client before porting to the main tool:

`stable state -> route -> one NPC open -> observe current server GameDialog -> identify live Trị liệu selection -> one semantic action -> observe real follow-up -> verify heal/result`.

Protected architecture direction:
`read-only state -> state machine -> max one mutable action -> valid game/MainThread context -> game-owned semantic action -> fresh state proof`.

---

## 2. RUNTIME-CONFIRMED WORKING PATH

Confirmed across user tests:
- raw MapID/X/Y can be read from the client;
- route/mount/AutoPath works;
- tool can stop/dismount;
- Map 5 healer candidate NPC `339 = Đỗ Thanh Đằng` can be interacted with semantically;
- `ClickNPC(339)` opens the intended server-driven GameDialog;
- v1.1.8 removed the historical WaitTreatment controller reopen loop for the tested transaction: one NPC open then fail-closed observation.

Do not casually rewrite these working parts while investigating later layers.

---

## 3. CANONICAL GAMEDIALOG FACTS

VERIFIED from client Lua/UI knowledge:
- `GameDialogData.Selections[selectionID] = visibleText`.
- Selection IDs are current server/runtime data. There is **no verified universal static Trị liệu ID**.
- `CMD_SHOW_GAMEDIALOG = 100007`.
- submit payload = `selectionID:SelectedItemID`.
- ordinary function choice commonly uses `SelectedItemID=-1`.
- built-in AutoFight quest/FuBen flows inspect actual current Selections instead of assuming a fixed button ID.
- inbound GameDialog handling may destroy/recreate GameDialog; visual rebuild/flicker is not completion proof.
- MessageBox OK follows its stored callback path; if MessageBox is current, use its semantic callback rather than forcing it into the GameDialog selection model.

---

## 4. BUG-001 LAYER-BY-LAYER RUNTIME HISTORY

### v1.1.0–v1.1.2
Initial route + UI button experiments. NPC/dialog open could work, complete Treatment flow did not.

### v1.1.3
Tried `GameDialog.FunctionButtonClicked(liveButton)` through `MonoBehaviourExecutor.ExecuteUIObject`. Still shared the old UI button discovery dependency.

### v1.1.4
Tried live button Tag/selectionID + exact GameDialog packet. Later correction: the action was still gated by the same button discovery, so historical failure did NOT independently prove packet/server failure.

### v1.1.5
Alternate NPC/map reproduced the same overall no-progress symptom; weakened NPC-339-only theory.

### v1.1.6
Added CTS/MainThread proof experiment and managed Action architecture support. BUILD PASS; no preserved full Treatment runtime PASS.

### v1.1.7
Observer redesign, but final CI failed and source still had first-text-only/reopen gaps. Not a valid handoff build.

### v1.1.8 — IMPORTANT RUNTIME ISOLATION
User runtime:
- route/open PASS;
- exactly one `ClickNPC(339)`;
- GameDialog present;
- every observer sample for ~15 seconds returned effectively:
  `DIALOG_V118 NO MATCH ... clickable=0 • texts=0 • labels=<none>`;
- fail closed; no NPC reopen.

Conclusion:
- anti-reopen component = runtime PASS for that transaction;
- `FindUI(GameDialog) -> UIRoot/CoreChildren -> UIButton/Text` representation = runtime FAILED for this dynamic dialog;
- do NOT return to tree-depth/text normalization tuning on that same empty representation.

### v1.1.9 — MANAGER RESOLVER FAILURE
User runtime after GameDialog opened:
`LUA_DIALOG_V119 PROBE FAIL • LUA_DIALOG_V119: LuaSystemManager instance unresolved`

Conclusion:
- v1.1.9 failed before `LuaEnv`, before `DoString`, before Selections, before Treatment ID/action/packet;
- downstream layers remained UNKNOWN, not failed.

Source flaw found:
`ResolveLuaEnvV119()` forced manager-instance resolution before checking whether `get_LuaEnv` itself could be static, and manager lookup only tried `get_Instance` + four guessed field names.

### v1.1.10 — LUAENV REACHED, DOSTRING RESOLVER FAILS
Latest user runtime (2026-08-16):

```text
Đã gửi AutoPath tới map=5 x=9454 y=5477
Đã gửi lệnh xuống ngựa
Đã gọi ClickNPC npcID=339
LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)
```

The DoString failure repeats while the dialog is open.

Critical interpretation from current source control flow:
- `RunLuaChunkV120()` first calls `ResolveLuaEnvV120()`;
- only after a non-null LuaEnv does it call old `FindLuaDoStringV119(...)`;
- therefore reaching `DoString unresolved` proves the v1.1.10 manager/LuaEnv boundary advanced successfully far enough to return a LuaEnv object;
- **LuaSystemManager/LuaEnv resolver = RUNTIME PARTIAL PASS**;
- **old DoString method resolver = RUNTIME FAIL**;
- Lua chunk execution, Selections, Treatment ID, action packet and server follow-up are still NOT REACHED / UNKNOWN.

Do not describe this as a generic “Lua failure”.

---

## 5. WHY v1.1.11 EXISTS

v1.1.10 reused old `FindLuaDoStringV119`, which only accepted a narrow method shape:
- non-static method named exactly `DoString`;
- mostly exact first parameter `System.String`;
- old enumeration path;
- no `System.Byte[]` overload support;
- weak diagnostics about actual runtime LuaEnv class/method signatures.

Upstream Tencent xLua source (research guidance, NOT proof of the exact client build) exposes both:
- `DoString(byte[] chunk, string chunkName, LuaTable env)`
- `DoString(string chunk, string chunkName, LuaTable env)`.

Therefore v1.1.11 fixes the exact failing boundary while keeping runtime metadata as truth.

---

## 6. v1.1.11 ACTIVE IMPLEMENTATION

### File: `src/bridge_lua_dostring_v1_1_11.inc`
Uses runtime-proven `ResolveLuaEnvV120()` and replaces only DoString resolution.

Resolver behavior:
1. obtain actual runtime class of returned LuaEnv;
2. also inspect declared return class of `LuaSystemManager.get_LuaEnv`;
3. enumerate non-static `DoString` methods on actual/declared class and parents;
4. prefer supported `System.String` chunk overload;
5. support `System.Byte[]` chunk overload as fallback;
6. support arity 3/2/1 when metadata really exposes it;
7. require second parameter to be String when present; require third parameter to be a reference type when present;
8. additionally try direct method lookup by arity because v1.1.10 failure might involve iterator/metadata visibility;
9. if unresolved, emit bounded diagnostic containing actual class, declared class, discovered DoString signatures and nearby interesting methods;
10. if Byte[] overload is chosen, allocate a managed `System.Byte[]` and UTF-8 encode the Lua chunk into it using IL2CPP array allocation; no raw client pointer guessing.

Expected failure marker:
`LUA_DOSTRING_V121 unresolved • actual=... • declared=... • DoString={...} • near={...}`

Expected successful dialog marker:
`LUA_DIALOG_V121 • route=... • GD=present • ... • T=<live id> ...`

### File: `src/bridge_action_v1_1_11.inc`
Only consumes V121 live IDs. It still:
- requires safe action prerequisites;
- re-reads current Selections immediately before action;
- refuses absent/guessed/cached ID;
- sends exact `CMD_SHOW_GAMEDIALOG=100007` with `<currentID>:-1` only after the semantic current choice exists;
- logs `ACTION_V121`;
- MessageBox confirmation remains semantic callback-based.

### Preserved lineage
v1.1.8, v1.1.9 and v1.1.10 implementations remain compiled under legacy names where useful. Do not erase failed attempts.

---

## 7. CURRENT HARD RULES / DO-NOT-DO

- No inferred healer X/Y as a universal static truth; this test lab uses user/runtime-captured position.
- No hardcoded Treatment selectionID.
- No stale UI pointer across server-driven dialog transitions.
- No controller `ClickNPC` retry after successful initial dialog open while Treatment observation is unresolved.
- No return to UIRoot/UIButton depth tuning for the v1.1.8-confirmed empty representation.
- No fixed Sleep as success proof.
- No claim that packet/server rejected Treatment until a current live Treatment ID is actually observed and sent.
- No broad GameAssembly reverse when canonical KB already answers the question.
- One mutable action in flight per PID.
- BUILD PASS, CI PASS and self-test PASS must remain distinct from runtime PASS.

---

## 8. CURRENT STATUS TABLE

- Route/mount/AutoPath: **RUNTIME PASS / protected partial path**
- Dismount: **RUNTIME PASS / protected partial path**
- ClickNPC 339 opens intended GameDialog: **RUNTIME PASS**
- WaitTreatment controller anti-reopen: **RUNTIME PASS for tested v1.1.8 transaction**
- UIRoot dynamic GameDialog button representation: **RUNTIME FAIL / abandoned active path**
- v1.1.9 LuaSystemManager singleton resolver: **RUNTIME FAIL / superseded**
- v1.1.10 LuaSystemManager -> LuaEnv: **RUNTIME PARTIAL PASS**
- v1.1.10 old DoString resolver: **RUNTIME FAIL**
- v1.1.11 overload-aware DoString resolver: **RUNTIME UNTESTED**
- live Treatment ID: **UNKNOWN / not yet observed**
- Treatment action send: **UNKNOWN / not yet reached in current Lua path**
- follow-up MessageBox/GameDialog: **UNKNOWN**
- HP/money completion proof: **UNKNOWN**
- complete Auto trị liệu: **NO KNOWN-GOOD VERSION**

---

## 9. NEXT RUNTIME TEST — READ THE EARLIEST NEW MARKER

Run the v1.1.11 artifact and capture log from `Đã gọi ClickNPC` onward.

Interpret in this order:

### Case A — DoString still unresolved
Look for:
`LUA_DOSTRING_V121 unresolved ...`

Use the actual/declared class and signature list from that line. Do not redesign route/NPC/GameDialog.

### Case B — Lua chunk executes but Treatment not found
Look for:
`LUA_DIALOG_V121 ... T=0 ... raw={...}`

Then investigate only current runtime table path/text representation from the returned samples.

### Case C — Treatment found
Look for:
`LUA_DIALOG_V121 ... T=<positive id>`

Only then evaluate:
- `MAINTHREAD_PROOF` / action prerequisite;
- `ACTION_V121 SENT ... selectionID=<same current id> • payload=<id>:-1`;
- fresh server/UI result.

### Case D — action sent but no expected result
Only at this point investigate request/follow-up/server semantics. Capture next GameDialog/MessageBox, HP and money state.

---

## 10. ARTIFACT/HANDOFF CONTRACT — ADDED AFTER USER REPORT

User correctly reported that prior ZIPs only exposed EXE/DLL and did not contain the requested consolidated project knowledge.

From v1.1.11 onward every CI artifact must include at minimum:
- `ThanLongTestAutoHeal_vX.Y.Z.exe`
- `ThanLongCleanRouteBridge.dll`
- `AI_PROJECT_HANDOFF_FULL.md`  ← read this first when only ZIP is available
- `AI_START_HERE.md`
- `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
- `AI_CLIENT_ANALYSIS_RULES.txt`
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
- `BUILD_EVIDENCE.txt`

`BUILD_EVIDENCE.txt` is generated during a successful build and records version, exact GitHub source SHA/run when available, artifact name, BUILD state and explicit runtime-untested state for the new build.

This packaging rule is a project requirement, not optional decoration.

---

## 11. IMPORTANT SOURCE FILES

Current active path:
- `src/bridge_lua_manager_v1_1_10.inc` — runtime-proven path to LuaEnv;
- `src/bridge_lua_dostring_v1_1_11.inc` — current DoString resolver/probe;
- `src/bridge_action_v1_1_11.inc` — current live selection action;
- `src/bridge.cpp` — version lineage wiring;
- `src/controller_part04.inc` — Auto Heal state machine / no WaitTreatment reopen;
- `src/protocol.h` — bridge shared protocol;
- `build.cmd` — architecture audit/build/handoff packaging;
- `.github/workflows/build.yml` — CI artifact content.

Knowledge source of truth:
- `PROJECT_KNOWLEDGE.md`
- `CHANGELOG.md`
- `docs/features/AUTO_HEAL_NPC.md`
- `docs/bugs/BUG_REGISTRY.md`
- `docs/evidence/EVIDENCE_REGISTRY.md`
- `docs/decisions/DECISIONS.md`
- `docs/history/VERSION_v1.1.10.md`
- `docs/history/VERSION_v1.1.11.md`
- `docs/investigations/V119_LUA_MANAGER_RUNTIME_FINDING.md`
- `docs/investigations/V120_DOSTRING_RUNTIME_FINDING.md`

---

## 12. HANDOFF PRINCIPLE

Continue from the earliest runtime-proven failing layer. Do not restart the investigation from mouse clicking, NPC coordinates, UIRoot or guessed selection IDs.

Current continuation point for v1.1.11 is:

`GameDialog open -> LuaEnv resolved -> DoString overload resolver -> execute bounded current-dialog probe -> current Selections -> live Treatment ID -> one semantic action -> real result proof`.

If evidence contradicts this handoff, runtime evidence outranks this file and this file/knowledge base must be corrected immediately.

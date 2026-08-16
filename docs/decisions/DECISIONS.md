# DECISION REGISTRY

## DEC-001 — Mandatory per-version startup knowledge
**Status:** ACTIVE  
Every version starts from `AI_START_HERE.md`, V2 protocol, client-analysis TXT, project knowledge/changelog, affected docs/source. Build audit preserves the two user-supplied mandatory files.

## DEC-002 — Canonical client KB before reverse
**Status:** ACTIVE  
Use `clinent-game-than-long-DATA-2222/AI_INDEX.md` routing and exact VERIFIED/database facts before any targeted binary work. No repeated broad reverse.

## DEC-003 — Runtime coordinates are user-captured in this test lab
**Status:** ACTIVE  
Do not infer/hardcode NPC X/Y. Current map mapping: Map 3 -> NPC 463 test candidate; Map 5 -> NPC 339 Đỗ Thanh Đằng.

## DEC-004 — MainThread remains the preferred production mutation boundary
**Status:** ACTIVE  
For live Unity/UI mutations, prefer legitimate managed `System.Action -> MainThread.Execute -> Unity Update`. The narrow GameDialog packet proof does not revoke this architecture.

## DEC-005 — MainThread proof is asynchronous
**Status:** ACTIVE  
Never enqueue and synchronously wait inside the same hook request. CTS proof begins, returns, and is observed later.

## DEC-006 — Never reopen NPC from WaitTreatment because dialog observation is unresolved
**Status:** ACTIVE / RUNTIME SUPPORTED  
The old retry loop polluted evidence and could reconstruct GameDialog. v1.1.8 removed it; user runtime log showed one NPC interaction for the tested transaction.

## DEC-007 — Dynamic GameDialog selection identity must come from current runtime state
**Status:** ACTIVE, wording superseded by DEC-008  
Never hardcode Treatment ID. Identity remains runtime/server data even though the source moved away from UIRoot button Tag.

## DEC-008 — Abandon UIRoot/UIButton as active observer for this dynamic GameDialog
**Date / Version:** 2026-08-16 / v1.1.9  
**Status:** ACTIVE  
Do not continue tree-depth/text/UIButton tuning against `FindUI("GameDialog") -> UIRoot/CoreChildren`; runtime v1.1.8 proved zero relevant nodes while dialog presence existed. Use runtime Lua dialog data instead.

## DEC-009 — Use live Lua Selections + exact semantic GameDialog request as a narrow proof
**Date / Version:** 2026-08-16 / v1.1.9  
**Status:** ACTIVE FOR TEST LAB ONLY  
Observe current `Selections`; at action time re-read them and submit `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentSelectionID>:-1`. No cached/guessed ID. This is a diagnostic/business-action proof, not blanket permission to bypass preferred MainThread production architecture.

## DEC-010 — MessageBox confirmation is not a GameDialog selection unless runtime says so
**Date / Version:** 2026-08-16 / v1.1.9  
**Status:** ACTIVE  
If a live `MessageBox` exists, execute semantic `ButtonOKClicked()`. Otherwise a current GameDialog `Xác nhận` selection uses its actual current ID.

## DEC-011 — Resolve LuaEnv without assuming a LuaSystemManager singleton contract

**Date / Version:** 2026-08-16 / v1.1.10  
**Status:** ACTIVE  
**Trigger:** delivered v1.1.9 runtime repeatedly failed at `LuaSystemManager instance unresolved` before LuaEnv/DoString. Source audit showed v1.1.9 required a manager instance first and only tried `get_Instance` plus four guessed field names. Canonical KB does not verify those singleton details.

**Decision:** resolve the actual requirement — `LuaEnv` — in evidence order:
1. call `get_LuaEnv` directly if metadata marks it static;
2. if instance getter is required, use semantic `get_Instance` only if present;
3. enumerate bounded static **reference** fields through IL2CPP metadata APIs, accepting only candidates whose runtime class validates as LuaSystemManager/derived;
4. use Unity typed object lookup only if the manager class hierarchy proves it derives from UnityEngine.Object;
5. otherwise fail closed with diagnostics.

**Rejected:** inventing more singleton field names; raw pointer/heap guessing; broad GameAssembly reverse; returning to UIRoot; hardcoding a Treatment ID.

**Consequences:** `bridge_lua_manager_v1_1_10.inc` emits `LUA_MANAGER_V120` / resolver route evidence. The downstream `Selections` probe and semantic action remain unchanged so the version isolates one failing variable.
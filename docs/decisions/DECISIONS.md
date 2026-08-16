# DECISION REGISTRY

## DEC-001 — Mandatory per-version startup knowledge
**Status:** ACTIVE  
Every version starts from `AI_START_HERE.md`, V2 protocol, client-analysis TXT, consolidated handoff/current knowledge/changelog, affected docs/source. Build audit preserves the user-supplied mandatory files.

## DEC-002 — Canonical client KB before reverse
**Status:** ACTIVE  
Use `clinent-game-than-long-DATA-2222/AI_INDEX.md` routing and exact VERIFIED/database facts before targeted binary work. No repeated broad reverse.

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
**Status:** ACTIVE / RUNTIME PARTIAL PASS  
Delivered v1.1.9 failed at manager instance resolution. v1.1.10 resolves the actual requirement in evidence order: static `get_LuaEnv` first; then semantic/metadata-validated manager candidates; typed Unity lookup only when class hierarchy allows it. Latest v1.1.10 runtime progressed beyond this boundary and failed later at DoString lookup, so the LuaEnv resolution direction is protected while investigating the next layer.

## DEC-012 — Resolve DoString from live overload metadata, not one exact guessed signature

**Date / Version:** 2026-08-16 / v1.1.11  
**Status:** ACTIVE  
**Trigger:** v1.1.10 runtime reached LuaEnv and repeatedly failed inside old `FindLuaDoStringV119`, which expected a narrow String-first method shape.

**Decision:**
1. keep the runtime-proven v1.1.10 LuaEnv resolver unchanged;
2. inspect the actual runtime class of the returned LuaEnv and the declared `get_LuaEnv` return class;
3. enumerate current `DoString` overloads and validate their real parameter types;
4. prefer `System.String` first parameter;
5. support `System.Byte[]` first parameter by constructing a managed UTF-8 byte array;
6. support only safe metadata-matched arity 3/2/1;
7. retain direct lookup by arity as a second metadata path if iterator visibility is incomplete;
8. if unresolved, return actual class/signature diagnostics instead of guessing a method address.

**Context:** official Tencent xLua source contains both String and Byte[] DoString overloads, but that upstream source is guidance only; the exact client runtime metadata remains authoritative.

**Rejected:** hardcoded method RVA; assuming upstream xLua build identity; raw native call into an unverified overload; reverting to UIRoot; declaring packet/server failure.

**Consequences:** new markers `LUA_DOSTRING_V121`, `LUA_DIALOG_V121`, `ACTION_V121` isolate the next live stage.

## DEC-013 — Every delivered artifact must contain a one-file AI handoff and current knowledge bundle

**Date / Version:** 2026-08-16 / v1.1.11  
**Status:** ACTIVE  
**Trigger:** user reported that the v1.1.10 ZIP did not contain the requested consolidated project file; CI uploaded only EXE/DLL.

**Decision:** every CI artifact from v1.1.11 onward includes:
- EXE + bridge DLL;
- `AI_PROJECT_HANDOFF_FULL.md` as the first-read one-file handoff;
- `AI_START_HERE.md`;
- V2 protocol;
- client-analysis rules TXT;
- `PROJECT_KNOWLEDGE.md`;
- `CHANGELOG.md`;
- generated `BUILD_EVIDENCE.txt` with exact source SHA/run when running in GitHub Actions.

**Reason:** project knowledge is part of the product and must travel with the test artifact, not remain discoverable only by browsing the repo.

**Consequence:** `build.cmd` architecture audit rejects missing handoff files; workflow uploads the knowledge bundle explicitly.

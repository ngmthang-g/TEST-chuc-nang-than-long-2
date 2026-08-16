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
For live Unity/UI mutations, prefer legitimate managed `System.Action -> MainThread.Execute -> Unity Update`. v1.1.9 does not revoke this architecture.

## DEC-005 — MainThread proof is asynchronous
**Status:** ACTIVE  
Never enqueue and synchronously wait inside the same hook request. CTS proof begins, returns, and is observed later.

## DEC-006 — Never reopen NPC from WaitTreatment because dialog observation is unresolved
**Status:** ACTIVE / RUNTIME SUPPORTED  
The old retry loop polluted evidence and could reconstruct GameDialog. v1.1.8 removed it; user runtime log showed only one `ClickNPC` for the tested transaction.

## DEC-007 — Dynamic GameDialog selection identity must come from current runtime state
**Status:** ACTIVE, wording superseded by DEC-008  
Never hardcode Treatment ID. v1.1.8 tried to derive identity through live button Tag. Runtime proved the active UIRoot representation had zero button/text nodes, so the identity rule remains but its source moves to `Selections` directly.

## DEC-008 — Abandon UIRoot/UIButton as active observer for this dynamic GameDialog

**Date / Version:** 2026-08-16 / v1.1.9  
**Status:** ACTIVE  
**Decision:** Do not continue tuning tree depth, text normalization or UIButton matching against `FindUI("GameDialog") -> UIRoot/CoreChildren` for Treatment. Use server/runtime Lua dialog data instead.  
**Evidence:** v1.1.8 user log: GameDialog present, one NPC open, then repeated `clickable=0 • texts=0 • labels=<none>` for 15 seconds.  
**Canonical support:** `GameDialog.Selections[selectionID]=visibleText`; built-in AutoFight dialog flows inspect current selections.  
**Why:** the representation being scanned is empty for the relevant dynamic content in the tested runtime; another matcher on the same empty tree cannot recover information that is not there.  
**Consequences:** v1.1.8 UIRoot observer remains compiled/preserved only for lineage; v1.1.9 active observer reads Lua runtime data.

## DEC-009 — v1.1.9 uses live Lua Selections + exact semantic GameDialog request as a narrow proof

**Date / Version:** 2026-08-16 / v1.1.9  
**Status:** ACTIVE FOR TEST LAB ONLY  
**Decision:** At observation time, recover current `Selections` through Lua runtime. At action time, re-read them, match the requested semantic text, and send the canonical request `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentSelectionID>:-1`.  
**Guards:** no cached ID, no guessed ID, SafeForAction, current game hook thread, IL2CPP-attached thread, CTS/MainThread proof prerequisite, max one controller action in flight.  
**Why direct network for this proof:** v1.1.8 cannot obtain a current UIButton instance from the active representation, while the exact packet/payload construction is already source-verified. This isolates observer + semantic request from unavailable UI object plumbing.  
**Important limitation:** this is not a blanket production permission to bypass `MainThread.Execute`. It is a narrow diagnostic/business-action proof. If successful, production integration should still prefer the cleanest game-owned semantic boundary available for the final architecture.  
**Expected evidence:** `LUA_DIALOG_V119` with actual IDs followed by `ACTION_V119 SENT ... selectionID=<same live id> • payload=<id>:-1`, then a concrete server/UI/result transition.

## DEC-010 — MessageBox confirmation is not a GameDialog selection unless runtime says so

**Date / Version:** 2026-08-16 / v1.1.9  
**Status:** ACTIVE  
If a live `MessageBox` exists, execute its semantic `ButtonOKClicked()` callback. If no MessageBox exists and current `Selections` contains `Xác nhận`, treat that as a dynamic GameDialog selection. Never force one model onto the other.

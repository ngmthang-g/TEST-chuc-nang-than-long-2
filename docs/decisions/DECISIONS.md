# DECISION REGISTRY

## DEC-001 — Mandatory per-version startup knowledge

**Date / Version:** 2026-08-16 / v1.1.6  
**Status:** ACTIVE  
**Decision:** Every version must start from `AI_START_HERE.md`, the V2 protocol, client-analysis TXT, project knowledge/changelog, then affected docs/source.  
**Context:** user requires the MD/TXT to become persistent project memory rather than chat-only instructions.  
**Alternatives:** rely on chat memory; mention rules only in README.  
**Why Rejected:** not durable across sessions/AI handoff.  
**Evidence:** user requirement + V2 protocol.  
**Consequences:** build audit checks mandatory root protocol/rules files; future handoff must preserve them.

## DEC-002 — Canonical client KB before reverse

**Date / Version:** 2026-08-16  
**Status:** ACTIVE  
**Decision:** For client analysis, route through `clinent-game-than-long-DATA-2222` index/bootstrap/router/context pack and exact VERIFIED/database records. Binary/native analysis is only for exact missing facts.  
**Context:** canonical client is intentionally a large AI-native KB; repeated broad reverse wastes time and risks conflicting conclusions.  
**Alternatives:** re-scan GameAssembly/client each bug.  
**Why Rejected:** exact action/UI/packet/MainThread facts already exist.  
**Consequences:** every new client-facing investigation must name the relevant context pack/evidence source first.

## DEC-003 — Runtime coordinates are user-captured in this test lab

**Date / Version:** v1.1.1  
**Status:** ACTIVE for current test UX  
**Decision:** do not infer/hardcode NPC X/Y; persist raw MapID/X/Y captured by user.  
**Context:** inferred coordinate scale was wrong.  
**Consequences:** Map 3 maps to test NPC 463; Map 5 maps to 339; unsupported maps fail closed.

## DEC-004 — Keep mutable UI actions on MainThread queue once reached

**Date / Version:** 2026-08-16 / v1.1.6, wording corrected v1.1.7  
**Status:** ACTIVE  
**Decision:** Once a decisive live UI mutation is genuinely reached, execute it as a legitimate managed `System.Action` through `FGStudio.Engine.Utilities.MainThread.Execute(Action)` rather than directly/re-entrantly mutating UI from the WH_GETMESSAGE request.  
**Context:** canonical MainThread contract establishes the game-owned action boundary. v1.1.7 evidence correction means this mechanism is no longer asserted as the primary BUG-001 root cause; old runs may have failed earlier in observer/discovery.  
**Alternatives:** direct hook mutation; arbitrary fixed delay.  
**Why Rejected:** weaker execution/lifecycle guarantees.  
**Evidence:** canonical MainThread docs / EVID-003.  
**Consequences:** v1.1.7 retains the v1.1.6 queued action layer while changing only observer/discovery.

## DEC-005 — MainThread proof is asynchronous

**Date / Version:** 2026-08-16 / v1.1.6  
**Status:** ACTIVE  
**Decision:** Begin/enqueue proof, return from hook, poll on later request. Never enqueue then synchronously wait within the same hook callback.  
**Evidence:** canonical `MAINTHREAD_BRIDGE_V1`.  
**Consequences:** first Treatment action may report proof pending; later observation/poll advances it.

## DEC-006 — Prove current GameDialog observation before redesigning Treatment action again

**Date / Version:** 2026-08-16 / v1.1.7  
**Status:** ACTIVE  
**Decision:** If current Lua `GameDialog` is already present, do not reopen the NPC merely because the exact Treatment button was not resolved in that tick. First prove the dynamic UI hierarchy/text observation and log the current candidates. Do not change packet/callback semantics again until observer reachability is proven.  
**Context:** original runtime log showed repeated `ClickNPC` while `CHỜ DIALOG TRỊ LIỆU`; old `WalkForButton` required matching text and click handler on the same object. That common observer dependency was shared by multiple historical action experiments.  
**Alternatives:** add delays; rotate more NPCs; rewrite packet/action again; broad reverse UI internals.  
**Why Rejected:** they change the wrong variable before proving that the action stage is reached and repeat already low-information experiments.  
**Evidence:** EVID-004, EVID-005, EVID-006 + canonical GameDialog lifecycle docs.  
**Reason:** isolate the earliest unproven stage in the chain.  
**Consequences:** v1.1.7 adds descendant-label button discovery, semantic text normalization and `GameDialog`-presence anti-reopen guard. A future action redesign requires runtime logs showing `DIALOG_V117 MATCH`/action reachability or another exact observer result.  
**Affected Features:** Auto Heal; principle may apply to other server-driven dynamic GameDialog automation.

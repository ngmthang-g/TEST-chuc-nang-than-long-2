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

## DEC-004 — Do not accept direct hook mutation as authoritative UI action

**Date / Version:** 2026-08-16 / v1.1.6  
**Status:** ACTIVE  
**Decision:** For the broken heal UI mutation, do not directly invoke the decisive UIButton/GameDialog mutation re-entrantly from the WH_GETMESSAGE request. Use a legitimate `System.Action` queued through `FGStudio.Engine.Utilities.MainThread.Execute(Action)` and let Unity Update invoke it.  
**Context:** v1.1.0–v1.1.5 produced the same flicker/no-progress across several action styles and two NPC/maps. Canonical MainThread contract explicitly separates producer hook from normal Update execution boundary.  
**Alternatives:** add more delay; resend packet; test more NPCs; call `HandleClickEvent` directly again.  
**Why Rejected:** those repeat failed layers or do not address the common execution boundary.  
**Evidence:** EVID-001, EVID-002, EVID-003.  
**Reason:** isolate one shared architectural variable using an already-VERIFIED game-owned dispatcher.  
**Consequences:** v1.1.6 first runs a harmless CTS Action proof; Treatment UI event is blocked unless proof succeeds.  
**Affected Features:** Auto Heal now; potential future mutation bridge if runtime-verified.

## DEC-005 — MainThread proof is asynchronous

**Date / Version:** 2026-08-16 / v1.1.6  
**Status:** ACTIVE  
**Decision:** Begin/enqueue proof, return from hook, poll on later request. Never enqueue then synchronously wait within the same hook callback.  
**Evidence:** canonical `MAINTHREAD_BRIDGE_V1`.  
**Consequences:** first Treatment attempt may log `MAINTHREAD_PROOF PENDING`; a later timer tick polls/advances.

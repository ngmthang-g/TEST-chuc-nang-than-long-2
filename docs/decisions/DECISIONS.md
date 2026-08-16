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
**Context:** canonical MainThread contract establishes the game-owned action boundary.  
**Evidence:** EVID-003.  
**Consequences:** v1.1.8 retains the dispatcher/proof layer and changes observer/semantic gating around it.

## DEC-005 — MainThread proof is asynchronous

**Date / Version:** 2026-08-16 / v1.1.6  
**Status:** ACTIVE  
**Decision:** Begin/enqueue proof, return from hook, poll on later request. Never enqueue then synchronously wait within the same hook callback.  
**Evidence:** canonical `MAINTHREAD_BRIDGE_V1`.  
**Consequences:** first Treatment action may report proof pending; later observation/poll advances it.

## DEC-006 — Prove current GameDialog observation before redesigning Treatment action again

**Date / Version:** 2026-08-16 / v1.1.7, strengthened v1.1.8  
**Status:** ACTIVE  
**Decision:** A Treatment transaction must not reopen the NPC from `WaitTreatment` because current GameDialog discovery is temporarily absent/unresolved. Observe the server-driven dialog until timeout and fail closed.  
**Context:** repeated `ClickNPC` was visible in old runtime logs, and v1.1.7 still retained a transient-absence retry path. Canonical GameDialog lifecycle destroys/recreates UI during server transitions.  
**Alternatives:** fixed delays, repeated NPC open, rotating NPCs.  
**Why Rejected:** each can create/recreate the very dialog being observed and destroys causal evidence.  
**Evidence:** EVID-004, EVID-005, EVID-008.  
**Consequences:** v1.1.8 removes the `WaitTreatment` reopen loop. Runtime must prove whether flicker stops.

## DEC-007 — Require live semantic selection identity before GameDialog mutation

**Date / Version:** 2026-08-16 / v1.1.8  
**Status:** ACTIVE  
**Decision:** For dynamic GameDialog choices, a visible-text match alone is insufficient. The freshly resolved current button must also yield a valid live `Tag`/`selectionID > 0` before the action is queued.  
**Context:** canonical client source defines `Selections[selectionID] = visibleText` and clones buttons with `Tag = selectionID`; IDs are server/runtime data and must not be guessed.  
**Alternatives:** hardcode Treatment ID; enqueue a label-matched button without proving its current semantic identity; immediately re-enable direct packet dispatch.  
**Why Rejected:** hardcoded IDs violate runtime state; label-only action is weaker evidence; changing packet and observer simultaneously would again mix variables.  
**Evidence:** canonical Auto Heal/GameDialog docs + EVID-008 source audit.  
**Consequences:** v1.1.8 emits live selectionID diagnostics and fail-closes when Tag cannot be resolved. Direct packet dispatch remains historical/not active until runtime evidence justifies a narrower next experiment.

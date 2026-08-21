# VERSION v0.6.1.6 — Independent clients + Mount/StartPath guard

## User-approved scope

Clean all ten remaining coordination assumptions inherited from physical mouse ownership, while preserving business/safety state machines. Add the existing AutoFight-OFF Travel Guard before both Mount and StartPath.

## Implemented

- Removed global hidden-action busy/owner/recording state.
- Trade sequence is now a business-only Trade Workflow Lock; unrelated PID actions do not consult it.
- Removed global SELL-phase barrier from trade coordinator.
- Recorder blocks only its configured PID/pair.
- P1→P2→P3 priority is local per account.
- ReadState Bridge polling fans out concurrently per PID; mutation remains controller-thread serialized for UI/profile safety.
- Central decision gate applies one shared `EnsureAutoFightOffForTravel` to both Mount and StartPath.
- Existing Stop #1 → Stop #2 → AUTO/Attack reset → repeat policy is reused, not duplicated.
- Client Freeze, F4, death lifecycle, route ownership, FIFO/re-lock and stable business verification remain.

## Verification before CI

- `tools/verify_v0616_logic.py`: PASS locally.
- Eight native pure C++ logic groups: PASS locally.
- Windows MSVC x64: pending GitHub Actions.
- Live client: untested.

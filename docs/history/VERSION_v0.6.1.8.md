# VERSION v0.6.1.8 — Route/M87/Dead-code maintenance

## User request
Duyệt audit items 2, 5, 7, 8; clean thật sạch và làm thật cẩn thận.

## Approved implementation
1. **Route completion:** preserve cross-map ownership across intermediate maps; release only at physical final destination (`atTarget && !autoPathing && !riding`). Wired to normal Hold and robust-travel arrival.
2. **Map 87:** proactive underworld AutoFight guard requires an active destination outside M87. Same-map M87 training is not stopped by this special guard; shared movement guard still protects Mount/StartPath.
3. **Dead code:** remove 5 uncalled helpers and 4 unread runtime fields identified by the audit.
4. **Trade no-op lock:** remove Acquire/ReleaseTradeWorkflowLock wrappers and callsites. `tradeTxn_ + tradeHeld` remains the only trade business serialization.

## Explicitly unchanged
- v0.6.1.7 `AUTO → 500 ms → DỪNG AUTO 2` hidden sequence.
- FIFO admission/order, trade pass proof, SELL logic, revive, rotation, Client Freeze, REC behavior.
- Bridge InputSync transport and client metadata/resolvers.
- Synchronous Bridge architecture.

## Validation
- Local static verifier: PASS.
- Native pure tests: route 15/15 PASS, rotation 8/8 PASS, trade/background/fixed-slot/unity/internal-UI/travel-guard PASS.
- Windows MSVC/GitHub Actions result: pending publish/CI at authoring time; append after run.

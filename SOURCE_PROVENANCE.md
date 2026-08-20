# SOURCE PROVENANCE — v0.6.2

- Direct base: attached `ThanLongItemConsolidator_Source_v0.5(2).zip`, SHA-256 `d84b0f57a7da17e549c85e793232b37b5f8a518fe58f494d97fea4fd7393126d`.
- Donor reference: attached `ThanLongAutoTrain_Source_v0.8.4 hoan thanh chu trinh ban do.zip`, SHA-256 `fdc93bb6653418705b058cd2c2a2b56a820351a2d9bd38c85ce062a51fabac86`.
- v0.6 implemented the learned semantic callback model inside the v0.5 Bridge; it did not copy donor `remote_worker.S` or `CreateRemoteThread` transport.
- v0.6.1 separated control discovery from optional Lua/Executor capabilities. Its checked-in package came from source commit `3cfa9347`, CI run `32388213200`, artifact `9413784891`; hashes remain in `release/SHA256SUMS_v0.6.1.txt`.
- v0.6.2 is a direct change on v0.6.1 driven by user runtime EVID-002. Confirm/Revive code paths were kept; AUTO lookup/fallback, F4 delivery and sell candidate/completion handling were changed narrowly.
- The exact v0.5 `ToggleGlobalPause` body was source-compared and retained; v0.6.2 only adds a shared-latch polling delivery path around it.
- Client semantic facts used for analysis: `TopIcon.AutoTrainClick/AutoStopClick`, `AutoFight_Main.StartAutoFight(Train/None)`, structured `GetItemsAtSite(Bag)`, item instance identity, `CMD_NPC_SHOP_SELL_REQUEST=200036`, and mutation-safe rescan. These facts guide the design; v0.6.2 does not claim packet-level sell implementation.
- GitHub Actions Windows x64 run 280 (`32394492512`) on source commit `17d5aa7049dcdc3daba7d3d5efd62455699b34b8`, plus `tools/verify_v062_logic.py` and all self-tests: BUILD PASS; artifact `9416119841`. Only new user live-client evidence may promote v0.6.2 runtime status.

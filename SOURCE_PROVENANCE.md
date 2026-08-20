# SOURCE PROVENANCE — v0.6.1.1

- Direct hotfix base: exact v0.6.1 release tree `fd55f16c6415e56469867ff55902c7e73756c4e2` / GitHub commit `1048c2064ec6be05756e25aa8a0adc7893df3f38`.

- Direct base: attached `ThanLongItemConsolidator_Source_v0.5(2).zip`, SHA-256 `d84b0f57a7da17e549c85e793232b37b5f8a518fe58f494d97fea4fd7393126d`.
- Base source passed its supplied `tools/verify_v05_logic.py`; supplied EXE/DLL matched `BUILD_HASHES_v0.5.txt`.
- Donor reference: attached `ThanLongAutoTrain_Source_v0.8.4 hoan thanh chu trinh ban do.zip`, SHA-256 `fdc93bb6653418705b058cd2c2a2b56a820351a2d9bd38c85ce062a51fabac86`.
- Donor code was analyzed for UI discovery, callback invocation, treatment, selling, MessageBox confirmation, revive and TopIcon AUTO actions.
- v0.6 did not copy donor `remote_worker.S` or `CreateRemoteThread` transport. It implemented the learned semantic-action model inside the existing v0.5 Bridge.
- v0.6.1 is a direct hotfix on v0.6 using user runtime evidence EVID-001. It does not import new donor code or alter v0.5 business-state logic.
- The donor established the Executor's native action surface/RVA, not the managed namespace assumed by v0.6. v0.6.1 therefore uses validated metadata discovery and records uncertainty explicitly.
- GitHub Actions Windows x64 plus `tools/verify_v061_logic.py` and all self-tests are the build acceptance gate. Only user live-client evidence may promote runtime status.
- The checked-in v0.6.1 ZIP is the unmodified artifact from source commit `3cfa9347`, CI run `32388213200`, artifact `9413784891`; its GitHub digest and extracted binary hashes were independently rechecked before packaging.
- v0.6.1.1 does not import v0.6.2 changes. It changes only the v0.6.1 item-cell stage, protocol/version/test/document surfaces required to ship the matching EXE/DLL pair.
- The knowledge repository `ngmthang-g/clinent-game-than-long-DATA-2222` was consulted for Quick Sell and bag-grid semantics. It confirms Quick Sell redirects each bag-cell click to the normal one-item sell request and that inventory UI refreshes after removal. Its recommended instance-ID packet seller was deliberately not introduced in this scoped hotfix.

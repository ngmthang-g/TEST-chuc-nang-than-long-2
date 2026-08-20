# SOURCE PROVENANCE — v0.6.1

- Direct base: attached `ThanLongItemConsolidator_Source_v0.5(2).zip`, SHA-256 `d84b0f57a7da17e549c85e793232b37b5f8a518fe58f494d97fea4fd7393126d`.
- Base source passed its supplied `tools/verify_v05_logic.py`; supplied EXE/DLL matched `BUILD_HASHES_v0.5.txt`.
- Donor reference: attached `ThanLongAutoTrain_Source_v0.8.4 hoan thanh chu trinh ban do.zip`, SHA-256 `fdc93bb6653418705b058cd2c2a2b56a820351a2d9bd38c85ce062a51fabac86`.
- Donor code was analyzed for UI discovery, callback invocation, treatment, selling, MessageBox confirmation, revive and TopIcon AUTO actions.
- v0.6 did not copy donor `remote_worker.S` or `CreateRemoteThread` transport. It implemented the learned semantic-action model inside the existing v0.5 Bridge.
- v0.6.1 is a direct hotfix on v0.6 using user runtime evidence EVID-001. It does not import new donor code or alter v0.5 business-state logic.
- The donor established the Executor's native action surface/RVA, not the managed namespace assumed by v0.6. v0.6.1 therefore uses validated metadata discovery and records uncertainty explicitly.
- GitHub Actions Windows x64 plus `tools/verify_v061_logic.py` and all self-tests are the build acceptance gate. Only user live-client evidence may promote runtime status.

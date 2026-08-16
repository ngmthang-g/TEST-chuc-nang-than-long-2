# AI START HERE — mandatory before every version

This repository is a runtime test lab. Before any analysis, code change, build, or new version, AI must read these files in this order:

1. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
2. `AI_CLIENT_ANALYSIS_RULES.txt`
3. `AI_PROJECT_HANDOFF_FULL.md`
4. `PROJECT_KNOWLEDGE.md`
5. `CHANGELOG.md`
6. affected feature / bug / decision / evidence docs under `docs/`
7. current source/tests for the affected subsystem

If you received only a build artifact ZIP, read `AI_PROJECT_HANDOFF_FULL.md` first, then the mandatory protocol/rules/current-knowledge files packaged beside it.

When client knowledge is needed, use canonical repository:

`ngmthang-g/clinent-game-than-long-DATA-2222`

Mandatory client-read route:

`AI_INDEX.md -> AI_BOOTSTRAP.md -> AUTO_TOOL_SCOPE.md -> AI_ROUTER.md -> exactly one matching contexts/BUILD_*.md -> only REQUIRED docs -> exact VERIFIED/database lookup`

Rules:

- Do not broad reverse-engineer the client again.
- Only inspect binary/native implementation when the exact fact required by the current task is missing from VERIFIED/database/canonical docs.
- Runtime evidence outranks assumptions.
- BUILD/CI PASS does not equal RUNTIME PASS.
- Preserve failed attempts and user runtime reports.
- Every meaningful version must update project knowledge before handoff.
- Do not silently replace or remove the two mandatory protocol/rules files.
- Every delivered CI artifact must be self-describing and contain at minimum: `AI_PROJECT_HANDOFF_FULL.md`, this file, the V2 protocol, client-analysis TXT, `PROJECT_KNOWLEDGE.md`, `CHANGELOG.md`, and generated `BUILD_EVIDENCE.txt` beside EXE/DLL.

Current feature route for Auto trị liệu:

`contexts/BUILD_AUTO_HEAL.md`

If the failure is in the mutation execution boundary/MainThread bridge, additionally read:

`contexts/BUILD_MAINTHREAD_BRIDGE.md`

Current continuation point after v1.1.10 runtime:

`GameDialog open -> LuaEnv resolved -> DoString overload resolver -> current Selections -> live Treatment ID -> one semantic action -> fresh result proof`.

This file is the startup pointer only; detailed history belongs in `AI_PROJECT_HANDOFF_FULL.md`, `PROJECT_KNOWLEDGE.md` and `docs/`.

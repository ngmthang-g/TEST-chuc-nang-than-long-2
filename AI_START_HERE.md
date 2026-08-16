# AI START HERE — mandatory before every version

This repository is a runtime test lab. Before any analysis, code change, build, or new version, AI must read these files in this order:

1. `AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md`
2. `AI_CLIENT_ANALYSIS_RULES.txt`
3. `PROJECT_KNOWLEDGE.md`
4. `CHANGELOG.md`
5. affected feature / bug / decision / evidence docs under `docs/`
6. current source/tests for the affected subsystem

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

Current feature route for Auto trị liệu:

`contexts/BUILD_AUTO_HEAL.md`

If the failure is in the mutation execution boundary/MainThread bridge, additionally read:

`contexts/BUILD_MAINTHREAD_BRIDGE.md`

This file is the startup pointer only; detailed history belongs in `PROJECT_KNOWLEDGE.md` and `docs/`.

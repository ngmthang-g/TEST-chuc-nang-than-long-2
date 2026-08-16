# EVIDENCE REGISTRY

## EVID-001 — Complete Treatment flow remains unproven
**Type:** USER_RUNTIME  
Route/NPC open has runtime evidence; no complete Auto Heal known-good version exists.

## EVID-002 — MainThread dispatcher contract
**Type:** CANONICAL CLIENT SOURCE  
`MainThread.Execute(System.Action)` queues managed work drained by Unity Update.

## EVID-003 — GameDialog is server-driven
**Type:** CANONICAL CLIENT SOURCE  
Inbound dialog handling may destroy/recreate UI; visual flicker is not completion proof.

## EVID-004 — UIRoot representation failed
**Type:** USER_RUNTIME / v1.1.8  
GameDialog existed while bridge-visible UIRoot observer repeatedly saw no relevant text/buttons. UIRoot is abandoned for this feature.

## EVID-005 — Canonical Selections contract
**Type:** CANONICAL CLIENT SOURCE/KB  
`GameDialogData.Selections[selectionID]=visibleText`; `CMD_SHOW_GAMEDIALOG=100007` submits current `selectionID:SelectedItemID`.

## EVID-006 — v1.1.9 manager singleton failure
**Type:** USER_RUNTIME + SOURCE  
Failed before returned script object/DoString/Selections.

## EVID-007 — v1.1.11 MoonSharp identity/signature
**Type:** USER_RUNTIME + RUNTIME METADATA  
Runtime reports `MoonSharp.Interpreter.Script` and exact `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`. V121 source rejected the correct method before invocation.

## EVID-008 — Official MoonSharp API cross-check
**Type:** EXTERNAL PRIMARY SOURCE / TARGETED RESEARCH  
Official API matches live String/Table/String -> DynValue shape and DynValue.String getter. Live client runtime remains authoritative.

## EVID-009 — v1.1.12 build lineage
**Type:** CI / BUILD  
Initial run `31937607280` failed before compiler due audit false-positive. Corrected source run `31937703988` and final run `31938051044` passed. Final v1.1.12 HEAD `03b33595e122217baa4c006a0fe8998af3395d44`.

## EVID-010 — v1.1.12 runtime passes MoonSharp execution/result
**Type:** USER_RUNTIME + EXACT LOG + SOURCE CORRELATION  
After one NPC open:
```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```
Supports current Script resolution PASS, exact DoString invocation PASS, DynValue.String extraction PASS, GameDialog/AutoFight_Main table presence. Does not prove Treatment absent or server rejection.

## EVID-011 — V122 N correction
**Type:** SOURCE CORRECTION  
Probe increments `nodes` once per traversed table and returns `N=nodes`. Thus `N=4` means four table nodes, not four selections.

## EVID-012 — V122 semantic access is raw-only
**Type:** SOURCE  
Probe calls `rawget(t,"Selections")` and rawget priority fields. Lua rawget bypasses `__index`/metatable lookup. This is a concrete observer limitation consistent with V122 runtime, but does not prove the live data definitely resides behind `__index`.

## EVID-013 — Canonical Selections contract still applies
**Type:** CANONICAL CLIENT KB  
Canonical docs continue to verify server-provided `GameDialogData.Selections` and built-in semantic selection handling. No contrary runtime evidence establishes a changed contract.

## EVID-014 — v1.1.13 source-bearing build passes
**Type:** CI / BUILD ARTIFACT + ARTIFACT INSPECTION  
Source commit `79235f61ded9d393694be807c996128b082f67b4`. Run `31939000139`: architecture audit PASS, Route FSM PASS, Heal FSM PASS, bridge DLL/PE PASS, controller EXE PASS, knowledge packaging PASS, artifact upload PASS.  
Artifact `ThanLongTestAutoHeal-v1.1.13`, ID `9261527342`, ZIP SHA-256 `1cb886860b998b955efe68164bf5379642a2c2bde739acb425c7a9a650ecee5f`.  
EXE SHA-256 `764fc10e98afd0fcd608bd9dd50c9fa86e51b467524cab4aebbe5001646d037a`.  
DLL SHA-256 `8d8b9e3feeecf0be509610ad990bbe3852761fb750d1862aee09d5f51eadb7f7`.  
Exactly 9 required members verified. `BUILD_EVIDENCE` records source head `79235f61...`, PR checkout `f4f3bded...`, run `31939000139`, BUILD PASS, RUNTIME UNTESTED.

## EVID-015 — v1.1.13 experiment scope
**Type:** SOURCE DESIGN  
V123 keeps runtime-proven MoonSharp execution and changes only read-only representation access: normal indexing under `pcall`, bounded child/metatable/table-`__index` traversal, and diagnostics `NODE/ST/SV/MT/KS/S`. Mutation remains fail-closed until a positive current ID is re-read.

## Current boundary
`MoonSharp execution PASS -> current Lua representation/access -> Selections -> live Treatment ID -> semantic action -> follow-up/result`.

# VERSION v1.1.12-test — exact MoonSharp DoString + DynValue result

## A. Identity / Lineage
- Version: v1.1.12-test
- Date: 2026-08-16
- Based On: v1.1.11-test
- Reason Created: v1.1.11 runtime identified `MoonSharp.Interpreter.Script` and exact `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`, but rejected the real method before invocation.
- Last Known-Good full Auto Heal: NONE.
- Related BUG: BUG-001.
- Related Decision: DEC-014.

## B. State Before Modification
Protected runtime path before v1.1.12:
- runtime MapID/X/Y capture;
- route/mount/AutoPath/dismount;
- `ClickNPC(339)` opens intended GameDialog;
- no WaitTreatment controller reopen in tested v1.1.8 transaction;
- V120 reaches current returned Script object;
- V121 identifies MoonSharp and real DoString metadata.

Not reached before v1.1.12: actual DoString invocation, returned probe string, current Selections/Treatment ID, current-ID action, follow-up/result.

## C. Confirmed v1.1.11 root cause
V121 accepted-shape logic required parameter index 1 to be `System.String`. Live method has `MoonSharp.Interpreter.Table` at index 1. Correct method was enumerated but rejected before `runtime_invoke`.

## D. v1.1.12 changes
### `src/bridge_lua_moonsharp_v1_1_12.inc`
- retains V120 returned-object resolver;
- requires exact current metadata `DoString(String,Table,String)->DynValue`;
- invokes `DoString(code,null,"ThanLongAutoHealV122")`;
- captures managed exception diagnostics;
- resolves `DynValue.get_String()` -> System.String;
- runs existing bounded GameDialog/AutoFight_Main probe;
- markers `LUA_MOONSHARP_V122`, `LUA_DIALOG_V122`.

### `src/bridge_action_v1_1_12.inc`
- re-reads V122 current semantic IDs immediately before action;
- refuses guessed/cached/absent ID;
- submits `CMD_SHOW_GAMEDIALOG=100007` with `<actualCurrentID>:-1` only after current match;
- marker `ACTION_V122`;
- MessageBox callback behavior preserved.

## E. Build / CI History
### Initial source build
- Commit `ad2f0403863251330b1aca80eb1eba9681b58c9a`
- Run `31937607280`
- **CI FAILED before compilation** because architecture audit incorrectly required a class-name literal in resolver source.
- No artifact.

### Corrected source-bearing build
- Commit `33a3b56b44724d76ec6983bf7ec4dcff1edfa2b1`
- Run `31937703988`: **CI/BUILD PASS**
- Artifact `ThanLongTestAutoHeal-v1.1.12`, ID `9261162703`
- ZIP SHA-256 `d25f999934be62152cf02fd0251f533743f3e9eaa0d4178f74c2fa1007d85c19`
- EXE SHA-256 `32a3977469a26f5807be192a620630fba9ea3242e265706522fd7d93bb27c823`
- DLL SHA-256 `553c1d245c06c614561b9c2c1e6368daf30cb671b46602aec60b1938f326a037`
- Exactly 9 required artifact members.

### Final HEAD
- `03b33595e122217baa4c006a0fe8998af3395d44`
- Run `31938051044`: **CI/BUILD PASS**
- Artifact ID `9261253049`
- Final ZIP SHA-256 `8b05c3c9b4ced7d71f0dba7234920721b3e7acb1170af1ad56979d3a281c72f3`
- Final EXE/DLL were byte-identical to source-bearing hashes.

## F. Runtime Result — UPDATED AFTER DELIVERY
User tested delivered final-head v1.1.12 on 2026-08-16. After one `ClickNPC npcID=339`, repeated:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

### Runtime PASS established
- current Script resolution;
- exact MoonSharp DoString invocation;
- returned DynValue.String extraction;
- GameDialog table presence;
- AutoFight_Main table presence.

### Runtime unresolved/fail boundary
V122 observer did not expose T/C/K or selection samples. Current live Treatment ID/action/server follow-up were not reached.

## G. Critical diagnostic correction
V122 `N` is the variable `nodes`, incremented once per traversed table. `N=4` therefore means four table nodes were scanned. It is **not** a count of GameDialog selections.

## H. Newly identified observer limitation
The reused V119/V122 probe searches semantic fields with `rawget`, including `rawget(t,"Selections")`. `rawget` bypasses normal `__index`/metatable lookup. This becomes the earliest concrete observer weakness after MoonSharp execution itself passed.

Canonical client contract still verifies `GameDialogData.Selections[selectionID]=visibleText`. A representation/access mismatch is LIKELY but requires V123 runtime proof.

## I. Known-Good Established
NONE for complete Auto Heal. v1.1.12 establishes a new protected partial boundary: MoonSharp execution/result PASS.

## J. Superseded by v1.1.13
v1.1.13 keeps V122 MoonSharp execution unchanged and replaces only the read-only GameDialog table observer with normal indexing + bounded metatable diagnostics.

Next evidence is `LUA_DIALOG_V123`, especially `T`, `ST`, `SV`, `MT`, `KS`, `S`. No packet/server conclusion is valid before a current live ID is actually sent.

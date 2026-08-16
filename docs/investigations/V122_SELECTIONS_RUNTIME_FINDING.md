# Investigation — v1.1.12 runtime: MoonSharp PASS, Selections observer boundary

## Scope
This note records the exact runtime boundary exposed by delivered v1.1.12 and prevents future versions from re-investigating already-proven MoonSharp execution.

## Exact runtime evidence
After semantic `ClickNPC npcID=339`, the user repeatedly captured:

```text
LUA_DIALOG_V122 • route=static LuaSystemManager.get_LuaEnv -> MoonSharp.Script.DoString(String,Table,String) • GD=present • MB=absent • T=0 • C=0 • K=0 • raw={T=0;C=0;K=0;GD=table;AF=table;N=4;WT=;WC=;WK=;S=}
```

## What this proves
The C++ bridge only emits `LUA_DIALOG_V122` after:
1. `ResolveLuaEnvV120()` returns the current script object;
2. V122 resolves exact live `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`;
3. runtime invocation completes without managed exception;
4. a non-null DynValue returns;
5. `DynValue.get_String()` resolves and returns a managed System.String;
6. C++ copies/parses that returned string.

Therefore this transaction is runtime PASS through MoonSharp invocation and string-result extraction.

The returned Lua string also establishes:
- `GUI.FindUI("GameDialog")` produced a table;
- `GUI.FindUI("AutoFight_Main")` produced a table;
- current MessageBox was absent;
- V122 did not locate Treatment/Confirm/Know through its observer.

## Critical correction — N is not selections
V122 reuses `kGameDialogProbeLuaV119`. Its variable:

```lua
local nodes = 0
...
nodes = nodes + 1
...
N=..tostring(nodes)
```

counts traversed table nodes. Thus `N=4` means four tables were traversed. It cannot be used as evidence that the dialog had four options.

## Exact observer limitation
The old probe reads:

```lua
scanSelections(rawget(t, "Selections"), ...)
...
local v = rawget(t, key)
```

`rawget` deliberately bypasses a table's `__index` metamethod. That is useful when avoiding side effects, but it can miss values exposed by a class/object table through normal indexing.

Runtime now proves `gd`/`af` are tables and the probe executes, so this raw-only access becomes the earliest concrete observer weakness.

## Canonical client contract remains unchanged
Client knowledge verifies:
- `GameDialogData.Selections[selectionID]=visibleText`;
- built-in AutoFight/FuBen code inspects current server-provided Selections;
- selection submit uses `CMD_SHOW_GAMEDIALOG=100007` and the current selection ID.

Nothing in V122 runtime proves the server omitted Treatment or changed the GameDialog data contract.

## v1.1.13 targeted experiment
Do not modify MoonSharp execution. Replace only the read-only observer with:
- normal indexing under `pcall`;
- bounded semantic-key lookup;
- bounded child traversal;
- bounded metatable/table-`__index` traversal;
- diagnostics `NODE`, `ST`, `SV`, `MT`, `KS`, `S`.

Interpretation:
- `ST>0` proves a table named/exposed as Selections was actually reached;
- `SV>0` proves values inside that table were enumerated;
- `S` shows bounded current value samples;
- `MT`/`KS` identify representation clues if no Selections table is reached.

## Root-cause confidence
`rawget` / representation mismatch is a **LIKELY** explanation, not yet CONFIRMED. V123 runtime decides it.

## Do not infer
This evidence does NOT justify:
- four selections from `N=4`;
- a missing Treatment service;
- a bad packet ID/payload;
- server rejection;
- a need to return to UIButton/UIRoot;
- a need to redesign MoonSharp invocation.

# V121 MOONSHARP RUNTIME FINDING

## Scope
Targeted runtime investigation for BUG-001 after v1.1.11. Do not use this document to reopen solved route/NPC/UIRoot questions.

## Exact user runtime evidence
Delivered v1.1.11 repeatedly reports:

```text
LUA_DIALOG_V121 PROBE FAIL • LUA_DOSTRING_V121 unresolved • actual=MoonSharp.Interpreter.Script • declared=MoonSharp.Interpreter.Script • DoString={DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue | ...}
```

The same boundary reproduced after stopping and restarting Auto Heal.

## What the runtime proves
1. `LuaSystemManager.get_LuaEnv` path returns an object whose actual runtime class is `MoonSharp.Interpreter.Script`.
2. The declared return class visible to current metadata is also `MoonSharp.Interpreter.Script`.
3. Current IL2CPP method enumeration exposes:
   `DoString(System.String,MoonSharp.Interpreter.Table,System.String)->MoonSharp.Interpreter.DynValue`.
4. Therefore the earlier generic/xLua identity assumption is not current truth at this boundary.

## Source correlation — why v1.1.11 still said unresolved
`ScoreDoStringV121` accepts a first parameter String/Byte[] but, for any method with at least two parameters, requires parameter index 1 to equal `System.String`.

The live MoonSharp method has:
- index 0 = `System.String` code;
- index 1 = `MoonSharp.Interpreter.Table` global context;
- index 2 = `System.String` friendly name.

So the method was found and logged, then rejected by the V121 scorer. The failure occurs before `runtime_invoke`.

Root cause status: **CONFIRMED**.

## What remains unproven
The V121 runtime does NOT prove:
- the bounded Lua/MoonSharp probe executes;
- `GUI.FindUI`/table access works from a direct MoonSharp chunk;
- current Treatment text/selectionID is found;
- `CMD_SHOW_GAMEDIALOG` is sent by the current path;
- server accepts Treatment;
- MessageBox/follow-up/HP/money completion behavior.

All those stages remain UNKNOWN until reached.

## Targeted primary-source cross-check
Canonical client KB had no MoonSharp-specific DoString contract, so exact external source lookup was justified under the project's no-broad-reverse rule.

Official MoonSharp source defines:

`Script.DoString(string code, Table globalContext = null, string codeFriendlyName = null) -> DynValue`

The method loads the chunk and calls it.

Official `DynValue` exposes:

`public string String { get { return m_Object as string; } }`

This primary-source contract independently matches the live client metadata and supplies the correct argument order/result access for v1.1.12.

## v1.1.12 implementation consequence
Use:

```text
current Script object
 -> exact DoString(String, Table, String)
 -> args: code, null, friendlyName
 -> DynValue
 -> get_String()
 -> probe text
 -> parse T/C/K
```

Do not use the old xLua argument order `{code, chunkName, env}`.
Do not interpret the returned object as `object[]`.

## Do-not-do
- no UIRoot tuning;
- no hardcoded Treatment ID;
- no guessed MoonSharp RVA;
- no raw native invocation that ignores current metadata;
- no packet/server diagnosis until a current live selection is actually sent;
- do not erase the xLua investigation history, but mark it superseded/disproven for current engine identity.

## Next runtime discriminator
- `LUA_MOONSHARP_V122 ...`: exact method/invocation/result problem remains;
- `LUA_DIALOG_V122 ... T=0`: invocation/result passed; current runtime table/text path is next;
- `LUA_DIALOG_V122 ... T>0`: current Treatment ID observed; action stage becomes next;
- `ACTION_V122 SENT`: only then inspect real server/UI/result follow-up.

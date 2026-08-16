# INVESTIGATION — v1.1.10 runtime DoString failure

Date: 2026-08-16  
Related: BUG-001, EVID-016, EVID-017, DEC-012  
Versions: v1.1.10 -> v1.1.11

## 1. Runtime evidence
User ran the delivered v1.1.10 against Map 5 / healer NPC 339 and reported:

```text
Đã gửi AutoPath tới map=5 x=9454 y=5477
Đã gửi lệnh xuống ngựa
Đã gọi ClickNPC npcID=339
LUA_DIALOG_V120 PROBE FAIL • LUA_DIALOG_V120: DoString unresolved • LUA_DIALOG_V119: không resolve LuaEnv.DoString(string,...)
```

The final probe failure repeats.

## 2. What the log proves from current source
`RunLuaChunkV120()` performs this order:
1. `ResolveLuaEnvV120(luaEnv,...)`;
2. `object_get_class(luaEnv)`;
3. `FindLuaDoStringV119(envClass,...)`;
4. only after that would it allocate/invoke the Lua chunk.

Therefore the shown message cannot be reached if the V120 LuaEnv resolver itself returned false/null.

Classification:
- manager/LuaEnv resolution: **RUNTIME PARTIAL PASS**;
- old DoString resolver: **RUNTIME FAIL**;
- Lua code execution: NOT REACHED;
- `Selections`: NOT REACHED;
- live Treatment ID: NOT REACHED;
- `ACTION_V120`: NOT REACHED;
- packet/server follow-up: UNKNOWN.

## 3. Old resolver limitation
`FindLuaDoStringV119` was written before this runtime proof. It primarily required:
- non-static `DoString`;
- exact String-first parameter expectation;
- old class-method iterator discovery;
- accepted arity 3 String/String or fallback arity 1 String.

It did not expose the actual LuaEnv class or all discovered signatures in its failure log and did not support a Byte[] chunk overload.

## 4. Canonical client knowledge check
Canonical Thần Long KB verifies:
- `FGStudio.LuaSystem.LuaSystemManager`;
- `get_LuaEnv/set_LuaEnv`;
- Lua is a first-class gameplay/UI layer;
- GameDialog current semantic source is `Selections[selectionID]=visibleText`.

It did not already provide exact client `LuaEnv.DoString` overload metadata. Therefore a targeted next-layer investigation is justified; broad client reverse is not.

## 5. Targeted upstream primary-source comparison
Official Tencent xLua `LuaEnv.cs` exposes two relevant overloads:
- `object[] DoString(byte[] chunk, string chunkName = "chunk", LuaTable env = null)`;
- `object[] DoString(string chunk, string chunkName = "chunk", LuaTable env = null)`.

The String overload UTF-8 encodes and delegates to the Byte[] overload.

Important limitation:
This is **not proof that the game ships the same xLua version or that both methods survived IL2CPP stripping**. It is only enough to reject the old assumption that a robust resolver should model one String-first shape and nothing else.

## 6. v1.1.11 diagnostic/implementation response
The next resolver must let live metadata decide:
- actual object class;
- declared get_LuaEnv return class;
- current DoString overloads;
- safe first parameter String or Byte[];
- safe arity/remaining parameters.

It also uses direct lookup by arity in addition to enumeration because the live failure could be method-iterator visibility rather than absence.

If no supported method exists, the next runtime log must include:
`LUA_DOSTRING_V121 unresolved • actual=... • declared=... • DoString={...} • near={...}`.

That result will distinguish:
- stripped/no DoString;
- different class/wrapper;
- different parameter types;
- enumeration visibility issue;
without changing route/NPC/UI representation or guessing native addresses.

## 7. Do-not-conclude-yet
Do NOT conclude from v1.1.10 that:
- Lua runtime is inaccessible;
- GameDialog Selections are absent;
- Treatment ID is wrong;
- packet 100007 failed;
- server refuses healing.

None of those stages were reached by this run.

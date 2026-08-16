@echo off
setlocal
cd /d "%~dp0"
where zig >nul 2>nul
if errorlevel 1 (echo KHONG TIM THAY ZIG & exit /b 1)
if not exist dist mkdir dist
del /q dist\* >nul 2>nul

echo [1/9] Architecture audit...
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop'; foreach($p in @('AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md','AI_CLIENT_ANALYSIS_RULES.txt','AI_START_HERE.md','AI_PROJECT_HANDOFF_FULL.md','PROJECT_KNOWLEDGE.md','CHANGELOG.md')){if(-not(Test-Path $p)){throw ('Missing mandatory knowledge/handoff file: '+$p)}}; $files=(Get-ChildItem 'src' -File | Where-Object {$_.Extension -in '.cpp','.h','.inc'} | ForEach-Object {$_.FullName}); $s=($files|%%{Get-Content $_ -Raw}) -join [Environment]::NewLine;" ^
  "$forbidden=@('CreateRemoteThread','WriteProcessMemory','remote_worker','RemoteExecutor'); foreach($x in $forbidden){if($s -match [regex]::Escape($x)){throw ('Forbidden legacy token: '+$x)}};" ^
  "$moon=Get-Content 'src/bridge_lua_moonsharp_v1_1_12.inc' -Raw; foreach($x in @('ResolveMoonSharpDoStringV122','MoonSharp.Interpreter.Table','MoonSharp.Interpreter.DynValue','get_String','RunLuaChunkV122','ReadLuaDialogIdsV122','LUA_MOONSHARP_V122','LUA_DIALOG_V122')){if($moon -notmatch [regex]::Escape($x)){throw ('v1.1.12 MoonSharp resolver missing '+$x)}};" ^
  "$action=Get-Content 'src/bridge_action_v1_1_12.inc' -Raw; foreach($x in @('SendCurrentLuaDialogSelectionV122','ReadLuaDialogIdsV122','ACTION_V122','current Lua GameDialogData.Selections')){if($action -notmatch [regex]::Escape($x)){throw ('v1.1.12 semantic action missing '+$x)}};" ^
  "$manager=Get-Content 'src/bridge_lua_manager_v1_1_10.inc' -Raw; foreach($x in @('ResolveLuaEnvV120','ScanStaticReferencesV120','LUA_MANAGER_V120')){if($manager -notmatch [regex]::Escape($x)){throw ('runtime-proven v1.1.10 LuaEnv resolver missing '+$x)}};" ^
  "$legacy=Get-Content 'src/bridge_lua_dostring_v1_1_11.inc' -Raw; foreach($x in @('LUA_DOSTRING_V121','DescribeMethodV121','TypeNameV121')){if($legacy -notmatch [regex]::Escape($x)){throw ('v1.1.11 lineage/metadata helper missing '+$x)}};" ^
  "$mt=Get-Content 'src/bridge_mainthread_v1_1_6.inc' -Raw; foreach($x in @('CancellationTokenSource','MainThread','Execute','System.Action','MAINTHREAD_PROOF PASS')){if($mt -notmatch [regex]::Escape($x)){throw ('MainThread proof helper missing '+$x)}};" ^
  "$entry=Get-Content 'src/bridge.cpp' -Raw; foreach($x in @('bridge_lua_moonsharp_v1_1_12.inc','bridge_action_v1_1_12.inc','InspectHealDialogLegacyV121','ClickHealDialogChoiceLegacyV121')){if($entry -notmatch [regex]::Escape($x)){throw ('v1.1.12 bridge wiring missing '+$x)}};" ^
  "$heal=Get-Content 'src/controller_part04.inc' -Raw; foreach($x in @('mapID == 3','return 463','mapID == 5','return 339','HealNpcIdForCapturedMap')){if($heal -notmatch [regex]::Escape($x)){throw ('v1.1.12 controller guard missing '+$x)}}; if($heal -match 'healNpcRetries_\s*<'){throw 'WaitTreatment ClickNPC retry loop still active'}; if($heal -match 'THỬ MỞ LẠI NPC'){throw 'WaitTreatment reopen string still active'};" ^
  "$protocol=Get-Content 'src/protocol.h' -Raw; if($protocol -notmatch '0x00010112u'){throw 'Protocol not bumped to v1.1.12'};" ^
  "$handoff=Get-Content 'AI_PROJECT_HANDOFF_FULL.md' -Raw; foreach($x in @('v1.1.12','v1.1.11','MoonSharp.Interpreter.Script','DoString(System.String,MoonSharp.Interpreter.Table,System.String)','DynValue','artifact')){if($handoff -notmatch [regex]::Escape($x)){throw ('handoff missing '+$x)}};" ^
  "Write-Host 'ARCHITECTURE AUDIT PASS: v1.1.12 uses runtime-proven MoonSharp Script.DoString(String,Table,String) and preserves the self-contained AI handoff.'"
if errorlevel 1 exit /b 1

echo [2/9] Route FSM self-test...
zig c++ -target x86_64-windows-gnu -O2 -std=c++17 -Wall -Wextra -Werror src\route_logic_test.cpp -o dist\RouteLogicTest.exe
if errorlevel 1 exit /b 1
dist\RouteLogicTest.exe
if errorlevel 1 exit /b 1

echo [3/9] Heal FSM self-test...
zig c++ -target x86_64-windows-gnu -O2 -std=c++17 -Wall -Wextra -Werror src\heal_logic_test.cpp -o dist\HealLogicTest.exe
if errorlevel 1 exit /b 1
dist\HealLogicTest.exe
if errorlevel 1 exit /b 1

echo [4/9] Build bridge DLL...
zig c++ -target x86_64-windows-gnu -O2 -std=c++17 -Wall -Wextra -Werror -shared -s ^
  src\bridge.cpp -luser32 -lkernel32 -o dist\ThanLongCleanRouteBridge.dll
if errorlevel 1 exit /b 1

echo [5/9] Verify bridge PE DLL...
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop'; $p='dist\ThanLongCleanRouteBridge.dll'; $b=[IO.File]::ReadAllBytes($p); if($b.Length -lt 256){throw 'Bridge DLL too small'}; if($b[0] -ne 0x4D -or $b[1] -ne 0x5A){throw 'Bridge is not PE/MZ'}; $pe=[BitConverter]::ToInt32($b,0x3C); if($pe -lt 0 -or $pe+24 -ge $b.Length){throw 'Invalid PE header offset'}; if($b[$pe] -ne 0x50 -or $b[$pe+1] -ne 0x45 -or $b[$pe+2] -ne 0 -or $b[$pe+3] -ne 0){throw 'Missing PE signature'}; $ch=[BitConverter]::ToUInt16($b,$pe+22); if(($ch -band 0x2000) -eq 0){throw 'PE does not have DLL characteristic'}; Write-Host ('BRIDGE PE DLL PASS characteristics=0x{0:X4}' -f $ch)"
if errorlevel 1 exit /b 1

echo [6/9] Build resources...
pushd resources
zig rc /c 65001 /fo ..\dist\app.res app.rc
popd
if errorlevel 1 exit /b 1

echo [7/9] Build controller EXE...
zig c++ -target x86_64-windows-gnu -O2 -std=c++17 -Wall -Wextra -Werror -municode -static -s ^
  src\controller.cpp dist\app.res -Wl,--subsystem,windows ^
  -lcomctl32 -luser32 -lkernel32 -lgdi32 ^
  -o dist\ThanLongTestAutoHeal_v1.1.12.exe
if errorlevel 1 exit /b 1

echo [8/9] Package AI handoff + knowledge...
copy /y AI_PROJECT_HANDOFF_FULL.md dist\AI_PROJECT_HANDOFF_FULL.md >nul
copy /y AI_START_HERE.md dist\AI_START_HERE.md >nul
copy /y AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md dist\AI_PROJECT_KNOWLEDGE_PROTOCOL_V2_OPTIMIZED.md >nul
copy /y AI_CLIENT_ANALYSIS_RULES.txt dist\AI_CLIENT_ANALYSIS_RULES.txt >nul
copy /y PROJECT_KNOWLEDGE.md dist\PROJECT_KNOWLEDGE.md >nul
copy /y CHANGELOG.md dist\CHANGELOG.md >nul
if errorlevel 1 exit /b 1
set "SRC_SHA=%SOURCE_HEAD_SHA%"
if not defined SRC_SHA set "SRC_SHA=%GITHUB_SHA%"
if not defined SRC_SHA set "SRC_SHA=LOCAL"
set "CHECKOUT_SHA=%GITHUB_SHA%"
if not defined CHECKOUT_SHA set "CHECKOUT_SHA=LOCAL"
set "RUN_ID=%GITHUB_RUN_ID%"
if not defined RUN_ID set "RUN_ID=LOCAL"
set "RUN_NUMBER=%GITHUB_RUN_NUMBER%"
if not defined RUN_NUMBER set "RUN_NUMBER=LOCAL"
> dist\BUILD_EVIDENCE.txt echo PROJECT=ThanLongTestAutoHeal
>> dist\BUILD_EVIDENCE.txt echo VERSION=v1.1.12-test
>> dist\BUILD_EVIDENCE.txt echo SOURCE_HEAD_SHA=%SRC_SHA%
>> dist\BUILD_EVIDENCE.txt echo CHECKOUT_SHA=%CHECKOUT_SHA%
>> dist\BUILD_EVIDENCE.txt echo GITHUB_RUN_ID=%RUN_ID%
>> dist\BUILD_EVIDENCE.txt echo GITHUB_RUN_NUMBER=%RUN_NUMBER%
>> dist\BUILD_EVIDENCE.txt echo ARTIFACT=ThanLongTestAutoHeal-v1.1.12
>> dist\BUILD_EVIDENCE.txt echo BUILD=PASS
>> dist\BUILD_EVIDENCE.txt echo RUNTIME=UNTESTED_FOR_V1.1.12
>> dist\BUILD_EVIDENCE.txt echo NOTE=v1.1.11 runtime proved LuaEnv is MoonSharp.Interpreter.Script and exposed exact DoString String-Table-String signature; see AI_PROJECT_HANDOFF_FULL.md

echo [9/9] Done.
echo BUILD THANH CONG

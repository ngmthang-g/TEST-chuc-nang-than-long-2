@echo off
setlocal
cd /d "%~dp0"
where zig >nul 2>nul
if errorlevel 1 (echo KHONG TIM THAY ZIG & exit /b 1)
if not exist dist mkdir dist
del /q dist\* >nul 2>nul

echo [1/8] Architecture audit...
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop'; $files=(Get-ChildItem 'src' -File | Where-Object {$_.Extension -in '.cpp','.h','.inc'} | ForEach-Object {$_.FullName}); $s=($files|%%{Get-Content $_ -Raw}) -join [Environment]::NewLine;" ^
  "$forbidden=@('CreateRemoteThread','WriteProcessMemory','remote_worker','RemoteExecutor'); foreach($x in $forbidden){if($s -match [regex]::Escape($x)){throw ('Forbidden legacy token: '+$x)}};" ^
  "$bridge=((Get-ChildItem 'src' -Filter 'bridge_part*.inc' | Sort-Object Name | ForEach-Object {Get-Content $_.FullName -Raw}) -join [Environment]::NewLine); foreach($x in @('SendToggleRideState','StartAutoPath','StopAutoPath','ClickNPC','InspectHealDialog','ClickHealDialogChoice','FindScriptUIRoots')){if($bridge -notmatch [regex]::Escape($x)){throw ('Missing required method/feature '+$x)}};" ^
  "$proto=Get-Content 'src/protocol.h' -Raw; foreach($x in @('ReadState = 1','ToggleRide = 2','StartPath = 3','StopPath = 4','ClickNpc = 5','InspectHealDialog = 6','ClickHealDialogChoice = 7')){if($proto -notmatch [regex]::Escape($x)){throw ('Protocol missing '+$x)}};" ^
  "Write-Host 'ARCHITECTURE AUDIT PASS: route + auto-heal test surface only.'"
if errorlevel 1 exit /b 1

echo [2/8] Route FSM self-test...
zig c++ -target x86_64-windows-gnu -O2 -std=c++17 -Wall -Wextra -Werror src\route_logic_test.cpp -o dist\RouteLogicTest.exe
if errorlevel 1 exit /b 1
dist\RouteLogicTest.exe
if errorlevel 1 exit /b 1

echo [3/8] Heal FSM self-test...
zig c++ -target x86_64-windows-gnu -O2 -std=c++17 -Wall -Wextra -Werror src\heal_logic_test.cpp -o dist\HealLogicTest.exe
if errorlevel 1 exit /b 1
dist\HealLogicTest.exe
if errorlevel 1 exit /b 1

echo [4/8] Build bridge DLL...
zig c++ -target x86_64-windows-gnu -O2 -std=c++17 -Wall -Wextra -Werror -shared -s ^
  src\bridge.cpp -luser32 -lkernel32 -o dist\ThanLongCleanRouteBridge.dll
if errorlevel 1 exit /b 1

echo [5/8] Verify bridge PE DLL...
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop'; $p='dist\ThanLongCleanRouteBridge.dll'; $b=[IO.File]::ReadAllBytes($p); if($b.Length -lt 256){throw 'Bridge DLL too small'}; if($b[0] -ne 0x4D -or $b[1] -ne 0x5A){throw 'Bridge is not PE/MZ'}; $pe=[BitConverter]::ToInt32($b,0x3C); if($pe -lt 0 -or $pe+24 -ge $b.Length){throw 'Invalid PE header offset'}; if($b[$pe] -ne 0x50 -or $b[$pe+1] -ne 0x45 -or $b[$pe+2] -ne 0 -or $b[$pe+3] -ne 0){throw 'Missing PE signature'}; $ch=[BitConverter]::ToUInt16($b,$pe+22); if(($ch -band 0x2000) -eq 0){throw 'PE does not have DLL characteristic'}; Write-Host ('BRIDGE PE DLL PASS characteristics=0x{0:X4}' -f $ch)"
if errorlevel 1 exit /b 1

echo [6/8] Build resources...
pushd resources
zig rc /c 65001 /fo ..\dist\app.res app.rc
popd
if errorlevel 1 exit /b 1

echo [7/8] Build controller EXE...
zig c++ -target x86_64-windows-gnu -O2 -std=c++17 -Wall -Wextra -Werror -municode -static -s ^
  src\controller.cpp dist\app.res -Wl,--subsystem,windows ^
  -lcomctl32 -luser32 -lkernel32 -lgdi32 ^
  -o dist\ThanLongTestAutoHeal_v1.1.0.exe
if errorlevel 1 exit /b 1

echo [8/8] Done.
echo BUILD THANH CONG

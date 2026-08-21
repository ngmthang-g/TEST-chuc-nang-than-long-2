from pathlib import Path

root = Path(__file__).resolve().parents[1]
lic = (root / 'src' / 'license_manager.h').read_text(encoding='utf-8')
ctl = (root / 'src' / 'controller.cpp').read_text(encoding='utf-8')
iss = (root / 'installer' / 'ThanLongSetup.iss').read_text(encoding='utf-8')
cmake = (root / 'CMakeLists.txt').read_text(encoding='utf-8')

checks = {
    'v2 machine license': 'StateV2' in lic and 'machineHash' in lic,
    'ProgramData primary': '\\System\\license.dat' in lic,
    'HKLM primary': 'HKEY_LOCAL_MACHINE' in lic and 'SOFTWARE\\ThanLong\\License' in lic,
    'DPAPI LocalMachine': 'CRYPTPROTECT_LOCAL_MACHINE' in lic,
    'used-key history': 'usedKeyBits' in lic and 'KeyAlreadyUsed' in lic,
    'same-key preserves activation': 'preserves the original activation date' in lic,
    'runtime rollback state': 'runtime.dat' in lic and 'ClockRollback' in lic,
    'installer activation CLI': '--install-activate=' in ctl and 'ActivateDetailed' in ctl,
    'normal app no portable activation': 'Hãy chạy ThanLong_Setup.exe' in ctl,
    'setup is admin': 'PrivilegesRequired=admin' in iss,
    'setup keeps activation state': '[UninstallDelete]' not in iss,
    'setup ProgramData dirs': '{commonappdata}\\ThanLong\\System' in iss,
    'version 0.6.1.12': 'VERSION 0.6.1.12' in cmake and 'v0.6.1.12' in ctl,
}
failed = [name for name, ok in checks.items() if not ok]
if failed:
    for name in failed:
        print('FAIL:', name)
    raise SystemExit(1)
for name in checks:
    print('PASS:', name)

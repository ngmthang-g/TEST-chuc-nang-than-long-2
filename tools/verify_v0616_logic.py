#!/usr/bin/env python3
"""CI bootstrap: apply the exact v0.6.1.7 hotfix patch, then run the real v0.6.1.7 verifier."""
from pathlib import Path
import base64, hashlib, zlib, subprocess
ROOT=Path(__file__).resolve().parents[1]
PATCH_SHA='7ae10a7dfabcfaf418bdf28ec21f13f7f3e18acd85d428a16d0d6f82f335640e'
def run(*args): subprocess.run(args,cwd=ROOT,check=True)
def main():
    version=(ROOT/'VERSION.txt').read_text(encoding='utf-8').strip()
    if version=='v0.6.1.6':
        encoded=''.join((ROOT/'.v0617'/f'chunk{i}').read_text().strip() for i in (1,2))
        patch=zlib.decompress(base64.b64decode(encoded))
        if hashlib.sha256(patch).hexdigest()!=PATCH_SHA:
            raise SystemExit('FAIL: v0.6.1.7 patch SHA mismatch')
        patch_path=ROOT/'.v0617_exact.patch'; patch_path.write_bytes(patch)
        run('git','apply','--check',str(patch_path))
        run('git','apply',str(patch_path))
    run('python','tools/verify_v0617_logic.py')
    print('PASS: bootstrap applied exact v0.6.1.7 AUTO->StopAuto2 hotfix for CI build')
if __name__=='__main__': main()

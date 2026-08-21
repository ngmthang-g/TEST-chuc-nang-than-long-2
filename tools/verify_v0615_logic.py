#!/usr/bin/env python3
"""One-shot CI bootstrap: reconstruct and apply the exact v0.6.1.6 patch, then run its verifier."""

from pathlib import Path
import base64
import subprocess
import sys
import zlib
import hashlib

ROOT = Path(__file__).resolve().parents[1]
CHUNK_DIR = ROOT / ".v0616"
EXPECTED_SHA256 = "fa29c25b7c47f91aabfccacbb5d4d214d3d1dbabd9179f09d55ecd0c8dd5c426"


def main() -> None:
    encoded = "".join((CHUNK_DIR / f"chunk{i}").read_text(encoding="utf-8").strip() for i in range(1, 5))
    patch = zlib.decompress(base64.b64decode(encoded))
    actual = hashlib.sha256(patch).hexdigest()
    if actual != EXPECTED_SHA256:
        raise SystemExit(f"FAIL: v0.6.1.6 patch hash mismatch: {actual}")

    patch_path = ROOT / ".v0616_exact.patch"
    patch_path.write_bytes(patch)
    subprocess.run(["git", "apply", "--check", str(patch_path)], cwd=ROOT, check=True)
    subprocess.run(["git", "apply", str(patch_path)], cwd=ROOT, check=True)

    verifier = ROOT / "tools" / "verify_v0616_logic.py"
    if not verifier.exists():
        raise SystemExit("FAIL: v0.6.1.6 verifier was not created by patch")
    subprocess.run([sys.executable, str(verifier)], cwd=ROOT, check=True)
    print("PASS: bootstrap applied exact v0.6.1.6 patch for CI build")


if __name__ == "__main__":
    main()

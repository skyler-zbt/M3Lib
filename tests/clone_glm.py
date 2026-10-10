#!/usr/bin/env python3
"""Clone icaven/glm into tests/third_party/glm for reference testing.

Run from any working directory with:
    python tests/clone_glm.py

An optional Git branch or tag can be selected with --ref.
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path


GLM_REPOSITORY = "https://github.com/icaven/glm.git"
DEFAULT_REF = "master"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--ref",
        default=DEFAULT_REF,
        help=f"GLM branch or tag to clone (default: {DEFAULT_REF})",
    )
    parser.add_argument(
        "--destination",
        type=Path,
        help="destination directory (default: tests/third_party/glm)",
    )
    args = parser.parse_args()

    git = shutil.which("git")
    if git is None:
        parser.error("git was not found; install Git and ensure it is on PATH")

    tests_dir = Path(__file__).resolve().parent
    destination = args.destination or tests_dir / "third_party" / "glm"
    if not destination.is_absolute():
        destination = (Path.cwd() / destination).resolve()

    if destination.exists():
        if (destination / ".git").is_dir():
            print(f"GLM already exists at {destination}; leaving it unchanged.")
            return 0
        parser.error(f"destination already exists and is not a Git checkout: {destination}")

    destination.parent.mkdir(parents=True, exist_ok=True)
    command = [git, "clone", "--depth", "1", "--branch", args.ref, GLM_REPOSITORY, str(destination)]
    print(f"Cloning GLM ({args.ref}) into {destination}...")
    try:
        subprocess.run(command, check=True)
    except subprocess.CalledProcessError as exc:
        # Remove only the partial destination created by this failed clone.
        if destination.exists():
            shutil.rmtree(destination)
        return exc.returncode
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

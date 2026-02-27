#!/usr/bin/env python3
import sys
import re
import subprocess
from pathlib import Path

def cmd(command: list[str]) -> int:
    try:
        subprocess.run(command, check=True, capture_output=True, text=True)
        return 0
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {' '.join(command)}")
        print(e.stderr)
        return 3

def main() -> int:
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} {{major|minor|patch}}")
        return 2

    part = sys.argv[1]
    version_file = Path("VERSION")

    if not version_file.exists():
        print("Error: VERSION file not found")
        return 1

    version_text = version_file.read_text().strip()
    match = re.match(r"^(\d+)\.(\d+)\.(\d+)$", version_text)
    if not match:
        print("Invalid VERSION format. Expected: MAJOR.MINOR.PATCH")
        return 1

    major, minor, patch = map(int, match.groups())

    if part == "major":
        major += 1
        minor = 0
        patch = 0
    elif part == "minor":
        minor += 1
        patch = 0
    elif part == "patch":
        patch += 1
    else:
        print("Invalid argument")
        return 2

    new_version = f"{major}.{minor}.{patch}"
    version_file.write_text(new_version)

    if c := cmd(["git", "add", "VERSION"])                                                != 0: return c
    if c := cmd(["git", "commit", "-m", f"Bump version to {new_version}"])                != 0: return c
    if c := cmd(["git", "tag", "-a", f"v{new_version}", "-m", f"Release v{new_version}"]) != 0: return c

    print(f"Version bumped to {new_version}")
    return 0

if __name__ == "__main__":
    exit(main())

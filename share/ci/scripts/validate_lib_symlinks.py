#!/usr/bin/env python3

# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

import os
import re
import sys
import platform
from pathlib import Path

def extract_version(config_path):
    with open(config_path) as f:
        for line in f:
            match = re.match(r'#define IMATH_LIB_VERSION_STRING "([^"]+)"', line)
            if match:
                return match.group(1).split('.')
    raise RuntimeError("IMATH_LIB_VERSION_STRING not found in config file")

def check_symlink(src, expected_target):
    if not src.is_symlink():
        print(f"❌ {src} is not a symbolic link")
        sys.exit(1)

    actual_target = os.readlink(src)
    if actual_target != expected_target:
        print(f"❌ {src} points to {actual_target}, expected {expected_target}")
        sys.exit(1)

    print(f"✅ {src} → {expected_target}")

def main():
    if len(sys.argv) < 3:
        print("Usage: validate_lib_symlinks.py <library path> <ImathConfig.h> [suffix] [python suffix]")
        sys.exit(1)

    base_path = Path(sys.argv[1]).resolve()
    config_file = Path(sys.argv[2]).resolve()
    suffix_arg = sys.argv[3] if len(sys.argv) > 3 else None

    pythonmajor = sys.version_info.major
    pythonminor = sys.version_info.minor
    python_suffix = sys.argv[4] if len(sys.argv) > 4 else f"_Python{pythonmajor}_{pythonminor}"

    os.chdir(base_path.parent)
    base = base_path.name

    debug_postfix = ""
    if base.endswith("_d"):
        base = base[:-2]
        debug_postfix = "_d"

    soversion, major, minor, patch = extract_version(config_file)
    suffix = suffix_arg if suffix_arg else f"-{major}_{minor}"

    system = platform.system()
    if system == "Darwin":
        ext = ".dylib"
    elif system == "Linux":
        ext = ".so"
    elif system.startswith("MINGW") or system.startswith("MSYS") or system.startswith("CYGWIN") or system == "Windows":
        print("Skipping symbolic link checks on Windows")
        return
    else:
        print("Unsupported OS")
        sys.exit(1)

    unversioned = f"{base}{debug_postfix}{ext}"
    suffixed = f"{base}{python_suffix}{suffix}{debug_postfix}{ext}"
    soversioned = f"{base}{python_suffix}{suffix}{debug_postfix}.{soversion}{ext}"
    fullversioned = f"{base}{python_suffix}{suffix}{debug_postfix}.{soversion}.{major}.{minor}.{patch}{ext}"

    check_symlink(Path(unversioned), suffixed)
    check_symlink(Path(suffixed), soversioned)
    check_symlink(Path(soversioned), fullversioned)

    if not Path(fullversioned).is_file():
        print(f"❌ {fullversioned} does not exist")
        sys.exit(1)

    print(f"✅ {fullversioned} exists and is a shared object")

if __name__ == "__main__":
    main()

#!/usr/bin/env python3

import os, sys, site, glob
from pathlib import Path

print(f"{__file__}: PYTHONPATH={os.environ['PYTHONPATH']}")
pythonpath = os.environ.get('PYTHONPATH')
if pythonpath:
    # Use os.pathsep which is ':' on Unix/macOS and ';' on Windows
    paths = pythonpath.split(os.pathsep)
    for path in paths:
        if not path:
            continue
        resolved_path = Path(path).expanduser().resolve()
        print(f"Contents of: {resolved_path}")
        if resolved_path.is_dir():
            for root, dirs, files in os.walk(resolved_path):
                for f in files:
                    file_path = Path(root) / f
                    print(f"  {file_path}")
        else:
            print(f"  [Not a directory or does not exist]")
        print()
PATH = os.environ.get('PATH')
if PATH:
    print(f"PATH={PATH}")
    # Use os.pathsep which is ':' on Unix/macOS and ';' on Windows
    paths = PATH.split(os.pathsep)
    for path in paths:
        if not path:
            continue
        resolved_path = Path(path).expanduser().resolve()
        print(f"Contents of: {resolved_path}")
        if resolved_path.is_dir():
            for root, dirs, files in os.walk(resolved_path):
                for f in files:
                    file_path = Path(root) / f
                    print(f"  {file_path}")
        else:
            print(f"  [Not a directory or does not exist]")
        print()


def test_import():

    try:
        import pybindimath
        print()
        print(f"pybindimath imported from {pybindimath.__file__}")
        print("ok")
    except Exception as e:
        print(f"test_import_pybindimath.py failed. Can't import pybindimath: {e}")
        print("meh")

test_import()

#!/usr/bin/env python3

import os, site, sys, glob


def test_import():

    print(f"test_import: sys.path={sys.path}")
    print(f"PYTHONPATH: {os.environ['PYTHONPATH']}")
    print(f"cwd: {os.getcwd()}")
    print(f"looking for imath...")
    found = False
    for root, _, files in os.walk(os.getcwd()):
        for file in files:
            if file.startswith("imath.") and file.endswith((".so",".pyd")):
                print(f"  found: {os.path.join(root, file)}")
                found = True
    if not found:
        print("not found.")

    try:
        import imath
        print()
        print(f"imath imported from {imath.__file__}")
        print("ok")
    except Exception as e:
        print(f"test_import.py failed. Can't import imath: {e}")
        print("meh")

test_import()

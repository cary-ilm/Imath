#!/usr/bin/env python3

import os, sys, site, glob

def test_import():

    print(f"test_import: {sys.path}")

    print(f"looking for pybindimath...")
    found = False
    for root, _, files in os.walk(os.getcwd()):
        for file in files:
            if file.startswith("imath.") and file.endswith((".so",".pyd")):
                print(f"  found: {os.path.join(root, file)}")
                found = True
    if not found:
        print("not found.")

    try:
        import pybindimath
        print()
        print(f"pybindimath imported from {pybindimath.__file__}")
        print("ok")
    except Exception as e:
        print(f"test_import.py failed. Can't import pybindimath: {e}")
        print("meh")

test_import()

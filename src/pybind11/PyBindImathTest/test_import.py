#!/usr/bin/env python3

import os, sys, site, glob

def test_import():

    print(f"test_import: {sys.path}")

    print(f"looking for pybindimath...")
    found = False
    for directory in site.getsitepackages():
        pattern = os.path.join(directory, "pybindimath.*")
        matches = glob.glob(pattern)
        if matches:
            for match in matches:
                print(f"  found: {match}")
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

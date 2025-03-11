#!/usr/bin/env python3

import os, site, sys, glob


def test_import():

    print(f"test_import: {sys.path}")

    print(f"looking for imath...")
    found = False
    for directory in site.getsitepackages():
        pattern = os.path.join(directory, "imath.*")
        matches = glob.glob(pattern)
        if matches:
            for match in matches:
                print(f"  found: {match}")
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

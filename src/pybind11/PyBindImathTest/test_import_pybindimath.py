#!/usr/bin/env python3

import os, sys, site, glob

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

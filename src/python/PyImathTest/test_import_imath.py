#!/usr/bin/env python3

import os, site, sys, glob


def test_import():

    try:
        import imath
        print()
        print(f"imath imported from {imath.__file__}")
        print("ok")
    except Exception as e:
        print(f"test_import_imath.py failed. Can't import imath: {e}")
        print("meh")

test_import()

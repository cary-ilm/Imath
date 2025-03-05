#!/usr/bin/env python3

import os, site, sys, glob


def test_import():

    try:
        import imathnumpy
        print()
        print(f"imathnumpy imported from {imathnumpy.__file__}")
        print("ok")
    except Exception as e:
        print(f"test_import_imathnumpy.py failed. Can't import imathnumpy: {e}")
        print("meh")

test_import()

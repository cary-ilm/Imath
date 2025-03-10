#!/usr/bin/env python3

import os, site


def test_import():
    
    print("test_import:")
    print("  site_packages:")
    site_packages = site.getsitepackages()[0]
    for root, dirs, files in os.walk(site_packages):
        for file in files:
            print(f"    {os.path.join(root, file)}")

    print("looking for imath...")
    try:
        import imath
        print()
        print(f"imath imported from {imath.__file__}")
        print("ok")
    except Exception as e:
        print(f"test_import.py failed. Can't import imathL {e}")
        print("meh")
        
test_import()

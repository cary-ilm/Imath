#!/usr/bin/env python3

def test_import():
    try:
        import pybindimath
        print()
        print(f"pybindimath imported from {pybindimath.__file__}")
        print("ok")
    except:
        print(f"test_import.py failed. Can't import pybindimath")
        print("meh")
        
test_import()

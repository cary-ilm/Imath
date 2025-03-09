#!/usr/bin/env python3

import os

def print_pythonpath_files():
    pythonpath = os.getenv("PYTHONPATH")
    if not pythonpath:
        print("PYTHONPATH is not set.")
        return

    print(f"PYTHONPATH:")
    for path in pythonpath.split(os.pathsep):
        if not os.path.isdir(path):
            continue  # Skip if not a valid directory

        for root, _, files in os.walk(path):
            for file in files:
                file_path = os.path.join(root, file)
                print(f"    {file_path}")

def test_import():
    
    print_pythonpath_files()

    try:
        import pybindimath
        print()
        print(f"pybindimath imported from {pybindimath.__file__}")
        print("ok")
    except Exception as e:
        print(f"test_import.py failed. Can't import pybindimathL {e}")
        print("meh")
        
test_import()

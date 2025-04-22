#!/usr/bin/env python3

import os, sys, site, glob
from pathlib import Path

def print_files_in_path(var):
    path_env = os.environ.get(var, '')
    print(f"{var}={path_env}")
    for directory in path_env.split(os.pathsep):
        if directory:
            directory = os.path.normpath(directory)
            if os.path.isdir(directory):
                for file in os.listdir(directory):
                    full_path = os.path.join(directory, file)
                    if os.path.isfile(full_path):
                        print(f'    {full_path}')

def test_import():

    try:
        import imath
        print()
        print(f"imath imported from {imath.__file__}")
        print("ok")
    except Exception as e:
        print(f"{__file__} failed. Can't import imath: {e}")
        #print_files_in_path('PATH')
        #print_files_in_path('PYTHONPATH')

        import traceback
        traceback.print_exc()
        
        import ctypes
        try:
            print('ctypes.cdll.LoadLibrary("/a/Imath/Imath/_install/python/imath.pyd")')

            lib = ctypes.cdll.LoadLibrary("/a/Imath/Imath/_install/python/imath.pyd")

            print(f"lib={lib}")
            
        except Exception as e:
            print(f"Error in cdll.LoadLibrary: {e}")

        sys.exit(1)

test_import()

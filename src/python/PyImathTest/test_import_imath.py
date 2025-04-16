#!/usr/bin/env python3

import os, site, sys, glob

def list_files_in_path(var):
    path_env = os.environ.get(var, '')
    path_dirs = path_env.split(os.pathsep)
    all_files = []
    for directory in path_dirs:
        if not directory:
            continue
        directory = os.path.normpath(directory)
        if os.path.isdir(directory):
            try:
                for entry in os.listdir(directory):
                    full_path = os.path.join(directory, entry)
                    if os.path.isfile(full_path):
                        all_files.append(full_path)
            except PermissionError:
                print(f"Permission denied accessing: {directory}")
            except OSError as e:
                print(f"Error accessing {directory}: {e}")
    
    return path_env, all_files

path_env, path_files = list_files_in_path('PATH')
print(f"{__file__}: PATH={path_env}")
for f in path_files:
    print(f"  {f}")
pythonpath_env, pythonpath_files = list_files_in_path('PYTHONPATH')
print(f"PYTHONPATH={pythonpath_env}")
for f in pythonpath_files:
    print(f"  {f}")


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

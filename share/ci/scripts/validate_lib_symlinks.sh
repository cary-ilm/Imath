#!/usr/bin/env bash

# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

# Validate that the shared objects have the proper symbolic links between the so-versioned libraries:
#
# libImath_d.dylib -> libImath-3_2_d.dylib
# libImath-3_2_d.dylib -> libImath-3_2_d.30.dylib
# libImath-3_2_d.30.dylib -> libImath-3_2_d.30.3.2.0.dylib
#
# or:
#
# libPyImath_d.dylib -> libPyImath_Python3_9-3_2_d.dylib
# libPyImath_Python3_9-3_2_d.dylib -> libPyImath_Python3_9-3_2_d.30.dylib
# libPyImath_Python3_9-3_2_d.30.dylib -> libPyImath_Python3_9-3_2_d.30.3.2.0.dylib

# $1 is the path of the unversioned lib: libImath.so
# $2 is the the path of ImathConfig.h, which contains the version numbers
# $3 is the "-3_2" suffix if it is manualy specified
# $4 is the "_Python3_2" suffix if it is manualy specified
# 

set -euo pipefail

base="$1"
CONFIG_FILE="$2"

# Do it in the directory of library, that way no worrying about paths

cd $(dirname $base)
base=$(basename $base)

# Check if the base library is Debug, i.e. ends in "_d"
debug_postfix=""
if [[ "$base" == *_d ]]; then
  base="${base%_d}"
  debug_postfix="_d"
fi

# Extract version string from IMATH_LIB_VERSION_STRING in ImathConfig.h 
version_string=$(sed -n 's/#define IMATH_LIB_VERSION_STRING \"\([^"]*\)\"/\1/p' "$CONFIG_FILE")
IFS='.' read -r soversion major minor patch <<< "$version_string"

suffix=${3:-"-${major}_${minor}"}
python="${4:-}"

case "$(uname -s)" in
    Darwin)
        unversioned=${base}${debug_postfix}.dylib # libImath_d.dylib
        suffixed=${base}${python}${suffix}${debug_postfix}.dylib # libImath-3_2_d.dylib
        soversioned=${base}${python}${suffix}${debug_postfix}.${soversion}.dylib # libImath-3_2_d.30.dylib
        fullversioned=${base}${python}${suffix}${debug_postfix}.${soversion}.${major}.${minor}.${patch}.dylib # libImath-3_2_d.30.3.2.0.dylib
        ;;
    Linux)
        unversioned=${base}${debug_postfix}.so # libImath_d.so
        suffixed=${base}${python}${suffix}${debug_postfix}.so # libImath-3_2_d.so
        soversioned=${base}${python}${suffix}${debug_postfix}.so.${soversion} # libImath-3_2_d.so.30
        fullversioned=${base}${python}${suffix}${debug_postfix}.so.${soversion}.${major}.${minor}.${patch} # libImath-3_2_d.so.30.3.2.0
        ;;
    MINGW*|MSYS*|CYGWIN*|Windows_NT)
        lib_suffix="dll"
        ;;
    *)
        echo "Unsupported OS"
        exit 1
        ;;
esac

check_symlink() {
    local src="$1"
    local expected_target="$2"

    if [[ ! -L "$src" ]]; then
        echo "❌ $src is not a symbolic link"
        exit 1
    fi

    local actual_target
    actual_target=$(readlink "$src")
    if [[ "$actual_target" != "$expected_target" ]]; then
        echo "❌ $src points to $actual_target, expected $expected_target"
        exit 1
    fi

    echo "✅ $src → $expected_target"
}

# Validate the link chain
check_symlink "${unversioned}" "${suffixed}"
check_symlink "${suffixed}" "${soversioned}"
check_symlink "${soversioned}" "${fullversioned}"

# Validate that the final target exists and is a regular file
if [[ ! -f "${fullversioned}" ]]; then
    echo "❌ ${fullversioned} does not exist"
    exit 1
fi

echo "✅ ${fullversioned} exists and is a shared object"

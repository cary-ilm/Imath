# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

get_target_property(TEST_PYIMATH_OUTDIR imath LIBRARY_OUTPUT_DIRECTORY)

add_test(NAME PyImath.RunTests.py COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src/python/PyImathPythonTest/RunTests.py ${TEST_PYIMATH_OUTDIR})

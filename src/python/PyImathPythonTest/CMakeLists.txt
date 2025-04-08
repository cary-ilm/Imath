# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

get_target_property(TEST_PYIMATH_OUTDIR imath LIBRARY_OUTPUT_DIRECTORY)

message (STATUS "TEST_PYIMATH_OUTDIR=${TEST_PYIMATH_OUTDIR}")

add_test(NAME PyImath_Python_TEST COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src/python/PyImathPythonTest/RunTests.py ${TEST_PYIMATH_OUTDIR})

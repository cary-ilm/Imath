# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

add_test(PyImath.PyImathNumpyTest_Python3
  ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathNumpyTest.in
)

set_tests_properties(PyImath.PyImathNumpyTest_Python3 PROPERTIES
  ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}"
)


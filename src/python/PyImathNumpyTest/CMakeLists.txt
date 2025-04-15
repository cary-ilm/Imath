# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message (STATUS "Configuring PyImathNumpyTest")

add_test(PyImath.PyImathNumpyTest
  ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathNumpyTest.in
)

set_tests_properties(PyImath.PyImathNumpyTest PROPERTIES
  ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}"
)


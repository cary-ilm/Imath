# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message (STATUS "Configuring PyBindImathTest")

find_package(Python3 QUIET REQUIRED COMPONENTS Interpreter)

if(NOT Python3_Interpreter_FOUND)
    message(FATAL_ERROR "Cannot find Python3 interpreter")
endif()

add_test(PyBindImath.PyBindImathTest 
  ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyBindImathTest
)

set_tests_properties(PyBindImath.PyBindImathTest PROPERTIES
    ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}"
)


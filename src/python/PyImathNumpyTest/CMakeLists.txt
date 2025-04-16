# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message (STATUS "Configuring PyImathNumpyTest")

set(PYIMATHNUMPY_MODULE imathnumpy)
add_test(NAME PyImath.PyImathNumpyTest
  COMMAND ${CMAKE_COMMAND} -E env
          "PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATHNUMPY_MODULE}>"
          "PATH=$<TARGET_FILE_DIR:${PYIMATHNUMPY_MODULE}>;$ENV{PATH}"
          ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathNumpyTest.in
)

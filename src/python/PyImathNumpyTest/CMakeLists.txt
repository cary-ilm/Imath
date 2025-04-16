# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message (STATUS "Configuring PyImathNumpyTest")

set(PYIMATHNUMPY_MODULE imathnumpy)
set(PYIMATH_LIBRARY PyImath_Python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}${IMATH_LIB_SUFFIX}) 
add_test(NAME PyImath.PyImathNumpyTest
  COMMAND ${CMAKE_COMMAND} -E env
          "PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATHNUMPY_MODULE}>"
          "PATH=$<TARGET_FILE_DIR:${PYIMATH_LIBRARY}>;$ENV{PATH}"
          ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathNumpyTest.in
)

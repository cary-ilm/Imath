# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message (STATUS "Configuring PyImathNumpyTest")

set(PYIMATHNUMPY_MODULE imathnumpy)

if (NOT "${PYIMATH_LIB_PYTHONVER_ROOT}" STREQUAL "")
   set (PYTHON_VER_STRING "${PYIMATH_LIB_PYTHONVER_ROOT}${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}")
endif()
set(PYIMATH_LIBRARY PyImath${PYTHON_VER_STRING}${PYIMATH_LIB_SUFFIX}) 

# Execute the test by invoking python with the script as an
# argument, with PATH and PYTHONPATH set properly

add_test(NAME PyImath.PyImathNumpyTest
  COMMAND ${CMAKE_COMMAND} -E env
          "PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATHNUMPY_MODULE}>"
          "PATH=$<TARGET_FILE_DIR:${PYIMATH_LIBRARY}>:$ENV{PATH}"
          ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathNumpyTest.in
)

# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

include(GNUInstallDirs)

include(config/PyImathSetup.cmake)

#if(POLICY CMP0177)
#  cmake_policy(SET CMP0177 NEW)
#endif()

#
# Python and Boost
#

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

if(NOT Python3_FOUND)
    message(FATAL_ERROR "Could not find Python")
endif()

find_package(Boost REQUIRED COMPONENTS python)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost")
endif()

#
# PyImath and PyImathNumpy
#

add_subdirectory( PyImath )
add_subdirectory( PyImathNumpy )

#
# Tests
#

include(CTest)

# This option allows the tests to be built but not run, helpful to
# catch build failures even if the test itself fails

option(IMATH_TEST_PYTHON "Include the python tests when BUILD_TESTING is on" ON)

if(BUILD_TESTING AND IMATH_TEST_PYTHON)
  enable_testing()
  add_subdirectory( PyImathPythonTest )
  add_subdirectory( PyImathTest )
  add_subdirectory( PyImathSpeedTest )
  add_subdirectory( PyImathNumpyTest )
endif()

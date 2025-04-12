# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

include(config/PyImathSetup.cmake)

if(POLICY CMP0177)
  cmake_policy(SET CMP0177 NEW)
endif()

set(Python_LIBRARIES "C:\\hostedtoolcache\\windows\\Python\\3.9.13\\x64\\libs")
set(Python_LIBRARY "C:\\hostedtoolcache\\windows\\Python\\3.9.13\\x64\\libs")
set(Python3_USE_DEBUG_LIBRARY OFF)
set(Python3_DEBUG_LIBRARIES "${Python3_LIBRARIES}")
set(Python3_LIBRARY_DEBUG "${Python3_LIBRARY}")

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

if(NOT Python3_FOUND)
    message(FATAL_ERROR "Could not find Python")
endif()

find_package(Boost REQUIRED COMPONENTS python)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost")
endif()


add_subdirectory( PyImath )
add_subdirectory( PyImathNumpy )

##########################
# Tests
##########################
include(CTest)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory( PyImathPythonTest )
  add_subdirectory( PyImathTest )
  add_subdirectory( PyImathSpeedTest )
  add_subdirectory( PyImathNumpyTest )
endif()

# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

include(config/PyImathSetup.cmake)

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

if(NOT Python_FOUND)
    message(FATAL_ERROR "Could not find Python")
endif()

find_package(Boost COMPONENTS python REQUIRED)

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
#  add_subdirectory( PyImathNumpyTest )
endif()

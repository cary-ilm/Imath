# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

include(config/PyImathSetup.cmake)

add_subdirectory(config)

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

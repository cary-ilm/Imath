# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# ImathConfig
#

if (IMATH_ENABLE_LARGE_STACK)
  set(IMATH_HAVE_LARGE_STACK ON)
endif()
if (IMATH_USE_DEFAULT_VISIBILITY)
  set(IMATH_ENABLE_API_VISIBILITY OFF)
else()
  set(IMATH_ENABLE_API_VISIBILITY ON)
endif()

configure_file(ImathConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/ImathConfig.h)
add_library(ImathConfig INTERFACE)
add_library(Imath::Config ALIAS ImathConfig)

target_include_directories(ImathConfig INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}>)

#
# The main export of the configuration: this is the cmake equivalent
# of a pkg-config file and replaces the Find*.cmake of the "old" cmake
#

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/ImathConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/ImathConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Imath
)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/ImathConfigVersion.cmake"
  VERSION ${IMATH_VERSION}
  COMPATIBILITY SameMajorVersion
)

if (IMATH_INSTALL)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ImathConfig.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/ImathConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Imath
  )

  install(EXPORT Imath
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Imath
    FILE ImathTargets.cmake
    NAMESPACE Imath::
    EXPORT_LINK_INTERFACE_LIBRARIES
 )

  export(EXPORT Imath
    FILE "${CMAKE_CURRENT_BINARY_DIR}/ImathTargets.cmake"
    NAMESPACE Imath::
  )
endif()

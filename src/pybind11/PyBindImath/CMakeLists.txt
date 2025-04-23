# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# Declare the libPyImath library, identified by PYIMATH_LIBRARY
#
# The name has a "_Python<major>_<minor>-<release>" suffix,
# i.e. libPyBindImath_Python3_10-3_2.so, subject to
# PYIMATH_LIB_PYTHONVER_ROOT and PYIMATH_LIB_SUFFIX
#

find_package(pybind11 CONFIG REQUIRED)

set(PYBINDIMATH_LIBRARY "PyBindImath${PYBINDIMATH_LIB_SUFFIX}")

message(STATUS "Configuring pybindimath module and ${PYBINDIMATH_LIBRARY} library")

set(PYBINDIMATH_SOURCES
    PyBindImathFun.cpp
    PyBindImathBox.cpp
    PyBindImathVec.cpp
    PyBindImathPlane.cpp
    PyBindImathLine.cpp
#    PyBindImathEuler.cpp
#    PyBindImathFrustum.cpp
)

set(PYBINDIMATH_HEADERS
    PyBindImathExport.h
    PyBindImath.h
)

#
# Shared Library
#

add_library(${PYBINDIMATH_LIBRARY} SHARED ${PYBINDIMATH_SOURCES})

set_target_properties(${PYBINDIMATH_LIBRARY} PROPERTIES
  SOVERSION ${IMATH_LIB_SOVERSION}
  VERSION ${IMATH_LIB_VERSION}
  POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(${PYBINDIMATH_LIBRARY}
  PRIVATE Imath::Imath pybind11::module)

target_include_directories(${PYBINDIMATH_LIBRARY}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>
)

#
# Python module, identified by PYBINDIMATH_MODULE e.g. pybindimath.cpython-311-x86_64-linux-gnu.so
#

set(PYBINDIMATH_MODULE  pybindimath) # import pybindimath

pybind11_add_module(${PYBINDIMATH_MODULE} pybindimathmodule.cpp)

set_target_properties(${PYBINDIMATH_MODULE} PROPERTIES
    PREFIX ""
    DEBUG_POSTFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}/"
)

target_link_libraries(${PYBINDIMATH_MODULE}
    PRIVATE
        ${PYBINDIMATH_LIBRARY}
        Imath::Imath
)


#
# Installation
#

IF (IMATH_INSTALL)

  if (NOT DEFINED PYTHON_INSTALL_DIR)

    #
    # Set the PYTHON_INSTALL_DIR but only if it's not already set,
    # which allows an externally-set value to take effect.
    #
  
    set(PYTHON_INSTALL_DIR "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
    message(STATUS "installing ${PYBINDIMATH_MODULE} to ${PYTHON_INSTALL_DIR}")

  else()

    message(STATUS "installing ${PYBINDIMATH_MODULE} to ${PYTHON_INSTALL_DIR} (set externally)")
  
  endif()
  
  #
  # Install shared library
  # 

  install(TARGETS ${PYBINDIMATH_LIBRARY}
    EXPORT "${PYBINDIMATH_LIBRARY}Targets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  # Install headers

  install(FILES ${PYBINDIMATH_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PYBINDIMATH_LIBRARY})

  # Install export set for downstream CMake

  install(EXPORT ${PYBINDIMATH_LIBRARY}Targets
    FILE ${PYBINDIMATH_LIBRARY}Targets.cmake
    NAMESPACE ${PYBINDIMATH_LIBRARY}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PYBINDIMATH_LIBRARY}
  )

  #
  # Install Python module 
  #
  
  install(TARGETS ${PYBINDIMATH_MODULE} LIBRARY
    DESTINATION ${PYTHON_INSTALL_DIR})

  # Config file for find_package()
  include(CMakePackageConfigHelpers)

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PYBINDIMATH_LIBRARY}ConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
  )

  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PYBINDIMATH_LIBRARY}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PYBINDIMATH_LIBRARY}
  )

  if (BUILD_SHARED_LIBS AND (NOT "${IMATH_LIB_SUFFIX}" STREQUAL "") AND IMATH_INSTALL_SYM_LINK)

    # create symlinks for the shared object so versions

    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(verlibname ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH_LIBRARY}${pythonver_root}${IMATH_LIB_SUFFIX}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(baselibname ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH_LIBRARY}${pythonver_root}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
    file(CREATE_LINK ${verlibname} ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} SYMBOLIC)
    if(WIN32)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
      install(CODE "message(STATUS \"Creating symlink ${CMAKE_INSTALL_FULL_BINDIR}/${baselibname} -> ${verlibname}\")")
    else()
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
      install(CODE "message(STATUS \"Creating symlink ${CMAKE_INSTALL_FULL_LIBDIR}/${baselibname} -> ${verlibname}\")")
    endif()
  endif()

  # pkgconfig

  if(IMATH_INSTALL_PKG_CONFIG)
    set(pcinfile PyBindImath.pc.in)
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix "\${prefix}")
    set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
    set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
    string(REPLACE ".in" "" pcout ${pcinfile})
    configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    message(STATUS "generating ${pcout}") 
  else()
    message(STATUS "pkg-config generation disabled for PyBindImath")
  endif()

endif()

if(IMATH_BUILD_APPLE_FRAMEWORKS)
    set_target_properties(${PYBINDIMATH_LIBRARY} PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION PYBINDIMATH_LIB_SUFFIX
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER github.com/AcademySoftwareFoundation/Imath.PyBindImath
        # OUTPUT_NAME "pybindimath"
        MACOSX_FRAMEWORK_IDENTIFIER github.com/AcademySoftwareFoundation/Imath.PyBindImath
        MACOSX_FRAMEWORK_BUNDLE_VERSION PYBINDIMATH_LIB_SUFFIX
        MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${Imath_VERSION})
endif()

if (MSVC)
    target_compile_options(${PYBINDIMATH_LIBRARY} PRIVATE /bigobj)
endif()

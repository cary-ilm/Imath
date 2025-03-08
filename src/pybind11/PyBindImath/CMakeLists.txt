# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message(STATUS "Configuring pybindimath module")

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

set(PYBINDIMATH_LIBRARY PyBindImath) # libPyBindImath.so
set(PYBINDIMATH_MODULE  pybindimath) # import pybindimath

set(PYBINDIMATH_SOURCES
    PyBindImathBox.cpp
    PyBindImathVec.cpp
    PyBindImathPlane.cpp
    PyBindImathLine.cpp
    PyBindImathFun.cpp
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

target_link_libraries(${PYBINDIMATH_LIBRARY} PRIVATE Imath::Imath pybind11::module)

target_include_directories(${PYBINDIMATH_LIBRARY}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

#
# Python module
#

pybind11_add_module(${PYBINDIMATH_MODULE} pybindimathmodule.cpp $<TARGET_OBJECTS:${PYBINDIMATH_LIBRARY}>)
target_link_libraries(${PYBINDIMATH_MODULE} PRIVATE ${PYBINDIMATH_LIBRARY} Imath::Imath)

#
# Installation
#

include(GNUInstallDirs)

if(NOT SKBUILD)
  set(PYTHON_INSTALL_DIR "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
endif()

# Install shared library
install(TARGETS ${PYBINDIMATH_LIBRARY}
    EXPORT "${PYBINDIMATH_LIBRARY}Targets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(FILES ${PYBINDIMATH_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PYBINDIMATH_LIBRARY})

# Install export set for downstream CMake
install(EXPORT ${PYBINDIMATH_LIBRARY}Targets
    FILE ${PYBINDIMATH_LIBRARY}Targets.cmake
    NAMESPACE ${PYBINDIMATH_LIBRARY}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PYBINDIMATH_LIBRARY}
)

# Install Python module 
if(NOT SKBUILD)
  install(TARGETS pybindimath
      LIBRARY DESTINATION ${PYTHON_INSTALL_DIR}
  )
endif()

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

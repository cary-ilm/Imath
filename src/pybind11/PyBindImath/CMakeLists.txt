# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# Declare the libPyBindImath library, identified by PYBINDIMATH_LIBRARY
#
# The name has a "_Python<major>_<minor>-<release>" suffix,
# i.e. libPyBindImath_Python3_10-3_2.so, subject to
# PYIMATH_LIB_PYTHONVER_ROOT and PYIMATH_LIB_SUFFIX
#

find_package(pybind11 CONFIG REQUIRED)

set(PYBINDIMATH_LIBRARY PyBindImath)

message(STATUS "Configuring pybindimath module and ${PYBINDIMATH_LIBRARY} library")

set(PYBINDIMATH_SOURCES
    PyBindImathFun.cpp
    PyBindImathBox.cpp
    PyBindImathVec2.cpp
    PyBindImathVec3.cpp
    PyBindImathVec4.cpp
    PyBindImathMatrix.cpp
    PyBindImathPlane.cpp
    PyBindImathLine.cpp
    PyBindImathQuat.cpp
    PyBindImathFrustum.cpp
)

set(PYBINDIMATH_HEADERS
    PyBindImathExport.h
    PyBindImath.h
)

#
# libPyBindImath library
#

if(BUILD_SHARED_LIBS)
  add_library(${PYBINDIMATH_LIBRARY} SHARED ${PYBINDIMATH_HEADERS} ${PYBINDIMATH_SOURCES})
  if(WIN32)
    # IMATH_DLL controls the export declarations for Windows
    target_compile_definitions(${PYBINDIMATH_LIBRARY} PUBLIC IMATH_DLL)

    # PYBINDIMATH_BUILD is used by PyBindImathExport.h to set
    # PYBINDIMATH_EXPORT to either dllexport or dllimport
    target_compile_definitions(${PYBINDIMATH_LIBRARY} PRIVATE PYBINDIMATH_BUILD)
  endif()
else()
  add_library(${PYBINDIMATH_LIBRARY} STATIC ${PYBINDIMATH_HEADERS} ${PYBINDIMATH_SOURCES})
endif()

add_library(Imath::PyBindImath ALIAS ${PYBINDIMATH_LIBRARY})

set_property(TARGET ${PYBINDIMATH_LIBRARY} PROPERTY PUBLIC_HEADER ${PYBINDIMATH_HEADERS})

#
# Include directories - installed to "include/PyBindImath", or under PyBindImath.framework/Headers for apple framework
#

if (IMATH_BUILD_APPLE_FRAMEWORK)
  target_include_directories(${PYBINDIMATH_LIBRARY}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/PyBindImath.framework/Headers>
  )
else()
  target_include_directories(${PYBINDIMATH_LIBRARY}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include/$PYBINDIMATH_LIBRARY>
  )
endif()

#
# Target properties
#

if (IMATH_BUILD_APPLE_FRAMEWORK)
  set_target_properties(${PYBINDIMATH_LIBRARY} PROPERTIES 
    FRAMEWORK TRUE
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "org.aswf.PyBindImath"
    MACOSX_FRAMEWORK_IDENTIFIER "org.aswf.PyBindImath"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${IMATH_LIB_VERSION}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${Imath_VERSION}"
    MACOSX_RPATH TRUE
  )
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # Don't set FRAMEWORK_VERSION for iOS
    set_target_properties(${PYBINDIMATH_LIBRARY} PROPERTIES
      FRAMEWORK_VERSION "${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}"
    )
  endif()
else()
  set_target_properties(${PYBINDIMATH_LIBRARY} PROPERTIES
    OUTPUT_NAME "${PYBINDIMATH_LIBRARY}${PYBINDIMATH_LIB_SUFFIX}"
    SOVERSION ${IMATH_LIB_SOVERSION}
    VERSION ${IMATH_LIB_VERSION}
    POSITION_INDEPENDENT_CODE ON
  )
endif()

target_compile_definitions(${PYBINDIMATH_LIBRARY} PRIVATE IMATH_EXPORTS)

target_link_libraries(${PYBINDIMATH_LIBRARY} PUBLIC Imath)
target_link_libraries(${PYBINDIMATH_LIBRARY} PRIVATE pybind11::module)

#
# Python module, identified by PYBINDIMATH_MODULE e.g. pybindimath.cpython-311-x86_64-linux-gnu.so
#

# NOTE: the target needs the "_module" suffix to distinquish it from
# the PyBindImath library target. The OUTPUT_NAME gets set below to name
# it properly

set(PYBINDIMATH_MODULE_NAME pybindimath)
set(PYBINDIMATH_MODULE ${PYBINDIMATH_MODULE_NAME}_module)

set(PYBINDIMATH_MODULE_SOURCES pybindimathmodule.cpp)

pybind11_add_module(${PYBINDIMATH_MODULE} ${PYBINDIMATH_MODULE_SOURCES})

set_target_properties(${PYBINDIMATH_MODULE} PROPERTIES
    OUTPUT_NAME ${PYBINDIMATH_MODULE_NAME} # override the _d suffix for Debug builds
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}/"
    PREFIX ""
    DEBUG_POSTFIX ""
)

target_link_libraries(${PYBINDIMATH_MODULE}
    PRIVATE
        ${PYBINDIMATH_LIBRARY}
        Imath::Imath
)

#
# Installation
#

if (IMATH_INSTALL)

  #
  # Use PYTHON_INSTALL_DIR if it's set, which allows an
  # externally-set value to take precedence
  #
  
  if (DEFINED PYTHON_INSTALL_DIR)
    set(python_destination ${PYTHON_INSTALL_DIR})
    message(STATUS "installing ${PYBINDIMATH_MODULE} to ${python_destination} (set externally)")
  else()
    if(APPLE AND IMATH_BUILD_APPLE_FRAMEWORK)
      if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        # iOS framework does not use Versions
        set(python_destination PyBindImath.framework/Resources/python)
      else()
        set(python_destination PyBindImath.framework/Versions/${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}/Resources/python)
      endif()
    else()
      set(python_destination "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
    endif()
    message(STATUS "installing ${PYBINDIMATH_MODULE} to ${python_destination}")
  endif()
  
  #
  # Install the python module 
  #
  
  install(TARGETS ${PYBINDIMATH_MODULE} LIBRARY DESTINATION ${python_destination})

  #
  # Install the library
  #

  if(APPLE AND IMATH_BUILD_APPLE_FRAMEWORK)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
      # iOS framework is does not version
      set(cmake_destination PyBindImath.framework/Resources/CMake/PyBindImath)
    else()
      # macOS framework Resources is a link to Versions
      set(cmake_destination PyBindImath.framework/Versions/${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}/Resources/CMake/PyBindImath)
    endif()
    set(includes_destination PyBindImath.framework/Headers)
    set(public_header_destination PyBindImath.framework/Headers)
  else()
    set(cmake_destination ${CMAKE_INSTALL_LIBDIR}/cmake/PyBindImath)
    set(includes_destination ${CMAKE_INSTALL_INCLUDEDIR}/PyBindImath")
    set(public_header_destination ${CMAKE_INSTALL_INCLUDEDIR}/PyBindImath")
  endif()

  install(TARGETS ${PYBINDIMATH_LIBRARY}
    EXPORT PyBindImath
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX}
      COMPONENT runtime
      OPTIONAL
    INCLUDES DESTINATION ${includes_destination}
    PUBLIC_HEADER DESTINATION ${public_header_destination}
  )

  # Install export set for downstream CMake

  install(EXPORT ${PYBINDIMATH_LIBRARY}
    DESTINATION ${cmake_destination}
    FILE ${PYBINDIMATH_LIBRARY}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    EXPORT_LINK_INTERFACE_LIBRARIES
  )
  
  if(BUILD_SHARED_LIBS
     AND (NOT "${PYBINDIMATH_LIB_SUFFIX}" STREQUAL "")
     AND IMATH_INSTALL_SYM_LINK
     AND NOT IMATH_BUILD_APPLE_FRAMEWORK)

    # create symlink from libPyBindImath_Python3_11-3_2.so to libPyBindImath.so
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(postfix ${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX})
    set(VERSIONED_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH_LIBRARY}${PYBINDIMATH_LIB_SUFFIX}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(BASE_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH_LIBRARY}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})

    file(CREATE_LINK ${VERSIONED_LIB_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${BASE_LIB_FILENAME} SYMBOLIC)
    if(WIN32)
      set(INSTALL_DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
    else()
      set(INSTALL_DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
    endif()
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BASE_LIB_FILENAME} DESTINATION ${INSTALL_DESTINATION})
    install(CODE "message(STATUS \"Creating symlink ${INSTALL_DESTINATION}/${BASE_LIB_FILENAME} -> ${VERSIONED_LIB_FILENAME}\")")

  endif()

  # pkgconfig

  if(IMATH_INSTALL_PKG_CONFIG AND NOT IMATH_BUILD_APPLE_FRAMEWORK)
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
    message(STATUS "pkg-config generation disabled for ${PYBINDIMATH_LIBRARY}")
  endif()

endif()

if (MSVC)
    target_compile_options(${PYBINDIMATH_LIBRARY} PRIVATE /bigobj)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    ${PYBINDIMATH_MODULE_SOURCES} ${PYBINDIMATH_SOURCES} ${PYBINDIMATH_HEADERS}
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
endif()

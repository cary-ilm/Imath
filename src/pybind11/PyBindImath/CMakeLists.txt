# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# The PyBindImath directory declares both the pybindimath module and the
# PyBindImath library, which is used by downstream projects
# (e.g. Alembic) that provide python wrappings based on Imath.
#

#
# Declare the libPyBindImath library, identified by PYBINDIMATH_LIBRARY
#
# The name has a "_Python<major>_<minor>-<release>" suffix,
# i.e. libPyBindImath_Python3_10-3_2.so, subject to
# PYBINDIMATH_LIB_PYTHONVER_ROOT and PYBINDIMATH_LIB_SUFFIX
#

find_package(pybind11 CONFIG REQUIRED)

set(PYBINDIMATH PyBindImath)

message(STATUS "Configuring pybindimath module and ${PYBINDIMATH} library")

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
  add_library(${PYBINDIMATH} SHARED ${PYBINDIMATH_HEADERS} ${PYBINDIMATH_SOURCES})
  if(WIN32)
    # IMATH_DLL controls the export declarations for Windows
    target_compile_definitions(${PYBINDIMATH} PUBLIC IMATH_DLL)

    # PYBINDIMATH_BUILD is used by PyBindImathExport.h to set
    # PYBINDIMATH_EXPORT to either dllexport or dllimport
    target_compile_definitions(${PYBINDIMATH} PRIVATE PYBINDIMATH_BUILD)
  endif()

  set_target_properties(${PYBINDIMATH} PROPERTIES
    SOVERSION ${IMATH_LIB_SOVERSION}
    VERSION ${IMATH_LIB_VERSION}
  )

else()
  add_library(${PYBINDIMATH} STATIC ${PYBINDIMATH_HEADERS} ${PYBINDIMATH_SOURCES})
endif()

#add_library(Imath::PyBindImath ALIAS ${PYBINDIMATH})

#
# Compile options
#

target_compile_features(${PYBINDIMATH}
    PRIVATE cxx_std_${IMATH_CXX_STANDARD}
    INTERFACE cxx_std_11
)

set_target_properties(${PYBINDIMATH} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

if (NOT IMATH_USE_DEFAULT_VISIBILITY)
    set_target_properties(${PYBINDIMATH} PROPERTIES
      C_VISIBILITY_PRESET hidden
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
    )
else()
    target_compile_definitions(${PYBINDIMATH} PUBLIC IMATH_USE_DEFAULT_VISIBILITY)
endif()

target_compile_definitions(${PYBINDIMATH} PRIVATE IMATH_EXPORTS)

set_property(TARGET ${PYBINDIMATH} PROPERTY PUBLIC_HEADER ${PYBINDIMATH_HEADERS})

# Include files are under "include/PyBindImath", or under
# PyBindImath.framework/Headers for Apple framework

if (IMATH_BUILD_APPLE_FRAMEWORK)
  target_include_directories(${PYBINDIMATH}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${PYBINDIMATH}.framework/Headers>
  )
else()
  target_include_directories(${PYBINDIMATH}
    INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PYBINDIMATH}>
  )
endif()

#
# Link options
#

target_link_libraries(${PYBINDIMATH} PUBLIC Imath)
target_link_libraries(${PYBINDIMATH} PRIVATE pybind11::module)

if (IMATH_BUILD_APPLE_FRAMEWORK)
  set_target_properties(${PYBINDIMATH} PROPERTIES 
    FRAMEWORK TRUE
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "org.aswf.${PYBINDIMATH}"
    MACOSX_FRAMEWORK_IDENTIFIER "org.aswf.${PYBINDIMATH}"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${IMATH_LIB_VERSION}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${Imath_VERSION}"
    MACOSX_RPATH TRUE
  )
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # Don't set FRAMEWORK_VERSION for iOS
    set_target_properties(${PYBINDIMATH} PROPERTIES
      FRAMEWORK_VERSION "${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}"
    )
  endif()
else()
  set_target_properties(${PYBINDIMATH} PROPERTIES
    OUTPUT_NAME "${PYBINDIMATH}${PYBINDIMATH_LIB_SUFFIX}"
  )
endif()

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
        ${PYBINDIMATH}
        Imath::Imath
)

#
# Install configuration
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
        set(python_destination ${PYBINDIMATH}.framework/Resources/python)
      else()
        set(python_destination ${PYBINDIMATH}.framework/Versions/${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}/Resources/python)
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
      set(cmake_destination ${PYBINDIMATH}.framework/Resources/CMake/${PYBINDIMATH})
    else()
      # macOS framework Resources is a link to Versions
      set(cmake_destination ${PYBINDIMATH}.framework/Versions/${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}/Resources/CMake/${PYBINDIMATH})
    endif()
    set(includes_destination ${PYBINDIMATH}.framework/Headers)
    set(public_header_destination ${PYBINDIMATH}.framework/Headers)
  else()
    set(cmake_destination ${CMAKE_INSTALL_LIBDIR}/cmake/Imath)
    set(includes_destination ${CMAKE_INSTALL_INCLUDEDIR})
    set(public_header_destination ${CMAKE_INSTALL_INCLUDEDIR}/${PYBINDIMATH})
  endif()

  install(TARGETS ${PYBINDIMATH}
    EXPORT ${PYBINDIMATH}
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

  install(EXPORT ${PYBINDIMATH}
    DESTINATION ${cmake_destination}
    FILE ${PYBINDIMATH}Targets.cmake
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
    set(VERSIONED_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH}${PYBINDIMATH_LIB_SUFFIX}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(BASE_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYBINDIMATH}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})

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
    set(pcinfile ${PYBINDIMATH}.pc.in)
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix "\${prefix}")
    set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
    set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
    string(REPLACE ".in" "" pcout ${pcinfile})
    configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    message(STATUS "PyImath pkg-config generation enabled")
  else()
    message(STATUS "PyImath pkg-config generation disabled")
  endif()

endif()

if (MSVC)
    target_compile_options(${PYBINDIMATH} PRIVATE /bigobj)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    ${PYBINDIMATH_MODULE_SOURCES} ${PYBINDIMATH_SOURCES} ${PYBINDIMATH_HEADERS}
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
endif()


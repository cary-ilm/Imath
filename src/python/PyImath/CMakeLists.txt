# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# The PyImath directory declares both the imath module and the PyImath
# library, which is used by downstream projects (e.g. Alembic) that
# provide python wrappings based on Imath.
#

#
# Declare the libPyImath library, identified by PYIMATH_LIBRARY.
# Note that PYIMATH_LIB_SUFFIX adds the "_Python3_9-3_4" to the library filename.
#

set(PYIMATH_LIBRARY PyImath)

message(STATUS "Configuring imath module and ${PYIMATH_LIBRARY} library")

set(PYIMATH_SOURCES
    PyImath.cpp
    PyImathAutovectorize.cpp
    PyImathBox2Array.cpp
    PyImathBox3Array.cpp
    PyImathBox.cpp
    PyImathBufferProtocol.cpp
    PyImathColor3.cpp
    PyImathColor4.cpp
    PyImathEuler.cpp
    PyImathFixedArray.cpp
    PyImathFrustum.cpp
    PyImathLine.cpp
    PyImathMatrix22.cpp
    PyImathMatrix33.cpp
    PyImathMatrix44.cpp
    PyImathPlane.cpp
    PyImathQuat.cpp
    PyImathRandom.cpp
    PyImathShear.cpp
    PyImathStringArray.cpp
    PyImathStringTable.cpp
    PyImathTask.cpp
    PyImathUtil.cpp
    PyImathFixedVArray.cpp
    PyImathVec2fd.cpp
    PyImathVec2si.cpp
    PyImathVec3fd.cpp
    PyImathVec3siArray.cpp
    PyImathVec3si.cpp
    PyImathVec4fd.cpp
    PyImathVec4siArray.cpp
    PyImathVec4si.cpp
)

set(PYIMATH_HEADERS
    PyImath.h
    PyImathAPI.h
    PyImathAutovectorize.h
    PyImathBasicTypes.h
    PyImathBox.h
    PyImathBoxArrayImpl.h
    PyImathBufferProtocol.h
    PyImathColor.h
    PyImathColor3ArrayImpl.h
    PyImathColor4Array2DImpl.h
    PyImathColor4ArrayImpl.h
    PyImathDecorators.h
    PyImathEuler.h
    PyImathExport.h
    PyImathFixedArray.h
    PyImathFixedArray2D.h
    PyImathFixedArrayTraits.h
    PyImathFixedMatrix.h
    PyImathFixedVArray.h
    PyImathFrustum.h
    PyImathFun.h
    PyImathLine.h
    PyImathMathExc.h
    PyImathMatrix.h
    PyImathOperators.h
    PyImathPlane.h
    PyImathQuat.h
    PyImathQuatOperators.h
    PyImathRandom.h
    PyImathShear.h
    PyImathStringArray.h
    PyImathStringArrayRegister.h
    PyImathStringTable.h
    PyImathTask.h
    PyImathUtil.h
    PyImathVec.h
    PyImathVec2Impl.h
    PyImathVec3ArrayImpl.h
    PyImathVec3Impl.h
    PyImathVec4ArrayImpl.h
    PyImathVec4Impl.h
    PyImathVecOperators.h
)

#
# libPyImath library
#

if(BUILD_SHARED_LIBS)
  add_library(${PYIMATH_LIBRARY} SHARED ${PYIMATH_HEADERS} ${PYIMATH_SOURCES})
  if(WIN32)
    # IMATH_DLL controls the export declarations for Windows
    target_compile_definitions(${PYIMATH_LIBRARY} PUBLIC IMATH_DLL)

    # PYIMATH_BUILD is used by PyImathExport.h to set
    # PYIMATH_EXPORT to either dllexport or dllimport
    target_compile_definitions(${PYIMATH_LIBRARY} PRIVATE PYIMATH_BUILD)
  endif()

  set_target_properties(${PYIMATH_LIBRARY} PROPERTIES
    SOVERSION ${IMATH_LIB_SOVERSION}
    VERSION ${IMATH_LIB_VERSION}
  )

else()
  add_library(${PYIMATH_LIBRARY} STATIC ${PYIMATH_HEADERS} ${PYIMATH_SOURCES})
endif()

add_library(Imath::PyImath ALIAS ${PYIMATH_LIBRARY})

#
# Compile options
#

target_compile_features(${PYIMATH_LIBRARY}
    PRIVATE cxx_std_${IMATH_CXX_STANDARD}
    INTERFACE cxx_std_11
)

set_target_properties(${PYIMATH_LIBRARY} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

if (NOT IMATH_USE_DEFAULT_VISIBILITY)
    set_target_properties(${PYIMATH_LIBRARY} PROPERTIES
      C_VISIBILITY_PRESET hidden
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
    )
else()
    target_compile_definitions(${PYIMATH_LIBRARY} PUBLIC IMATH_USE_DEFAULT_VISIBILITY)
endif()

target_compile_definitions(${PYIMATH_LIBRARY} PRIVATE IMATH_EXPORTS)

set_property(TARGET ${PYIMATH_LIBRARY} PROPERTY PUBLIC_HEADER ${PYIMATH_HEADERS})

# Include files are under "include/PyImath", or under
# PyImath.framework/Headers for Apple framework

if (IMATH_BUILD_APPLE_FRAMEWORK)
  target_include_directories(${PYIMATH_LIBRARY}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/PyImath.framework/Headers>
      ${Python3_INCLUDE_DIRS}
      ${Boost_INCLUDE_DIRS}
  )
else()
  target_include_directories(${PYIMATH_LIBRARY}
    INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PYIMATH_LIBRARY}>
      ${Python3_INCLUDE_DIRS}
      ${Boost_INCLUDE_DIRS}
  )
endif()

#
# Link options
#

target_link_libraries(${PYIMATH_LIBRARY} PUBLIC Imath)
target_link_libraries(${PYIMATH_LIBRARY} PRIVATE Boost::boost Boost::python Python3::Module)

if (IMATH_BUILD_APPLE_FRAMEWORK)
  set_target_properties(${PYIMATH_LIBRARY} PROPERTIES
    FRAMEWORK TRUE
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "org.aswf.PyImath"
    MACOSX_FRAMEWORK_IDENTIFIER "org.aswf.PyImath"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${IMATH_VERSION}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}"
    MACOSX_RPATH TRUE
  )
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # Don't set FRAMEWORK_VERSION for iOS
    set_target_properties(${PYIMATH_LIBRARY} PROPERTIES
      FRAMEWORK_VERSION "${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}"
    )
  endif()
else()
  set_target_properties(${PYIMATH_LIBRARY} PROPERTIES
    OUTPUT_NAME "${PYIMATH_LIBRARY}${PYIMATH_LIB_SUFFIX}"
  )
endif()

#
# Python module, identified by PYIMATH_MODULE (e.g. "imath).
# Note that CMake on Windows apparently does not respect case in
# target names, and since the target for the main Imath library is
# "Imath", the target here is "imath_module". The 
#

set(PYIMATH_MODULE_NAME imath) 
set(PYIMATH_MODULE ${PYIMATH_MODULE_NAME}_module)

set(PYIMATH_MODULE_SOURCES
    imathmodule.cpp
    PyImathFun.cpp
    PyImathBasicTypes.cpp
)

add_library(${PYIMATH_MODULE} MODULE ${PYIMATH_MODULE_SOURCES})

if(WIN32)
    # Ensure correct module extension on Windows
    set_target_properties(${PYIMATH_MODULE} PROPERTIES SUFFIX ".pyd")
endif()

set_target_properties(${PYIMATH_MODULE} PROPERTIES
    OUTPUT_NAME ${PYIMATH_MODULE_NAME} # override the _d suffix for Debug builds
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}/"
    PREFIX ""
    DEBUG_POSTFIX ""
)

target_link_libraries(${PYIMATH_MODULE} 
    PRIVATE
        ${PYIMATH_LIBRARY}
        Imath::Imath
        Boost::boost
        Boost::python
        Python3::Module
)
#
#if(APPLE AND IMATH_BUILD_APPLE_FRAMEWORK)
#  if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
#    # iOS framework does not use Versions
#    set(python_destination PyImath.framework/Resources/python)
#  else()
#    set(python_destination PyImath.framework/Versions/${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}/Resources/python)
#  endif()
#else()
#  set(python_destination "${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}")
#endif()

target_include_directories(${PYIMATH_MODULE} PRIVATE ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

#
# PyImathConfig
#

add_library(PyImathConfig INTERFACE)
target_include_directories(PyImathConfig INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
install(TARGETS PyImathConfig EXPORT ${PROJECT_NAME})
add_library(PyImath::Config ALIAS PyImathConfig)


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
    message(STATUS "installing ${PYIMATH_MODULE} to ${python_destination} (set externally)")
  else()
    if(APPLE AND IMATH_BUILD_APPLE_FRAMEWORK)
      if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        # iOS framework does not use Versions
        set(python_destination PyImath.framework/Resources/python)
      else()
        set(python_destination PyImath.framework/Versions/${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}/Resources/python)
      endif()
    else()
      set(python_destination "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
    endif()
    message(STATUS "installing ${PYIMATH_MODULE} to ${python_destination}")
  endif()

  #
  # Install the python module 
  #

  install(TARGETS ${PYIMATH_MODULE} LIBRARY DESTINATION ${python_destination})

  #
  # Install the library
  #

  if(APPLE AND IMATH_BUILD_APPLE_FRAMEWORK)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
      # iOS framework does not use Versions
      set(cmake_destination Imath.framework/Resources/CMake/Imath)
    else()
      # macOS framework Resources is a link to Versions
      set(cmake_destination Imath.framework/Versions/${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}/Resources/CMake/Imath)
    endif()
    set(includes_destination PyImath.framework/Headers)
    set(public_header_destination PyImath.framework/Headers)
  else()
    set(cmake_destination ${CMAKE_INSTALL_LIBDIR}/cmake/Imath)
    set(includes_destination ${CMAKE_INSTALL_INCLUDEDIR})
    set(public_header_destination ${CMAKE_INSTALL_INCLUDEDIR}/PyImath)
  endif()

  install(TARGETS ${PYIMATH_LIBRARY}
    EXPORT PyImath
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

  install(EXPORT ${PYIMATH_LIBRARY}
    DESTINATION ${cmake_destination}
    FILE ${PYIMATH_LIBRARY}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    EXPORT_LINK_INTERFACE_LIBRARIES
  )
  
  if(BUILD_SHARED_LIBS
     AND (NOT "${PYIMATH_LIB_SUFFIX}" STREQUAL "")
     AND IMATH_INSTALL_SYM_LINK
     AND NOT IMATH_BUILD_APPLE_FRAMEWORK)

    # create symlink from libPyImath_Python3_11-3_2.so to libPyImath.so
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(postfix ${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX})
    set(VERSIONED_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYIMATH_LIBRARY}${PYIMATH_LIB_SUFFIX}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(BASE_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PYIMATH_LIBRARY}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})

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
    set(pcinfile PyImath.pc.in)
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix "\${prefix}")
    set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
    set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
    string(REPLACE ".in" "" pcout ${pcinfile})
    configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    message(STATUS "generating ${pcout}") 
  else()
    message(STATUS "pkg-config generation disabled for PyImath")
  endif()

endif()

#
# disable the self-assign warnings:
#
#   warning: explicitly assigning value of variable of type 'boost::python::self_ns::self_t' to itself [-Wself-assign-overloaded]
#        .def(self ^= self)
#

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    ${PYIMATH_MODULE_SOURCES} ${PYIMATH_SOURCES} ${PYIMATH_HEADERS}
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_source_files_properties(
    ${PYIMATH_MODULE_SOURCES} ${PYIMATH_SOURCES} ${PYIMATH_HEADERS}
    PROPERTIES COMPILE_FLAGS "-Wdeprecated-declarations"
  )
endif()

if (MSVC)
    target_compile_options(${PYIMATH_LIBRARY} PRIVATE /bigobj)
endif()

#
# Suppress boost's pragma messages about deprecated headers:
#    include/boost/detail/iterator.hpp:13:1: note: ‘#pragma message: This header is deprecated. Use <iterator> instead.’
#    BOOST_HEADER_DEPRECATED("<iterator>")
#

target_compile_definitions(${PYIMATH_LIBRARY} PRIVATE BOOST_ALLOW_DEPRECATED_HEADERS)
target_compile_definitions(${PYIMATH_MODULE} PRIVATE BOOST_ALLOW_DEPRECATED_HEADERS)


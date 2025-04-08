# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message(STATUS "Configuring imath module")

#
# Declare the libPyImath library, identified by PYIMATH_LIBRARY
#
# The name has a "_Python3_<minor>" suffix, i.e. libPyImath_Python3_10.so
#

set(PYIMATH_LIBRARY PyImath_Python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}) 

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

add_library(${PYIMATH_LIBRARY} SHARED ${PYIMATH_SOURCES})

add_library(PyImath::PyImath ALIAS ${PYIMATH_LIBRARY})

if(BUILD_SHARED_LIBS)
    target_compile_definitions(${PYIMATH_LIBRARY} PRIVATE IMATH_EXPORTS)
    if(WIN32)
        target_compile_definitions(${PYIMATH_LIBRARY} PUBLIC IMATH_DLL)
    endif()
endif()

target_include_directories(${PYIMATH_LIBRARY} PUBLIC ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

#
# The PYIMATH_BUILD C++ macro is used by PyImathExport.h to set
# PYIMATH_EXPORT to either dllexport or dllimport
#
target_compile_definitions(${PYIMATH_LIBRARY} PRIVATE PYIMATH_BUILD)

target_link_libraries(${PYIMATH_LIBRARY} PUBLIC Imath::Imath)
target_link_libraries(${PYIMATH_LIBRARY} PRIVATE Boost::boost Boost::python Python3::Module)

target_include_directories(${PYIMATH_LIBRARY}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/${PYIMATH_LIBRARY}>
)
set_target_properties(${PYIMATH_LIBRARY} PROPERTIES
    SOVERSION ${IMATH_LIB_SOVERSION}
    VERSION ${IMATH_LIB_VERSION}
    POSITION_INDEPENDENT_CODE ON
)

#
# Declare the python module, identified by PYIMATH_MODULE
#

set(PYIMATH_MODULE imath) 

set(PYIMATH_MODULE_SOURCES
    imathmodule.cpp
    PyImathFun.cpp
    PyImathBasicTypes.cpp
)

add_library(${PYIMATH_MODULE} MODULE ${PYIMATH_MODULE_SOURCES})

# Ensure correct module extension on Windows
if(WIN32)
    set_target_properties(${PYIMATH_MODULE} PROPERTIES SUFFIX ".pyd")
endif()

set_target_properties(${PYIMATH_MODULE} PROPERTIES PREFIX "")
target_include_directories(${PYIMATH_MODULE} PRIVATE ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(${PYIMATH_MODULE} 
    PRIVATE
        ${PYIMATH_LIBRARY}
        Boost::boost
        Boost::python
        Python3::Module
        Imath::Imath
)

add_library(PyImathConfig INTERFACE)
target_include_directories(PyImathConfig INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
install(TARGETS PyImathConfig EXPORT ${PROJECT_NAME})
add_library(PyImath::Config ALIAS PyImathConfig)

if(IMATH_INSTALL_PKG_CONFIG)
  # use a helper function to avoid variable pollution, but pretty simple
  function(pyimath_pkg_config_help pcinfile)
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix "\${prefix}")
    set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
    set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(LIB_SUFFIX_DASH ${IMATH_LIB_SUFFIX}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX})
    string(REPLACE ".in" "" pcout ${pcinfile})
    configure_file(${pcinfile} ${CMAKE_CURRENT_BINARY_DIR}/${pcout} @ONLY)
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${pcout}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
  endfunction()
  pyimath_pkg_config_help(PyImath.pc.in)
else()
  message(STATUS "pkg-config generation disabled")
endif()


#
# Installation
#

if (IMATH_INSTALL)

  include(GNUInstallDirs)

  if(SKBUILD)

    #
    # scikit-build, for the python wheel. Ignore the library, we only
    # need the module. 
    #
    
    message(STATUS "setting PYTHON_INSTALL_DIR to skbuild: ${SKBUILD_PLATLIB_DIR}")
    set(PYTHON_INSTALL_DIR ${SKBUILD_PLATLIB_DIR})
    set(CMAKE_BINARY_DIR ${SKBUILD_PLATLIB_DIR})

    install(TARGETS ${PYIMATH_MODULE} LIBRARY DESTINATION ${PYTHON_INSTALL_DIR})

  else()

    #
    # Set the PYTHON_INSTALL_DIR but only if it's not already set,
    # which allows an externally-set value to take effect.
    #
    
    if (NOT DEFINED PYTHON_INSTALL_DIR)

      set(PYTHON_INSTALL_DIR "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
      message(STATUS "installing ${PYIMATH_MODULE} module to ${PYTHON_INSTALL_DIR}")

    else()

      message(STATUS "installing ${PYIMATH_MODULE} module to ${PYTHON_INSTALL_DIR} (set externally)")
  
    endif()
  
    #
    # Install the python module 
    #

    install(TARGETS ${PYIMATH_MODULE} LIBRARY DESTINATION ${PYTHON_INSTALL_DIR})

    #
    # Install the library
    #
  
    install(TARGETS ${PYIMATH_LIBRARY}
      EXPORT "${PYIMATH_LIBRARY}Targets"
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install headers

    install(FILES ${PYIMATH_HEADERS}
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PYIMATH_LIBRARY})

    # Install export set for downstream CMake

    install(EXPORT ${PYIMATH_LIBRARY}Targets
      FILE ${PYIMATH_LIBRARY}Targets.cmake
      NAMESPACE ${PYIMATH_LIBRARY}::
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PYIMATH_LIBRARY}
    )

    # Config file for find_package()
    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
      "${CMAKE_CURRENT_BINARY_DIR}/${PYIMATH_LIBRARY}ConfigVersion.cmake"
      VERSION 1.0.0
      COMPATIBILITY AnyNewerVersion
    )

    install(FILES
      "${CMAKE_CURRENT_BINARY_DIR}/${PYIMATH_LIBRARY}ConfigVersion.cmake"
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PYIMATH_LIBRARY}
    )
  endif()

endif()

#
# disable the self-assig warnings:
#
#   warning: explicitly assigning value of variable of type 'boost::python::self_ns::self_t' to itself [-Wself-assign-overloaded]
#        .def(self ^= self)
#

message (STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    ${PYIMATH_MODULE_SOURCES} ${PYIMATH_SOURCES} ${PYIMATH_HEADERS}
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
endif()


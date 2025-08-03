# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# Imath
#

set(IMATH_LIBRARY Imath)

set(IMATH_SOURCES
    half.cpp
    ImathColorAlgo.cpp
    ImathFun.cpp
    ImathMatrixAlgo.cpp
    ImathRandom.cpp
    toFloat.h
)

set(IMATH_HEADERS
    half.h
    halfFunction.h
    halfLimits.h
    ImathBox.h
    ImathBoxAlgo.h
    ImathColor.h
    ImathColorAlgo.h
    ImathEuler.h
    ImathExport.h
    ImathForward.h
    ImathFrame.h
    ImathFrustum.h
    ImathFrustumTest.h
    ImathFun.h
    ImathGL.h
    ImathGLU.h
    ImathInt64.h
    ImathInterval.h
    ImathLine.h
    ImathLineAlgo.h
    ImathMath.h
    ImathMatrix.h
    ImathMatrixAlgo.h
    ImathNamespace.h
    ImathPlane.h
    ImathPlatform.h
    ImathQuat.h
    ImathRandom.h
    ImathRoots.h
    ImathShear.h
    ImathSphere.h
    ImathTypeTraits.h
    ImathVec.h
    ImathVecAlgo.h
    ${IMATH_CONFIG_DIR}/ImathConfig.h
)

#
# libImath library
#

if(BUILD_SHARED_LIBS)

    add_library(${IMATH_LIBRARY} SHARED ${IMATH_HEADERS} ${IMATH_SOURCES})

    target_compile_definitions(${IMATH_LIBRARY} PRIVATE IMATH_EXPORTS)
    if(WIN32)
      target_compile_definitions(${IMATH_LIBRARY} PUBLIC IMATH_DLL)
    endif()

    set_target_properties(${IMATH_LIBRARY} PROPERTIES
      SOVERSION ${IMATH_LIB_SOVERSION}
      VERSION ${IMATH_LIB_VERSION}
    )

else()

    add_library(${IMATH_LIBRARY} STATIC ${IMATH_HEADERS} ${IMATH_SOURCES})

endif()

add_library(Imath::Imath ALIAS ${IMATH_LIBRARY})

#
# Compile options
#

target_compile_features(${IMATH_LIBRARY}
    PRIVATE cxx_std_${IMATH_CXX_STANDARD}
    INTERFACE cxx_std_11
)

set_target_properties(${IMATH_LIBRARY} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

if (NOT IMATH_USE_DEFAULT_VISIBILITY)
    set_target_properties(${IMATH_LIBRARY} PROPERTIES
      C_VISIBILITY_PRESET hidden
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
    )
else()
    target_compile_definitions(${IMATH_LIBRARY} PUBLIC IMATH_USE_DEFAULT_VISIBILITY)
endif()

if (MSVC)
    target_compile_options(${IMATH_LIBRARY} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/EHsc>")
endif()

set_property(TARGET ${IMATH_LIBRARY} PROPERTY PUBLIC_HEADER ${IMATH_HEADERS})

# Include files are under "include/Imath", or under Imath.framework/Headers for Apple framework
if (IMATH_BUILD_APPLE_FRAMEWORK)
    target_include_directories(${IMATH_LIBRARY}
       PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/Imath.framework/Headers>
    )
else()
    target_include_directories(${IMATH_LIBRARY}
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}>
    )
endif()

#
# Link options
#

target_link_libraries(${IMATH_LIBRARY} PUBLIC ImathConfig)

include(CheckLibraryExists)
check_library_exists(m sin "" HAVE_LIB_M)
if (HAVE_LIB_M)
    target_link_libraries(${IMATH_LIBRARY} PUBLIC m)
endif()

if (IMATH_BUILD_APPLE_FRAMEWORK)
  set_target_properties(${IMATH_LIBRARY} PROPERTIES
    FRAMEWORK TRUE
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "org.aswf.Imath"
    MACOSX_FRAMEWORK_IDENTIFIER "org.aswf.Imath"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${IMATH_VERSION}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}"
    MACOSX_RPATH TRUE
    RESOURCE "${IMATH_CMAKE_CONFIG_BUILD_DIR}/ImathConfig.cmake;${IMATH_CMAKE_CONFIG_BUILD_DIR}/ImathConfigVersion.cmake"
  )
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # Don't set FRAMEWORK_VERSION for iOS, since iOS doesn't version headers
    set_target_properties(${IMATH_LIBRARY} PROPERTIES
      FRAMEWORK_VERSION "${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}"
    )
  endif()
else()
  set_target_properties(${IMATH_LIBRARY} PROPERTIES OUTPUT_NAME "Imath${IMATH_LIB_SUFFIX}" )
endif()
  
#
# Install configuration
#

if (IMATH_INSTALL)

  if(APPLE AND IMATH_BUILD_APPLE_FRAMEWORK)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
      # iOS framework is does not version
      set(cmake_destination Imath.framework/Resources/CMake/Imath)
    else()
      # macOS framework Resources is a link to Versions
      set(cmake_destination Imath.framework/Versions/${Imath_VERSION_MAJOR}.${Imath_VERSION_MINOR}/Resources/CMake/Imath)
    endif()
    set(includes_destination Imath.framework/Headers)
    set(public_header_destination Imath.framework/Headers)
  else()
    set(cmake_destination ${CMAKE_INSTALL_LIBDIR}/cmake/Imath)
    set(includes_destination ${CMAKE_INSTALL_INCLUDEDIR}/Imath)
    set(public_header_destination ${CMAKE_INSTALL_INCLUDEDIR}/Imath)
  endif()

  install(TARGETS ${IMATH_LIBRARY}
    EXPORT Imath
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX}
      COMPONENT runtime
      OPTIONAL
    INCLUDES DESTINATION ${includes_destination}
    PUBLIC_HEADER DESTINATION ${public_header_destination}
  )

  install(EXPORT Imath
    DESTINATION ${cmake_destination}
    FILE ImathTargets.cmake
    NAMESPACE Imath::
    EXPORT_LINK_INTERFACE_LIBRARIES
  )

  if(BUILD_SHARED_LIBS
     AND (NOT "${IMATH_LIB_SUFFIX}" STREQUAL "")
     AND IMATH_INSTALL_SYM_LINK
     AND NOT IMATH_BUILD_APPLE_FRAMEWORK)

    # create symlink from libImath.so to libImath-3_2.so

    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    set(postfix ${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX})
    set(VERSIONED_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${IMATH_LIBRARY}${IMATH_LIB_SUFFIX}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(BASE_LIB_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${IMATH_LIBRARY}${postfix}${CMAKE_SHARED_LIBRARY_SUFFIX})

    file(CREATE_LINK ${VERSIONED_LIB_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${BASE_LIB_FILENAME} SYMBOLIC)
    if(WIN32)
      set(INSTALL_DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
    else()
      set(INSTALL_DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
    endif()
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BASE_LIB_FILENAME} DESTINATION ${INSTALL_DESTINATION})
    install(CODE "message(STATUS \"Creating symlink ${INSTALL_DESTINATION}/${BASE_LIB_FILENAME} -> ${VERSIONED_LIB_FILENAME}\")")

  endif()

endif()


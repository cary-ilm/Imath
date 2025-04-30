# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# Imath
#

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
    ${CMAKE_BINARY_DIR}/config/ImathConfig.h
)

add_library(Imath ${IMATH_HEADERS} ${IMATH_SOURCES})

target_compile_features(Imath
    PRIVATE cxx_std_${IMATH_CXX_STANDARD}
    INTERFACE cxx_std_11
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(Imath PRIVATE IMATH_EXPORTS)
    if(WIN32)
      target_compile_definitions(Imath PUBLIC IMATH_DLL)
    endif()
endif()

target_include_directories(Imath INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(Imath PUBLIC ImathConfig)

set_target_properties(Imath PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

if (NOT IMATH_USE_DEFAULT_VISIBILITY)
    set_target_properties(Imath PROPERTIES
      C_VISIBILITY_PRESET hidden
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
    )
else()
    target_compile_definitions(Imath PUBLIC IMATH_USE_DEFAULT_VISIBILITY)
endif()

if (MSVC)
    target_compile_options(Imath PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/EHsc>")
endif()

set_property(TARGET Imath PROPERTY PUBLIC_HEADER ${IMATH_HEADERS})

if(BUILD_SHARED_LIBS)
    set_target_properties(Imath PROPERTIES
      SOVERSION ${IMATH_LIB_SOVERSION}
      VERSION ${IMATH_LIB_VERSION}
    )
endif()

include(CheckLibraryExists)
check_library_exists(m sin "" HAVE_LIB_M)
if (HAVE_LIB_M)
    target_link_libraries(Imath PUBLIC m)
endif()

if (NOT IMATH_BUILD_APPLE_FRAMEWORKS)
    set_target_properties(Imath PROPERTIES OUTPUT_NAME "Imath${IMATH_LIB_SUFFIX}" )
endif()

add_library(Imath::Imath ALIAS Imath)


if (IMATH_BUILD_APPLE_FRAMEWORKS)
    # Mark files as resources
    set_source_files_properties(${IMATH_RESOURCE_FILES} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
    set_target_properties(Imath PROPERTIES 
      FRAMEWORK TRUE
      FRAMEWORK_VERSION "${IMATH_LIB_VERSION}"
      XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "github.com/AcademySoftwareFoundation/Imath"
      MACOSX_FRAMEWORK_IDENTIFIER "github.com/AcademySoftwareFoundation/Imath"
      MACOSX_FRAMEWORK_BUNDLE_VERSION "${IMATH_LIB_VERSION}"
      MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${Imath_VERSION}"
      MACOSX_RPATH TRUE)

    # Copy resource files to the build directory's framework Resources/
    set(RES_DEST_DIR "$<TARGET_FILE_DIR:Imath>/Resources")
    add_custom_command(TARGET Imath POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "${RES_DEST_DIR}"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${IMATH_RESOURCE_FILES}
              "${CMAKE_BINARY_DIR}/config/ImathTargets.cmake"
              "${RES_DEST_DIR}/"
      COMMENT "Copying resource files to Imath.framework/Resources in build directory"
    )
endif()
  
if (IMATH_INSTALL)

    install(TARGETS Imath
      EXPORT Imath
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT runtime
        OPTIONAL
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}
    )

    if(BUILD_SHARED_LIBS AND (NOT "${IMATH_LIB_SUFFIX}" STREQUAL "")
    AND IMATH_INSTALL_SYM_LINK AND NOT IMATH_BUILD_APPLE_FRAMEWORKS)
      string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
      set(verlibname ${CMAKE_SHARED_LIBRARY_PREFIX}Imath${IMATH_LIB_SUFFIX}${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
      set(baselibname ${CMAKE_SHARED_LIBRARY_PREFIX}Imath${CMAKE_${uppercase_CMAKE_BUILD_TYPE}_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
      file(CREATE_LINK ${verlibname} ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} SYMBOLIC)
      if(WIN32)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
        install(CODE "message(STATUS \"Creating symlink ${CMAKE_INSTALL_FULL_BINDIR}/${baselibname} -> ${verlibname}\")")
      else()
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${baselibname} DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
        install(CODE "message(STATUS \"Creating symlink ${CMAKE_INSTALL_FULL_LIBDIR}/${baselibname} -> ${verlibname}\")")
      endif()
      set(verlibname)
      set(baselibname)
    endif()

endif()










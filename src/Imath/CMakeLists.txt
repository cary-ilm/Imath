# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

#
# Imath (header-only)
#

set(IMATH_LIBRARY Imath)

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
    toFloat.h
    ${IMATH_CONFIG_DIR}/ImathConfig.h
)

if(IMATH_BUILD_APPLE_FRAMEWORKS)
    message(FATAL_ERROR
        "IMATH_BUILD_APPLE_FRAMEWORKS is not supported for header-only Imath")
endif()

add_library(${IMATH_LIBRARY} INTERFACE)
add_library(Imath::Imath ALIAS ${IMATH_LIBRARY})

target_compile_features(${IMATH_LIBRARY} INTERFACE cxx_std_17)

target_include_directories(${IMATH_LIBRARY} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}>)

target_link_libraries(${IMATH_LIBRARY} INTERFACE ImathConfig)

include(CheckLibraryExists)
check_library_exists(m sin "" HAVE_LIB_M)
if(HAVE_LIB_M)
    target_link_libraries(${IMATH_LIBRARY} INTERFACE m)
endif()

#
# Install configuration
#

if(IMATH_INSTALL)

    install(TARGETS ${IMATH_LIBRARY} EXPORT Imath)

    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR}
        FILES_MATCHING PATTERN "*.h")

    install(
        FILES ${IMATH_CONFIG_DIR}/ImathConfig.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${IMATH_OUTPUT_SUBDIR})

endif()

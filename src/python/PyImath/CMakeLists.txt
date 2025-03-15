# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message(STATUS "Configuring imath module")

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

if(Python_FOUND)
    message(STATUS "Found Python ${Python_VERSION}")
    message(STATUS "Python_INCLUDE_DIRS: ${Python_INCLUDE_DIRS}")
    message(STATUS "Python_LIBRARY: ${Python_LIBRARY}")
else()
    message(FATAL_ERROR "Could not find Python")
endif()

find_package(Boost COMPONENTS python REQUIRED)

if(Boost_FOUND)
    message(STATUS "Found Boost includes: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Found Boost libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Could not find Boost")
endif()

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "variable ${_variableName}=${${_variableName}}")
#endforeach()

set(PYIMATH_LIBRARY PyImath_Python${Python_VERSION_MAJOR}_${Python_VERSION_MINOR}) # libPyImath_Python3_10.so
set(PYIMATH_MODULE imath) # import imath

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

if(BUILD_SHARED_LIBS)
    target_compile_definitions(${PYIMATH_LIBRARY} PRIVATE IMATH_EXPORTS)
    if(WIN32)
        target_compile_definitions(${PYIMATH_LIBRARY} PUBLIC IMATH_DLL)
    endif()
endif()

target_include_directories(${PYIMATH_LIBRARY} PRIVATE ${Python_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(${PYIMATH_LIBRARY}
    PRIVATE
        Boost::boost
        Boost::python
        Python::Module
        Imath::Imath
)

set_target_properties(${PYIMATH_LIBRARY} PROPERTIES
    SOVERSION ${IMATH_LIB_SOVERSION}
    VERSION ${IMATH_LIB_VERSION}
    POSITION_INDEPENDENT_CODE ON
)

add_library(${PYIMATH_MODULE} MODULE
    imathmodule.cpp
#    PyImathFun.cpp
#    PyImathBasicTypes.cpp
    $<TARGET_OBJECTS:${PYIMATH_LIBRARY}>
)

set_target_properties(${PYIMATH_MODULE} PROPERTIES PREFIX "")
target_include_directories(${PYIMATH_MODULE} PRIVATE ${Python_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(${PYIMATH_MODULE} PRIVATE
    PRIVATE
        Boost::boost
        Boost::python
        Python::Module
        Imath::Imath
)

set(PYIMATH_SOURCES2
)

set(PYIMATH_HEADERS2
)

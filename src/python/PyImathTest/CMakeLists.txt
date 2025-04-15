# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

set(PYIMATH_TEST_SOURCES main.cpp testStringTable.cpp)

if("${CMAKE_PROJECT_NAME}" STREQUAL "")

  message(STATUS "Building PyImathTest as standalone project")

  cmake_minimum_required(VERSION 3.12)
  project(PyImathTest)
  find_package(Imath)

  add_executable(PyImathTestC ${PYIMATH_TEST_SOURCES})

  target_link_libraries(PyImathTestC 
      Imath::Config
      PyImath::PyImath
      Boost::boost
      Boost::python
      Python3::Python
  )

else()

  message (STATUS "Configuring PyImathTest")

  add_executable(PyImathTestC ${PYIMATH_TEST_SOURCES})

  add_test(PyImath.PyImathTest
    ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathTest.in
  )

  set_tests_properties(PyImath.PyImathTest PROPERTIES
    ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}"
  )

  target_link_libraries(PyImathTestC 
      Imath::Config
      PyImath::PyImath
      Boost::boost
      Boost::python
      Python3::Python
  )


  add_test(NAME PyImath.PyImathTestC COMMAND $<TARGET_FILE:PyImathTestC>)
  set(PYTHONPATH ${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR})
  set_tests_properties(PyImath.PyImathTestC PROPERTIES ENVIRONMENT PYTHONPATH=${PYTHONPATH})

endif()

target_include_directories(PyImathTestC PUBLIC ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

set_target_properties(PyImathTestC PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#
# disable the self-assig warnings:
#
#   warning: explicitly assigning value of variable of type 'boost::python::self_ns::self_t' to itself [-Wself-assign-overloaded]
#        .def(self ^= self)
#

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    ${PYIMATH_TEST_SOURCES} 
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_source_files_properties(
    ${PYIMATH_TEST_SOURCES} 
    PROPERTIES COMPILE_FLAGS "-Wdeprecated-declarations"
  )
endif()


  

  

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

  set(PYIMATH_MODULE imath)
  set(PYIMATH_LIBRARY PyImath_Python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}${IMATH_LIB_SUFFIX}) 
  add_test(NAME PyImath.PyImathTest
    COMMAND ${CMAKE_COMMAND} -E env
            "PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATH_MODULE}>"
            "PATH=$<TARGET_FILE_DIR:${PYIMATH_LIBRARY}>;$ENV{PATH}"
            ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathTest.in
  )

  target_link_libraries(PyImathTestC 
      Imath::Config
      PyImath::PyImath
      Boost::boost
      Boost::python
      Python3::Python
  )


  add_test(NAME PyImath.PyImathTestC COMMAND $<TARGET_FILE:PyImathTestC>)
  set_tests_properties(PyImath.PyImathTestC PROPERTIES ENVIRONMENT PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATH_MODULE}>)

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


  

  

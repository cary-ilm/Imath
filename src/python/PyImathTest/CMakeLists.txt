# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

set(PYIMATH_TEST_SOURCES main.cpp testStringTable.cpp)

if("${CMAKE_PROJECT_NAME}" STREQUAL "")

  #
  # This allows the PyImathTest project to serve as an example of a
  # project using Imath as an extern dependency, used in the CI workflow
  # to validate that the project exports the proper CMake config.
  #
  # Treat the test source and executable as a project relying on Imath
  # as a pre-built library
  #

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

  # Test of the python API.  Execute the test by invoking python
  # with the script as an argument, with PATH and PYTHONPATH set properly.
  
  add_test(NAME PyImath.PyImathTest
    COMMAND ${CMAKE_COMMAND} -E env
            "PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATH_MODULE}>"
            "PATH=$<TARGET_FILE_DIR:${PYIMATH_LIBRARY}>:$ENV{PATH}"
            ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathTest.py
  )

  target_link_libraries(PyImathTestC 
      Imath::Config
      PyImath::PyImath
      Boost::boost
      Boost::python
      Python3::Python
  )

  # Test of the exported PyImath C++ API

  add_test(NAME PyImath.PyImathTestC COMMAND $<TARGET_FILE:PyImathTestC>)
  set_tests_properties(PyImath.PyImathTestC PROPERTIES ENVIRONMENT PYTHONPATH=$<TARGET_FILE_DIR:${PYIMATH_MODULE}>)

  target_include_directories(PyImathTestC PUBLIC ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

  set_target_properties(PyImathTestC PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

endif()

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


  

  

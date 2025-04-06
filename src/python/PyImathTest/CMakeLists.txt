# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message(STATUS "CMAKE_PROJECT_NAME=${CMAKE_PROJECT_NAME}")

if("${CMAKE_PROJECT_NAME}" STREQUAL "")

  message(STATUS "Building PyImathTest as standalone project")

  cmake_minimum_required(VERSION 3.12)
  project(PyImathTest)
  find_package(Imath)

  find_package(Python REQUIRED COMPONENTS Interpreter Development)
  if(NOT Python_FOUND)
      message(FATAL_ERROR "Could not find Python")
  endif()

  find_package(Boost REQUIRED COMPONENTS python)
  if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost")
  endif()

  add_executable(PyImathTestC main.cpp testStringTable.cpp)

  target_link_libraries(PyImathTestC
    Imath::PyImath_Python${Python_VERSION_MAJOR}_${Python_VERSION_MINOR}
    Boost::python
    Boost::boost
    Python::Python
  )

else()

  add_executable(PyImathTestC main.cpp testStringTable.cpp)

  add_test(PyImath.PyImathTest_Python3
    ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pyImathTest.in
  )
  set_tests_properties(PyImath.PyImathTest_Python3 PROPERTIES
    ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}"
  )

  target_link_libraries(PyImathTestC 
      Imath::Config
      Imath::PyImath_Python${Python_VERSION_MAJOR}_${Python_VERSION_MINOR}
      Python3::Python
      Boost::${PYIMATH_BOOST_PY_COMPONENT}
  )


  add_test(NAME Imath.PyImathTestC COMMAND $<TARGET_FILE:PyImathTestC>)
  set(PYTHONPATH ${CMAKE_BINARY_DIR}/python${Python_VERSION_MAJOR}_${Python_VERSION_MINOR})
  set_tests_properties(Imath.PyImathTestC PROPERTIES ENVIRONMENT PYTHONPATH=${PYTHONPATH})

endif()

target_include_directories(PyImathTestC PUBLIC ${Python_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

set_target_properties(PyImathTestC PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

  

  

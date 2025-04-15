# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

message(STATUS "Configuring imathnumpy module")

set(PYIMATHNUMPY_MODULE imathnumpy)

add_library(${PYIMATHNUMPY_MODULE} MODULE imathnumpymodule.cpp)

target_link_libraries(${PYIMATHNUMPY_MODULE}
      Imath::Config
      PyImath::PyImath
      Boost::boost
      Boost::python
      Python3::Module
)

set_target_properties(${PYIMATHNUMPY_MODULE} PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python${Python3_VERSION_MAJOR}_${Python3_VERSION_MINOR}/"
)

execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_include_directories(${PYIMATHNUMPY_MODULE}
  PUBLIC ${NUMPY_INCLUDE_DIR} ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

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

    install(TARGETS ${PYIMATHNUMPY_MODULE} LIBRARY DESTINATION ${PYTHON_INSTALL_DIR})

  else()

    #
    # Set the PYTHON_INSTALL_DIR but only if it's not already set,
    # which allows an externally-set value to take effect.
    #
    
    if (NOT DEFINED PYTHON_INSTALL_DIR)

      set(PYTHON_INSTALL_DIR "lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")
      message(STATUS "installing ${PYIMATHNUMPY_MODULE} module to ${PYTHON_INSTALL_DIR}")

    else()

      message(STATUS "installing ${PYIMATHNUMPY_MODULE} module to ${PYTHON_INSTALL_DIR} (set externally)")
  
    endif()
  
    install(TARGETS ${PYIMATHNUMPY_MODULE} LIBRARY DESTINATION ${PYTHON_INSTALL_DIR})

  endif()

endif()

#
# disable the self-assig warnings:
#
#   warning: explicitly assigning value of variable of type 'boost::python::self_ns::self_t' to itself [-Wself-assign-overloaded]
#        .def(self ^= self)
#

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set_source_files_properties(
    imathnumpymodule.cpp
    PROPERTIES COMPILE_FLAGS "-Wno-self-assign-overloaded"
  )
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_source_files_properties(
    imathnumpymodule.cpp
    PROPERTIES COMPILE_FLAGS "-Wdeprecated-declarations"
  )
endif()

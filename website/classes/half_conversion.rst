..
  SPDX-License-Identifier: BSD-3-Clause
  Copyright Contributors to the OpenEXR Project.

.. _half-float-conversion-configuration-options:

Build-time half-float Conversion Configuration Options
######################################################

The Imath library supports three options for conversion between 16-bit
half and 32-bit float:

1. Conversion from half to float via a 16-bit lookup table. Prior to
   Imath v3.1, this was the only method supported.

2. F16C SSE instructions: single-instruction conversion for machine
   architectures that support it. When available, this is the fastest
   option, by far.

3. Bit-shift conversion algorithm.

To use the F16C SSE instruction set on an architecture that supports
it, simply provide the appropriate compiler flags when building an
application that includes ``half.h``. For g++ and clang,
for example:
::

    $ cmake -DCMAKE_CXX_FLAGS="-m16fc" <source directory> 
    
When code including ``half.h`` is compiled with F16C enabled, it will
automatically perform conversions using the instruction set. F16C
compiler flags take precedence over other lookup-table-related Imath
CMake settings.

On architectures that do not support F16C, you may choose at
compile-time between the bit-shift conversion and lookup table
conversion via the ``IMATH_HALF_USE_LOOKUP_TABLE`` CMake option:
::

    $ cmake -DIMATH_HALF_USE_LOOKUP_TABLE=OFF <source directory>

When ``IMATH_HALF_USE_LOOKUP_TABLE`` is off, the lookup table is not
compiled into the Imath library and the ``IMATH_HALF_USE_LOOKUP_TABLE``
preprocessor symbol is not defined in ``ImathConfig.h``. Half-to-float
conversion then uses F16C instructions (if available) or the bit-shift
algorithm.

Furthermore, an implementation wishing to receive ``FE_OVERFLOW`` and
``FE_UNDERFLOW`` floating point exceptions when converting float to
half by the bit-shift algorithm can define the preprocessor symbol
``IMATH_HALF_ENABLE_FP_EXCEPTIONS`` prior to including ``half.h``:
::
   
    #define IMATH_HALF_ENABLE_FP_EXCEPTIONS

By default, no exceptions are raised on overflow and underflow.







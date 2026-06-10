//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// Lookup table for half-to-float conversion.
// Compiled only when IMATH_HALF_USE_LOOKUP_TABLE is enabled.
//

#include "half.h"

#if defined(IMATH_DLL)
#    define EXPORT_CONST __declspec(dllexport)
#else
#    define EXPORT_CONST
#endif

// clang-format off
const imath_half_uif_t imath_half_to_float_table_data[1 << 16] =
#include "toFloat.h"
// clang-format on

extern "C" {
EXPORT_CONST const imath_half_uif_t* imath_half_to_float_table =
    imath_half_to_float_table_data;
} // extern "C"

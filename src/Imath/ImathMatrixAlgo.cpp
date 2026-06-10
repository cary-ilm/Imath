//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

///
/// @file  ImathMatrixAlgo.cpp
///
/// @brief Implementation of non-template items declared in ImathMatrixAlgo.h
///

#include "ImathMatrixAlgo.h"

#if defined(IMATH_DLL)
#    define EXPORT_CONST __declspec(dllexport)
#else
#    define EXPORT_CONST const
#endif

IMATH_INTERNAL_NAMESPACE_SOURCE_ENTER

EXPORT_CONST M22f identity22f ( 1, 0,
				0, 1);
EXPORT_CONST M22d identity22d ( 1, 0,
				0, 1);
EXPORT_CONST M33f identity33f ( 1, 0, 0,
				0, 1, 0,
				0, 0, 1);
EXPORT_CONST M33d identity33d ( 1, 0, 0,
				0, 1, 0,
				0, 0, 1);
EXPORT_CONST M44f identity44f ( 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);
EXPORT_CONST M44d identity44d ( 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);

IMATH_INTERNAL_NAMESPACE_SOURCE_EXIT

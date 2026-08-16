//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// Regression test for AcademySoftwareFoundation/openexr#2101.
//
// OpenEXR's ImfBoxAttribute.cpp forward-declares Vec2/Vec3/Box (tagged
// with the export/type-visibility attribute macro) *before* including
// <Imath/ImathBox.h>:
//
//   template <class V> class IMF_EXPORT_TEMPLATE_TYPE Vec2;
//   template <class V> class IMF_EXPORT_TEMPLATE_TYPE Vec3;
//   template <class V> class IMF_EXPORT_TEMPLATE_TYPE Box;
//   #include <ImathBox.h>
//
// This is the reverse of the order ImathForward.h and Imath's own
// tests use (forward declarations only appear *after* the real
// headers, guarded by include-guards, so they are usually skipped).
// On MSYS2 CLANG64 (clang targeting the MinGW/PE ABI) this ordering
// caused clang to implicitly instantiate Vec2<short> etc. at the
// point of the early forward declaration, so the explicit
// specializations (the `= delete` overloads) further down in
// ImathVec.h were then rejected as "explicit specialization after
// instantiation". This is reproduced here by replicating that same
// forward-declare-before-include pattern.
//

#include "ImathExport.h"
#include "ImathNamespace.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER
template <class V> class IMATH_EXPORT_TEMPLATE_TYPE Vec2;
template <class V> class IMATH_EXPORT_TEMPLATE_TYPE Vec3;
template <class V> class IMATH_EXPORT_TEMPLATE_TYPE Box;
IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#include <ImathBox.h>

#include "testForwardDeclOrder.h"

#include <cassert>
#include <iostream>

void
testForwardDeclOrder ()
{
    std::cout << "Testing forward declaration order (openexr#2101)" << std::endl;

    IMATH_INTERNAL_NAMESPACE::Box2s box (
        IMATH_INTERNAL_NAMESPACE::V2s (0, 0), IMATH_INTERNAL_NAMESPACE::V2s (1, 1));
    assert (!box.isEmpty ());

    std::cout << "ok\n" << std::endl;
}

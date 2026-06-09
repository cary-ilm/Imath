//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include "testVec.h"
#include <ImathFun.h>
#include <ImathTypeTraits.h>
#include <ImathVec.h>
#include <cassert>
#include <cmath>
#include <half.h>
#include <iostream>
#include <limits>
#include <type_traits>

// Include ImathForward *after* other headers to validate forward declarations
#include <ImathForward.h>

using namespace std;
using namespace IMATH_INTERNAL_NAMESPACE;

namespace
{

template <class T>
void
testLength2T ()
{
    const T s = std::sqrt (std::numeric_limits<T>::min ());
    const T e = 4 * std::numeric_limits<T>::epsilon ();

    Vec2<T> v;

    v = Vec2<T> (0, 0);
    assert (v.length () == 0);
    assert (v.normalized ().length () == 0);

    v = Vec2<T> (3, 4);
    assert (v.length () == 5);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec2<T> (3000, 4000);
    assert (v.length () == 5000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    T t = s * (1 << 4);

    v = Vec2<T> (t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (-t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (2), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 4);

    v = Vec2<T> (t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (-t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (2), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 20);

    v = Vec2<T> (t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec2<T> (-t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (2), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
}

template <class T>
void
testLength3T ()
{
    const T s = std::sqrt (std::numeric_limits<T>::min ());
    const T e = 4 * std::numeric_limits<T>::epsilon ();

    Vec3<T> v;

    v = Vec3<T> (0, 0, 0);
    assert (v.length () == 0);
    assert (v.normalized ().length () == 0);

    v = Vec3<T> (3, 4, 0);
    assert (v.length () == 5);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec3<T> (3000, 4000, 0);
    assert (v.length () == 5000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec3<T> (1, -1, 1);
    assert (
        IMATH_INTERNAL_NAMESPACE::equal (v.length (), 1 * std::sqrt (3), e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec3<T> (1000, -1000, 1000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), 1000 * std::sqrt (3), 1000 * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    T t = s * (1 << 4);

    v = Vec3<T> (t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (-t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (3), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 4);

    v = Vec3<T> (t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (-t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (3), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 20);

    v = Vec3<T> (t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec3<T> (-t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (
        v.length (), t * std::sqrt (3), t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
}

template <class T>
void
testLength4T ()
{
    const T s = std::sqrt (std::numeric_limits<T>::min ());
    const T e = 4 * std::numeric_limits<T>::epsilon ();

    Vec4<T> v;

    v = Vec4<T> (0, 0, 0, 0);
    assert (v.length () == 0);
    assert (v.normalized ().length () == 0);

    v = Vec4<T> (3, 4, 0, 0);
    assert (v.length () == 5);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec4<T> (3000, 4000, 0, 0);
    assert (v.length () == 5000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec4<T> (1, -1, 1, 1);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), 2, e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    v = Vec4<T> (1000, -1000, 1000, 1000);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), 2000, 1000 * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    T t = s * (1 << 4);

    v = Vec4<T> (t, 0, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (-t, -t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t * 2, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 4);

    v = Vec4<T> (t, 0, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (-t, -t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t * 2, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));

    t = s / (1 << 20);

    v = Vec4<T> (t, 0, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, t, 0, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, t, 0);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (0, 0, 0, t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
    v = Vec4<T> (-t, -t, -t, -t);
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.length (), t * 2, t * e));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), 1, e));
}

/// Custom scalar with the operations ``Vec::length()`` needs.
class Foo
{
public:
    constexpr Foo () IMATH_NOEXCEPT : _v (0) {}
    constexpr Foo (float v) IMATH_NOEXCEPT : _v (v) {}
    constexpr explicit Foo (int i) IMATH_NOEXCEPT
        : _v (static_cast<float> (i))
    {}

    constexpr operator float () const IMATH_NOEXCEPT { return _v; }

    constexpr Foo operator* (Foo o) const IMATH_NOEXCEPT
    {
        return Foo (_v * o._v);
    }
    constexpr Foo operator* (float f) const IMATH_NOEXCEPT
    {
        return Foo (_v * f);
    }
    constexpr Foo operator+ (Foo o) const IMATH_NOEXCEPT
    {
        return Foo (_v + o._v);
    }
    constexpr Foo operator/ (Foo o) const IMATH_NOEXCEPT
    {
        return Foo (_v / o._v);
    }
    constexpr bool operator< (Foo o) const IMATH_NOEXCEPT
    {
        return _v < o._v;
    }
    Foo& operator/= (Foo o) IMATH_NOEXCEPT
    {
        _v /= o._v;
        return *this;
    }

private:
    float _v;
};

/// Custom scalar with no arithmetic; not suitable for ``length()``.
class Bar
{
public:
    constexpr Bar () IMATH_NOEXCEPT = default;
};

} // namespace

namespace std
{

template <>
class numeric_limits<Foo>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_integer     = false;

    static constexpr Foo min () IMATH_NOEXCEPT { return Foo (0); }
    static constexpr Foo lowest () IMATH_NOEXCEPT
    {
        return Foo (-std::numeric_limits<float>::max ());
    }
    static constexpr Foo max () IMATH_NOEXCEPT
    {
        return Foo (std::numeric_limits<float>::max ());
    }
    static constexpr Foo epsilon () IMATH_NOEXCEPT
    {
        return Foo (std::numeric_limits<float>::epsilon ());
    }
};

} // namespace std

namespace
{

void
testLength2Foo ()
{
    const Foo e (4 * std::numeric_limits<float>::epsilon ());

    Vec2<Foo> v;

    v = Vec2<Foo> (Foo (0), Foo (0));
    assert (v.length () == Foo (0));
    assert (v.normalized ().length () == Foo (0));

    v = Vec2<Foo> (Foo (3), Foo (4));
    assert (v.length () == Foo (5));
    assert (IMATH_INTERNAL_NAMESPACE::equal (v.normalized ().length (), Foo (1), e));
}

// Detection traits: does Vec<T>::length() exist?
template <typename V, typename = void>
struct has_length : std::false_type {};

template <typename V>
struct has_length<V, decltype(void(std::declval<V>().length()))> : std::true_type {};

// Detection traits: does Vec<T>::normalize() exist?
template <typename V, typename = void>
struct has_normalize : std::false_type {};

template <typename V>
struct has_normalize<V, decltype(void(std::declval<V>().normalize()))> : std::true_type {};

// Detection traits: does Vec<T>::normalized() exist?
template <typename V, typename = void>
struct has_normalized : std::false_type {};

template <typename V>
struct has_normalized<V, decltype(void(std::declval<V>().normalized()))> : std::true_type {};

static_assert(
    is_imath_floating_point<float>::value, "float must be imath floating point");
static_assert(
    is_imath_floating_point<double>::value, "double must be imath floating point");
static_assert(
    is_imath_floating_point<half>::value, "half must be imath floating point");
static_assert(
    !is_imath_floating_point<int>::value, "int must not be imath floating point");
static_assert(
    !is_imath_floating_point<short>::value,
    "short must not be imath floating point");
static_assert(
    is_imath_floating_point<Foo>::value,
    "Foo must be imath floating point");
static_assert(
    !is_imath_floating_point<Bar>::value,
    "Bar must not be imath floating point");

// Floating-point Vec types must have length/normalize/normalized.
static_assert(has_length<Vec2<Foo>>::value,    "Vec2<Foo> must have length()");
static_assert(has_normalize<Vec2<Foo>>::value,  "Vec2<Foo> must have normalize()");
static_assert(has_normalized<Vec2<Foo>>::value, "Vec2<Foo> must have normalized()");

static_assert(!has_length<Vec2<Bar>>::value,    "Vec2<Bar> must not have length()");
static_assert(!has_normalize<Vec2<Bar>>::value,  "Vec2<Bar> must not have normalize()");
static_assert(!has_normalized<Vec2<Bar>>::value, "Vec2<Bar> must not have normalized()");

static_assert(has_length<Vec2<half>>::value,    "Vec2<half> must have length()");
static_assert(has_length<Vec2<float>>::value,    "Vec2<float> must have length()");
static_assert(has_length<Vec2<double>>::value,   "Vec2<double> must have length()");
static_assert(has_length<Vec3<half>>::value,    "Vec3<half> must have length()");
static_assert(has_length<Vec3<float>>::value,    "Vec3<float> must have length()");
static_assert(has_length<Vec3<double>>::value,   "Vec3<double> must have length()");
static_assert(has_length<Vec4<half>>::value,    "Vec4<half> must have length()");
static_assert(has_length<Vec4<float>>::value,    "Vec4<float> must have length()");
static_assert(has_length<Vec4<double>>::value,   "Vec4<double> must have length()");

static_assert(has_normalize<Vec2<half>>::value,  "Vec2<half> must have normalize()");
static_assert(has_normalize<Vec2<float>>::value,  "Vec2<float> must have normalize()");
static_assert(has_normalize<Vec2<double>>::value, "Vec2<double> must have normalize()");
static_assert(has_normalize<Vec3<half>>::value,  "Vec3<half> must have normalize()");
static_assert(has_normalize<Vec3<float>>::value,  "Vec3<float> must have normalize()");
static_assert(has_normalize<Vec3<double>>::value, "Vec3<double> must have normalize()");
static_assert(has_normalize<Vec4<half>>::value,  "Vec4<half> must have normalize()");
static_assert(has_normalize<Vec4<float>>::value,  "Vec4<float> must have normalize()");
static_assert(has_normalize<Vec4<double>>::value, "Vec4<double> must have normalize()");

static_assert(has_normalized<Vec2<half>>::value,  "Vec2<half> must have normalized()");
static_assert(has_normalized<Vec2<float>>::value,  "Vec2<float> must have normalized()");
static_assert(has_normalized<Vec2<double>>::value, "Vec2<double> must have normalized()");
static_assert(has_normalized<Vec3<half>>::value,  "Vec3<half> must have normalized()");
static_assert(has_normalized<Vec3<float>>::value,  "Vec3<float> must have normalized()");
static_assert(has_normalized<Vec3<double>>::value, "Vec3<double> must have normalized()");
static_assert(has_normalized<Vec4<half>>::value,  "Vec4<half> must have normalized()");
static_assert(has_normalized<Vec4<float>>::value,  "Vec4<float> must have normalized()");
static_assert(has_normalized<Vec4<double>>::value, "Vec4<double> must have normalized()");

// Integer Vec types must NOT have length/normalize/normalized.
static_assert(!has_length<Vec2<int>>::value,    "Vec2<int> must not have length()");
static_assert(!has_length<Vec2<short>>::value,  "Vec2<short> must not have length()");
static_assert(!has_length<Vec3<int>>::value,    "Vec3<int> must not have length()");
static_assert(!has_length<Vec3<short>>::value,  "Vec3<short> must not have length()");
static_assert(!has_length<Vec4<int>>::value,    "Vec4<int> must not have length()");
static_assert(!has_length<Vec4<short>>::value,  "Vec4<short> must not have length()");

static_assert(!has_normalize<Vec2<int>>::value,    "Vec2<int> must not have normalize()");
static_assert(!has_normalize<Vec2<short>>::value,  "Vec2<short> must not have normalize()");
static_assert(!has_normalize<Vec3<int>>::value,    "Vec3<int> must not have normalize()");
static_assert(!has_normalize<Vec3<short>>::value,  "Vec3<short> must not have normalize()");
static_assert(!has_normalize<Vec4<int>>::value,    "Vec4<int> must not have normalize()");
static_assert(!has_normalize<Vec4<short>>::value,  "Vec4<short> must not have normalize()");

static_assert(!has_normalized<Vec2<int>>::value,    "Vec2<int> must not have normalized()");
static_assert(!has_normalized<Vec2<short>>::value,  "Vec2<short> must not have normalized()");
static_assert(!has_normalized<Vec3<int>>::value,    "Vec3<int> must not have normalized()");
static_assert(!has_normalized<Vec3<short>>::value,  "Vec3<short> must not have normalized()");
static_assert(!has_normalized<Vec4<int>>::value,    "Vec4<int> must not have normalized()");
static_assert(!has_normalized<Vec4<short>>::value,  "Vec4<short> must not have normalized()");

} // namespace

void
testVec ()
{
    cout << "Testing some basic vector operations" << endl;

    testLength2T<float> ();
    testLength2T<double> ();
    testLength3T<float> ();
    testLength3T<double> ();
    testLength4T<float> ();
    testLength4T<double> ();

    testLength2Foo ();

    cout << "ok\n" << endl;
}

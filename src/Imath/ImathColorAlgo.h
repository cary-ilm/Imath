//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
// Color conversion functions and general color algorithms
//

#ifndef INCLUDED_IMATHCOLORALGO_H
#define INCLUDED_IMATHCOLORALGO_H

#include "ImathExport.h"
#include "ImathNamespace.h"

#include "ImathColor.h"
#include "ImathMath.h"

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

///
/// Convert 3-channel hsv to rgb.
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3<T>
hsv2rgb (const Vec3<T>& hsv) IMATH_NOEXCEPT
{
    if (std::numeric_limits<T>::is_integer)
    {
        const double invMax =
            1.0 / double (std::numeric_limits<T>::max ());
        const double maxVal = double (std::numeric_limits<T>::max ());
        const Vec3<double> norm (
            hsv.x * invMax, hsv.y * invMax, hsv.z * invMax);
        const Vec3<double> rgb = hsv2rgb<double> (norm);
        return Vec3<T> (
            (T) (rgb.x * maxVal),
            (T) (rgb.y * maxVal),
            (T) (rgb.z * maxVal));
    }

    T hue = hsv.x;
    T sat = hsv.y;
    T val = hsv.z;

    T x = T (0), y = T (0), z = T (0);

    if (hue == T (1))
        hue = T (0);
    else
        hue *= T (6);

    const int i = int (std::floor (hue));
    const T   f = hue - T (i);
    const T   p = val * (T (1) - sat);
    const T   q = val * (T (1) - (sat * f));
    const T   t = val * (T (1) - (sat * (T (1) - f)));

    switch (i)
    {
        case 0:
            x = val;
            y = t;
            z = p;
            break;
        case 1:
            x = q;
            y = val;
            z = p;
            break;
        case 2:
            x = p;
            y = val;
            z = t;
            break;
        case 3:
            x = p;
            y = q;
            z = val;
            break;
        case 4:
            x = t;
            y = p;
            z = val;
            break;
        case 5:
            x = val;
            y = p;
            z = q;
            break;
    }

    return Vec3<T> (x, y, z);
}

///
/// Convert 4-channel hsv to rgb (with alpha).
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Color4<T>
hsv2rgb (const Color4<T>& hsv) IMATH_NOEXCEPT
{
  const Vec3<T> rgb =
      hsv2rgb (Vec3<T> (hsv.r, hsv.g, hsv.b));
  return Color4<T> (rgb.x, rgb.y, rgb.z, hsv.a);
}

///
/// Convert 3-channel rgb to hsv.
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Vec3<T>
rgb2hsv (const Vec3<T>& rgb) IMATH_NOEXCEPT
{
    if (std::numeric_limits<T>::is_integer)
    {
        const double invMax =
            1.0 / double (std::numeric_limits<T>::max ());
        const double maxVal = double (std::numeric_limits<T>::max ());
        const Vec3<double> norm (
            rgb.x * invMax, rgb.y * invMax, rgb.z * invMax);
        const Vec3<double> hsv = rgb2hsv<double> (norm);
        return Vec3<T> (
            (T) (hsv.x * maxVal),
            (T) (hsv.y * maxVal),
            (T) (hsv.z * maxVal));
    }

    const T& x = rgb.x;
    const T& y = rgb.y;
    const T& z = rgb.z;

    const T max =
        (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
    const T min =
        (x < y) ? ((x < z) ? x : z) : ((y < z) ? y : z);
    const T range = max - min;
    T       val   = max;
    T       sat   = T (0);
    T       hue   = T (0);

    if (max != T (0)) sat = range / max;

    if (sat != T (0))
    {
        T h;

        if (x == max)
            h = (y - z) / range;
        else if (y == max)
            h = T (2) + (z - x) / range;
        else
            h = T (4) + (x - y) / range;

        hue = h / T (6);

        if (hue < T (0)) hue += T (1);
    }

    return Vec3<T> (hue, sat, val);
}

///
/// Convert 4-channel rgb to hsv (with alpha).
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 Color4<T>
rgb2hsv (const Color4<T>& rgb) IMATH_NOEXCEPT
{
  const Vec3<T> hsv =
      rgb2hsv (Vec3<T> (rgb.r, rgb.g, rgb.b));
  return Color4<T> (hsv.x, hsv.y, hsv.z, rgb.a);
}

///
/// Backwards-compatible double-precision helpers. Prefer hsv2rgb<double>()
/// and rgb2hsv<double>().
///

inline IMATH_EXPORT Vec3<double>
hsv2rgb_d (const Vec3<double>& hsv) IMATH_NOEXCEPT
{
    return hsv2rgb<double> (hsv);
}

inline IMATH_EXPORT Color4<double>
hsv2rgb_d (const Color4<double>& hsv) IMATH_NOEXCEPT
{
    return hsv2rgb<double> (hsv);
}

inline IMATH_EXPORT Vec3<double>
rgb2hsv_d (const Vec3<double>& rgb) IMATH_NOEXCEPT
{
    return rgb2hsv<double> (rgb);
}

inline IMATH_EXPORT Color4<double>
rgb2hsv_d (const Color4<double>& rgb) IMATH_NOEXCEPT
{
    return rgb2hsv<double> (rgb);
}

///
/// Convert 3-channel rgb to PackedColor
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 PackedColor
rgb2packed (const Vec3<T>& c) IMATH_NOEXCEPT
{
    if (std::numeric_limits<T>::is_integer)
    {
        float x = c.x / float (std::numeric_limits<T>::max ());
        float y = c.y / float (std::numeric_limits<T>::max ());
        float z = c.z / float (std::numeric_limits<T>::max ());
        return rgb2packed (V3f (x, y, z));
    }
    else
    {
        // clang-format off
	return (  (PackedColor) (c.x * 255)		|
		(((PackedColor) (c.y * 255)) << 8)	|
		(((PackedColor) (c.z * 255)) << 16)	| 0xFF000000 );
        // clang-format on
    }
}

///
/// Convert 4-channel rgb to PackedColor (with alpha)
///

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 PackedColor
rgb2packed (const Color4<T>& c) IMATH_NOEXCEPT
{
    if (std::numeric_limits<T>::is_integer)
    {
        float r = c.r / float (std::numeric_limits<T>::max ());
        float g = c.g / float (std::numeric_limits<T>::max ());
        float b = c.b / float (std::numeric_limits<T>::max ());
        float a = c.a / float (std::numeric_limits<T>::max ());
        return rgb2packed (C4f (r, g, b, a));
    }
    else
    {
        // clang-format off
	return (  (PackedColor) (c.r * 255)		|
		(((PackedColor) (c.g * 255)) << 8)	|
		(((PackedColor) (c.b * 255)) << 16)	|
		(((PackedColor) (c.a * 255)) << 24));
        // clang-format on
    }
}

///
/// Convert PackedColor to 3-channel rgb. Return the result in the
/// `out` parameter.
///

template <class T>
IMATH_HOSTDEVICE void
packed2rgb (PackedColor packed, Vec3<T>& out) IMATH_NOEXCEPT
{
    if (std::numeric_limits<T>::is_integer)
    {
        T f   = std::numeric_limits<T>::max () / ((PackedColor) 0xFF);
        out.x = (packed & 0xFF) * f;
        out.y = ((packed & 0xFF00) >> 8) * f;
        out.z = ((packed & 0xFF0000) >> 16) * f;
    }
    else
    {
        T f   = T (1) / T (255);
        out.x = (packed & 0xFF) * f;
        out.y = ((packed & 0xFF00) >> 8) * f;
        out.z = ((packed & 0xFF0000) >> 16) * f;
    }
}

///
/// Convert PackedColor to 4-channel rgb (with alpha). Return the
/// result in the `out` parameter.
///

template <class T>
IMATH_HOSTDEVICE void
packed2rgb (PackedColor packed, Color4<T>& out) IMATH_NOEXCEPT
{
    if (std::numeric_limits<T>::is_integer)
    {
        T f   = std::numeric_limits<T>::max () / ((PackedColor) 0xFF);
        out.r = (packed & 0xFF) * f;
        out.g = ((packed & 0xFF00) >> 8) * f;
        out.b = ((packed & 0xFF0000) >> 16) * f;
        out.a = ((packed & 0xFF000000) >> 24) * f;
    }
    else
    {
        T f   = T (1) / T (255);
        out.r = (packed & 0xFF) * f;
        out.g = ((packed & 0xFF00) >> 8) * f;
        out.b = ((packed & 0xFF0000) >> 16) * f;
        out.a = ((packed & 0xFF000000) >> 24) * f;
    }
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHCOLORALGO_H

//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
// 

//
// 2D, 3D and 4D point/vector class templates
//

#ifndef INCLUDED_IMATHVEC_H
#define INCLUDED_IMATHVEC_H

#include "ImathNamespace.h"

#include <iostream>
#include <limits>
#include <stdexcept>

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

template <class T> class Vec4;

/// Enum for the Vec4 to Vec3 conversion constructor
enum IMATH_EXPORT_ENUM InfException
{
    INF_EXCEPTION
};

///
/// 4-element vector
///

template <class T> class IMATH_EXPORT_TEMPLATE_TYPE Vec4
{
  public:

    /// @{
    /// @name Direct access to elements

    T x, y, z, w;

    /// @}
    
    /// Element access by index.  
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T& operator[] (int i) IMATH_NOEXCEPT;

    /// Element access by index.  
    IMATH_HOSTDEVICE constexpr const T& operator[] (int i) const IMATH_NOEXCEPT;

    /// @{
    ///	@name Constructors and Assignment

    /// Uninitialized by default
    IMATH_HOSTDEVICE Vec4() IMATH_NOEXCEPT;                            // no initialization

    /// Initialize to a scalar `(a,a,a,a)`
    IMATH_HOSTDEVICE constexpr explicit Vec4 (T a) IMATH_NOEXCEPT;

    /// Initialize to given elements `(a,b,c,d)`
    IMATH_HOSTDEVICE constexpr Vec4 (T a, T b, T c, T d) IMATH_NOEXCEPT;

    /// Copy constructor
    IMATH_HOSTDEVICE constexpr Vec4 (const Vec4& v) IMATH_NOEXCEPT;

    /// Construct from Vec4 of another base type
    template <class S> IMATH_HOSTDEVICE constexpr Vec4 (const Vec4<S>& v) IMATH_NOEXCEPT;

    /// Vec3 to Vec4 conversion, sets w to 1.
    template <class S> IMATH_HOSTDEVICE explicit constexpr Vec4 (const Vec3<S>& v) IMATH_NOEXCEPT;

    /// Assignment
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4& operator= (const Vec4& v) IMATH_NOEXCEPT;

    /// Destructor
    ~Vec4() IMATH_NOEXCEPT = default;

    /// @}

    /// @{
    /// @name Arithmetic and Comparison
    
    /// Equality
    template <class S> IMATH_HOSTDEVICE constexpr bool operator== (const Vec4<S>& v) const IMATH_NOEXCEPT;

    /// Inequality
    template <class S> IMATH_HOSTDEVICE constexpr bool operator!= (const Vec4<S>& v) const IMATH_NOEXCEPT;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and `m` are the same
    /// with an absolute error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i][j] - m[i][j]) <= e
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool equalWithAbsError (const Vec4<T>& v, T e) const IMATH_NOEXCEPT;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and m are the same with
    /// a relative error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i] - v[i][j]) <= e * abs (this[i][j])
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 bool equalWithRelError (const Vec4<T>& v, T e) const IMATH_NOEXCEPT;

    /// Dot product
    IMATH_HOSTDEVICE constexpr T dot (const Vec4& v) const IMATH_NOEXCEPT;

    /// Dot product
    IMATH_HOSTDEVICE constexpr T operator^ (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise addition
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4& operator+= (const Vec4& v) IMATH_NOEXCEPT;

    /// Component-wise addition
    IMATH_HOSTDEVICE constexpr Vec4 operator+ (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise subtraction
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4& operator-= (const Vec4& v) IMATH_NOEXCEPT;

    /// Component-wise subtraction
    IMATH_HOSTDEVICE constexpr Vec4 operator- (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise multiplication by -1
    IMATH_HOSTDEVICE constexpr Vec4 operator-() const IMATH_NOEXCEPT;

    /// Component-wise multiplication by -1
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4& negate() IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4& operator*= (const Vec4& v) IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4& operator*= (T a) IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE constexpr Vec4 operator* (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise multiplication
    IMATH_HOSTDEVICE constexpr Vec4 operator* (T a) const IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4& operator/= (const Vec4& v) IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 const Vec4& operator/= (T a) IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE constexpr Vec4 operator/ (const Vec4& v) const IMATH_NOEXCEPT;

    /// Component-wise division
    IMATH_HOSTDEVICE constexpr Vec4 operator/ (T a) const IMATH_NOEXCEPT;

    /// @}

    /// @{
    /// @name Query and Manipulation

    /// Return the Euclidean norm
    IMATH_HOSTDEVICE T length() const IMATH_NOEXCEPT;

    /// Return the square of the Euclidean norm, i.e. the dot product
    /// with itself.
    IMATH_HOSTDEVICE constexpr T length2() const IMATH_NOEXCEPT;

    /// Normalize in place. If length()==0, return a null vector.
    IMATH_HOSTDEVICE const Vec4& normalize() IMATH_NOEXCEPT; // modifies *this

    /// Normalize in place. If length()==0, throw an exception.
    const Vec4& normalizeExc();

    /// Normalize without any checks for length()==0. Slightly faster
    /// than the other normalization routines, but if v.length() is
    /// 0.0, the result is undefined.
    IMATH_HOSTDEVICE const Vec4& normalizeNonNull() IMATH_NOEXCEPT;

    /// Return a normalized vector. Does not modify *this.
    IMATH_HOSTDEVICE Vec4<T> normalized() const IMATH_NOEXCEPT; // does not modify *this

    /// Return a normalized vector. Does not modify *this. Throw an
    /// exception if length()==0.
    Vec4<T> normalizedExc() const;

    /// Return a normalized vector. Does not modify *this, and does
    /// not check for length()==0. Slightly faster than the other
    /// normalization routines, but if v.length() is 0.0, the result
    /// is undefined.
    IMATH_HOSTDEVICE Vec4<T> normalizedNonNull() const IMATH_NOEXCEPT;

    /// @}
    
    /// @{
    /// @name Numeric Limits
    
    /// Largest possible negative value
    IMATH_HOSTDEVICE constexpr static T baseTypeLowest() IMATH_NOEXCEPT { return std::numeric_limits<T>::lowest(); }

    /// Largest possible positive value
    IMATH_HOSTDEVICE constexpr static T baseTypeMax() IMATH_NOEXCEPT { return std::numeric_limits<T>::max(); }

    /// Smallest possible positive value
    IMATH_HOSTDEVICE constexpr static T baseTypeSmallest() IMATH_NOEXCEPT { return std::numeric_limits<T>::min(); }

    /// Smallest possible e for which 1+e != 1
    IMATH_HOSTDEVICE constexpr static T baseTypeEpsilon() IMATH_NOEXCEPT { return std::numeric_limits<T>::epsilon(); }

    /// @}
    
    /// Return the number of dimensions, i.e. 4
    IMATH_HOSTDEVICE constexpr static unsigned int dimensions() IMATH_NOEXCEPT { return 4; }

    /// The base type: In templates that accept a parameter `V`, you
    /// can refer to `T` as `V::BaseType`
    typedef T BaseType;

  private:
    IMATH_HOSTDEVICE IMATH_CONSTEXPR14 T lengthTiny() const IMATH_NOEXCEPT;
};

/// Stream output, as "(x y z w)"
template <class T> std::ostream& operator<< (std::ostream& s, const Vec4<T>& v);

/// Reverse multiplication: S * Vec4<T>
template <class T> IMATH_HOSTDEVICE constexpr Vec4<T> operator* (T a, const Vec4<T>& v) IMATH_NOEXCEPT;

//-------------------------
// Typedefs for convenience
//-------------------------

/// Vec4 of short
typedef Vec4<short> V4s;

/// Vec4 of integer
typedef Vec4<int> V4i;

/// Vec4 of int64_t
typedef Vec4<int64_t> V4i64;

/// Vec4 of float
typedef Vec4<float> V4f;

/// Vec4 of double
typedef Vec4<double> V4d;

//----------------------------------------------------------------------------
// Specializations for VecN<short>, VecN<int>
//
// Normalize and length don't make sense for integer vectors, so disable them.
//----------------------------------------------------------------------------

// Vec4<short>
template <> IMATH_HOSTDEVICE short Vec4<short>::length() const IMATH_NOEXCEPT = delete;
template <> IMATH_HOSTDEVICE const Vec4<short>& Vec4<short>::normalize() IMATH_NOEXCEPT = delete;
template <> const Vec4<short>& Vec4<short>::normalizeExc() = delete;
template <> IMATH_HOSTDEVICE const Vec4<short>& Vec4<short>::normalizeNonNull() IMATH_NOEXCEPT = delete;
template <> IMATH_HOSTDEVICE Vec4<short> Vec4<short>::normalized() const IMATH_NOEXCEPT = delete;
template <> Vec4<short> Vec4<short>::normalizedExc() const = delete;
template <> IMATH_HOSTDEVICE Vec4<short> Vec4<short>::normalizedNonNull() const IMATH_NOEXCEPT = delete;

// Vec4<int>
template <> IMATH_HOSTDEVICE int Vec4<int>::length() const IMATH_NOEXCEPT = delete;
template <> IMATH_HOSTDEVICE const Vec4<int>& Vec4<int>::normalize() IMATH_NOEXCEPT = delete;
template <> const Vec4<int>& Vec4<int>::normalizeExc() = delete;
template <> IMATH_HOSTDEVICE const Vec4<int>& Vec4<int>::normalizeNonNull() IMATH_NOEXCEPT = delete;
template <> IMATH_HOSTDEVICE Vec4<int> Vec4<int>::normalized() const IMATH_NOEXCEPT = delete;
template <> Vec4<int> Vec4<int>::normalizedExc() const = delete;
template <> IMATH_HOSTDEVICE Vec4<int> Vec4<int>::normalizedNonNull() const IMATH_NOEXCEPT = delete;

// Vec4<int64_t>
template <> IMATH_HOSTDEVICE int64_t Vec4<int64_t>::length() const IMATH_NOEXCEPT = delete;
template <> IMATH_HOSTDEVICE const Vec4<int64_t>& Vec4<int64_t>::normalize() IMATH_NOEXCEPT = delete;
template <> const Vec4<int64_t>& Vec4<int64_t>::normalizeExc() = delete;
template <> IMATH_HOSTDEVICE const Vec4<int64_t>& Vec4<int64_t>::normalizeNonNull() IMATH_NOEXCEPT = delete;
template <> IMATH_HOSTDEVICE Vec4<int64_t> Vec4<int64_t>::normalized() const IMATH_NOEXCEPT = delete;
template <> Vec4<int64_t> Vec4<int64_t>::normalizedExc() const = delete;
template <> IMATH_HOSTDEVICE Vec4<int64_t> Vec4<int64_t>::normalizedNonNull() const IMATH_NOEXCEPT = delete;


//-----------------------
// Implementation of Vec4
//-----------------------

template <class T>
IMATH_HOSTDEVICE
IMATH_CONSTEXPR14 inline T&
Vec4<T>::operator[] (int i) IMATH_NOEXCEPT
{
    return (&x)[i]; // NOSONAR - suppress SonarCloud bug report.
}

template <class T>
IMATH_HOSTDEVICE constexpr inline const T&
Vec4<T>::operator[] (int i) const IMATH_NOEXCEPT
{
    return (&x)[i]; // NOSONAR - suppress SonarCloud bug report.
}

template <class T> IMATH_HOSTDEVICE inline Vec4<T>::Vec4() IMATH_NOEXCEPT
{
    // empty, and not constexpr because data is uninitialized.
}

template <class T> IMATH_HOSTDEVICE constexpr inline Vec4<T>::Vec4 (T a) IMATH_NOEXCEPT
    : x(a), y(a), z(a), w(a)
{
}

template <class T> IMATH_HOSTDEVICE constexpr inline Vec4<T>::Vec4 (T a, T b, T c, T d) IMATH_NOEXCEPT
    : x(a), y(b), z(c), w(d)
{
}

template <class T> IMATH_HOSTDEVICE constexpr inline Vec4<T>::Vec4 (const Vec4& v) IMATH_NOEXCEPT
    : x(v.x), y(v.y), z(v.z), w(v.w)
{
}

template <class T> template <class S>
IMATH_HOSTDEVICE constexpr inline Vec4<T>::Vec4 (const Vec4<S>& v) IMATH_NOEXCEPT
    : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w))
{
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T>&
Vec4<T>::operator= (const Vec4& v) IMATH_NOEXCEPT
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

template <class T> template <class S>
IMATH_HOSTDEVICE constexpr inline Vec4<T>::Vec4 (const Vec3<S>& v) IMATH_NOEXCEPT
    : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(1))
{
}

template <class T>
template <class S>
IMATH_HOSTDEVICE constexpr inline bool
Vec4<T>::operator== (const Vec4<S>& v) const IMATH_NOEXCEPT
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

template <class T>
template <class S>
IMATH_HOSTDEVICE constexpr inline bool
Vec4<T>::operator!= (const Vec4<S>& v) const IMATH_NOEXCEPT
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Vec4<T>::equalWithAbsError (const Vec4<T>& v, T e) const IMATH_NOEXCEPT
{
    for (int i = 0; i < 4; i++)
        if (!IMATH_INTERNAL_NAMESPACE::equalWithAbsError ((*this)[i], v[i], e))
            return false;

    return true;
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline bool
Vec4<T>::equalWithRelError (const Vec4<T>& v, T e) const IMATH_NOEXCEPT
{
    for (int i = 0; i < 4; i++)
        if (!IMATH_INTERNAL_NAMESPACE::equalWithRelError ((*this)[i], v[i], e))
            return false;

    return true;
}

template <class T>
IMATH_HOSTDEVICE constexpr inline T
Vec4<T>::dot (const Vec4& v) const IMATH_NOEXCEPT
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

template <class T>
IMATH_HOSTDEVICE constexpr inline T
Vec4<T>::operator^ (const Vec4& v) const IMATH_NOEXCEPT
{
    return dot (v);
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T>&
Vec4<T>::operator+= (const Vec4& v) IMATH_NOEXCEPT
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

template <class T>
IMATH_HOSTDEVICE constexpr inline Vec4<T>
Vec4<T>::operator+ (const Vec4& v) const IMATH_NOEXCEPT
{
    return Vec4 (x + v.x, y + v.y, z + v.z, w + v.w);
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T>&
Vec4<T>::operator-= (const Vec4& v) IMATH_NOEXCEPT
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

template <class T>
IMATH_HOSTDEVICE constexpr inline Vec4<T>
Vec4<T>::operator- (const Vec4& v) const IMATH_NOEXCEPT
{
    return Vec4 (x - v.x, y - v.y, z - v.z, w - v.w);
}

template <class T>
IMATH_HOSTDEVICE constexpr inline Vec4<T>
Vec4<T>::operator-() const IMATH_NOEXCEPT
{
    return Vec4 (-x, -y, -z, -w);
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T>&
Vec4<T>::negate() IMATH_NOEXCEPT
{
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T>&
Vec4<T>::operator*= (const Vec4& v) IMATH_NOEXCEPT
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T>&
Vec4<T>::operator*= (T a) IMATH_NOEXCEPT
{
    x *= a;
    y *= a;
    z *= a;
    w *= a;
    return *this;
}

template <class T>
IMATH_HOSTDEVICE constexpr inline Vec4<T>
Vec4<T>::operator* (const Vec4& v) const IMATH_NOEXCEPT
{
    return Vec4 (x * v.x, y * v.y, z * v.z, w * v.w);
}

template <class T>
IMATH_HOSTDEVICE constexpr inline Vec4<T>
Vec4<T>::operator* (T a) const IMATH_NOEXCEPT
{
    return Vec4 (x * a, y * a, z * a, w * a);
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T>&
Vec4<T>::operator/= (const Vec4& v) IMATH_NOEXCEPT
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline const Vec4<T>&
Vec4<T>::operator/= (T a) IMATH_NOEXCEPT
{
    x /= a;
    y /= a;
    z /= a;
    w /= a;
    return *this;
}

template <class T>
IMATH_HOSTDEVICE constexpr inline Vec4<T>
Vec4<T>::operator/ (const Vec4& v) const IMATH_NOEXCEPT
{
    return Vec4 (x / v.x, y / v.y, z / v.z, w / v.w);
}

template <class T>
IMATH_HOSTDEVICE constexpr inline Vec4<T>
Vec4<T>::operator/ (T a) const IMATH_NOEXCEPT
{
    return Vec4 (x / a, y / a, z / a, w / a);
}

template <class T>
IMATH_HOSTDEVICE IMATH_CONSTEXPR14 inline T
Vec4<T>::lengthTiny() const IMATH_NOEXCEPT
{
    T absX = (x >= T (0)) ? x : -x;
    T absY = (y >= T (0)) ? y : -y;
    T absZ = (z >= T (0)) ? z : -z;
    T absW = (w >= T (0)) ? w : -w;

    T max = absX;

    if (max < absY)
        max = absY;

    if (max < absZ)
        max = absZ;

    if (max < absW)
        max = absW;

    if (IMATH_UNLIKELY(max == T (0)))
        return T (0);

    //
    // Do not replace the divisions by max with multiplications by 1/max.
    // Computing 1/max can overflow but the divisions below will always
    // produce results less than or equal to 1.
    //

    absX /= max;
    absY /= max;
    absZ /= max;
    absW /= max;

    return max * std::sqrt (absX * absX + absY * absY + absZ * absZ + absW * absW);
}

template <class T>
IMATH_HOSTDEVICE inline T
Vec4<T>::length() const IMATH_NOEXCEPT
{
    T length2 = dot (*this);

    if (IMATH_UNLIKELY(length2 < T (2) * std::numeric_limits<T>::min()))
        return lengthTiny();

    return std::sqrt (length2);
}

template <class T>
IMATH_HOSTDEVICE constexpr inline T
Vec4<T>::length2() const IMATH_NOEXCEPT
{
    return dot (*this);
}

template <class T>
IMATH_HOSTDEVICE const inline Vec4<T>&
Vec4<T>::normalize() IMATH_NOEXCEPT
{
    T l = length();

    if (IMATH_LIKELY(l != T (0)))
    {
        //
        // Do not replace the divisions by l with multiplications by 1/l.
        // Computing 1/l can overflow but the divisions below will always
        // produce results less than or equal to 1.
        //

        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    return *this;
}

template <class T>
const inline Vec4<T>&
Vec4<T>::normalizeExc()
{
    T l = length();

    if (IMATH_UNLIKELY(l == T (0)))
        throw std::domain_error ("Cannot normalize null vector.");

    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
}

template <class T>
IMATH_HOSTDEVICE inline const Vec4<T>&
Vec4<T>::normalizeNonNull() IMATH_NOEXCEPT
{
    T l = length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
}

template <class T>
IMATH_HOSTDEVICE inline Vec4<T>
Vec4<T>::normalized() const IMATH_NOEXCEPT
{
    T l = length();

    if (IMATH_UNLIKELY(l == T (0)))
        return Vec4 (T (0));

    return Vec4 (x / l, y / l, z / l, w / l);
}

template <class T>
inline Vec4<T>
Vec4<T>::normalizedExc() const
{
    T l = length();

    if (IMATH_UNLIKELY(l == T (0)))
        throw std::domain_error ("Cannot normalize null vector.");

    return Vec4 (x / l, y / l, z / l, w / l);
}

template <class T>
IMATH_HOSTDEVICE inline Vec4<T>
Vec4<T>::normalizedNonNull() const IMATH_NOEXCEPT
{
    T l = length();
    return Vec4 (x / l, y / l, z / l, w / l);
}


template <class T>
std::ostream&
operator<< (std::ostream& s, const Vec4<T>& v)
{
    return s << '(' << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << ')';
}

template <class T>
IMATH_HOSTDEVICE constexpr inline Vec4<T>
operator* (T a, const Vec4<T>& v) IMATH_NOEXCEPT
{
    return Vec4<T> (a * v.x, a * v.y, a * v.z, a * v.w);
}

#if (defined _WIN32 || defined _WIN64) && defined _MSC_VER
#    pragma warning(pop)
#endif

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHVEC_H

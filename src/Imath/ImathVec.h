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

namespace Imath {
    
template <class T> class Vec4;

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


#if (defined _WIN32 || defined _WIN64) && defined _MSC_VER
#    pragma warning(pop)
#endif

}


#endif // INCLUDED_IMATHVEC_H

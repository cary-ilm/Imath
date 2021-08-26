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

template <class T> class Vec4
{
  public:

    /// @{
    /// @name Direct access to elements

    T x, y, z, w;

    /// @}
    
    /// Element access by index.  
     constexpr T& operator[] (int i) noexcept;

    /// Element access by index.  
     constexpr const T& operator[] (int i) const noexcept;

    /// @{
    ///	@name Constructors and Assignment

    /// Uninitialized by default
     Vec4() noexcept;                            // no initialization

    /// Initialize to a scalar `(a,a,a,a)`
     constexpr explicit Vec4 (T a) noexcept;

    /// Initialize to given elements `(a,b,c,d)`
     constexpr Vec4 (T a, T b, T c, T d) noexcept;

    /// Copy constructor
     constexpr Vec4 (const Vec4& v) noexcept;

    /// Construct from Vec4 of another base type
    template <class S>  constexpr Vec4 (const Vec4<S>& v) noexcept;

    /// Vec3 to Vec4 conversion, sets w to 1.
    template <class S>  explicit constexpr Vec4 (const Vec3<S>& v) noexcept;

    /// Assignment
     constexpr const Vec4& operator= (const Vec4& v) noexcept;

    /// Destructor
    ~Vec4() noexcept = default;

    /// @}

    /// @{
    /// @name Arithmetic and Comparison
    
    /// Equality
    template <class S>  constexpr bool operator== (const Vec4<S>& v) const noexcept;

    /// Inequality
    template <class S>  constexpr bool operator!= (const Vec4<S>& v) const noexcept;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and `m` are the same
    /// with an absolute error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i][j] - m[i][j]) <= e
     constexpr bool equalWithAbsError (const Vec4<T>& v, T e) const noexcept;

    /// Compare two matrices and test if they are "approximately equal":
    /// @return True if the coefficients of this and m are the same with
    /// a relative error of no more than e, i.e., for all i, j:
    ///
    ///     abs (this[i] - v[i][j]) <= e * abs (this[i][j])
     constexpr bool equalWithRelError (const Vec4<T>& v, T e) const noexcept;

    /// Dot product
     constexpr T dot (const Vec4& v) const noexcept;

    /// Dot product
     constexpr T operator^ (const Vec4& v) const noexcept;

    /// Component-wise addition
     constexpr const Vec4& operator+= (const Vec4& v) noexcept;

    /// Component-wise addition
     constexpr Vec4 operator+ (const Vec4& v) const noexcept;

    /// Component-wise subtraction
     constexpr const Vec4& operator-= (const Vec4& v) noexcept;

    /// Component-wise subtraction
     constexpr Vec4 operator- (const Vec4& v) const noexcept;

    /// Component-wise multiplication by -1
     constexpr Vec4 operator-() const noexcept;

    /// Component-wise multiplication by -1
     constexpr const Vec4& negate() noexcept;

    /// Component-wise multiplication
     constexpr const Vec4& operator*= (const Vec4& v) noexcept;

    /// Component-wise multiplication
     constexpr const Vec4& operator*= (T a) noexcept;

    /// Component-wise multiplication
     constexpr Vec4 operator* (const Vec4& v) const noexcept;

    /// Component-wise multiplication
     constexpr Vec4 operator* (T a) const noexcept;

    /// Component-wise division
     constexpr const Vec4& operator/= (const Vec4& v) noexcept;

    /// Component-wise division
     constexpr const Vec4& operator/= (T a) noexcept;

    /// Component-wise division
     constexpr Vec4 operator/ (const Vec4& v) const noexcept;

    /// Component-wise division
     constexpr Vec4 operator/ (T a) const noexcept;

    /// @}

    /// @{
    /// @name Query and Manipulation

    /// Return the Euclidean norm
     T length() const noexcept;

    /// Return the square of the Euclidean norm, i.e. the dot product
    /// with itself.
     constexpr T length2() const noexcept;

    /// Normalize in place. If length()==0, return a null vector.
     const Vec4& normalize() noexcept; // modifies *this

    /// Normalize in place. If length()==0, throw an exception.
    const Vec4& normalizeExc();

    /// Normalize without any checks for length()==0. Slightly faster
    /// than the other normalization routines, but if v.length() is
    /// 0.0, the result is undefined.
     const Vec4& normalizeNonNull() noexcept;

    /// Return a normalized vector. Does not modify *this.
     Vec4<T> normalized() const noexcept; // does not modify *this

    /// Return a normalized vector. Does not modify *this. Throw an
    /// exception if length()==0.
    Vec4<T> normalizedExc() const;

    /// Return a normalized vector. Does not modify *this, and does
    /// not check for length()==0. Slightly faster than the other
    /// normalization routines, but if v.length() is 0.0, the result
    /// is undefined.
     Vec4<T> normalizedNonNull() const noexcept;

    /// @}
    
    /// @{
    /// @name Numeric Limits
    
    /// Largest possible negative value
     constexpr static T baseTypeLowest() noexcept { return std::numeric_limits<T>::lowest(); }

    /// Largest possible positive value
     constexpr static T baseTypeMax() noexcept { return std::numeric_limits<T>::max(); }

    /// Smallest possible positive value
     constexpr static T baseTypeSmallest() noexcept { return std::numeric_limits<T>::min(); }

    /// Smallest possible e for which 1+e != 1
     constexpr static T baseTypeEpsilon() noexcept { return std::numeric_limits<T>::epsilon(); }

    /// @}
    
    /// Return the number of dimensions, i.e. 4
     constexpr static unsigned int dimensions() noexcept { return 4; }

    /// The base type: In templates that accept a parameter `V`, you
    /// can refer to `T` as `V::BaseType`
    typedef T BaseType;

  private:
     constexpr T lengthTiny() const noexcept;
};

template <class T>  constexpr inline Vec4<T>::Vec4 (const Vec4& v) noexcept
    : x(v.x), y(v.y), z(v.z), w(v.w)
{
}

template <class T> template <class S>
 constexpr inline Vec4<T>::Vec4 (const Vec4<S>& v) noexcept
    : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w))
{
}



}


#endif // INCLUDED_IMATHVEC_H

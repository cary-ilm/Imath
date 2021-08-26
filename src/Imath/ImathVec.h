//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
// 

///
/// 4-element vector
///

template <class T> class Vec4
{
  public:

    T x;

    /// Copy constructor
     constexpr Vec4 (const Vec4& v) noexcept;

    /// Construct from Vec4 of another base type
    template <class S>  constexpr Vec4 (const Vec4<S>& v) noexcept;
};

template <class T>  constexpr inline Vec4<T>::Vec4 (const Vec4& v) noexcept
    : x(v.x)
{
}

template <class T> template <class S>
 constexpr inline Vec4<T>::Vec4 (const Vec4<S>& v) noexcept
    : x(T(v.x))
{
}






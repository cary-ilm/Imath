//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
// 

namespace Imath {
    
template <class T> class Vec4
{
  public:

    constexpr Vec4 (const Vec4& v) noexcept;
    template <class S>  constexpr Vec4 (const Vec4<S>& v) noexcept;
};

template <class T>
constexpr inline Vec4<T>::Vec4 (const Vec4& v) noexcept
{
}

template <class T> template <class S>
constexpr inline Vec4<T>::Vec4 (const Vec4<S>& v) noexcept
{
}

}




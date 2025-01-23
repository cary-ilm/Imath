//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathVec.h>
#include <ImathVecAlgo.h>

namespace py = pybind11;

namespace PyBindImath {


template <class V>
void setValue2(V& v, typename V::BaseType x, typename V::BaseType y)
{
    v.setValue(x, y);
}
    
template <class Vec>
py::class_<Vec> register_vec(py::class_<Vec>& c)
{
    typedef typename Vec::BaseType T;
    
    return c.def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self + py::self)
        .def(py::self += py::self)
        .def(py::self * py::self)
        .def(py::self * int())
        .def(py::self * float())
        .def(py::self * double())
        .def(py::self *= py::self)
        .def(py::self *= int())
        .def(py::self *= float())
        .def(py::self *= double())
        .def(py::self / py::self)
        .def(py::self / int())
        .def(py::self / float())
        .def(py::self / double())
        .def(py::self /= py::self)
        .def(py::self /= int())
        .def(py::self /= float())
        .def(py::self /= double())
        .def(py::self - py::self)
        .def(py::self -= py::self)
        .def(py::self ^ py::self)
        .def(-py::self)
        .def("__getitem__", [](const Vec &v, size_t i) {
            return v[i];  // Read-only index operator
        })
        .def("__setitem__", [](Vec &v, size_t i, T value) {
            v[i] = value;  // Writable index operator
        })
        .def_static("baseTypeEpsilon", &Vec::baseTypeEpsilon,"baseTypeEpsilon() epsilon value of the base type of the vector")
        .def_static("baseTypeMax", &Vec::baseTypeMax,"baseTypeMax() max value of the base type of the vector")
        .def_static("baseTypeLowest", &Vec::baseTypeLowest,"baseTypeLowest() largest negative value of the base type of the vector")
        .def_static("baseTypeSmallest", &Vec::baseTypeSmallest,"baseTypeSmallest() smallest value of the base type of the vector")
        .def("dot", &Vec::dot, "v1.dot(v2) inner product of the two vectors")
        .def("dimensions", &Vec::dimensions, "dimensions() number of dimensions in the vector")
        .def("equalWithAbsError", &Vec::equalWithAbsError, "v1.equalWithAbsError(v2) true if the elements of v1 and v2 are the same with an absolute error of no more than e, i.e., abs(v1[i] - v2[i]) <= e")
        .def("equalWithRelError", &Vec::equalWithRelError, "v1.equalWithRelError(v2) true if the elements of v1 and v2 are the same with a relative error of no more than e, i.e., abs(v1[i] - v2[i]) <= e * abs(v1[i])")

        // things from ImathVecAlgo
        .def("closestVertex", &IMATH_NAMESPACE::closestVertex<Vec>)
        ;
}

template <class Vec>
py::class_<Vec> register_vec_nonint(py::class_<Vec> c)
{
    return c.def("length", &Vec::length, "length() magnitude of the vector")
        .def("length2", &Vec::length2,"length2() square magnitude of the vector")
        .def("negate", &Vec::negate)
        .def("normalize", &Vec::normalize, "v.normalize() destructively normalizes v and returns a reference to it")
        .def("normalizeExc", &Vec::normalizeExc, "v.normalizeExc() destructively normalizes V and returns a reference to it, throwing an exception if length() == 0")
        .def("normalizeNonNull",  &Vec::normalizeNonNull, "v.normalizeNonNull() destructively normalizes V and returns a reference to it, faster if length() != 0")
        .def("normalized", &Vec::normalized, "v.normalized() returns a normalized copy of v")
        .def("normalizedExc", &Vec::normalizedExc, "v.normalizedExc() returns a normalized copy of v, throwing an exception if length() == 0")
        .def("normalizedNonNull", &Vec::normalizedNonNull, "v.normalizedNonNull() returns a normalized copy of v, faster if lngth() != 0")
        .def("orthogonal", &IMATH_NAMESPACE::orthogonal<Vec>)
        .def("project", &IMATH_NAMESPACE::project<Vec>)
        .def("reflect", &IMATH_NAMESPACE::reflect<Vec>);
}

template <class Vec>
py::class_<Vec> register_vec2(py::module& m, const char * name)
{
    typedef typename Vec::BaseType T;

    // no %=

    py::class_<Vec> c(m, name);
    c.def(py::init<>())
        .def(py::init<T>())
        .def(py::init<T, T>())
        .def(py::self % py::self)
        .def("cross", &Vec::cross, "v1.cross(v2) right handed cross product")
        .def_readwrite("x", &Vec::x)
        .def_readwrite("y", &Vec::y)
        .def("setValue",
             [](Vec& self, T x, T y) {
                 setValue2(self, x, y);
             },
             py::arg("x"), py::arg("y"),
             "set to the given x,y values");

    register_vec<Vec>(c);

    return py::cast<py::class_<Vec>>(c);
}

template <class Vec>
py::class_<Vec> register_vec3(py::module& m, const char * name)
{
    typedef typename Vec::BaseType T;

    py::class_<Vec> c(m, name);
    c.def(py::init<>())
        .def(py::init<T>())
        .def(py::init<T, T, T>())
        .def(py::self % py::self)
        .def(py::self %= py::self)
        .def("cross", &Vec::cross, "v1.cross(v2) right handed cross product")
        .def_readwrite("x", &Vec::x)
        .def_readwrite("y", &Vec::y)
        .def_readwrite("z", &Vec::z);

    register_vec<Vec>(c);

    return py::cast<py::class_<Vec>>(c);
}

template <class Vec>
py::class_<Vec> register_vec4(py::module& m, const char * name)
{
    typedef typename Vec::BaseType T;

    // Does a cross product not exist for Vec4? if not then thats why there is no % %= cross
    py::class_<Vec> c(m, name);
    c.def(py::init<>())
        .def(py::init<T>())
        .def(py::init<T, T, T, T>())
        .def_readwrite("x", &Vec::x)
        .def_readwrite("y", &Vec::y)
        .def_readwrite("z", &Vec::z)
        .def_readwrite("w", &Vec::z);

    register_vec<Vec>(c);

    return py::cast<py::class_<Vec>>(c);
}

void register_imath_vec(py::module& m)
{
    register_vec2<IMATH_NAMESPACE::V2s>(m, "V2s");
    register_vec2<IMATH_NAMESPACE::V2i>(m, "V2i");
    register_vec_nonint(register_vec2<IMATH_NAMESPACE::V2f>(m, "V2f"));
    register_vec_nonint(register_vec2<IMATH_NAMESPACE::V2d>(m, "V2d"));

    register_vec3<IMATH_NAMESPACE::V3s>(m, "V3s");
    register_vec3<IMATH_NAMESPACE::V3i>(m, "V3i");
    register_vec_nonint(register_vec3<IMATH_NAMESPACE::V3f>(m, "V3f"));
    register_vec_nonint(register_vec3<IMATH_NAMESPACE::V3d>(m, "V3d"));

    register_vec4<IMATH_NAMESPACE::V4s>(m, "V4s");
    register_vec4<IMATH_NAMESPACE::V4i>(m, "V4i");
    register_vec_nonint(register_vec4<IMATH_NAMESPACE::V4f>(m, "V4f"));
    register_vec_nonint(register_vec4<IMATH_NAMESPACE::V4d>(m, "V4d"));
}

}

//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include "PyBindImath.h"
#include <ImathMatrix.h>
#include <ImathMatrixAlgo.h>

namespace py = pybind11;
using namespace IMATH_NAMESPACE;

namespace {

template <class Matrix>
std::string
repr(const char* name, const Matrix& m)
{
    typedef typename Matrix::BaseType T;

    std::stringstream s;

    if constexpr (std::is_same_v<T, float>) {
        s.precision(9);
    } else if constexpr (std::is_same_v<T, double>) {
        s.precision(17);
    }
    s << std::fixed;
    s << name << "(";
    const int n = static_cast<int>(m.dimensions());
    int k = n * n;
    for (int x=0; x<n; x++)
        for (int y=0; y<n; y++)
        {
            s << m[x][y];
            if (--k > 0)
                s << ", ";
        }
    s << ")";
    return s.str();
}

template <class Matrix>
static bool
lessThan(const Matrix& mat1, const Matrix& mat2)
{
    const int n = static_cast<int>(mat1.dimensions());
    for (int x=0; x<n; x++)
        for (int y=0; y<n; y++)
            if(mat1[x][y] > mat2[x][y])
                return false;
    
    return mat1 != mat2;
}

template <class Matrix, class Vec>
static void
multDirMatrix(Matrix& mat, const Vec& src, Vec& dst)
{
    mat.multDirMatrix(src, dst);    
}

template <class Matrix, class Vec>
static Vec
multDirMatrix_return_value(Matrix& mat, const Vec& src)
{
    Vec dst;
    mat.multDirMatrix(src, dst);    
    return dst;
}

// Row proxy for M22f
template <class M>
class RowProxy {
  public:
    using T = typename M::BaseType;
    
    RowProxy(T* r) : row(r) {}

    T& operator[](size_t col)
    {
        if (col >= M::dimensions())
            throw py::index_error();
        return row[col];
    }

  private:

    T* row;
};

template <template <class> class M, template <class> class V, class T>
py::class_<M<T>>
register_matrix(py::class_<M<T>>& m, const char* name)
{
    using Matrix = M<T>;
    using Vec = V<T>;

    auto ri = py::return_value_policy::reference_internal;

    return m.def("__repr__", [name](const Matrix& self) { return repr(name, self); })
        .def(py::init([](){return Matrix();}))
        .def(py::init<T>())
        .def("__getitem__", [](Matrix& self, size_t i) {
            return RowProxy<Matrix>(self[i]);
        })
        .def("__len__", [](const Matrix& self) {
            return self.dimensions();
        })
        .def("makeIdentity", &Matrix::makeIdentity)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__iadd__", [](Matrix &self, const M<float>& other) -> const Matrix& { return self += Matrix(other); }, ri)
        .def("__iadd__", [](Matrix &self, const M<double>& other) -> const Matrix& { return self += Matrix(other); }, ri)
        .def("__iadd__", [](Matrix &self, T v) -> const Matrix& { return self += v; }, ri)
        .def("__add__", [](const Matrix &self, const Matrix &other) { return self + other; })
        .def("__add__", [](const Matrix &self, T v) { return self + v; })
        .def("__radd__", [](const Matrix &self, T v) { return Matrix(v) + self; })

        .def("__isub__", [](Matrix &self, const M<float>& other) -> const Matrix& { return self -= Matrix(other); }, ri)
        .def("__isub__", [](Matrix &self, const M<double>& other) -> const Matrix& { return self -= Matrix(other); }, ri)
        .def("__isub__", [](Matrix &self, T v) -> const Matrix& { return self -= v; }, ri)
        .def("__sub__", [](const Matrix &self, const Matrix &other) { return self - other; })
        .def("__sub__", [](const Matrix &self, T v) { return self - v; })
        .def("__rsub__", [](const Matrix &self, T v) { return Matrix(v) - self; })

        .def("negate", [](Matrix &self) -> const Matrix& { return self = -self; }, ri)
        .def("__neg__", [](Matrix &self) -> const Matrix& { return self = -self; }, ri)

        .def("__imul__", [](Matrix &self, const M<float>& other) -> const Matrix& { return self *= Matrix(other); }, ri)
        .def("__imul__", [](Matrix &self, const M<double>& other) -> const Matrix& { return self *= Matrix(other); }, ri)
        .def("__imul__", [](Matrix &self, T v) -> const Matrix& { return self *= v; }, ri)
        .def("__mul__", [](const Matrix &self, const M<float> &other) { return self * Matrix(other); })
        .def("__mul__", [](const Matrix &self, const M<double> &other) { return self * Matrix(other); })
        .def("__mul__", [](const Matrix &self, T v) { return self * v; })
        .def("__rmul__", [](const Matrix &self, const M<float> &other) { return Matrix(other) * self; })
        .def("__rmul__", [](const Matrix &self, const M<double> &other) { return Matrix(other) * self; })
        .def("__rmul__", [](const Matrix &self, T v) { return self * v; })

        .def("__idiv__", [](Matrix &self, T v) -> const Matrix& { return self /= v; }, ri)
        .def("__div__", [](const Matrix &self, T v) { return self / v; })

        .def("__itruediv__", [](Matrix &self, T v) -> const Matrix& { return self /= v; }, ri)
        .def("__truediv__", [](const Matrix &self, T v) { return self / v; })

        .def("__lt__", &lessThan<Matrix>)
        .def("__le__", [](const Matrix& a, const Matrix& b) { return !lessThan(b, a); })
        .def("__gt__", [](const Matrix& a, const Matrix& b) { return lessThan(b, a); })
        .def("__ge__", [](const Matrix& a, const Matrix& b) { return !lessThan(a, b); })
        .def("equalWithAbsError", [](Matrix& self, const Matrix& other, float e) {
            return self.equalWithAbsError(other, typename Matrix::BaseType(e));
        })
        .def("equalWithRelError", [](Matrix& self, const Matrix& other, float e) {
            return self.equalWithRelError(other, typename Matrix::BaseType(e));
        })

        .def("multDirMatrix", &multDirMatrix<Matrix,Vec>, "mult matrix")
        .def("multDirMatrix", &multDirMatrix_return_value<Matrix,Vec>, "mult matrix")
   
        ;
    
}
    
template <class T>
py::class_<Matrix22<T>>
register_matrix22(py::module& module, const char * name)
{
    py::class_<Matrix22<T>> m(module, name);
    m.def(py::init<T,T,T,T>())
        .def(py::init([](std::tuple<T, T> row0, std::tuple<T, T> row1) {
            return Matrix22<T>(std::get<0>(row0), std::get<1>(row0),
                               std::get<0>(row1), std::get<1>(row1));
        }))

        ;
    
    register_matrix<Matrix22, Vec2, T>(m, name);
    return py::cast<py::class_<Matrix22<T>>>(m);
}

template <class T>
py::class_<Matrix33<T>>
register_matrix33(py::module& module, const char * name)
{
    py::class_<Matrix33<T>> m(module, name);
    m.def(py::init<T,T,T,T,T,T,T,T,T>())
        .def(py::init([](std::tuple<T, T, T> row0, std::tuple<T, T, T> row1, std::tuple<T, T, T> row2) {
            return Matrix33<T>(std::get<0>(row0), std::get<1>(row0), std::get<2>(row0),
                               std::get<0>(row1), std::get<1>(row1), std::get<2>(row1),
                               std::get<0>(row2), std::get<1>(row2), std::get<2>(row2));
        }))
        ;
    
    register_matrix<Matrix33, Vec2, T>(m, name);
    return py::cast<py::class_<Matrix33<T>>>(m);
}

template <class M>
void
register_rowproxy(py::module& module, const char* name)
{
    using T = typename M::BaseType;

    py::class_<RowProxy<M>>(module, name, py::module_local())
        .def("__getitem__", [](RowProxy<M>& r, size_t i) -> T& {
            return r[i];
        })
        .def("__setitem__", [](RowProxy<M>& r, size_t i, T val) {
            r[i] = val;
        });
}
    
} // namespace

namespace PyBindImath {

void
register_imath_matrix(py::module& module)
{
    register_rowproxy<M22f>(module, "RowProxy22f");
    register_rowproxy<M22d>(module, "RowProxy22d");
    register_rowproxy<M33f>(module, "RowProxy33f");
    register_rowproxy<M33d>(module, "RowProxy33d");
        
    auto m22f = register_matrix22<float>(module, "M22f");
    auto m22d = register_matrix22<double>(module, "M22d");

    auto m33f = register_matrix33<float>(module, "M33f");
    auto m33d = register_matrix33<double>(module, "M33d");
}

} // namespace PyBindImath


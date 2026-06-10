//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

//
//
// Functions operating on Matrix22, Matrix33, and Matrix44 types
//
// This file also defines a few predefined constant matrices.
//

#ifndef INCLUDED_IMATHMATRIXALGO_H
#define INCLUDED_IMATHMATRIXALGO_H

#include "ImathEuler.h"
#include "ImathExport.h"
#include "ImathMatrix.h"
#include "ImathNamespace.h"
#include "ImathQuat.h"
#include "ImathVec.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <math.h>

IMATH_INTERNAL_NAMESPACE_HEADER_ENTER

//------------------
// Identity matrices
//------------------

/// M22f identity matrix
IMATH_EXPORT_CONST M22f identity22f;
/// M33f identity matrix
IMATH_EXPORT_CONST M33f identity33f;
/// M44f identity matrix
IMATH_EXPORT_CONST M44f identity44f;
/// M22d identity matrix
IMATH_EXPORT_CONST M22d identity22d;
/// M33d identity matrix
IMATH_EXPORT_CONST M33d identity33d;
/// M44d identity matrix
IMATH_EXPORT_CONST M44d identity44d;

//----------------------------------------------------------------------
// Extract scale, shear, rotation, and translation values from a matrix:
//
// Notes:
//
// This implementation follows the technique described in the paper by
// Spencer W. Thomas in the Graphics Gems II article: "Decomposing a
// Matrix into Simple Transformations", p. 320.
//
// - Some of the functions below have an optional exc parameter
//   that determines the functions' behavior when the matrix'
//   scaling is very close to zero:
//
//   If exc is true, the functions throw a std::domain_error exception.
//
//   If exc is false:
//
//      extractScaling (m, s)            returns false, s is invalid
//	sansScaling (m)		         returns m
//	removeScaling (m)	         returns false, m is unchanged
//      sansScalingAndShear (m)          returns m
//      removeScalingAndShear (m)        returns false, m is unchanged
//      extractAndRemoveScalingAndShear (m, s, h)
//                                       returns false, m is unchanged,
//                                                      (sh) are invalid
//      checkForZeroScaleInRow ()        returns false
//	extractSHRT (m, s, h, r, t)      returns false, (shrt) are invalid
//
// - Functions extractEuler(), extractEulerXYZ() and extractEulerZYX()
//   assume that the matrix does not include shear or non-uniform scaling,
//   but they do not examine the matrix to verify this assumption.
//   Matrices with shear or non-uniform scaling are likely to produce
//   meaningless results.  Therefore, you should use the
//   removeScalingAndShear() routine, if necessary, prior to calling
//   extractEuler...() .
//
// - All functions assume that the matrix does not include perspective
//   transformation(s), but they do not examine the matrix to verify
//   this assumption.  Matrices with perspective transformations are
//   likely to produce meaningless results.
//
//----------------------------------------------------------------------

//
// Declarations for 4x4 matrix.
//

/// Extract the scaling component of the given 4x4 matrix.
///
/// @param[in] mat The input matrix
/// @param[out] scl The extracted scale, i.e. the output value
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T>
bool extractScaling (const Matrix44<T>& mat, Vec3<T>& scl, bool exc = true);

/// Return the given 4x4 matrix with scaling removed.
///
/// @param[in] mat The input matrix
/// @param[in] exc If true, throw an exception if the scaling in `mat`
template <class T>
Matrix44<T> sansScaling (const Matrix44<T>& mat, bool exc = true);

/// Remove scaling from the given 4x4 matrix in place.  Return true if the
/// scale could be successfully extracted, false if the matrix is
/// degenerate.
//
/// @param[in] mat The matrix to operate on
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T> bool removeScaling (Matrix44<T>& mat, bool exc = true);

/// Extract the scaling and shear components of the given 4x4 matrix.
/// Return true if the scale could be successfully extracted, false if
/// the matrix is degenerate.
///
/// @param[in] mat The input matrix
/// @param[out] scl The extracted scale
/// @param[out] shr The extracted shear
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T>
bool extractScalingAndShear (
    const Matrix44<T>& mat, Vec3<T>& scl, Vec3<T>& shr, bool exc = true);

/// Return the given 4x4 matrix with scaling and shear removed.
///
/// @param[in] mat The input matrix
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
template <class T>
Matrix44<T> sansScalingAndShear (const Matrix44<T>& mat, bool exc = true);

/// Extract scaling and shear from the given 4x4 matrix in-place.
///
/// @param[in,out] result The output matrix
/// @param[in] mat The return value if `result` is degenerate
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
template <class T>
void sansScalingAndShear (
    Matrix44<T>& result, const Matrix44<T>& mat, bool exc = true);

/// Remove scaling and shear from the given 4x4 matrix in place.
//
/// @param[in,out] mat The matrix to operate on
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T>
bool removeScalingAndShear (Matrix44<T>& mat, bool exc = true);

/// Remove scaling and shear from the given 4x4 matrix in place, returning
/// the extracted values.
//
/// @param[in,out] mat The matrix to operate on
/// @param[out] scl The extracted scale
/// @param[out] shr The extracted shear
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T>
bool extractAndRemoveScalingAndShear (
    Matrix44<T>& mat, Vec3<T>& scl, Vec3<T>& shr, bool exc = true);

/// Extract the rotation from the given 4x4 matrix in the form of XYZ
/// euler angles.
///
/// @param[in] mat The input matrix
/// @param[out] rot The extracted XYZ euler angle vector
template <class T> void extractEulerXYZ (const Matrix44<T>& mat, Vec3<T>& rot);

/// Extract the rotation from the given 4x4 matrix in the form of ZYX
/// euler angles.
///
/// @param[in] mat The input matrix
/// @param[out] rot The extracted ZYX euler angle vector
template <class T> void extractEulerZYX (const Matrix44<T>& mat, Vec3<T>& rot);

/// Extract the rotation from the given 4x4 matrix in the form of a quaternion.
///
/// @param[in] mat The input matrix
/// @return The extracted quaternion
template <class T> Quat<T> extractQuat (const Matrix44<T>& mat);

/// Extract the scaling, shear, rotation, and translation components
/// of the given 4x4 matrix. The values are such that:
///
///     M = S * H * R * T
///
/// @param[in] mat The input matrix
/// @param[out] s The extracted scale
/// @param[out] h The extracted shear
/// @param[out] r The extracted rotation
/// @param[out] t The extracted translation
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @param[in] rOrder The order with which to extract the rotation
/// @return True if the values could be extracted, false if the matrix is degenerate.
template <class T>
bool extractSHRT (
    const Matrix44<T>&       mat,
    Vec3<T>&                 s,
    Vec3<T>&                 h,
    Vec3<T>&                 r,
    Vec3<T>&                 t,
    bool                     exc /*= true*/,
    typename Euler<T>::Order rOrder);

/// Extract the scaling, shear, rotation, and translation components
/// of the given 4x4 matrix.
///
/// @param[in] mat The input matrix
/// @param[out] s The extracted scale
/// @param[out] h The extracted shear
/// @param[out] r The extracted rotation, in XYZ euler angles
/// @param[out] t The extracted translation
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the values could be extracted, false if the matrix is degenerate.
template <class T>
bool extractSHRT (
    const Matrix44<T>& mat,
    Vec3<T>&           s,
    Vec3<T>&           h,
    Vec3<T>&           r,
    Vec3<T>&           t,
    bool               exc = true);

/// Extract the scaling, shear, rotation, and translation components
/// of the given 4x4 matrix.
///
/// @param[in] mat The input matrix
/// @param[out] s The extracted scale
/// @param[out] h The extracted shear
/// @param[out] r The extracted rotation, in Euler angles
/// @param[out] t The extracted translation
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the values could be extracted, false if the matrix is degenerate.
template <class T>
bool extractSHRT (
    const Matrix44<T>& mat,
    Vec3<T>&           s,
    Vec3<T>&           h,
    Euler<T>&          r,
    Vec3<T>&           t,
    bool               exc = true);

/// Return true if the given scale can be removed from the given row
/// matrix, false if `scl` is small enough that the operation would
/// overflow. If `exc` is true, throw an exception on overflow.
template <class T>
bool checkForZeroScaleInRow (const T& scl, const Vec3<T>& row, bool exc = true);

/// Return the 4x4 outer product two 4-vectors
template <class T>
Matrix44<T> outerProduct (const Vec4<T>& a, const Vec4<T>& b);

///
/// Return a 4x4 matrix that rotates the vector `fromDirection` to `toDirection`
///
template <class T>
Matrix44<T>
rotationMatrix (const Vec3<T>& fromDirection, const Vec3<T>& toDirection);

///
/// Return a 4x4 matrix that rotates the `fromDir` vector
/// so that it points towards `toDir1.  You may also
/// specify that you want the up vector to be pointing
/// in a certain direction 1upDir`.
template <class T>
Matrix44<T> rotationMatrixWithUpDir (
    const Vec3<T>& fromDir, const Vec3<T>& toDir, const Vec3<T>& upDir);

///
/// Construct a 4x4 matrix that rotates the z-axis so that it points
/// towards `targetDir`.  You must also specify that you want the up
/// vector to be pointing in a certain direction `upDir`.
///
/// Notes: The following degenerate cases are handled:
/// (a) when the directions given by `toDir` and `upDir`
/// are parallel or opposite (the direction vectors must have a non-zero cross product);
/// (b) when any of the given direction vectors have zero length
///
/// @param[out] result The output matrix
/// @param[in] targetDir The target direction vector
/// @param[in] upDir The up direction vector
template <class T>
void
alignZAxisWithTargetDir (Matrix44<T>& result, Vec3<T> targetDir, Vec3<T> upDir);

/// Compute an orthonormal direct 4x4 frame from a position, an x axis
/// direction and a normal to the y axis. If the x axis and normal are
/// perpendicular, then the normal will have the same direction as the
/// z axis.
///
/// @param[in] p The position of the frame
/// @param[in] xDir The x axis direction of the frame
/// @param[in] normal A normal to the y axis of the frame
/// @return The orthonormal frame
template <class T>
Matrix44<T> computeLocalFrame (
    const Vec3<T>& p, const Vec3<T>& xDir, const Vec3<T>& normal);

/// Add a translate/rotate/scale offset to a 4x4 input frame
/// and put it in another frame of reference
///
/// @param[in] inMat Input frame
/// @param[in] tOffset Translation offset
/// @param[in] rOffset Rotation offset in degrees
/// @param[in] sOffset Scale offset
/// @param[in] ref Frame of reference
/// @return The offsetted frame
template <class T>
Matrix44<T> addOffset (
    const Matrix44<T>& inMat,
    const Vec3<T>&     tOffset,
    const Vec3<T>&     rOffset,
    const Vec3<T>&     sOffset,
    const Vec3<T>&     ref);

/// Compute 4x4 translate/rotate/scale matrix from `A` with the
/// rotate/scale of `B`.
///
/// @param[in] keepRotateA If true, keep rotate from matrix `A`, use `B` otherwise
/// @param[in] keepScaleA If true, keep scale  from matrix `A`, use `B` otherwise
/// @param[in] A Matrix A
/// @param[in] B Matrix B
/// @return Matrix `A` with tweaked rotation/scale
template <class T>
Matrix44<T> computeRSMatrix (
    bool               keepRotateA,
    bool               keepScaleA,
    const Matrix44<T>& A,
    const Matrix44<T>& B);

//
// Declarations for 3x3 matrix.
//

/// Extract the scaling component of the given 3x3 matrix.
///
/// @param[in] mat The input matrix
/// @param[out] scl The extracted scale, i.e. the output value
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T>
bool extractScaling (const Matrix33<T>& mat, Vec2<T>& scl, bool exc = true);

/// Return the given 3x3 matrix with scaling removed.
///
/// @param[in] mat The input matrix
/// @param[in] exc If true, throw an exception if the scaling in `mat`
template <class T>
Matrix33<T> sansScaling (const Matrix33<T>& mat, bool exc = true);

/// Remove scaling from the given 3x3 matrix in place.  Return true if the
/// scale could be successfully extracted, false if the matrix is
/// degenerate.
//
/// @param[in] mat The matrix to operate on
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T> bool removeScaling (Matrix33<T>& mat, bool exc = true);

/// Extract the scaling and shear components of the given 3x3 matrix.
/// Return true if the scale could be successfully extracted, false if
/// the matrix is degenerate.
///
/// @param[in] mat The input matrix
/// @param[out] scl The extracted scale
/// @param[out] shr The extracted shear
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T>
bool extractScalingAndShear (
    const Matrix33<T>& mat, Vec2<T>& scl, T& shr, bool exc = true);

/// Return the given 3x3 matrix with scaling and shear removed.
///
/// @param[in] mat The input matrix
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
template <class T>
Matrix33<T> sansScalingAndShear (const Matrix33<T>& mat, bool exc = true);

/// Remove scaling and shear from the given 3x3e matrix in place.
//
/// @param[in,out] mat The matrix to operate on
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T>
bool removeScalingAndShear (Matrix33<T>& mat, bool exc = true);

/// Remove scaling and shear from the given 3x3 matrix in place, returning
/// the extracted values.
//
/// @param[in,out] mat The matrix to operate on
/// @param[out] scl The extracted scale
/// @param[out] shr The extracted shear
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the scale could be extracted, false if the matrix is degenerate.
template <class T>
bool extractAndRemoveScalingAndShear (
    Matrix33<T>& mat, Vec2<T>& scl, T& shr, bool exc = true);

/// Extract the rotation from the given 2x2 matrix
///
/// @param[in] mat The input matrix
/// @param[out] rot The extracted rotation value
template <class T> void extractEuler (const Matrix22<T>& mat, T& rot);

/// Extract the rotation from the given 3x3 matrix
///
/// @param[in] mat The input matrix
/// @param[out] rot The extracted rotation value
template <class T> void extractEuler (const Matrix33<T>& mat, T& rot);

/// Extract the scaling, shear, rotation, and translation components
/// of the given 3x3 matrix. The values are such that:
///
///     M = S * H * R * T
///
/// @param[in] mat The input matrix
/// @param[out] s The extracted scale
/// @param[out] h The extracted shear
/// @param[out] r The extracted rotation
/// @param[out] t The extracted translation
/// @param[in] exc If true, throw an exception if the scaling in `mat` is very close to zero.
/// @return True if the values could be extracted, false if the matrix is degenerate.
template <class T>
bool extractSHRT (
    const Matrix33<T>& mat,
    Vec2<T>&           s,
    T&                 h,
    T&                 r,
    Vec2<T>&           t,
    bool               exc = true);

/// Return true if the given scale can be removed from the given row
/// matrix, false if `scl` is small enough that the operation would
/// overflow. If `exc` is true, throw an exception on overflow.
template <class T>
bool checkForZeroScaleInRow (const T& scl, const Vec2<T>& row, bool exc = true);

/// Return the 3xe outer product two 3-vectors
template <class T>
Matrix33<T> outerProduct (const Vec3<T>& a, const Vec3<T>& b);

//------------------------------
// Implementation for 4x4 Matrix
//------------------------------

template <class T>
bool
extractScaling (const Matrix44<T>& mat, Vec3<T>& scl, bool exc)
{
    Vec3<T>     shr;
    Matrix44<T> M (mat);

    if (!extractAndRemoveScalingAndShear (M, scl, shr, exc)) return false;

    return true;
}

template <class T>
Matrix44<T>
sansScaling (const Matrix44<T>& mat, bool exc)
{
    Vec3<T> scl;
    Vec3<T> shr;
    Vec3<T> rot;
    Vec3<T> tran;

    if (!extractSHRT (mat, scl, shr, rot, tran, exc)) return mat;

    Matrix44<T> M;

    M.translate (tran);
    M.rotate (rot);
    M.shear (shr);

    return M;
}

template <class T>
bool
removeScaling (Matrix44<T>& mat, bool exc)
{
    Vec3<T> scl;
    Vec3<T> shr;
    Vec3<T> rot;
    Vec3<T> tran;

    if (!extractSHRT (mat, scl, shr, rot, tran, exc)) return false;

    mat.makeIdentity ();
    mat.translate (tran);
    mat.rotate (rot);
    mat.shear (shr);

    return true;
}

template <class T>
bool
extractScalingAndShear (
    const Matrix44<T>& mat, Vec3<T>& scl, Vec3<T>& shr, bool exc)
{
    Matrix44<T> M (mat);

    if (!extractAndRemoveScalingAndShear (M, scl, shr, exc)) return false;

    return true;
}

template <class T>
Matrix44<T>
sansScalingAndShear (const Matrix44<T>& mat, bool exc)
{
    Vec3<T>     scl;
    Vec3<T>     shr;
    Matrix44<T> M (mat);

    if (!extractAndRemoveScalingAndShear (M, scl, shr, exc)) return mat;

    return M;
}

template <class T>
void
sansScalingAndShear (Matrix44<T>& result, const Matrix44<T>& mat, bool exc)
{
    Vec3<T> scl;
    Vec3<T> shr;

    if (!extractAndRemoveScalingAndShear (result, scl, shr, exc)) result = mat;
}

template <class T>
bool
removeScalingAndShear (Matrix44<T>& mat, bool exc)
{
    Vec3<T> scl;
    Vec3<T> shr;

    if (!extractAndRemoveScalingAndShear (mat, scl, shr, exc)) return false;

    return true;
}

template <class T>
bool
extractAndRemoveScalingAndShear (
    Matrix44<T>& mat, Vec3<T>& scl, Vec3<T>& shr, bool exc)
{
    //
    // This implementation follows the technique described in the paper by
    // Spencer W. Thomas in the Graphics Gems II article: "Decomposing a
    // Matrix into Simple Transformations", p. 320.
    //

    Vec3<T> row[3];

    row[0] = Vec3<T> (mat[0][0], mat[0][1], mat[0][2]);
    row[1] = Vec3<T> (mat[1][0], mat[1][1], mat[1][2]);
    row[2] = Vec3<T> (mat[2][0], mat[2][1], mat[2][2]);

    T maxVal = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (IMATH_INTERNAL_NAMESPACE::abs (row[i][j]) > maxVal)
                maxVal = IMATH_INTERNAL_NAMESPACE::abs (row[i][j]);

    //
    // We normalize the 3x3 matrix here.
    // It was noticed that this can improve numerical stability significantly,
    // especially when many of the upper 3x3 matrix's coefficients are very
    // close to zero; we correct for this step at the end by multiplying the
    // scaling factors by maxVal at the end (shear and rotation are not
    // affected by the normalization).

    if (maxVal != 0)
    {
        for (int i = 0; i < 3; i++)
            if (!checkForZeroScaleInRow (maxVal, row[i], exc))
                return false;
            else
                row[i] /= maxVal;
    }

    // Compute X scale factor.
    scl.x = row[0].length ();
    if (!checkForZeroScaleInRow (scl.x, row[0], exc)) return false;

    // Normalize first row.
    row[0] /= scl.x;

    // An XY shear factor will shear the X coord. as the Y coord. changes.
    // There are 6 combinations (XY, XZ, YZ, YX, ZX, ZY), although we only
    // extract the first 3 because we can effect the last 3 by shearing in
    // XY, XZ, YZ combined rotations and scales.
    //
    // shear matrix <   1,  YX,  ZX,  0,
    //                 XY,   1,  ZY,  0,
    //                 XZ,  YZ,   1,  0,
    //                  0,   0,   0,  1 >

    // Compute XY shear factor and make 2nd row orthogonal to 1st.
    shr[0] = row[0].dot (row[1]);
    row[1] -= shr[0] * row[0];

    // Now, compute Y scale.
    scl.y = row[1].length ();
    if (!checkForZeroScaleInRow (scl.y, row[1], exc)) return false;

    // Normalize 2nd row and correct the XY shear factor for Y scaling.
    row[1] /= scl.y;
    shr[0] /= scl.y;

    // Compute XZ and YZ shears, orthogonalize 3rd row.
    shr[1] = row[0].dot (row[2]);
    row[2] -= shr[1] * row[0];
    shr[2] = row[1].dot (row[2]);
    row[2] -= shr[2] * row[1];

    // Next, get Z scale.
    scl.z = row[2].length ();
    if (!checkForZeroScaleInRow (scl.z, row[2], exc)) return false;

    // Normalize 3rd row and correct the XZ and YZ shear factors for Z scaling.
    row[2] /= scl.z;
    shr[1] /= scl.z;
    shr[2] /= scl.z;

    // At this point, the upper 3x3 matrix in mat is orthonormal.
    // Check for a coordinate system flip. If the determinant
    // is less than zero, then negate the matrix and the scaling factors.
    if (row[0].dot (row[1].cross (row[2])) < 0)
        for (int i = 0; i < 3; i++)
        {
            scl[i] *= -1;
            row[i] *= -1;
        }

    // Copy over the orthonormal rows into the returned matrix.
    // The upper 3x3 matrix in mat is now a rotation matrix.
    for (int i = 0; i < 3; i++)
    {
        mat[i][0] = row[i][0];
        mat[i][1] = row[i][1];
        mat[i][2] = row[i][2];
    }

    // Correct the scaling factors for the normalization step that we
    // performed above; shear and rotation are not affected by the
    // normalization.
    scl *= maxVal;

    return true;
}

template <class T>
void
extractEulerXYZ (const Matrix44<T>& mat, Vec3<T>& rot)
{
    //
    // Normalize the local x, y and z axes to remove scaling.
    //

    Vec3<T> i (mat[0][0], mat[0][1], mat[0][2]);
    Vec3<T> j (mat[1][0], mat[1][1], mat[1][2]);
    Vec3<T> k (mat[2][0], mat[2][1], mat[2][2]);

    i.normalize ();
    j.normalize ();
    k.normalize ();

    Matrix44<T> M (
        i[0],
        i[1],
        i[2],
        0,
        j[0],
        j[1],
        j[2],
        0,
        k[0],
        k[1],
        k[2],
        0,
        0,
        0,
        0,
        1);

    //
    // Extract the first angle, rot.x.
    //

    rot.x = std::atan2 (M[1][2], M[2][2]);

    //
    // Remove the rot.x rotation from M, so that the remaining
    // rotation, N, is only around two axes, and gimbal lock
    // cannot occur.
    //

    Matrix44<T> N;
    N.rotate (Vec3<T> (-rot.x, 0, 0));
    N = N * M;

    //
    // Extract the other two angles, rot.y and rot.z, from N.
    //

    T cy  = std::sqrt (N[0][0] * N[0][0] + N[0][1] * N[0][1]);
    rot.y = std::atan2 (-N[0][2], cy);
    rot.z = std::atan2 (-N[1][0], N[1][1]);
}

template <class T>
void
extractEulerZYX (const Matrix44<T>& mat, Vec3<T>& rot)
{
    //
    // Normalize the local x, y and z axes to remove scaling.
    //

    Vec3<T> i (mat[0][0], mat[0][1], mat[0][2]);
    Vec3<T> j (mat[1][0], mat[1][1], mat[1][2]);
    Vec3<T> k (mat[2][0], mat[2][1], mat[2][2]);

    i.normalize ();
    j.normalize ();
    k.normalize ();

    Matrix44<T> M (
        i[0],
        i[1],
        i[2],
        0,
        j[0],
        j[1],
        j[2],
        0,
        k[0],
        k[1],
        k[2],
        0,
        0,
        0,
        0,
        1);

    //
    // Extract the first angle, rot.x.
    //

    rot.x = -std::atan2 (M[1][0], M[0][0]);

    //
    // Remove the x rotation from M, so that the remaining
    // rotation, N, is only around two axes, and gimbal lock
    // cannot occur.
    //

    Matrix44<T> N;
    N.rotate (Vec3<T> (0, 0, -rot.x));
    N = N * M;

    //
    // Extract the other two angles, rot.y and rot.z, from N.
    //

    T cy  = std::sqrt (N[2][2] * N[2][2] + N[2][1] * N[2][1]);
    rot.y = -std::atan2 (-N[2][0], cy);
    rot.z = -std::atan2 (-N[1][2], N[1][1]);
}

template <class T>
Quat<T>
extractQuat (const Matrix44<T>& mat)
{
    T       tr, s;
    T       q[4];
    int     i, j, k;
    Quat<T> quat;

    int nxt[3] = {1, 2, 0};
    tr         = mat[0][0] + mat[1][1] + mat[2][2];

    // check the diagonal
    if (tr > 0.0)
    {
        s      = std::sqrt (tr + T (1.0));
        quat.r = s / T (2.0);
        s      = T (0.5) / s;

        quat.v.x = (mat[1][2] - mat[2][1]) * s;
        quat.v.y = (mat[2][0] - mat[0][2]) * s;
        quat.v.z = (mat[0][1] - mat[1][0]) * s;
    }
    else
    {
        // diagonal is negative
        i = 0;
        if (mat[1][1] > mat[0][0]) i = 1;
        if (mat[2][2] > mat[i][i]) i = 2;

        j = nxt[i];
        k = nxt[j];
        s = std::sqrt ((mat[i][i] - (mat[j][j] + mat[k][k])) + T (1.0));

        q[i] = s * T (0.5);
        if (s != T (0.0)) s = T (0.5) / s;

        q[3] = (mat[j][k] - mat[k][j]) * s;
        q[j] = (mat[i][j] + mat[j][i]) * s;
        q[k] = (mat[i][k] + mat[k][i]) * s;

        quat.v.x = q[0];
        quat.v.y = q[1];
        quat.v.z = q[2];
        quat.r   = q[3];
    }

    return quat;
}

template <class T>
bool
extractSHRT (
    const Matrix44<T>&       mat,
    Vec3<T>&                 s,
    Vec3<T>&                 h,
    Vec3<T>&                 r,
    Vec3<T>&                 t,
    bool                     exc /* = true */,
    typename Euler<T>::Order rOrder /* = Euler<T>::XYZ */)
{
    Matrix44<T> rot;

    rot = mat;
    if (!extractAndRemoveScalingAndShear (rot, s, h, exc)) return false;

    extractEulerXYZ (rot, r);

    t.x = mat[3][0];
    t.y = mat[3][1];
    t.z = mat[3][2];

    if (rOrder != Euler<T>::XYZ)
    {
        Euler<T> eXYZ (r, Euler<T>::XYZ);
        Euler<T> e (eXYZ, rOrder);
        r = e.toXYZVector ();
    }

    return true;
}

template <class T>
bool
extractSHRT (
    const Matrix44<T>& mat,
    Vec3<T>&           s,
    Vec3<T>&           h,
    Vec3<T>&           r,
    Vec3<T>&           t,
    bool               exc)
{
    return extractSHRT (mat, s, h, r, t, exc, Euler<T>::XYZ);
}

template <class T>
bool
extractSHRT (
    const Matrix44<T>& mat,
    Vec3<T>&           s,
    Vec3<T>&           h,
    Euler<T>&          r,
    Vec3<T>&           t,
    bool               exc /* = true */)
{
    return extractSHRT (mat, s, h, r, t, exc, r.order ());
}

template <class T>
bool
checkForZeroScaleInRow (const T& scl, const Vec3<T>& row, bool exc /* = true */)
{
    for (int i = 0; i < 3; i++)
    {
        if ((abs (scl) < 1 &&
             abs (row[i]) >= std::numeric_limits<T>::max () * abs (scl)))
        {
            if (exc)
                throw std::domain_error ("Cannot remove zero scaling "
                                         "from matrix.");
            else
                return false;
        }
    }

    return true;
}

template <class T>
Matrix44<T>
outerProduct (const Vec4<T>& a, const Vec4<T>& b)
{
    return Matrix44<T> (
        a.x * b.x,
        a.x * b.y,
        a.x * b.z,
        a.x * b.w,
        a.y * b.x,
        a.y * b.y,
        a.y * b.z,
        a.x * b.w,
        a.z * b.x,
        a.z * b.y,
        a.z * b.z,
        a.x * b.w,
        a.w * b.x,
        a.w * b.y,
        a.w * b.z,
        a.w * b.w);
}

template <class T>
Matrix44<T>
rotationMatrix (const Vec3<T>& from, const Vec3<T>& to)
{
    Quat<T> q;
    q.setRotation (from, to);
    return q.toMatrix44 ();
}

template <class T>
Matrix44<T>
rotationMatrixWithUpDir (
    const Vec3<T>& fromDir, const Vec3<T>& toDir, const Vec3<T>& upDir)
{
    //
    // The goal is to obtain a rotation matrix that takes
    // "fromDir" to "toDir".  We do this in two steps and
    // compose the resulting rotation matrices;
    //    (a) rotate "fromDir" into the z-axis
    //    (b) rotate the z-axis into "toDir"
    //

    // The from direction must be non-zero; but we allow zero to and up dirs.
    if (fromDir.length () == 0)
        return Matrix44<T> ();

    else
    {
        Matrix44<T> zAxis2FromDir (UNINITIALIZED);
        alignZAxisWithTargetDir (zAxis2FromDir, fromDir, Vec3<T> (0, 1, 0));

        Matrix44<T> fromDir2zAxis = zAxis2FromDir.transposed ();

        Matrix44<T> zAxis2ToDir (UNINITIALIZED);
        alignZAxisWithTargetDir (zAxis2ToDir, toDir, upDir);

        return fromDir2zAxis * zAxis2ToDir;
    }
}

template <class T>
void
alignZAxisWithTargetDir (Matrix44<T>& result, Vec3<T> targetDir, Vec3<T> upDir)
{
    //
    // Ensure that the target direction is non-zero.
    //

    if (targetDir.length () == 0) targetDir = Vec3<T> (0, 0, 1);

    //
    // Ensure that the up direction is non-zero.
    //

    if (upDir.length () == 0) upDir = Vec3<T> (0, 1, 0);

    //
    // Check for degeneracies.  If the upDir and targetDir are parallel
    // or opposite, then compute a new, arbitrary up direction that is
    // not parallel or opposite to the targetDir.
    //

    if (upDir.cross (targetDir).length () == 0)
    {
        upDir = targetDir.cross (Vec3<T> (1, 0, 0));
        if (upDir.length () == 0) upDir = targetDir.cross (Vec3<T> (0, 0, 1));
    }

    //
    // Compute the x-, y-, and z-axis vectors of the new coordinate system.
    //

    Vec3<T> targetPerpDir = upDir.cross (targetDir);
    Vec3<T> targetUpDir   = targetDir.cross (targetPerpDir);

    //
    // Rotate the x-axis into targetPerpDir (row 0),
    // rotate the y-axis into targetUpDir   (row 1),
    // rotate the z-axis into targetDir     (row 2).
    //

    Vec3<T> row[3];
    row[0] = targetPerpDir.normalized ();
    row[1] = targetUpDir.normalized ();
    row[2] = targetDir.normalized ();

    result.x[0][0] = row[0][0];
    result.x[0][1] = row[0][1];
    result.x[0][2] = row[0][2];
    result.x[0][3] = (T) 0;

    result.x[1][0] = row[1][0];
    result.x[1][1] = row[1][1];
    result.x[1][2] = row[1][2];
    result.x[1][3] = (T) 0;

    result.x[2][0] = row[2][0];
    result.x[2][1] = row[2][1];
    result.x[2][2] = row[2][2];
    result.x[2][3] = (T) 0;

    result.x[3][0] = (T) 0;
    result.x[3][1] = (T) 0;
    result.x[3][2] = (T) 0;
    result.x[3][3] = (T) 1;
}

// Compute an orthonormal direct frame from : a position, an x axis direction and a normal to the y axis
// If the x axis and normal are perpendicular, then the normal will have the same direction as the z axis.
// Inputs are :
//     -the position of the frame
//     -the x axis direction of the frame
//     -a normal to the y axis of the frame
// Return is the orthonormal frame
template <class T>
Matrix44<T>
computeLocalFrame (const Vec3<T>& p, const Vec3<T>& xDir, const Vec3<T>& normal)
{
    Vec3<T> _xDir (xDir);
    Vec3<T> x = _xDir.normalize ();
    Vec3<T> y = (normal % x).normalize ();
    Vec3<T> z = (x % y).normalize ();

    Matrix44<T> L;
    L[0][0] = x[0];
    L[0][1] = x[1];
    L[0][2] = x[2];
    L[0][3] = 0.0;

    L[1][0] = y[0];
    L[1][1] = y[1];
    L[1][2] = y[2];
    L[1][3] = 0.0;

    L[2][0] = z[0];
    L[2][1] = z[1];
    L[2][2] = z[2];
    L[2][3] = 0.0;

    L[3][0] = p[0];
    L[3][1] = p[1];
    L[3][2] = p[2];
    L[3][3] = 1.0;

    return L;
}

/// Add a translate/rotate/scale offset to an input frame and put it
/// in another frame of reference.
///
/// @param inMat input frame
/// @param tOffset translate offset
/// @param rOffset rotate offset in degrees
/// @param sOffset scale offset
/// @param ref Frame of reference
/// @return The offsetted frame
template <class T>
Matrix44<T>
addOffset (
    const Matrix44<T>& inMat,
    const Vec3<T>&     tOffset,
    const Vec3<T>&     rOffset,
    const Vec3<T>&     sOffset,
    const Matrix44<T>& ref)
{
    Matrix44<T> O;

    Vec3<T> _rOffset (rOffset);
    _rOffset *= T(M_PI / 180.0);
    O.rotate (_rOffset);

    O[3][0] = tOffset[0];
    O[3][1] = tOffset[1];
    O[3][2] = tOffset[2];

    Matrix44<T> S;
    S.scale (sOffset);

    Matrix44<T> X = S * O * inMat * ref;

    return X;
}

// Compute Translate/Rotate/Scale matrix from matrix A with the Rotate/Scale of Matrix B
// Inputs are :
//      -keepRotateA : if true keep rotate from matrix A, use B otherwise
//      -keepScaleA  : if true keep scale  from matrix A, use B otherwise
//      -Matrix A
//      -Matrix B
// Return Matrix A with tweaked rotation/scale
template <class T>
Matrix44<T>
computeRSMatrix (
    bool               keepRotateA,
    bool               keepScaleA,
    const Matrix44<T>& A,
    const Matrix44<T>& B)
{
    Vec3<T> as, ah, ar, at;
    if (!extractSHRT (A, as, ah, ar, at))
        throw std::domain_error ("degenerate A matrix in computeRSMatrix");

    Vec3<T> bs, bh, br, bt;
    if (!extractSHRT (B, bs, bh, br, bt))
        throw std::domain_error ("degenerate B matrix in computeRSMatrix");

    if (!keepRotateA) ar = br;

    if (!keepScaleA) as = bs;

    Matrix44<T> mat;
    mat.makeIdentity ();
    mat.translate (at);
    mat.rotate (ar);
    mat.scale (as);

    return mat;
}

//-----------------------------------------------------------------------------
// Implementation for 3x3 Matrix
//------------------------------

template <class T>
bool
extractScaling (const Matrix33<T>& mat, Vec2<T>& scl, bool exc)
{
    T           shr;
    Matrix33<T> M (mat);

    if (!extractAndRemoveScalingAndShear (M, scl, shr, exc)) return false;

    return true;
}

template <class T>
Matrix33<T>
sansScaling (const Matrix33<T>& mat, bool exc)
{
    Vec2<T> scl;
    T       shr;
    T       rot;
    Vec2<T> tran;

    if (!extractSHRT (mat, scl, shr, rot, tran, exc)) return mat;

    Matrix33<T> M;

    M.translate (tran);
    M.rotate (rot);
    M.shear (shr);

    return M;
}

template <class T>
bool
removeScaling (Matrix33<T>& mat, bool exc)
{
    Vec2<T> scl;
    T       shr;
    T       rot;
    Vec2<T> tran;

    if (!extractSHRT (mat, scl, shr, rot, tran, exc)) return false;

    mat.makeIdentity ();
    mat.translate (tran);
    mat.rotate (rot);
    mat.shear (shr);

    return true;
}

template <class T>
bool
extractScalingAndShear (const Matrix33<T>& mat, Vec2<T>& scl, T& shr, bool exc)
{
    Matrix33<T> M (mat);

    if (!extractAndRemoveScalingAndShear (M, scl, shr, exc)) return false;

    return true;
}

template <class T>
Matrix33<T>
sansScalingAndShear (const Matrix33<T>& mat, bool exc)
{
    Vec2<T>     scl;
    T           shr;
    Matrix33<T> M (mat);

    if (!extractAndRemoveScalingAndShear (M, scl, shr, exc)) return mat;

    return M;
}

template <class T>
bool
removeScalingAndShear (Matrix33<T>& mat, bool exc)
{
    Vec2<T> scl;
    T       shr;

    if (!extractAndRemoveScalingAndShear (mat, scl, shr, exc)) return false;

    return true;
}

template <class T>
bool
extractAndRemoveScalingAndShear (
    Matrix33<T>& mat, Vec2<T>& scl, T& shr, bool exc)
{
    Vec2<T> row[2];

    row[0] = Vec2<T> (mat[0][0], mat[0][1]);
    row[1] = Vec2<T> (mat[1][0], mat[1][1]);

    T maxVal = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (IMATH_INTERNAL_NAMESPACE::abs (mat[i][j]) > maxVal)
                maxVal = IMATH_INTERNAL_NAMESPACE::abs (mat[i][j]);

    //
    // We normalize the 2x2 matrix here.
    // It was noticed that this can improve numerical stability significantly,
    // especially when many of the upper 2x2 matrix's coefficients are very
    // close to zero; we correct for this step at the end by multiplying the
    // scaling factors by maxVal at the end (shear and rotation are not
    // affected by the normalization).

    if (maxVal != 0)
    {
        for (int i = 0; i < 2; i++)
            if (!checkForZeroScaleInRow (maxVal, row[i], exc))
                return false;
            else
                row[i] /= maxVal;
    }

    // Compute X scale factor.
    scl.x = row[0].length ();
    if (!checkForZeroScaleInRow (scl.x, row[0], exc)) return false;

    // Normalize first row.
    row[0] /= scl.x;

    // An XY shear factor will shear the X coord. as the Y coord. changes.
    // There are 2 combinations (XY, YX), although we only extract the XY
    // shear factor because we can effect the an YX shear factor by
    // shearing in XY combined with rotations and scales.
    //
    // shear matrix <   1,  YX,  0,
    //                 XY,   1,  0,
    //                  0,   0,  1 >

    // Compute XY shear factor and make 2nd row orthogonal to 1st.
    shr = row[0].dot (row[1]);
    row[1] -= shr * row[0];

    // Now, compute Y scale.
    scl.y = row[1].length ();
    if (!checkForZeroScaleInRow (scl.y, row[1], exc)) return false;

    // Normalize 2nd row and correct the XY shear factor for Y scaling.
    row[1] /= scl.y;
    shr /= scl.y;

    // At this point, the upper 2x2 matrix in mat is orthonormal.
    // Check for a coordinate system flip. If the determinant
    // is -1, then flip the rotation matrix and adjust the scale(Y)
    // and shear(XY) factors to compensate.
    if (row[0].x * row[1].y - row[0].y * row[1].x < 0)
    {
        row[1].x *= -1;
        row[1].y *= -1;
        scl.y *= -1;
        shr *= -1;
    }

    // Copy over the orthonormal rows into the returned matrix.
    // The upper 2x2 matrix in mat is now a rotation matrix.
    for (int i = 0; i < 2; i++)
    {
        mat[i][0] = row[i].x;
        mat[i][1] = row[i].y;
    }

    scl *= maxVal;

    return true;
}

template <class T>
void
extractEuler (const Matrix22<T>& mat, T& rot)
{
    //
    // Normalize the local x and y axes to remove scaling.
    //

    Vec2<T> i (mat[0][0], mat[0][1]);
    Vec2<T> j (mat[1][0], mat[1][1]);

    i.normalize ();
    j.normalize ();

    //
    // Extract the angle, rot.
    //

    rot = -std::atan2 (j[0], i[0]);
}

template <class T>
void
extractEuler (const Matrix33<T>& mat, T& rot)
{
    //
    // Normalize the local x and y axes to remove scaling.
    //

    Vec2<T> i (mat[0][0], mat[0][1]);
    Vec2<T> j (mat[1][0], mat[1][1]);

    i.normalize ();
    j.normalize ();

    //
    // Extract the angle, rot.
    //

    rot = -std::atan2 (j[0], i[0]);
}

template <class T>
bool
extractSHRT (
    const Matrix33<T>& mat, Vec2<T>& s, T& h, T& r, Vec2<T>& t, bool exc)
{
    Matrix33<T> rot;

    rot = mat;
    if (!extractAndRemoveScalingAndShear (rot, s, h, exc)) return false;

    extractEuler (rot, r);

    t.x = mat[2][0];
    t.y = mat[2][1];

    return true;
}

/// @cond Doxygen_Suppress
template <class T>
bool
checkForZeroScaleInRow (const T& scl, const Vec2<T>& row, bool exc /* = true */)
{
    for (int i = 0; i < 2; i++)
    {
        if ((abs (scl) < 1 &&
             abs (row[i]) >= std::numeric_limits<T>::max () * abs (scl)))
        {
            if (exc)
                throw std::domain_error (
                    "Cannot remove zero scaling from matrix.");
            else
                return false;
        }
    }

    return true;
}
/// @endcond

template <class T>
Matrix33<T>
outerProduct (const Vec3<T>& a, const Vec3<T>& b)
{
    return Matrix33<T> (
        a.x * b.x,
        a.x * b.y,
        a.x * b.z,
        a.y * b.x,
        a.y * b.y,
        a.y * b.z,
        a.z * b.x,
        a.z * b.y,
        a.z * b.z);
}

/// Computes the translation and rotation that brings the 'from' points
/// as close as possible to the 'to' points under the Frobenius norm.
/// To be more specific, let x be the matrix of 'from' points and y be
/// the matrix of 'to' points, we want to find the matrix A of the form
///    [ R t ]
///    [ 0 1 ]
/// that minimizes
///     || (A*x - y)^T * W * (A*x - y) ||_F
/// If doScaling is true, then a uniform scale is allowed also.
/// @param A From points
/// @param B To points
/// @param weights Per-point weights
/// @param numPoints The number of points in `A`, `B`, and `weights` (must be equal)
/// @param doScaling If true, include a scaling transformation
/// @return The procrustes transformation
template <typename T>
M44d procrustesRotationAndTranslation (
    const Vec3<T>* A,
    const Vec3<T>* B,
    const T*       weights,
    const size_t   numPoints,
    const bool     doScaling = false);

/// Computes the translation and rotation that brings the 'from' points
/// as close as possible to the 'to' points under the Frobenius norm.
/// To be more specific, let x be the matrix of 'from' points and y be
/// the matrix of 'to' points, we want to find the matrix A of the form
///    [ R t ]
///    [ 0 1 ]
/// that minimizes
///     || (A*x - y)^T * W * (A*x - y) ||_F
/// If doScaling is true, then a uniform scale is allowed also.
/// @param A From points
/// @param B To points
/// @param numPoints The number of points in `A` and `B` (must be equal)
/// @param doScaling If true, include a scaling transformation
/// @return The procrustes transformation
template <typename T>
M44d procrustesRotationAndTranslation (
    const Vec3<T>* A,
    const Vec3<T>* B,
    const size_t   numPoints,
    const bool     doScaling = false);

/// Compute the SVD of a 3x3 matrix using Jacobi transformations.  This method
/// should be quite accurate (competitive with LAPACK) even for poorly
/// conditioned matrices, and because it has been written specifically for the
/// 3x3/4x4 case it is much faster than calling out to LAPACK.
///
/// The SVD of a 3x3/4x4 matrix A is defined as follows:
///     A = U * S * V^T
/// where S is the diagonal matrix of singular values and both U and V are
/// orthonormal.  By convention, the entries S are all positive and sorted from
/// the largest to the smallest.  However, some uses of this function may
/// require that the matrix U*V^T have positive determinant; in this case, we
/// may make the smallest singular value negative to ensure that this is
/// satisfied.
///
/// Currently only available for single- and double-precision matrices.
template <typename T>
void jacobiSVD (
    const Matrix33<T>& A,
    Matrix33<T>&       U,
    Vec3<T>&           S,
    Matrix33<T>&       V,
    const T            tol = std::numeric_limits<T>::epsilon (),
    const bool         forcePositiveDeterminant = false);

/// Compute the SVD of a 3x3 matrix using Jacobi transformations.  This method
/// should be quite accurate (competitive with LAPACK) even for poorly
/// conditioned matrices, and because it has been written specifically for the
/// 3x3/4x4 case it is much faster than calling out to LAPACK.
///
/// The SVD of a 3x3/4x4 matrix A is defined as follows:
///     A = U * S * V^T
/// where S is the diagonal matrix of singular values and both U and V are
/// orthonormal.  By convention, the entries S are all positive and sorted from
/// the largest to the smallest.  However, some uses of this function may
/// require that the matrix U*V^T have positive determinant; in this case, we
/// may make the smallest singular value negative to ensure that this is
/// satisfied.
///
/// Currently only available for single- and double-precision matrices.
template <typename T>
void jacobiSVD (
    const Matrix44<T>& A,
    Matrix44<T>&       U,
    Vec4<T>&           S,
    Matrix44<T>&       V,
    const T            tol = std::numeric_limits<T>::epsilon (),
    const bool         forcePositiveDeterminant = false);

/// Compute the eigenvalues (S) and the eigenvectors (V) of a real
/// symmetric matrix using Jacobi transformation, using a given
/// tolerance `tol`.
///
/// Jacobi transformation of a 3x3/4x4 matrix A outputs S and V:
/// 	A = V * S * V^T
/// where V is orthonormal and S is the diagonal matrix of eigenvalues.
/// Input matrix A must be symmetric. A is also modified during
/// the computation so that upper diagonal entries of A become zero.
template <typename T>
void
jacobiEigenSolver (Matrix33<T>& A, Vec3<T>& S, Matrix33<T>& V, const T tol);

/// Compute the eigenvalues (S) and the eigenvectors (V) of
/// a real symmetric matrix using Jacobi transformation.
///
/// Jacobi transformation of a 3x3/4x4 matrix A outputs S and V:
/// 	A = V * S * V^T
/// where V is orthonormal and S is the diagonal matrix of eigenvalues.
/// Input matrix A must be symmetric. A is also modified during
/// the computation so that upper diagonal entries of A become zero.
template <typename T>
inline void
jacobiEigenSolver (Matrix33<T>& A, Vec3<T>& S, Matrix33<T>& V)
{
    jacobiEigenSolver (A, S, V, std::numeric_limits<T>::epsilon ());
}

/// Compute the eigenvalues (S) and the eigenvectors (V) of a real
/// symmetric matrix using Jacobi transformation, using a given
/// tolerance `tol`.
///
/// Jacobi transformation of a 3x3/4x4 matrix A outputs S and V:
/// 	A = V * S * V^T
/// where V is orthonormal and S is the diagonal matrix of eigenvalues.
/// Input matrix A must be symmetric. A is also modified during
/// the computation so that upper diagonal entries of A become zero.
template <typename T>
void
jacobiEigenSolver (Matrix44<T>& A, Vec4<T>& S, Matrix44<T>& V, const T tol);

/// Compute the eigenvalues (S) and the eigenvectors (V) of
/// a real symmetric matrix using Jacobi transformation.
///
/// Jacobi transformation of a 3x3/4x4 matrix A outputs S and V:
/// 	A = V * S * V^T
/// where V is orthonormal and S is the diagonal matrix of eigenvalues.
/// Input matrix A must be symmetric. A is also modified during
/// the computation so that upper diagonal entries of A become zero.
template <typename T>
inline void
jacobiEigenSolver (Matrix44<T>& A, Vec4<T>& S, Matrix44<T>& V)
{
    jacobiEigenSolver (A, S, V, std::numeric_limits<T>::epsilon ());
}

/// Compute a eigenvector corresponding to the abs max eigenvalue
/// of a real symmetric matrix using Jacobi transformation.
template <typename TM, typename TV> void maxEigenVector (TM& A, TV& S);

/// Compute a eigenvector corresponding to the abs min eigenvalue
/// of a real symmetric matrix using Jacobi transformation.
template <typename TM, typename TV> void minEigenVector (TM& A, TV& S);

namespace
{

class KahanSum
{
public:
    KahanSum () : _total (0), _correction (0) {}

    void operator+= (const double val)
    {
        const double y = val - _correction;
        const double t = _total + y;
        _correction    = (t - _total) - y;
        _total         = t;
    }

    double get () const { return _total; }

private:
    double _total;
    double _correction;
};

} // namespace

template <typename T>
M44d
procrustesRotationAndTranslation (
    const Vec3<T>* A,
    const Vec3<T>* B,
    const T*       weights,
    const size_t   numPoints,
    const bool     doScale)
{
    if (numPoints == 0) return M44d ();

    // Always do the accumulation in double precision:
    V3d    Acenter (0.0);
    V3d    Bcenter (0.0);
    double weightsSum = 0.0;

    if (weights == 0)
    {
        for (size_t i = 0; i < numPoints; ++i)
        {
            Acenter += (V3d) A[i];
            Bcenter += (V3d) B[i];
        }
        weightsSum = (double) numPoints;
    }
    else
    {
        for (size_t i = 0; i < numPoints; ++i)
        {
            const double w = weights[i];
            weightsSum += w;

            Acenter += w * (V3d) A[i];
            Bcenter += w * (V3d) B[i];
        }
    }

    if (weightsSum == 0) return M44d ();

    Acenter /= weightsSum;
    Bcenter /= weightsSum;

    //
    // Find Q such that |Q*A - B|  (actually A-Acenter and B-Bcenter, weighted)
    // is minimized in the least squares sense.
    // From Golub/Van Loan, p.601
    //
    // A,B are 3xn
    // Let C = B A^T   (where A is 3xn and B^T is nx3, so C is 3x3)
    // Compute the SVD: C = U D V^T  (U,V rotations, D diagonal).
    // Throw away the D part, and return Q = U V^T
    M33d C (0.0);
    if (weights == 0)
    {
        for (size_t i = 0; i < numPoints; ++i)
            C += outerProduct ((V3d) B[i] - Bcenter, (V3d) A[i] - Acenter);
    }
    else
    {
        for (size_t i = 0; i < numPoints; ++i)
        {
            const double w = weights[i];
            C +=
                outerProduct (w * ((V3d) B[i] - Bcenter), (V3d) A[i] - Acenter);
        }
    }

    M33d U, V;
    V3d  S;
    jacobiSVD (C, U, S, V, std::numeric_limits<double>::epsilon (), true);

    // We want Q.transposed() here since we are going to be using it in the
    // Imath style (multiplying vectors on the right, v' = v*A^T):
    const M33d Qt = V * U.transposed ();

    double s = 1.0;
    if (doScale && numPoints > 1)
    {
        // Finding a uniform scale: let us assume the Q is completely fixed
        // at this point (solving for both simultaneously seems much harder).
        // We are trying to compute (again, per Golub and van Loan)
        //    min || s*A*Q - B ||_F
        // Notice that we've jammed a uniform scale in front of the Q.
        // Now, the Frobenius norm (the least squares norm over matrices)
        // has the neat property that it is equivalent to minimizing the trace
        // of M^T*M (see your friendly neighborhood linear algebra text for a
        // derivation).  Thus, we can expand this out as
        //   min tr (s*A*Q - B)^T*(s*A*Q - B)
        // = min tr(Q^T*A^T*s*s*A*Q) + tr(B^T*B) - 2*tr(Q^T*A^T*s*B)  by linearity of the trace
        // = min s^2 tr(A^T*A) + tr(B^T*B) - 2*s*tr(Q^T*A^T*B)        using the fact that the trace is invariant
        //                                                            under similarity transforms Q*M*Q^T
        // If we differentiate w.r.t. s and set this to 0, we get
        // 0 = 2*s*tr(A^T*A) - 2*tr(Q^T*A^T*B)
        // so
        // 2*s*tr(A^T*A) = 2*s*tr(Q^T*A^T*B)
        // s = tr(Q^T*A^T*B) / tr(A^T*A)

        KahanSum traceATA;
        if (weights == 0)
        {
            for (size_t i = 0; i < numPoints; ++i)
                traceATA += ((V3d) A[i] - Acenter).length2 ();
        }
        else
        {
            for (size_t i = 0; i < numPoints; ++i)
                traceATA +=
                    ((double) weights[i]) * ((V3d) A[i] - Acenter).length2 ();
        }

        KahanSum traceBATQ;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                traceBATQ += Qt[j][i] * C[i][j];

        s = traceBATQ.get () / traceATA.get ();
    }

    // Q is the rotation part of what we want to return.
    // The entire transform is:
    //    (translate origin to Bcenter) * Q * (translate Acenter to origin)
    //                last                                first
    // The effect of this on a point is:
    //    (translate origin to Bcenter) * Q * (translate Acenter to origin) * point
    //  = (translate origin to Bcenter) * Q * (-Acenter + point)
    //  = (translate origin to Bcenter) * (-Q*Acenter + Q*point)
    //  = (translate origin to Bcenter) * (translate Q*Acenter to origin) * Q*point
    //  = (translate Q*Acenter to Bcenter) * Q*point
    // So what we want to return is:
    //    (translate Q*Acenter to Bcenter) * Q
    //
    // In block form, this is:
    //   [ 1 0 0  | ] [       0 ] [ 1 0 0  |  ]   [ 1 0 0  | ] [           |   ]   [                 ]
    //   [ 0 1 0 tb ] [  s*Q  0 ] [ 0 1 0 -ta ] = [ 0 1 0 tb ] [  s*Q  -s*Q*ta ] = [   Q   tb-s*Q*ta ]
    //   [ 0 0 1  | ] [       0 ] [ 0 0 1  |  ]   [ 0 0 1  | ] [           |   ]   [                 ]
    //   [ 0 0 0  1 ] [ 0 0 0 1 ] [ 0 0 0  1  ]   [ 0 0 0  1 ] [ 0 0 0     1   ]   [ 0 0 0    1      ]
    // (ofc the whole thing is transposed for Imath).
    const V3d translate = Bcenter - s * Acenter * Qt;

    return M44d (
        s * Qt.x[0][0],
        s * Qt.x[0][1],
        s * Qt.x[0][2],
        T (0),
        s * Qt.x[1][0],
        s * Qt.x[1][1],
        s * Qt.x[1][2],
        T (0),
        s * Qt.x[2][0],
        s * Qt.x[2][1],
        s * Qt.x[2][2],
        T (0),
        translate.x,
        translate.y,
        translate.z,
        T (1));
} // procrustesRotationAndTranslation

///
/// Return the procrustes transformation of a set of points: the
/// rotation, translation, and optionally the scale that comes closest
/// in a least squares sense to transforming the `A` points into
/// `B`.
///

template <typename T>
M44d
procrustesRotationAndTranslation (
    const Vec3<T>* A,
    const Vec3<T>* B,
    const size_t   numPoints,
    const bool     doScale)
{
    return procrustesRotationAndTranslation (
        A, B, (const T*) 0, numPoints, doScale);
} // procrustesRotationAndTranslation

namespace
{

// Applies the 2x2 Jacobi rotation
//   [  c s 0 ]    [ 1  0 0 ]    [  c 0 s ]
//   [ -s c 0 ] or [ 0  c s ] or [  0 1 0 ]
//   [  0 0 1 ]    [ 0 -s c ]    [ -s 0 c ]
// from the right; that is, computes
//   J * A
// for the Jacobi rotation J and the matrix A.  This is efficient because we
// only need to touch exactly the 2 columns that are affected, so we never
// need to explicitly construct the J matrix.
template <typename T, int j, int k>
void
jacobiRotateRight (
    IMATH_INTERNAL_NAMESPACE::Matrix33<T>& A, const T c, const T s)
{
    for (int i = 0; i < 3; ++i)
    {
        const T tau1 = A[i][j];
        const T tau2 = A[i][k];
        A[i][j]      = c * tau1 - s * tau2;
        A[i][k]      = s * tau1 + c * tau2;
    }
}

template <typename T>
void
jacobiRotateRight (
    IMATH_INTERNAL_NAMESPACE::Matrix44<T>& A,
    const int                              j,
    const int                              k,
    const T                                c,
    const T                                s)
{
    for (int i = 0; i < 4; ++i)
    {
        const T tau1 = A[i][j];
        const T tau2 = A[i][k];
        A[i][j]      = c * tau1 - s * tau2;
        A[i][k]      = s * tau1 + c * tau2;
    }
}

// This routine solves the 2x2 SVD:
//     [  c1   s1 ] [ w   x ] [  c2  s2 ]   [ d1    0 ]
//     [          ] [       ] [         ] = [         ]
//     [ -s1   c1 ] [ y   z ] [ -s2  c2 ]   [  0   d2 ]
// where
//      [ w   x ]
//  A = [       ]
//      [ y   z ]
// is the subset of A consisting of the [j,k] entries, A([j k], [j k]) in
// Matlab parlance.  The method is the 'USVD' algorithm described in the
// following paper:
//    'Computation of the Singular Value Decomposition using Mesh-Connected Processors'
//    by Richard P. Brent, Franklin T. Luk, and Charles Van Loan
// It breaks the computation into two steps: the first symmetrizes the matrix,
// and the second diagonalizes the symmetric matrix.
template <typename T, int j, int k, int l>
bool
twoSidedJacobiRotation (
    IMATH_INTERNAL_NAMESPACE::Matrix33<T>& A,
    IMATH_INTERNAL_NAMESPACE::Matrix33<T>& U,
    IMATH_INTERNAL_NAMESPACE::Matrix33<T>& V,
    const T                                tol)
{
    // Load everything into local variables to make things easier on the
    // optimizer:
    const T w = A[j][j];
    const T x = A[j][k];
    const T y = A[k][j];
    const T z = A[k][k];

    // We will keep track of whether we're actually performing any rotations,
    // since if the matrix is already diagonal we'll end up with the identity
    // as our Jacobi rotation and we can short-circuit.
    bool changed = false;

    // The first step is to symmetrize the 2x2 matrix,
    //   [ c  s ]^T [ w x ] = [ p q ]
    //   [ -s c ]   [ y z ]   [ q r ]
    T mu_1 = w + z;
    T mu_2 = x - y;

    T c, s;
    if (std::abs (mu_2) <=
        tol * std::abs (mu_1)) // Already symmetric (to tolerance)
    {                          // Note that the <= is important here
        c = T (1);             // because we want to bypass the computation
        s = T (0);             // of rho if mu_1 = mu_2 = 0.

        const T p = w;
        const T r = z;
        mu_1      = r - p;
        mu_2      = x + y;
    }
    else
    {
        // TODO is there a native inverse square root function?
        const T rho = mu_1 / mu_2;

        s = T (1) / std::sqrt (T (1) + rho * rho);
        if (rho < 0) s = -s;
        c = s * rho;

        mu_1 = s * (x + y) + c * (z - w); // = r - p
        mu_2 = T (2) * (c * x - s * z);   // = 2*q

        changed = true;
    }

    // The second stage diagonalizes,
    //   [ c2   s2 ]^T [ p q ] [ c2  s2 ]  = [ d1   0 ]
    //   [ -s2  c2 ]   [ q r ] [ -s2 c2 ]    [  0  d2 ]
    T c_2, s_2;
    if (std::abs (mu_2) <= tol * std::abs (mu_1))
    {
        c_2 = T (1);
        s_2 = T (0);
    }
    else
    {
        const T rho_2 = mu_1 / mu_2;
        T t_2 = T (1) / (std::abs (rho_2) + std::sqrt (1 + rho_2 * rho_2));
        if (rho_2 < 0) t_2 = -t_2;
        c_2 = T (1) / std::sqrt (T (1) + t_2 * t_2);
        s_2 = c_2 * t_2;

        changed = true;
    }

    const T c_1 = c_2 * c - s_2 * s;
    const T s_1 = s_2 * c + c_2 * s;

    if (!changed)
    {
        // We've decided that the off-diagonal entries are already small
        // enough, so we'll set them to zero.  This actually appears to result
        // in smaller errors than leaving them be, possibly because it prevents
        // us from trying to do extra rotations later that we don't need.
        A[k][j] = 0;
        A[j][k] = 0;
        return false;
    }

    const T d_1 = c_1 * (w * c_2 - x * s_2) - s_1 * (y * c_2 - z * s_2);
    const T d_2 = s_1 * (w * s_2 + x * c_2) + c_1 * (y * s_2 + z * c_2);

    // For the entries we just zeroed out, we'll just set them to 0, since
    // they should be 0 up to machine precision.
    A[j][j] = d_1;
    A[k][k] = d_2;
    A[k][j] = 0;
    A[j][k] = 0;

    // Rotate the entries that _weren't_ involved in the 2x2 SVD:
    {
        // Rotate on the left by
        //    [  c1 s1 0 ]^T      [  c1 0 s1 ]^T      [ 1   0  0 ]^T
        //    [ -s1 c1 0 ]    or  [   0 1  0 ]    or  [ 0  c1 s1 ]
        //    [   0  0 1 ]        [ -s1 0 c1 ]        [ 0 -s1 c1 ]
        // This has the effect of adding the (weighted) ith and jth _rows_ to
        // each other.
        const T tau1 = A[j][l];
        const T tau2 = A[k][l];
        A[j][l]      = c_1 * tau1 - s_1 * tau2;
        A[k][l]      = s_1 * tau1 + c_1 * tau2;
    }

    {
        // Rotate on the right by
        //    [  c2 s2 0 ]      [  c2 0 s2 ]      [ 1   0  0 ]
        //    [ -s2 c2 0 ]  or  [   0 1  0 ]  or  [ 0  c2 s2 ]
        //    [   0  0 1 ]      [ -s2 0 c2 ]      [ 0 -s2 c2 ]
        // This has the effect of adding the (weighted) ith and jth _columns_ to
        // each other.
        const T tau1 = A[l][j];
        const T tau2 = A[l][k];
        A[l][j]      = c_2 * tau1 - s_2 * tau2;
        A[l][k]      = s_2 * tau1 + c_2 * tau2;
    }

    // Now apply the rotations to U and V:
    // Remember that we have
    //    R1^T * A * R2 = D
    // This is in the 2x2 case, but after doing a bunch of these
    // we will get something like this for the 3x3 case:
    //   ... R1b^T * R1a^T * A * R2a * R2b * ... = D
    //   -----------------       ---------------
    //        = U^T                    = V
    // So,
    //   U = R1a * R1b * ...
    //   V = R2a * R2b * ...
    jacobiRotateRight<T, j, k> (U, c_1, s_1);
    jacobiRotateRight<T, j, k> (V, c_2, s_2);

    return true;
}

template <typename T>
bool
twoSidedJacobiRotation (
    IMATH_INTERNAL_NAMESPACE::Matrix44<T>& A,
    int                                    j,
    int                                    k,
    IMATH_INTERNAL_NAMESPACE::Matrix44<T>& U,
    IMATH_INTERNAL_NAMESPACE::Matrix44<T>& V,
    const T                                tol)
{
    // Load everything into local variables to make things easier on the
    // optimizer:
    const T w = A[j][j];
    const T x = A[j][k];
    const T y = A[k][j];
    const T z = A[k][k];

    // We will keep track of whether we're actually performing any rotations,
    // since if the matrix is already diagonal we'll end up with the identity
    // as our Jacobi rotation and we can short-circuit.
    bool changed = false;

    // The first step is to symmetrize the 2x2 matrix,
    //   [ c  s ]^T [ w x ] = [ p q ]
    //   [ -s c ]   [ y z ]   [ q r ]
    T mu_1 = w + z;
    T mu_2 = x - y;

    T c, s;
    if (std::abs (mu_2) <=
        tol * std::abs (mu_1)) // Already symmetric (to tolerance)
    {                          // Note that the <= is important here
        c = T (1);             // because we want to bypass the computation
        s = T (0);             // of rho if mu_1 = mu_2 = 0.

        const T p = w;
        const T r = z;
        mu_1      = r - p;
        mu_2      = x + y;
    }
    else
    {
        // TODO is there a native inverse square root function?
        const T rho = mu_1 / mu_2;

        s = T (1) / std::sqrt (T (1) + rho * rho);
        if (rho < 0) s = -s;
        c = s * rho;

        mu_1 = s * (x + y) + c * (z - w); // = r - p
        mu_2 = T (2) * (c * x - s * z);   // = 2*q

        changed = true;
    }

    // The second stage diagonalizes,
    //   [ c2   s2 ]^T [ p q ] [ c2  s2 ]  = [ d1   0 ]
    //   [ -s2  c2 ]   [ q r ] [ -s2 c2 ]    [  0  d2 ]
    T c_2, s_2;
    if (std::abs (mu_2) <= tol * std::abs (mu_1))
    {
        c_2 = T (1);
        s_2 = T (0);
    }
    else
    {
        const T rho_2 = mu_1 / mu_2;
        T t_2 = T (1) / (std::abs (rho_2) + std::sqrt (1 + rho_2 * rho_2));
        if (rho_2 < 0) t_2 = -t_2;
        c_2 = T (1) / std::sqrt (T (1) + t_2 * t_2);
        s_2 = c_2 * t_2;

        changed = true;
    }

    const T c_1 = c_2 * c - s_2 * s;
    const T s_1 = s_2 * c + c_2 * s;

    if (!changed)
    {
        // We've decided that the off-diagonal entries are already small
        // enough, so we'll set them to zero.  This actually appears to result
        // in smaller errors than leaving them be, possibly because it prevents
        // us from trying to do extra rotations later that we don't need.
        A[k][j] = 0;
        A[j][k] = 0;
        return false;
    }

    const T d_1 = c_1 * (w * c_2 - x * s_2) - s_1 * (y * c_2 - z * s_2);
    const T d_2 = s_1 * (w * s_2 + x * c_2) + c_1 * (y * s_2 + z * c_2);

    // For the entries we just zeroed out, we'll just set them to 0, since
    // they should be 0 up to machine precision.
    A[j][j] = d_1;
    A[k][k] = d_2;
    A[k][j] = 0;
    A[j][k] = 0;

    // Rotate the entries that _weren't_ involved in the 2x2 SVD:
    for (int l = 0; l < 4; ++l)
    {
        if (l == j || l == k) continue;

        // Rotate on the left by
        //    [ 1               ]
        //    [   .             ]
        //    [     c2   s2     ]  j
        //    [        1        ]
        //    [    -s2   c2     ]  k
        //    [             .   ]
        //    [               1 ]
        //          j    k
        //
        // This has the effect of adding the (weighted) ith and jth _rows_ to
        // each other.
        const T tau1 = A[j][l];
        const T tau2 = A[k][l];
        A[j][l]      = c_1 * tau1 - s_1 * tau2;
        A[k][l]      = s_1 * tau1 + c_1 * tau2;
    }

    for (int l = 0; l < 4; ++l)
    {
        // We set the A[j/k][j/k] entries already
        if (l == j || l == k) continue;

        // Rotate on the right by
        //    [ 1               ]
        //    [   .             ]
        //    [     c2   s2     ]  j
        //    [        1        ]
        //    [    -s2   c2     ]  k
        //    [             .   ]
        //    [               1 ]
        //          j    k
        //
        // This has the effect of adding the (weighted) ith and jth _columns_ to
        // each other.
        const T tau1 = A[l][j];
        const T tau2 = A[l][k];
        A[l][j]      = c_2 * tau1 - s_2 * tau2;
        A[l][k]      = s_2 * tau1 + c_2 * tau2;
    }

    // Now apply the rotations to U and V:
    // Remember that we have
    //    R1^T * A * R2 = D
    // This is in the 2x2 case, but after doing a bunch of these
    // we will get something like this for the 3x3 case:
    //   ... R1b^T * R1a^T * A * R2a * R2b * ... = D
    //   -----------------       ---------------
    //        = U^T                    = V
    // So,
    //   U = R1a * R1b * ...
    //   V = R2a * R2b * ...
    jacobiRotateRight (U, j, k, c_1, s_1);
    jacobiRotateRight (V, j, k, c_2, s_2);

    return true;
}

template <typename T>
void
swapColumns (IMATH_INTERNAL_NAMESPACE::Matrix33<T>& A, int j, int k)
{
    for (int i = 0; i < 3; ++i)
        std::swap (A[i][j], A[i][k]);
}

template <typename T>
IMATH_CONSTEXPR14 T
maxOffDiag (const IMATH_INTERNAL_NAMESPACE::Matrix33<T>& A)
{
    T result = 0;
    result   = std::max (result, std::abs (A[0][1]));
    result   = std::max (result, std::abs (A[0][2]));
    result   = std::max (result, std::abs (A[1][0]));
    result   = std::max (result, std::abs (A[1][2]));
    result   = std::max (result, std::abs (A[2][0]));
    result   = std::max (result, std::abs (A[2][1]));
    return result;
}

template <typename T>
IMATH_CONSTEXPR14 T
maxOffDiag (const IMATH_INTERNAL_NAMESPACE::Matrix44<T>& A)
{
    T result = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (i != j) result = std::max (result, std::abs (A[i][j]));
        }
    }

    return result;
}

template <typename T>
void
twoSidedJacobiSVD (
    IMATH_INTERNAL_NAMESPACE::Matrix33<T>  A,
    IMATH_INTERNAL_NAMESPACE::Matrix33<T>& U,
    IMATH_INTERNAL_NAMESPACE::Vec3<T>&     S,
    IMATH_INTERNAL_NAMESPACE::Matrix33<T>& V,
    const T                                tol,
    const bool                             forcePositiveDeterminant)
{
    // The two-sided Jacobi SVD works by repeatedly zeroing out
    // off-diagonal entries of the matrix, 2 at a time.  Basically,
    // we can take our 3x3 matrix,
    //    [* * *]
    //    [* * *]
    //    [* * *]
    // and use a pair of orthogonal transforms to zero out, say, the
    // pair of entries (0, 1) and (1, 0):
    //  [ c1 s1  ] [* * *] [ c2 s2  ]   [*   *]
    //  [-s1 c1  ] [* * *] [-s2 c2  ] = [  * *]
    //  [       1] [* * *] [       1]   [* * *]
    // When we go to zero out the next pair of entries (say, (0, 2) and (2, 0))
    // then we don't expect those entries to stay 0:
    //  [ c1 s1  ] [*   *] [ c2 s2  ]   [* *  ]
    //  [-s1 c1  ] [  * *] [-s2 c2  ] = [* * *]
    //  [       1] [* * *] [       1]   [  * *]
    // However, if we keep doing this, we'll find that the off-diagonal entries
    // converge to 0 fairly quickly (convergence should be roughly cubic).  The
    // result is a diagonal A matrix and a bunch of orthogonal transforms:
    //               [* * *]                [*    ]
    //  L1 L2 ... Ln [* * *] Rn ... R2 R1 = [  *  ]
    //               [* * *]                [    *]
    //  ------------ ------- ------------   -------
    //      U^T         A         V            S
    // This turns out to be highly accurate because (1) orthogonal transforms
    // are extremely stable to compute and apply (this is why QR factorization
    // works so well, FWIW) and because (2) by applying everything to the original
    // matrix A instead of computing (A^T * A) we avoid any precision loss that
    // would result from that.
    U.makeIdentity ();
    V.makeIdentity ();

    const int maxIter =
        20; // In case we get really unlucky, prevents infinite loops
    const T absTol =
        tol * maxOffDiag (A); // Tolerance is in terms of the maximum
    if (absTol != 0)          // _off-diagonal_ entry.
    {
        int numIter = 0;
        do
        {
            ++numIter;
            bool changed = twoSidedJacobiRotation<T, 0, 1, 2> (A, U, V, tol);
            changed      = twoSidedJacobiRotation<T, 0, 2, 1> (A, U, V, tol) ||
                      changed;
            changed = twoSidedJacobiRotation<T, 1, 2, 0> (A, U, V, tol) ||
                      changed;
            if (!changed) break;
        } while (maxOffDiag (A) > absTol && numIter < maxIter);
    }

    // The off-diagonal entries are (effectively) 0, so whatever's left on the
    // diagonal are the singular values:
    S.x = A[0][0];
    S.y = A[1][1];
    S.z = A[2][2];

    // Nothing thus far has guaranteed that the singular values are positive,
    // so let's go back through and flip them if not (since by contract we are
    // supposed to return all positive SVs):
    for (int i = 0; i < 3; ++i)
    {
        if (S[i] < 0)
        {
            // If we flip S[i], we need to flip the corresponding column of U
            // (we could also pick V if we wanted; it doesn't really matter):
            S[i] = -S[i];
            for (int j = 0; j < 3; ++j)
                U[j][i] = -U[j][i];
        }
    }

    // Order the singular values from largest to smallest; this requires
    // exactly two passes through the data using bubble sort:
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < (2 - i); ++j)
        {
            // No absolute values necessary since we already ensured that
            // they're positive:
            if (S[j] < S[j + 1])
            {
                // If we swap singular values we also have to swap
                // corresponding columns in U and V:
                std::swap (S[j], S[j + 1]);
                swapColumns (U, j, j + 1);
                swapColumns (V, j, j + 1);
            }
        }
    }

    if (forcePositiveDeterminant)
    {
        // We want to guarantee that the returned matrices always have positive
        // determinant.  We can do this by adding the appropriate number of
        // matrices of the form:
        //       [ 1       ]
        //  L =  [    1    ]
        //       [      -1 ]
        // Note that L' = L and L*L = Identity.  Thus we can add:
        //   U*L*L*S*V = (U*L)*(L*S)*V
        // if U has a negative determinant, and
        //   U*S*L*L*V = U*(S*L)*(L*V)
        // if V has a neg. determinant.
        if (U.determinant () < 0)
        {
            for (int i = 0; i < 3; ++i)
                U[i][2] = -U[i][2];
            S.z = -S.z;
        }

        if (V.determinant () < 0)
        {
            for (int i = 0; i < 3; ++i)
                V[i][2] = -V[i][2];
            S.z = -S.z;
        }
    }
}

template <typename T>
void
twoSidedJacobiSVD (
    IMATH_INTERNAL_NAMESPACE::Matrix44<T>  A,
    IMATH_INTERNAL_NAMESPACE::Matrix44<T>& U,
    IMATH_INTERNAL_NAMESPACE::Vec4<T>&     S,
    IMATH_INTERNAL_NAMESPACE::Matrix44<T>& V,
    const T                                tol,
    const bool                             forcePositiveDeterminant)
{
    // Please see the Matrix33 version for a detailed description of the algorithm.
    U.makeIdentity ();
    V.makeIdentity ();

    const int maxIter =
        20; // In case we get really unlucky, prevents infinite loops
    const T absTol =
        tol * maxOffDiag (A); // Tolerance is in terms of the maximum
    if (absTol != 0)          // _off-diagonal_ entry.
    {
        int numIter = 0;
        do
        {
            ++numIter;
            bool changed = twoSidedJacobiRotation (A, 0, 1, U, V, tol);
            changed = twoSidedJacobiRotation (A, 0, 2, U, V, tol) || changed;
            changed = twoSidedJacobiRotation (A, 0, 3, U, V, tol) || changed;
            changed = twoSidedJacobiRotation (A, 1, 2, U, V, tol) || changed;
            changed = twoSidedJacobiRotation (A, 1, 3, U, V, tol) || changed;
            changed = twoSidedJacobiRotation (A, 2, 3, U, V, tol) || changed;
            if (!changed) break;
        } while (maxOffDiag (A) > absTol && numIter < maxIter);
    }

    // The off-diagonal entries are (effectively) 0, so whatever's left on the
    // diagonal are the singular values:
    S.x = A[0][0];
    S.y = A[1][1];
    S.z = A[2][2];
    S.w = A[3][3];

    // Nothing thus far has guaranteed that the singular values are positive,
    // so let's go back through and flip them if not (since by contract we are
    // supposed to return all positive SVs):
    for (int i = 0; i < 4; ++i)
    {
        if (S[i] < 0)
        {
            // If we flip S[i], we need to flip the corresponding column of U
            // (we could also pick V if we wanted; it doesn't really matter):
            S[i] = -S[i];
            for (int j = 0; j < 4; ++j)
                U[j][i] = -U[j][i];
        }
    }

    // Order the singular values from largest to smallest using insertion sort:
    for (int i = 1; i < 4; ++i)
    {
        const IMATH_INTERNAL_NAMESPACE::Vec4<T> uCol (
            U[0][i], U[1][i], U[2][i], U[3][i]);
        const IMATH_INTERNAL_NAMESPACE::Vec4<T> vCol (
            V[0][i], V[1][i], V[2][i], V[3][i]);
        const T sVal = S[i];

        int j = i - 1;
        while (std::abs (S[j]) < std::abs (sVal))
        {
            for (int k = 0; k < 4; ++k)
                U[k][j + 1] = U[k][j];
            for (int k = 0; k < 4; ++k)
                V[k][j + 1] = V[k][j];
            S[j + 1] = S[j];

            --j;
            if (j < 0) break;
        }

        for (int k = 0; k < 4; ++k)
            U[k][j + 1] = uCol[k];
        for (int k = 0; k < 4; ++k)
            V[k][j + 1] = vCol[k];
        S[j + 1] = sVal;
    }

    if (forcePositiveDeterminant)
    {
        // We want to guarantee that the returned matrices always have positive
        // determinant.  We can do this by adding the appropriate number of
        // matrices of the form:
        //       [ 1          ]
        //  L =  [    1       ]
        //       [       1    ]
        //       [         -1 ]
        // Note that L' = L and L*L = Identity.  Thus we can add:
        //   U*L*L*S*V = (U*L)*(L*S)*V
        // if U has a negative determinant, and
        //   U*S*L*L*V = U*(S*L)*(L*V)
        // if V has a neg. determinant.
        if (U.determinant () < 0)
        {
            for (int i = 0; i < 4; ++i)
                U[i][3] = -U[i][3];
            S.w = -S.w;
        }

        if (V.determinant () < 0)
        {
            for (int i = 0; i < 4; ++i)
                V[i][3] = -V[i][3];
            S.w = -S.w;
        }
    }
}

} // namespace

/// TODO
template <typename T>
void
jacobiSVD (
    const IMATH_INTERNAL_NAMESPACE::Matrix33<T>& A,
    IMATH_INTERNAL_NAMESPACE::Matrix33<T>&       U,
    IMATH_INTERNAL_NAMESPACE::Vec3<T>&           S,
    IMATH_INTERNAL_NAMESPACE::Matrix33<T>&       V,
    const T                                      tol,
    const bool                                   forcePositiveDeterminant)
{
    twoSidedJacobiSVD (A, U, S, V, tol, forcePositiveDeterminant);
}

/// TODO
template <typename T>
void
jacobiSVD (
    const IMATH_INTERNAL_NAMESPACE::Matrix44<T>& A,
    IMATH_INTERNAL_NAMESPACE::Matrix44<T>&       U,
    IMATH_INTERNAL_NAMESPACE::Vec4<T>&           S,
    IMATH_INTERNAL_NAMESPACE::Matrix44<T>&       V,
    const T                                      tol,
    const bool                                   forcePositiveDeterminant)
{
    twoSidedJacobiSVD (A, U, S, V, tol, forcePositiveDeterminant);
}

namespace
{

template <int j, int k, typename TM>
inline void
jacobiRotateRight (
    TM& A, const typename TM::BaseType s, const typename TM::BaseType tau)
{
    typedef typename TM::BaseType T;

    for (unsigned int i = 0; i < TM::dimensions (); ++i)
    {
        const T nu1 = A[i][j];
        const T nu2 = A[i][k];
        A[i][j] -= s * (nu2 + tau * nu1);
        A[i][k] += s * (nu1 - tau * nu2);
    }
}

template <int j, int k, int l, typename T>
bool
jacobiRotation (Matrix33<T>& A, Matrix33<T>& V, Vec3<T>& Z, const T tol)
{
    // Load everything into local variables to make things easier on the
    // optimizer:
    const T x = A[j][j];
    const T y = A[j][k];
    const T z = A[k][k];

    // The first stage diagonalizes,
    //   [ c  s ]^T [ x y ] [ c -s ]  = [ d1   0 ]
    //   [ -s c ]   [ y z ] [ s  c ]    [  0  d2 ]
    const T mu1 = z - x;
    const T mu2 = 2 * y;

    if (std::abs (mu2) <= tol * std::abs (mu1))
    {
        // We've decided that the off-diagonal entries are already small
        // enough, so we'll set them to zero.  This actually appears to result
        // in smaller errors than leaving them be, possibly because it prevents
        // us from trying to do extra rotations later that we don't need.
        A[j][k] = 0;
        return false;
    }
    const T rho = mu1 / mu2;
    const T t   = (rho < 0 ? T (-1) : T (1)) /
                (std::abs (rho) + std::sqrt (1 + rho * rho));
    const T c   = T (1) / std::sqrt (T (1) + t * t);
    const T s   = t * c;
    const T tau = s / (T (1) + c);
    const T h   = t * y;

    // Update diagonal elements.
    Z[j] -= h;
    Z[k] += h;
    A[j][j] -= h;
    A[k][k] += h;

    // For the entries we just zeroed out, we'll just set them to 0, since
    // they should be 0 up to machine precision.
    A[j][k] = 0;

    // We only update upper triagnular elements of A, since
    // A is supposed to be symmetric.
    T&      offd1 = l < j ? A[l][j] : A[j][l];
    T&      offd2 = l < k ? A[l][k] : A[k][l];
    const T nu1   = offd1;
    const T nu2   = offd2;
    offd1         = nu1 - s * (nu2 + tau * nu1);
    offd2         = nu2 + s * (nu1 - tau * nu2);

    // Apply rotation to V
    jacobiRotateRight<j, k> (V, s, tau);

    return true;
}

template <int j, int k, int l1, int l2, typename T>
bool
jacobiRotation (Matrix44<T>& A, Matrix44<T>& V, Vec4<T>& Z, const T tol)
{
    const T x = A[j][j];
    const T y = A[j][k];
    const T z = A[k][k];

    const T mu1 = z - x;
    const T mu2 = T (2) * y;

    // Let's see if rho^(-1) = mu2 / mu1 is less than tol
    // This test also checks if rho^2 will overflow
    // when tol^(-1) < sqrt(std::numeric_limits<T>::max()).
    if (std::abs (mu2) <= tol * std::abs (mu1))
    {
        A[j][k] = 0;
        return true;
    }

    const T rho = mu1 / mu2;
    const T t   = (rho < 0 ? T (-1) : T (1)) /
                (std::abs (rho) + std::sqrt (1 + rho * rho));
    const T c   = T (1) / std::sqrt (T (1) + t * t);
    const T s   = c * t;
    const T tau = s / (T (1) + c);
    const T h   = t * y;

    Z[j] -= h;
    Z[k] += h;
    A[j][j] -= h;
    A[k][k] += h;
    A[j][k] = 0;

    {
        T&      offd1 = l1 < j ? A[l1][j] : A[j][l1];
        T&      offd2 = l1 < k ? A[l1][k] : A[k][l1];
        const T nu1   = offd1;
        const T nu2   = offd2;
        offd1 -= s * (nu2 + tau * nu1);
        offd2 += s * (nu1 - tau * nu2);
    }

    {
        T&      offd1 = l2 < j ? A[l2][j] : A[j][l2];
        T&      offd2 = l2 < k ? A[l2][k] : A[k][l2];
        const T nu1   = offd1;
        const T nu2   = offd2;
        offd1 -= s * (nu2 + tau * nu1);
        offd2 += s * (nu1 - tau * nu2);
    }

    jacobiRotateRight<j, k> (V, s, tau);

    return true;
}

template <typename TM>
IMATH_CONSTEXPR14 inline typename TM::BaseType
maxOffDiagSymm (const TM& A)
{
    typedef typename TM::BaseType T;
    T                             result = 0;
    for (unsigned int i = 0; i < TM::dimensions (); ++i)
        for (unsigned int j = i + 1; j < TM::dimensions (); ++j)
            result = std::max (result, std::abs (A[i][j]));

    return result;
}

} // namespace

template <typename T>
void
jacobiEigenSolver (Matrix33<T>& A, Vec3<T>& S, Matrix33<T>& V, const T tol)
{
    V.makeIdentity ();
    for (int i = 0; i < 3; ++i)
    {
        S[i] = A[i][i];
    }

    const int maxIter =
        20; // In case we get really unlucky, prevents infinite loops
    const T absTol =
        tol * maxOffDiagSymm (A); // Tolerance is in terms of the maximum
    if (absTol != 0)              // _off-diagonal_ entry.
    {
        int numIter = 0;
        do
        {
            // Z is for accumulating small changes (h) to diagonal entries
            // of A for one sweep. Adding h's directly to A might cause
            // a cancellation effect when h is relatively very small to
            // the corresponding diagonal entry of A and
            // this will increase numerical errors
            Vec3<T> Z (0, 0, 0);
            ++numIter;
            bool changed = jacobiRotation<0, 1, 2> (A, V, Z, tol);
            changed      = jacobiRotation<0, 2, 1> (A, V, Z, tol) || changed;
            changed      = jacobiRotation<1, 2, 0> (A, V, Z, tol) || changed;
            // One sweep passed. Add accumulated changes (Z) to singular values (S)
            // Update diagonal elements of A for better accuracy as well.
            for (int i = 0; i < 3; ++i)
            {
                A[i][i] = S[i] += Z[i];
            }
            if (!changed) break;
        } while (maxOffDiagSymm (A) > absTol && numIter < maxIter);
    }
}

template <typename T>
void
jacobiEigenSolver (Matrix44<T>& A, Vec4<T>& S, Matrix44<T>& V, const T tol)
{
    V.makeIdentity ();

    for (int i = 0; i < 4; ++i)
    {
        S[i] = A[i][i];
    }

    const int maxIter =
        20; // In case we get really unlucky, prevents infinite loops
    const T absTol =
        tol * maxOffDiagSymm (A); // Tolerance is in terms of the maximum
    if (absTol != 0)              // _off-diagonal_ entry.
    {
        int numIter = 0;
        do
        {
            ++numIter;
            Vec4<T> Z (0, 0, 0, 0);
            bool    changed = jacobiRotation<0, 1, 2, 3> (A, V, Z, tol);
            changed = jacobiRotation<0, 2, 1, 3> (A, V, Z, tol) || changed;
            changed = jacobiRotation<0, 3, 1, 2> (A, V, Z, tol) || changed;
            changed = jacobiRotation<1, 2, 0, 3> (A, V, Z, tol) || changed;
            changed = jacobiRotation<1, 3, 0, 2> (A, V, Z, tol) || changed;
            changed = jacobiRotation<2, 3, 0, 1> (A, V, Z, tol) || changed;
            for (int i = 0; i < 4; ++i)
            {
                A[i][i] = S[i] += Z[i];
            }
            if (!changed) break;
        } while (maxOffDiagSymm (A) > absTol && numIter < maxIter);
    }
}

template <typename TM, typename TV>
void
maxEigenVector (TM& A, TV& V)
{
    TV S;
    TM MV;
    jacobiEigenSolver (A, S, MV);

    int maxIdx (0);
    for (unsigned int i = 1; i < TV::dimensions (); ++i)
    {
        if (std::abs (S[i]) > std::abs (S[maxIdx])) maxIdx = i;
    }

    for (unsigned int i = 0; i < TV::dimensions (); ++i)
        V[i] = MV[i][maxIdx];
}

template <typename TM, typename TV>
void
minEigenVector (TM& A, TV& V)
{
    TV S;
    TM MV;
    jacobiEigenSolver (A, S, MV);

    int minIdx (0);
    for (unsigned int i = 1; i < TV::dimensions (); ++i)
    {
        if (std::abs (S[i]) < std::abs (S[minIdx])) minIdx = i;
    }

    for (unsigned int i = 0; i < TV::dimensions (); ++i)
        V[i] = MV[i][minIdx];
}

IMATH_INTERNAL_NAMESPACE_HEADER_EXIT

#endif // INCLUDED_IMATHMATRIXALGO_H

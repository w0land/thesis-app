// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2009 Benoit Jacob <jacob.benoit.1@gmail.com>
// Copyright (C) 2009 Gael Guennebaud <g.gael@free.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#ifndef EIGEN_HOUSEHOLDER_H
#define EIGEN_HOUSEHOLDER_H

template<int n> struct ei_decrement_size
{
  enum {
    ret = (n==1 || n==Dynamic) ? n : n-1
  };
};

template<typename EssentialPart>
void makeTrivialHouseholder(
  EssentialPart *essential,
  typename EssentialPart::RealScalar *beta)
{
  *beta = typename EssentialPart::RealScalar(0);
  essential->setZero();
}

template<typename Derived>
void MatrixBase<Derived>::makeHouseholderInPlace(Scalar *tau, RealScalar *beta)
{
  VectorBlock<Derived, ei_decrement_size<SizeAtCompileTime>::ret> essentialPart(derived(), 1, size()-1);
  makeHouseholder(&essentialPart, tau, beta);
}

/** Computes the elementary reflector H such that:
  * \f$ H *this = [ beta 0 ... 0]^T \f$
  * where the transformation H is:
  * \f$ H = I - tau v v^*\f$
  * and the vector v is:
  * \f$ v^T = [1 essential^T] \f$
  *
  * On output:
  * \param essential the essential part of the vector \c v
  * \param tau the scaling factor of the householder transformation
  * \param beta the result of H * \c *this
  *
  * \sa MatrixBase::makeHouseholderInPlace(), MatrixBase::applyHouseholderOnTheLeft(),
  *     MatrixBase::applyHouseholderOnTheRight()
  */
template<typename Derived>
template<typename EssentialPart>
void MatrixBase<Derived>::makeHouseholder(
  EssentialPart *essential,
  Scalar *tau,
  RealScalar *beta) const
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(EssentialPart)
  VectorBlock<Derived, EssentialPart::SizeAtCompileTime> tail(derived(), 1, size()-1);

  RealScalar tailSqNorm = size()==1 ? 0 : tail.squaredNorm();
  Scalar c0 = coeff(0);

  if(tailSqNorm == RealScalar(0) && ei_imag(c0)==RealScalar(0))
  {
    *tau = 0;
    *beta = ei_real(c0);
  }
  else
  {
    *beta = ei_sqrt(ei_abs2(c0) + tailSqNorm);
    if (ei_real(c0)>=0.)
      *beta = -*beta;
    *essential = tail / (c0 - *beta);
    *tau = ei_conj((*beta - c0) / *beta);
  }
}

template<typename Derived>
template<typename EssentialPart>
void MatrixBase<Derived>::applyHouseholderOnTheLeft(
  const EssentialPart& essential,
  const Scalar& tau,
  Scalar* workspace)
{
  Map<Matrix<Scalar, 1, ColsAtCompileTime, PlainMatrixType::Options, 1, MaxColsAtCompileTime> > tmp(workspace,cols());
  Block<Derived, EssentialPart::SizeAtCompileTime, Derived::ColsAtCompileTime> bottom(derived(), 1, 0, rows()-1, cols());
  tmp.noalias() = essential.adjoint() * bottom;
  tmp += row(0);
  row(0) -= tau * tmp;
  bottom.noalias() -= tau * essential * tmp;
}

template<typename Derived>
template<typename EssentialPart>
void MatrixBase<Derived>::applyHouseholderOnTheRight(
  const EssentialPart& essential,
  const Scalar& tau,
  Scalar* workspace)
{
  Map<Matrix<Scalar, RowsAtCompileTime, 1, PlainMatrixType::Options, MaxRowsAtCompileTime, 1> > tmp(workspace,rows());
  Block<Derived, Derived::RowsAtCompileTime, EssentialPart::SizeAtCompileTime> right(derived(), 0, 1, rows(), cols()-1);
  tmp.noalias() = right * essential.conjugate();
  tmp += col(0);
  col(0) -= tau * tmp;
  right.noalias() -= tau * tmp * essential.transpose();
}

#endif // EIGEN_HOUSEHOLDER_H

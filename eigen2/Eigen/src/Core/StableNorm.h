// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
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

#ifndef EIGEN_STABLENORM_H
#define EIGEN_STABLENORM_H

template<typename ExpressionType, typename Scalar>
inline void ei_stable_norm_kernel(const ExpressionType& bl, Scalar& ssq, Scalar& scale, Scalar& invScale)
{
  Scalar max = bl.cwise().abs().maxCoeff();
  if (max>scale)
  {
    ssq = ssq * ei_abs2(scale/max);
    scale = max;
    invScale = Scalar(1)/scale;
  }
  // TODO if the max is much much smaller than the current scale,
  // then we can neglect this sub vector
  ssq += (bl*invScale).squaredNorm();
}

/** \returns the \em l2 norm of \c *this avoiding underflow and overflow.
  * This version use a blockwise two passes algorithm:
  *  1 - find the absolute largest coefficient \c s
  *  2 - compute \f$ s \Vert \frac{*this}{s} \Vert \f$ in a standard way
  *
  * For architecture/scalar types supporting vectorization, this version
  * is faster than blueNorm(). Otherwise the blueNorm() is much faster.
  *
  * \sa norm(), blueNorm(), hypotNorm()
  */
template<typename Derived>
inline typename NumTraits<typename ei_traits<Derived>::Scalar>::Real
MatrixBase<Derived>::stableNorm() const
{
  const int blockSize = 4096;
  RealScalar scale = 0;
  RealScalar invScale = 1;
  RealScalar ssq = 0; // sum of square
  enum {
    Alignment = (int(Flags)&DirectAccessBit) || (int(Flags)&AlignedBit) ? ForceAligned : AsRequested
  };
  int n = size();
  int bi=0;
  if ((int(Flags)&DirectAccessBit) && !(int(Flags)&AlignedBit))
  {
    bi = ei_alignmentOffset(&const_cast_derived().coeffRef(0), n);
    if (bi>0)
      ei_stable_norm_kernel(start(bi), ssq, scale, invScale);
  }
  for (; bi<n; bi+=blockSize)
    ei_stable_norm_kernel(VectorBlock<Derived,Dynamic,Alignment>(derived(),bi,std::min(blockSize, n - bi)), ssq, scale, invScale);
  return scale * ei_sqrt(ssq);
}

/** \returns the \em l2 norm of \c *this using the Blue's algorithm.
  * A Portable Fortran Program to Find the Euclidean Norm of a Vector,
  * ACM TOMS, Vol 4, Issue 1, 1978.
  *
  * For architecture/scalar types without vectorization, this version
  * is much faster than stableNorm(). Otherwise the stableNorm() is faster.
  *
  * \sa norm(), stableNorm(), hypotNorm()
  */
template<typename Derived>
inline typename NumTraits<typename ei_traits<Derived>::Scalar>::Real
MatrixBase<Derived>::blueNorm() const
{
  static int nmax = -1;
  static RealScalar b1, b2, s1m, s2m, overfl, rbig, relerr;
  if(nmax <= 0)
  {
    int nbig, ibeta, it, iemin, iemax, iexp;
    RealScalar abig, eps;
    // This program calculates the machine-dependent constants
    // bl, b2, slm, s2m, relerr overfl, nmax
    // from the "basic" machine-dependent numbers
    // nbig, ibeta, it, iemin, iemax, rbig.
    // The following define the basic machine-dependent constants.
    // For portability, the PORT subprograms "ilmaeh" and "rlmach"
    // are used. For any specific computer, each of the assignment
    // statements can be replaced
    nbig  = std::numeric_limits<int>::max();            // largest integer
    ibeta = std::numeric_limits<RealScalar>::radix;         // base for floating-point numbers
    it    = std::numeric_limits<RealScalar>::digits;        // number of base-beta digits in mantissa
    iemin = std::numeric_limits<RealScalar>::min_exponent;  // minimum exponent
    iemax = std::numeric_limits<RealScalar>::max_exponent;  // maximum exponent
    rbig  = std::numeric_limits<RealScalar>::max();         // largest floating-point number

    iexp  = -((1-iemin)/2);
    b1    = RealScalar(std::pow(RealScalar(ibeta),RealScalar(iexp)));  // lower boundary of midrange
    iexp  = (iemax + 1 - it)/2;
    b2    = RealScalar(std::pow(RealScalar(ibeta),RealScalar(iexp)));   // upper boundary of midrange

    iexp  = (2-iemin)/2;
    s1m   = RealScalar(std::pow(RealScalar(ibeta),RealScalar(iexp)));   // scaling factor for lower range
    iexp  = - ((iemax+it)/2);
    s2m   = RealScalar(std::pow(RealScalar(ibeta),RealScalar(iexp)));   // scaling factor for upper range

    overfl  = rbig*s2m;             // overfow boundary for abig
    eps     = RealScalar(std::pow(double(ibeta), 1-it));
    relerr  = ei_sqrt(eps);         // tolerance for neglecting asml
    abig    = 1.0/eps - 1.0;
    if (RealScalar(nbig)>abig)  nmax = int(abig);  // largest safe n
    else                        nmax = nbig;
  }
  int n = size();
  RealScalar ab2 = b2 / RealScalar(n);
  RealScalar asml = RealScalar(0);
  RealScalar amed = RealScalar(0);
  RealScalar abig = RealScalar(0);
  for(int j=0; j<n; ++j)
  {
    RealScalar ax = ei_abs(coeff(j));
    if(ax > ab2)     abig += ei_abs2(ax*s2m);
    else if(ax < b1) asml += ei_abs2(ax*s1m);
    else             amed += ei_abs2(ax);
  }
  if(abig > RealScalar(0))
  {
    abig = ei_sqrt(abig);
    if(abig > overfl)
    {
      ei_assert(false && "overflow");
      return rbig;
    }
    if(amed > RealScalar(0))
    {
      abig = abig/s2m;
      amed = ei_sqrt(amed);
    }
    else
      return abig/s2m;
  }
  else if(asml > RealScalar(0))
  {
    if (amed > RealScalar(0))
    {
      abig = ei_sqrt(amed);
      amed = ei_sqrt(asml) / s1m;
    }
    else
      return ei_sqrt(asml)/s1m;
  }
  else
    return ei_sqrt(amed);
  asml = std::min(abig, amed);
  abig = std::max(abig, amed);
  if(asml <= abig*relerr)
    return abig;
  else
    return abig * ei_sqrt(RealScalar(1) + ei_abs2(asml/abig));
}

/** \returns the \em l2 norm of \c *this avoiding undeflow and overflow.
  * This version use a concatenation of hypot() calls, and it is very slow.
  *
  * \sa norm(), stableNorm()
  */
template<typename Derived>
inline typename NumTraits<typename ei_traits<Derived>::Scalar>::Real
MatrixBase<Derived>::hypotNorm() const
{
  return this->cwise().abs().redux(ei_scalar_hypot_op<RealScalar>());
}

#endif // EIGEN_STABLENORM_H

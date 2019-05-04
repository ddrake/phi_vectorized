#ifndef SIMD_UTIL_H
#define SIMD_UTIL_H


#include <fem.hpp>
using namespace ngfem;

INLINE double abs2(const Complex a) {
  return a.real()*a.real() + a.imag()*a.imag();
}

INLINE SIMD<double> abs2(const SIMD<Complex> & c)  {
  return c.real() * c.real() + c.imag() * c.imag();
}

INLINE SIMD<double> abs2(const FlatVector<SIMD<Complex>> & v) {
  SIMD<double> res(0);
  for (int i : Range(v.Size()))  res += abs2(v(i)); 
  return res;
}

// Given the size of a vector, return the size of the
// SIMD vector which will contain its elements.

int simd_size(int size)
{
  constexpr int slen = GetDefaultSIMDSize();
  int n = size / slen;
  int rem = size % slen;
  return n + (rem > 0 ? 1 : 0);
}


// Convert a generic Vector to a Vector (of smaller size)
// containing SIMD elements of the original Vector

void vec2simd(const FlatVector<double> &v, FlatVector<SIMD<double>> &sv)
{
  constexpr int slen = GetDefaultSIMDSize();
  int rem = v.Size() % slen;
  int n = v.Size() / slen;
  int k = 0;

  for (int i : Range(n)) {
    sv(i) = SIMD<double>(&v[k]);
    k+=slen;
  }
  if (rem > 0) 
    sv(n) = SIMD<double>(&v[k], SIMD<mask64>(rem));
}

// Convert a Vector of Complex values to a Vector (of smaller size)
// containing SIMD elements of the original Vector

void vec2simd(const FlatVector<Complex> &v, FlatVector<SIMD<Complex>> &sv)
{
  constexpr int slen = GetDefaultSIMDSize();
  double re[slen], im[slen];
  int rem = v.Size() % slen;
  int n = v.Size() / slen;
  int k = 0;

  for (int i : Range(n)) {
    for (int j : Range(slen)) {
      re[j]=v[k].real();
      im[j]=v[k].imag();
      k++;
    }
    sv(i) = SIMD<Complex>(SIMD<double>(re), SIMD<double>(im));
  }
  if (rem > 0) {
    for (int j : Range(rem)) {
      re[j]=v[k].real();
      im[j]=v[k].imag();
      k++;
    }
    SIMD<mask64> mask(rem);
    sv(n) = SIMD<Complex>(SIMD<double>(re, mask), SIMD<double>(im, mask));
  }  
}

  
// Convert a Matrix of Complex values to a Matrix of SIMD<Complex>
// by replacing each row of the matrix by the SIMD original row vector

void mat2simd(const Matrix<Complex> &mat, Matrix<SIMD<Complex>> &smat)
{
  for (int i : Range(mat.Height())) {
    FlatVector<SIMD<Complex>> vs(smat.Row(i));
    vec2simd(mat.Row(i), vs);
    smat.Row(i) = vs;
  }
}

template <class T>
INLINE auto SumH(const SIMD<T> a) { return HSum(a); }

INLINE auto SumH(Complex a) { return a; }

INLINE auto SumH(double  a) { return a; }

#endif

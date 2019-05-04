//  ngscxx -c explicit_ngs.cpp  ; ngsld explicit_ngs.o -lngfem

#include <fem.hpp>
#include "util.hpp"
using namespace ngfem;
using namespace std;

int main()
{
  const int n = 1001;
  const int sn = simd_size(n);
  auto v1 = Vector<>(n);
  auto v2 = Vector<>(n);
  auto v3 = Vector<SIMD<double>>(sn);
  
  cout << "declared v1, v2 v3" << endl;
  
  double x = 3;
  cout << "x = " << x << endl;
  
  for (int i=0; i<n; i++) {
    v1(i) = i / 3.0;
    v2(i) = i / 7.0;
  }
  cout << "v1(1) = " << v1(1) << endl;

  vec2simd(v1, v3);
  SIMD<double> total = 6.0;
  for (int i=0; i<sn; i++) {
    total += v3(i) * x;
  }
  cout << "total = " << HSum(total) << endl;
  
}


//  ngscxx -c simple_ngs.cpp  ; ngsld simple_ngs.o -lngfem

#include <fem.hpp>
using namespace ngfem;
using namespace std;

int main()
{
  auto v1 = Vector<double>(1000);
  auto v2 = Vector<double>(1000);
  auto v3 = Vector<double>(1000);
  
  cout << "declared v1, v2 v3" << endl;
  
  double x = 3;
  cout << "x = " << x << endl;
  
  for (int i=0; i<1000; i++) {
    v1(i) = i / 3.0;
    v2(i) = i / 7.0;
  }
  cout << "v1(1) = " << v1(1) << endl;
  
  for (int i=0; i<1000; i++) {
    v3(i) = v1(i) + v2(i);
  }
  
  double total = 6.0;
  for (int i=0; i<1000; i++) {
    total += v3(i) * x;
  }
  cout << "total = " << total << endl;
  
}


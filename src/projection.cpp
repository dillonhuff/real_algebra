#include "projection.h"

#include "algorithm.h"

using namespace std;

namespace ralg {

  std::vector<polynomial> projection_1(const int var_num,
				       const std::vector<polynomial>& polys) {
    vector<polynomial> p1;
    for (auto& p : polys) {
      concat(p1, coefficients_wrt(p, var_num));
    }
    return p1;
  }

  std::vector<polynomial> project(const int var_num,
				  const std::vector<polynomial>& polys) {
    vector<polynomial> proj1 =
      projection_1(var_num, polys);

    return proj1;
  }
  
}

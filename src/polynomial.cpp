#include "polynomial.h"

namespace ralg {

  class monomial zero_monomial(const int num_vars) {
    rational z("0.0");
    std::vector<int> coeffs;
    for (int i = 0; i < num_vars; i++) {
      coeffs.push_back(0);
    }

    return {z, coeffs, num_vars};
  }

  std::vector<polynomial> coefficients_wrt(const polynomial& p,
					   const int var_num) {
    return {};
  }

  
}

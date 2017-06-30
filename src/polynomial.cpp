#include "polynomial.h"

using namespace std;

namespace ralg {

  class monomial zero_monomial(const int num_vars) {
    rational z("0.0");
    std::vector<int> coeffs;
    for (int i = 0; i < num_vars; i++) {
      coeffs.push_back(0);
    }

    return {z, coeffs, num_vars};
  }

  int degree_wrt(const int var_num, const polynomial& p) {
    int max = 0;
    for (int i = 0; i < p.num_monos(); i++) {
      auto& m = p.monomial(i);
      int p_deg = m.power(var_num);
      if (p_deg > max) {
	max = p_deg;
      }
    }

    return max;
  }

  monomial delete_var(const int var_num, const monomial& m) {
    vector<int> vars;
    for (int i = 0; i < m.num_vars(); i++) {
      if (i != var_num) {
	vars.push_back(m.power(i));
      }
    }
    return monomial(m.coeff(), vars, m.num_vars() - 1);
  }

  std::vector<polynomial> coefficients_wrt(const polynomial& p,
					   const int var_num) {
    std::vector<std::vector<monomial> > monomial_groups(degree_wrt(var_num, p) + 1);
    for (int i = 0; i < monomial_groups.size(); i++) {
      monomial_groups[i] = {};
    }

    for (int i = 0; i < p.num_monos(); i++) {
      auto& m = p.monomial(i);
      int x_deg = m.power(var_num);
      monomial_groups[x_deg].push_back(delete_var(var_num, m));
    }

    vector<polynomial> polys;
    for (auto& ms : monomial_groups) {
      if (ms.size() > 0) {
	polys.push_back(polynomial(ms, p.num_vars() - 1));
      }
    }
    return polys;
  }

  
}

#include <ralg/polynomial.h>

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
      } else {
	polys.push_back(zero_polynomial(p.num_vars() - 1));
      }
    }
    return polys;
  }

  monomial derivative_wrt(const int var_num,
			  const monomial& p) {
    vector<int> vars;
    for (int i = 0; i < p.num_vars(); i++) {
      if (i == var_num) {
	vars.push_back(p.power(i) - 1);
      } else {
	vars.push_back(p.power(i));
      }
    }
    return monomial(rational(p.power(var_num))*p.coeff(), vars, p.num_vars());
  }
  
  polynomial derivative_wrt(const int var_num,
			    const polynomial& p) {
    vector<monomial> monos;
    for (int i = 0; i < p.num_monos(); i++) {
      monomial md = derivative_wrt(var_num, p.monomial(i));
      if (!is_zero(md)) {
	monos.push_back(md);
      }
    }
    return polynomial(monos, p.num_vars());
  }


  rational evaluate_at(const rational& val, const polynomial& p) {
    assert(p.num_vars() == 1);

    rational sum(0);
    for (int i = 0; i < p.num_monos(); i++) {
      auto m = p.monomial(i);

      rational res = m.coeff();
      for (int j = 0; j < m.power(0); j++) {
	res = res*val;
      }

      sum = sum + res;
    }
    return sum;
  }

  monomial evaluate_at(const std::vector<rational>& test_pt,
		       const monomial& m) {
    rational res = m.coeff();

    for (int i = 0; i < test_pt.size(); i++) {
      rational val = test_pt[i];
      for (int j = 0; j < m.power(i); j++) {
	res = res*val;
      }
    }

    return monomial(res, {m.power(m.num_vars() - 1)}, 1);
  }

  polynomial evaluate_at(const std::vector<rational>& test_pt,
			 const polynomial& p) {
    assert(test_pt.size() == (p.num_vars() - 1));

    polynomial sum = zero_polynomial(1);

    for (int i = 0; i < p.num_monos(); i++) {
      monomial m = evaluate_at(test_pt, p.monomial(i));
      sum = sum + polynomial({m}, 1);

      // auto m = p.monomial(i);


      // polynomial sum_mono();
      // sum = sum + res;
    }

    return sum;
  }

  polynomial pow(const polynomial& p, const int exp) {
    assert(exp >= 0);
    polynomial res = const_poly(1, p.num_vars());
    for (int i = 0; i < exp; i++) {
      res = res*p;
    }
    return res;
  }

  rational condition_number(const int coeff_num,
			    const rational& root,
			    const polynomial& p) {
    assert(coeff_num > 0);
    assert(p.num_vars() == 1);

    rational a_i = coefficients_wrt(p, 0)[coeff_num].monomial(0).coeff();

    cout << "a_i = " << a_i << endl;

    polynomial dv = derivative_wrt(0, p);

    cout << "dv = " << dv << endl;

    rational deriv = evaluate_at(root, dv);

    cout << "deriv = " << deriv << endl;

    return ((a_i + pow(root, coeff_num - 1)) / deriv).abs();
  }

  polynomial delete_var(const int var_num, const polynomial& p) {
    vector<monomial> ms;
    for (int i = 0; i < p.num_monos(); i++) {
      const class monomial& m = p.monomial(i);
      ms.push_back(delete_var(var_num, m));
    }
    return polynomial{ms, p.num_vars() - 1};
  }

}

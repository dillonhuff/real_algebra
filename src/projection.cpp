#include "projection.h"

#include "algorithm.h"
#include "matrix.h"

using namespace std;

namespace ralg {

  polynomial reductum(const int var_num,
		      const int k,
		      const polynomial& f) {
    return f.reductum(var_num, k);
  }

  polynomial psc(const int var_num,
		 const int l,
		 const polynomial sub) {
    vector<monomial> monos;
    for (int i = 0; i < sub.num_monos(); i++) {
      if (sub.monomial(i).power(var_num) == l) {
	monos.push_back(delete_var(var_num, sub.monomial(i)));
      }
    }
    return polynomial(monos, sub.num_vars() - 1);
  }

  polynomial psck(const int var_num,
		  const int k,
		  const polynomial f,
		  const polynomial g) {
    cout << "k = " << k << endl;
    polynomial sub = subresultant(var_num, k, f, g);
    cout << "Subresultant = " << sub << endl;
    return psc(var_num, k, sub);
  }
  
  std::vector<polynomial> projection_2(const int var_num,
				       const std::vector<polynomial>& polys) {
    vector<polynomial> p2;
    for (auto& f : polys) {
      // Believe that the description of proj2 in Mats Jirstrands
      // introduction has a typo, should be k <= d_i, not k <= d_i - 1
      for (int k = 0; k <= degree_wrt(var_num, f); k++) {

	polynomial f_k = reductum(var_num, k, f);

	polynomial d_f_k = derivative_wrt(var_num, f_k);

	//polynomial sub = subresultant(var_num, k, f_k, d_f_k);

	//cout << "Subresultant_" << k << " = " << sub << endl;

	for (int l = 0; l < k; l++) {
	  cout << "f_k = " << f_k << endl;
	  cout << "d_f_k = " << d_f_k << endl;

	  auto f_k_l = psck(var_num, l, f_k, d_f_k); //sub));
	  if (!f_k_l.is_constant()) {
	    p2.push_back(f_k_l);
	  }
	}
      }
    }

    return p2;
  }

  std::vector<polynomial> projection_1(const int var_num,
				       const std::vector<polynomial>& polys) {
    vector<polynomial> p1;
    for (auto& p : polys) {
      for (auto& c : coefficients_wrt(p, var_num)) {
	if (!c.is_constant()) {
	  p1.push_back(c);
	}
      }
    }
    return p1;
  }

  std::vector<polynomial> project(const int var_num,
				  const std::vector<polynomial>& polys) {
    vector<polynomial> proj1 =
      projection_1(var_num, polys);

    concat(proj1, projection_2(var_num, polys));
    return proj1;
  }
  
}

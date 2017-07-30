#include <ralg/projection.h>

#include <ralg/algorithm.h>
#include <ralg/matrix.h>

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
    polynomial sub = subresultant(var_num, k, f, g);

    return psc(var_num, k, sub);
  }

  std::vector<polynomial> projection_3(const int var_num,
				       const std::vector<polynomial>& polys) {
    vector<polynomial> p3;

    for (int i = 0; i < polys.size(); i++) {
      for (int j = i + 1; j < polys.size(); j++) {
	auto& f = polys[i];
	auto& g = polys[j];

	for (int k_i = 0; k_i <= degree_wrt(var_num, f); k_i++) {
	  for (int k_j = 0; k_j <= degree_wrt(var_num, g); k_j++) {
	    int m_bound = min(k_i, k_j);

	    polynomial f_k = reductum(var_num, k_i, f);
	    polynomial g_k = reductum(var_num, k_j, g);

	    for (int m = 0; m <= m_bound; m++) {
	      if (degree_wrt(var_num, f_k) >= degree_wrt(var_num, g_k)) {
		auto ps = psck(var_num, m, f_k, g_k);

		if (!ps.is_constant()) {
		  p3.push_back(ps);
		}
	      } else {
		auto ps = psck(var_num, m, g_k, f_k);

		if (!ps.is_constant()) {
		  p3.push_back(ps);
		}
	      }

	    }

	  }
	}
      }
    }

    return p3;
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

	for (int l = 0; l < k; l++) {

	  auto f_k_l = psck(var_num, l, f_k, d_f_k);
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
    concat(proj1, projection_3(var_num, polys));

    vector<polynomial> final;
    for (auto& p : proj1) {
      if (!elem(p, final)) {
	final.push_back(p);
      }
    }
    
    return final;
  }

  polynomial resultant(const int var_num,
		       const polynomial& f,
		       const polynomial& g) {
    polynomial sub = subresultant(var_num, 0, f, g);
    return sub;
  }

  polynomial discriminant(const int var_num, const polynomial& p) {
    polynomial dp = derivative_wrt(var_num, p);

    return resultant(var_num, p, dp);
  }

  std::vector<polynomial>
  discriminants(const int var_num,
		const std::vector<polynomial>& polys) {
    vector<polynomial> discs;
    for (auto& p : polys) {
      discs.push_back(discriminant(var_num, p));
    }
    return discs;
  }  

  std::vector<polynomial>
  resultants(const int var_num,
	     const std::vector<polynomial>& polys) {
    return {};
  }  
  
  std::vector<polynomial>
  mccallum_project(const int var_num,
		   const std::vector<polynomial>& polys) {
    vector<polynomial> proj1 =
      projection_1(var_num, polys);

    concat(proj1, discriminants(var_num, polys));
    concat(proj1, resultants(var_num, polys));

    return proj1;
  }
  
}

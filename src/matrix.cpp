#include "matrix.h"

#include "algorithm.h"

using namespace std;

namespace ralg {


  matrix build_M_matrix(const int var_num,
			const int k,
			const polynomial& f,
			const polynomial& g) {
    assert(f.num_vars() == g.num_vars());
    assert(degree_wrt(var_num, f) > degree_wrt(var_num, g));

    int m = degree_wrt(var_num, f);
    int n = degree_wrt(var_num, g);

    int nrows = (m - k) + (n - k);
    int ncols = (m - k) + n;
    matrix M_k(nrows, ncols, f.num_vars() - 1);

    vector<polynomial> f_coeffs = coefficients_wrt(f, var_num);
    reverse(f_coeffs);

    int col_offset = 0;
    for (int i = 0; i < n - k; i++) {
      int k = col_offset;
      for (auto& f : f_coeffs) {
	M_k.set(i, k, f);
	k++;
      }
      col_offset++;
    }

    vector<polynomial> g_coeffs = coefficients_wrt(g, var_num);
    reverse(g_coeffs);

    col_offset = 0;
    for (int i = n - k; i < (m - k) + (n - k); i++) {
      int k = col_offset;
      for (auto& g : g_coeffs) {
	M_k.set(i, k, g);
	k++;
      }

      col_offset++;
    }
    

    return M_k;
  }

  polynomial var_polynomial(const int var_num,
			    const int num_vars) {
    rational coeff(1);
    vector<int> vars;
    for (int i = 0; i < num_vars; i++) {
      if (i == var_num) {
	vars.push_back(1);
      } else {
	vars.push_back(0);
      }
    }
    monomial var_mono(coeff, vars, num_vars);
    return polynomial({var_mono}, num_vars);
  }

  polynomial subresultant(const int var_num,
			  const int ind,
			  const polynomial& f,
			  const polynomial& g) {
    matrix m_i = build_M_matrix(var_num, ind, f, g);

    polynomial d = zero_polynomial(f.num_vars());

    polynomial factor = var_polynomial(var_num, f.num_vars());
    for (int k = m_i.num_rows() - ind; k <= m_i.num_rows(); k++) {
      
    }
    return d; //determinant(m_i);
  }

}

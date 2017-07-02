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
    int ncols = (m - k - 1)*n;
    matrix M_k(nrows, ncols, f.num_vars() - 1);

    vector<polynomial> f_coeffs = coefficients_wrt(f, var_num);
    reverse(f_coeffs);
    for (int i = 0; i < n - k; i++) {
      for (int j = i; j < M_k.num_cols(); j++) {
	M_k.set(i, j, f_coeffs[j - i]);
      }
    }

    vector<polynomial> g_coeffs = coefficients_wrt(g, var_num);
    reverse(g_coeffs);
    int col_offset = 0;
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
}

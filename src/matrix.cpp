#include "matrix.h"

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
    matrix M_k(nrows, ncols, f.num_vars());

    return M_k;
  }
}

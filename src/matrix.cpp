#include <ralg/matrix.h>

#include <ralg/algorithm.h>

using namespace std;

namespace ralg {


  matrix build_M_matrix(const int var_num,
			const int k,
			const polynomial& f,
			const polynomial& g) {
    assert(f.num_vars() == g.num_vars());
    assert(degree_wrt(var_num, f) >= degree_wrt(var_num, g));

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

  matrix build_k_subsection(const int k,
			    const matrix& m) {
    matrix m_k(m.num_rows(), m.num_rows(), m.num_vars());
    for (int j = 0; j < m_k.num_cols() - 1; j++) {
      for (int i = 0; i < m_k.num_rows(); i++) {
	m_k.set(i, j, m.get(i, j));
      }
    }

    for (int i = 0; i < m_k.num_rows(); i++) {
      m_k.set(i, m_k.num_cols() - 1, m.get(i, k - 1));
    }

    return m_k;
  }

  polynomial one_polynomial(const int num_vars) {
    rational c(1);
    vector<int> zv;
    for (int i = 0; i < num_vars; i++) {
      zv.push_back(0);
    }
    monomial om(c, zv, num_vars);
    return polynomial({om}, num_vars);
  }

  monomial extend(const int var_insert_index, const monomial& m) {
    std::vector<int> vars;
    bool inserted = false;
    for (int i = 0; i < m.num_vars(); i++) {
      if (i == var_insert_index) {
	vars.push_back(0);
      } else {
	vars.push_back(m.power(i));
      }
    }
    if (!inserted) {
      vars.push_back(0);
    }
    return monomial(m.coeff(), vars, m.num_vars() + 1);
  }

  polynomial extend(const int var_insert_index, const polynomial& p) {
    vector<monomial> ms;
    for (int i = 0; i < p.num_monos(); i++) {
      ms.push_back(extend(var_insert_index, p.monomial(i)));
    }
    return polynomial(ms, p.num_vars() + 1);
  }

  polynomial subresultant(const int var_num,
			  const int ind,
			  const polynomial& f,
			  const polynomial& g) {

    if (ind == degree_wrt(var_num, g) + 1) {
      return f;
    }

    if (ind == degree_wrt(var_num, g)) {
      return g;
    }
    
    matrix m_i = build_M_matrix(var_num, ind, f, g);

    polynomial d = zero_polynomial(f.num_vars());

    polynomial factor = one_polynomial(f.num_vars());

    for (int k = m_i.num_cols(); k >= m_i.num_cols() - ind; k--) {

      matrix m_i_k = build_k_subsection(k, m_i);

      auto pd = determinant(m_i_k);

      d = d + extend(var_num, determinant(m_i_k))*factor;

      factor = factor * var_polynomial(var_num, f.num_vars());
    }
    return d;
  }

  polynomial lc(const int var_num,
		const polynomial& p) {
    // TODO: Accelerate the horribly slow general purpose call
    auto coeffs = coefficients_wrt(p, var_num);
    return coeffs.back();
  }
  
  std::vector<polynomial> subresultants_wrt(const int i,
					    const polynomial& p,
					    const polynomial& q) {
    assert(p.num_vars() == q.num_vars());
    assert(degree_wrt(i, p) >= degree_wrt(i, q));
    assert(degree_wrt(i, q) >= 1);

    vector<polynomial> S;
    polynomial s = pow(lc(i, q), degree_wrt(i, p) - degree_wrt(i, q));

    cout << "s = " << s << endl;

    polynomial A = q;
    polynomial B = prem(i, p, q);

    while (true) {
      int d = degree_wrt(i, A);
      int e = degree_wrt(i, B);

      if (is_zero(B)) {
	break;
      }

      S.push_back(B);

      int delta = d - e;

      polynomial C = const_poly(0, p.num_vars());
      if (delta > 1) {
	// TODO: Add proper C value computation
	polynomial tmp_num = pow(lc(i, B), delta - 1) * B;
	polynomial tmp_denom = pow(s, delta - 1);

	C = divide_wrt(i, tmp_num, tmp_denom);
	
	S.push_back(C);
      } else {
	C = B;
      }

      if (e == 0) {
	break;
      }

      // Add B value update
      polynomial tmp_num = prem(i, A, -1*B);
      polynomial tmp_denom = pow(s, delta)*lc(i, A);
      B = divide_wrt(i, tmp_num, tmp_denom);

      A = C;

      s = lc(i, A);
    }

    return S;
  }
}

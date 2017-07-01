#pragma once

#include <cassert>
#include <vector>

#include "polynomial.h"

namespace ralg {

  class matrix {
  protected:
    int n_rows;
    int n_cols;
    int num_vars;
    std::vector<polynomial> elems;
    
  public:

    matrix(const int p_n_rows,
	   const int p_n_cols,
	   const int p_num_vars) :
      n_rows(p_n_rows), n_cols(p_n_cols), num_vars(p_num_vars) {

      for (int j = 0; j < n_rows*n_cols; j++) {
	elems.push_back(zero_polynomial(num_vars));
      }
    }

    const polynomial& get(const int r, const int c) const {
      return elems[r + c*n_rows];
    }

    void set(const int r, const int c, const polynomial& e) {
      std::cout << "Setting r = " << r << ", c = " << c << std::endl;
      elems[r + c*n_rows] = e;
    }

    int num_rows() const { return n_rows; }
    int num_cols() const { return n_rows; }
  };

  inline polynomial determinant(const matrix& m) {
    assert(m.num_rows() == m.num_cols());
    
    if (m.num_rows() == 2) {
      return m.get(0, 0) * m.get(1, 1) - m.get(0, 1)*m.get(1, 0);
    }

    polynomial determinant = zero_polynomial(m.get(0, 0).num_vars());
    for (int i = 0; i < m.num_cols(); i++) {
    }

    return determinant;
  }

}

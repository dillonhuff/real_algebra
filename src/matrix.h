#pragma once

#include <cassert>
#include <vector>

namespace ralg {

  template<typename T>
  class matrix {
  protected:
    int n_rows;
    int n_cols;
    std::vector<T> elems;
    
  public:

    matrix(const int p_n_rows,
	   const int p_n_cols) :
      n_rows(p_n_rows), n_cols(p_n_cols), elems(p_n_rows*p_n_cols) {
    }

    const T& get(const int r, const int c) const {
      return elems[r + c*n_rows];
    }

    void set(const int r, const int c, const T& e) {
      elems[r + c*n_rows] = e;
    }

    int num_rows() const { return n_rows; }
    int num_cols() const { return n_rows; }
  };

  template<typename T>
  T determinant(const matrix<T>& m) {
    assert(m.num_rows() == m.num_cols());
    
    if (m.num_rows() == 2) {
      return m.get(0, 0) * m.get(1, 1) - m.get(0, 1)*m.get(1, 0);
    }

    T determinant;
    for (int i = 0; i < m.num_cols(); i++) {
      matrix<T> minor_i = minor(i, m);
      
    }
  }

}

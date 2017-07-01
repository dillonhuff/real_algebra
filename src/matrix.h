#pragma once

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
      n_rows(p_n_rows), n_cols(p_n_cols) {
      elems(n_rows * n_cols);
    }

    const T& get(const int r, const int c) const {
      return elems[r + c*n_rows];
    }

    void set(const int r, const int c, const T& e) {
      return elems[r + c*n_rows] = e;
    }

  };

}

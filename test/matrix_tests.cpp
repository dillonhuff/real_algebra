#include "catch.hpp"

#include "matrix.h"

#include <iostream>

using namespace std;

namespace ralg {

  TEST_CASE("Computing the determinant") {

    SECTION("Base case: two by two matrix") {
      matrix m(2, 2, 1);

      monomial one_m({"1"}, {0}, 1);
      polynomial one({one_m}, 1);

      //cout << "Setting values" << endl;

      m.set(0, 0, const_poly(1, 1));
      m.set(0, 1, one);
      m.set(1, 0, one);
      m.set(1, 1, one);

      // cout << "Setting values" << endl;
      // cout << "(0, 0) = " << endl; //m.get(0, 0) << endl;
      polynomial det = determinant(m);
      // cout << "Determinant = " << det << endl;

      REQUIRE(det == zero_polynomial(1));
    }

    SECTION("3 by 3 matrix") {
      matrix m(3, 3, 1);

      m.set(0, 0, const_poly(5, 1));
      m.set(0, 1, const_poly(-2, 1));
      m.set(0, 2, const_poly(1, 1));

      m.set(1, 0, const_poly(0, 1));
      m.set(1, 1, const_poly(3, 1));
      m.set(1, 2, const_poly(-1, 1));

      m.set(2, 0, const_poly(2, 1));
      m.set(2, 1, const_poly(0, 1));
      m.set(2, 2, const_poly(7, 1));
      
      polynomial det = determinant(m);

      cout << "Determinant = " << det << endl;
      REQUIRE(det == const_poly(103, 1));
    }
  }

  TEST_CASE("M matrix generation") {
    monomial x({"1"}, {1}, 1);
    monomial one({"1"}, {0}, 1);
    monomial one_c({"1"}, {0}, 0);
    polynomial f({x*x*x*x*x, -2*x*x*x*x, 3*x*x*x, -4*x*x, 5*x, -6*one}, 1);
    polynomial g({3*x*x*x, 5*x*x, 7*x, 9*one}, 1);

    matrix m = build_M_matrix(0, 0, f, g);

    REQUIRE(m.num_rows() == 8);
    REQUIRE(m.num_cols() == 8);

    auto p22 = m.get(2, 2);
    cout << "p22 = " << p22 << endl;
    cout << "p22 num vars = " << p22.num_vars() << endl;
    polynomial one_poly({one_c}, 0);
    cout << "one_poly num vars = " << one_poly.num_vars() << endl;

    REQUIRE(m.get(2, 2) == one_poly);
  }

}

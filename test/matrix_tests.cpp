#include "catch.hpp"

#include "matrix.h"

#include <iostream>

using namespace std;

namespace ralg {

  TEST_CASE("Computing the determinant") {

    SECTION("Base case: two by two matrix") {
      matrix m(2, 2);

      monomial one_m({"1"}, {0}, 1);
      polynomial one({one_m}, 1);

      cout << "Setting values" << endl;

      m.set(0, 0, one);
      m.set(0, 1, one);
      m.set(1, 0, one);
      m.set(1, 1, one);

      cout << "Setting values" << endl;
      cout << "(0, 0) = " << endl; //m.get(0, 0) << endl;
      // polynomial det = determinant(m);
      // cout << "Determinant = " << det << endl;

      //REQUIRE(det == zero_polynomial(1));
    }

    SECTION("3 by 3 matrix") {
      matrix m(3, 3);

      // m.set(0, 0, -3);
      // m.set(0, 1, 2);
      // m.set(0, 2, 1);

      // m.set(1, 0, 1);
      // m.set(1, 1, 0);
      // m.set(1, 2, 1);

      // m.set(2, 0, 7);
      // m.set(2, 1, 1);
      // m.set(2, 2, 9);

      polynomial det = determinant(m);

      cout << det << endl;
      
    }
  }

}
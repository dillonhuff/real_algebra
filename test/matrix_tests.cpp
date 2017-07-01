#include "catch.hpp"

#include "matrix.h"

#include <iostream>

using namespace std;

namespace ralg {

  TEST_CASE("Computing the determinant") {

    SECTION("Base case: two by two matrix") {
      matrix<double> m(2, 2);
      m.set(0, 0, 1);
      m.set(0, 1, 1);
      m.set(1, 0, 1);
      m.set(1, 1, 1);

      REQUIRE(determinant(m) == 0.0);
    }

    SECTION("3 by 3 matrix") {
      matrix<int> m(3, 3);

      m.set(0, 0, -3);
      m.set(0, 1, 2);
      m.set(0, 2, 1);

      m.set(1, 0, 1);
      m.set(1, 1, 0);
      m.set(1, 2, 1);

      m.set(2, 0, 7);
      m.set(2, 1, 1);
      m.set(2, 2, 9);

      double det = determinant(m);

      cout << det << endl;
      
    }
  }

}

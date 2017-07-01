#include "catch.hpp"

#include "matrix.h"

namespace ralg {

  TEST_CASE("Computing the determinant") {
    matrix<double> m(2, 2);
    m.set(0, 0, 1);
    m.set(0, 1, 1);
    m.set(1, 0, 1);
    m.set(1, 1, 1);

    REQUIRE(determinant(m) == 0.0);
  }

}

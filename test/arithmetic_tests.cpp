#include "catch.hpp"

#include "polynomial.h"

namespace ralg {

  TEST_CASE("Polynomial arithmetic") {

    SECTION("Adding polynomials with no common coefficients") {
      monomial two_x_sq({"2"}, {2, 0, 0}, 3);
      monomial mthree_x_y3({"-3"}, {1, 3, 0}, 3);

      polynomial p{{two_x_sq}, 3};
      polynomial q{{mthree_x_y3}, 3};

      polynomial expected{{two_x_sq, mthree_x_y3}, 3};

      polynomial res = p + q;

      REQUIRE(res == expected);
    }
  }

}

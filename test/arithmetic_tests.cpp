#include "catch.hpp"

#include "polynomial.h"

namespace ralg {

  TEST_CASE("Polynomial arithmetic") {

    SECTION("Adding polynomials") {
      monomial two_x_sq({"2"}, {2, 0, 0}, 3);
      monomial four_x_sq({"4"}, {2, 0, 0}, 3);
      monomial mthree_x_y3({"-3"}, {1, 3, 0}, 3);

      SECTION("with no common coefficients") {

	polynomial p{{two_x_sq}, 3};
	polynomial q{{mthree_x_y3}, 3};

	polynomial expected{{two_x_sq, mthree_x_y3}, 3};

	polynomial res = p + q;

	REQUIRE(res == expected);
      }

      SECTION("That are the same") {
	polynomial p({mthree_x_y3}, 3);

	polynomial res = p + p;

	polynomial expected({four_x_sq}, 3);
      }
    }
  }

}

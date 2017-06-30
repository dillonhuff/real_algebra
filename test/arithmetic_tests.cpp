#include "catch.hpp"

#include "polynomial.h"

using namespace std;

namespace ralg {

  TEST_CASE("Polynomial arithmetic") {

    monomial two_x_sq({"2"}, {2, 0, 0}, 3);
    monomial z_quart({"1"}, {0, 0, 4}, 3);
    monomial four_x_sq({"4"}, {2, 0, 0}, 3);
    monomial mthree_x_y3({"-3"}, {1, 3, 0}, 3);
    monomial m6_x_y3({"-6"}, {1, 3, 0}, 3);
    
    SECTION("Adding polynomials") {

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

	polynomial expected({m6_x_y3}, 3);

	REQUIRE(res == expected);
      }
    }

    SECTION("Subtracting polynomials") {

      SECTION("With identical coefficients") {
	polynomial p({two_x_sq, m6_x_y3}, 3);
	polynomial q({two_x_sq, m6_x_y3}, 3);

	REQUIRE((p - q) == zero_polynomial(3));
	REQUIRE((p - q) == (q - p));
      }

    }

    SECTION("Multiplying polynomials") {
      SECTION("One term polynomials") {
	polynomial p({two_x_sq}, 3);
	polynomial q({z_quart}, 3);

	polynomial res = p*q;

	polynomial expected({{{"2"}, {2, 0, 4}, 3}}, 3);

	REQUIRE(res == expected);
      }
    }
    
  }

  TEST_CASE("Rearranging polynomials") {
    monomial x({"1"}, {1, 0, 0}, 3);
    monomial y({"1"}, {0, 1, 0}, 3);
    monomial z({"1"}, {0, 0, 1}, 3);

    SECTION("x coefficients wrt x") {
      polynomial x_poly({x}, 3);
      vector<polynomial> ps = coefficients_wrt(x_poly, 0);

      REQUIRE(ps.size() == 1);

      monomial one_mono({"1"}, {0, 0}, 2);

      polynomial one_poly({one_mono}, 2);

      REQUIRE(ps.front() == one_poly);
    }
  }

}

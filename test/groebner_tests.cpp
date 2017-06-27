#include "catch.hpp"

#include "groebner.h"

using namespace std;

namespace ralg {

  TEST_CASE("Groebner basis computation") {

    monomial x_cubed({"1"}, {3, 0}, 2);
    monomial x({"1"}, {1, 0}, 2);
    monomial mtwo_xy({"-2"}, {1, 1}, 2);
    monomial x_sq_y({"1"}, {2, 1}, 2);
    monomial m2_y_sq({"-2"}, {0, 2}, 2);

    SECTION("Groebner basis for one polynomial is itself") {
      polynomial p({x_sq_y}, 2);

      vector<polynomial> basis =
	buchberger({p}, lexicographic_order);

      REQUIRE(basis.size() == 1);
    }

    SECTION("Groebner basis for 2 polynomials") {
      polynomial p1({x_cubed, mtwo_xy}, 2);
      polynomial p2({x_sq_y, m2_y_sq, x}, 2);

      vector<polynomial> basis =
	buchberger({p1, p2}, lexicographic_order);


      REQUIRE(basis.size() == 5);
    }
  }

}

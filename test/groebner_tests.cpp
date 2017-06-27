#include "catch.hpp"

#include "groebner.h"

namespace ralg {

  TEST_CASE("Groebner basis computation") {

    monomial x_cubed({"1"}, {3, 0}, 2);
    monomial two_xy({"2"}, {1, 1}, 2);
    monomial x_sq_y({"1"}, {2, 1}, 2);

    SECTION("Groebner basis for one polynomial is itself") {
      polynomial p({x_sq_y}, 2);

      
    }
  }

}

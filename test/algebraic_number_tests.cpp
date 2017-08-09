#include "catch.hpp"

#include "ralg/algebraic_number.h"

namespace ralg {

  TEST_CASE("Create 0 number") {
    algnum a(0);

    REQUIRE(a.in_interval(0));
  }

  TEST_CASE("Create 10 number") {
    algnum a(10);

    REQUIRE(!a.in_interval(12));
  }

  TEST_CASE("Square root of 2") {
    monomial x_m(1, {1}, 1);

    polynomial x({x_m}, 1);

    polynomial p = x*x - 2;

    interval i(-1, false, 1, false);

    algnum a(p, i);
  }
  
}

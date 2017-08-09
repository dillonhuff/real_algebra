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
  
}

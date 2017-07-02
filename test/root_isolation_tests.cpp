#include "catch.hpp"

#include "root_counting.h"

namespace ralg {

  TEST_CASE("Root counting") {
    monomial xv({"1"}, {1}, 1);
    polynomial x({xv}, 1);

    SECTION("Polynomial with no real roots") {
      polynomial p = x*x + const_poly(-4, 1)*x + const_poly(11, 1);
      
      REQUIRE(num_real_roots(p) == 0);
    }

    SECTION("Polynomial with one real root") {
      polynomial p = const_poly(-4, 1)*x + const_poly(11, 1);
      
      REQUIRE(num_real_roots(p) == 1);
    }
    
  }

}

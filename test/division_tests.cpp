#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "polynomial.h"

namespace ralg {

  TEST_CASE("Polynomial division") {
    monomial m1{{"1"}, {2, 1}, 2};
    monomial m2{{"1"}, {0, 1}, 2};

    polynomial f{{m1, m2}, 2};

    polynomial g1{{{{"1"}, {1, 1}, 2}, {{"1"}, {1, 0}, 2}}, 2};
    polynomial g2{{{{"1"}, {2, 0}, 2}, {{"1"}, {0, 0}, 2}}, 2};

    polynomial a1{{{{"1"}, {1, 0}, 2}}, 2};
    polynomial a2{{{{"-1"}, {0, 0}, 2}}, 2};
    polynomial r{{{{"-1"}, {0, 1}, 2}, {{"-1"}, {0, 0}, 2}}, 2};

    std::vector<polynomial > as{a1, a2};
    division_result expected_result{as, r};

    std::vector<polynomial > g{g1, g2};
    division_result res = divide(f, g, lexicographic_order);
    
    SECTION("Result adds up to f") {
      polynomial sum = zero_polynomial(2);
      for (int i = 0; i < res.as.size(); i++) {
	sum = sum + as[i]*g[i];
      }
      sum = sum + res.remainder;

      REQUIRE(sum == f);
    }

    SECTION("Remainder is correct") {


      REQUIRE(res.remainder == expected_result.remainder);
    }

    // SECTION("Remainder changes for ") {
    //   std::vector<polynomial > g{g2, g1};
    //   division_result res = divide(f, g, lexicographic_order);

    //   REQUIRE(!poly_eq(double_eq, res.remainder, expected_result.remainder));
    // }

  }
  
}

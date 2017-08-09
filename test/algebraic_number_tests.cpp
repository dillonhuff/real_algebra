#include "catch.hpp"

#include "ralg/algebraic_number.h"

using namespace std;

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

    rational_interval i(0, true, 10, true);

    algnum a(p, i);

    double d = a.decimal_approximation(0.0001);

    cout << "Decimal approximation = " << d << endl;

    REQUIRE(fabs(d - sqrt(2)) < 0.0001);
  }
  
}

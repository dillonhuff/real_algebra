#include "catch.hpp"

#include "root_counting.h"

using namespace std;

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

      SECTION("One root counted") {
	REQUIRE(num_real_roots(p) == 1);
      }


      SECTION("Isolation produces one interval") {
	vector<interval> intervals =
	  isolate_roots(p);

	REQUIRE(intervals.size() == 1);
      }

    }

    SECTION("x^2") {
      polynomial p = x*x;

      SECTION("One root counted") {
	REQUIRE(num_real_roots(p) == 1);
      }

      SECTION("One root is on (-1, 1)") {
	REQUIRE(num_roots_in_interval({ipt(-1), ipt(1)}, sturm_chain(p)) == 1);
      }

      SECTION("Isolation produces one interval") {
	vector<interval> intervals =
	  isolate_roots(p);

	REQUIRE(intervals.size() == 1);
      }

    }
    
  }

}

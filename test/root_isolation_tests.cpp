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

    SECTION("x^2 - 7x + 12") {
      polynomial p = x*x - 7*x + const_poly(12, 1);

      auto its = isolate_roots(p);
      REQUIRE(its.size() == 2);
    }

    SECTION("(x^2 - 7x + 12)*(x - 7)") {
      cout << "----- (x^2 - 7x + 12)*(x - 7)" << endl;
      polynomial p1 = x*x - 7*x + const_poly(12, 1);
      polynomial p2 = (x - const_poly(7, 1));
      polynomial p = p1*p2;

      auto its = isolate_roots(p);

      REQUIRE(its.size() == 3);

      for (auto& it : its) {
	cout << it << endl;
	REQUIRE(is_finite(it));
	REQUIRE(evaluate_at(it.start.value, p) != rational(0));
	REQUIRE(evaluate_at(it.end.value, p) != rational(0));
      }

    }
    
  }

}

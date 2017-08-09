#include "catch.hpp"

#include <ralg/root_counting.h>

using namespace std;

namespace ralg {

  TEST_CASE("Sign changes") {

    SECTION("Two sign changes one value") {
      vector<int> sgns{1, 1, -1, 1, 1};

      REQUIRE(num_sign_changes(sgns) == 2);
    }

    SECTION("Two sign changes interrupted") {
      vector<int> sgns{1, -1, -1, -1, 1};

      REQUIRE(num_sign_changes(sgns) == 2);
    }

    SECTION("One sign change") {
      vector<int> sgns{0, 0, -1};

      REQUIRE(num_sign_changes(sgns) == 1);
    }
    
  }
    

  

  TEST_CASE("Root counting") {
    monomial xv({"1"}, {1}, 1);
    polynomial x({xv}, 1);

    SECTION("Constant polynomial") {
      polynomial p = const_poly(8, 1);
      vector<interval> roots = isolate_roots(p);

      REQUIRE(roots.size() == 0);
    }
    
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

    SECTION("(x^2 + 3x + 2)*(x - 16)") {
      polynomial p1 = x*x + 3*x + const_poly(2, 1);
      polynomial p2 = (x - const_poly(16, 1));
      polynomial p = p1*p2;

      SECTION("Isolate finite intervals") {
	auto its = isolate_roots(p);

	REQUIRE(its.size() == 3);

	for (auto& it : its) {
	  cout << "( " << it.start.value.to_double() << " , " << it.end.value.to_double() << " )" << endl;

	  REQUIRE(is_finite(it));
	  REQUIRE(evaluate_at(it.start.value, p) != rational(0));
	  REQUIRE(evaluate_at(it.end.value, p) != rational(0));
	}

      }

      SECTION("Isolate and refine intervals") {

	rational max_interval_width("1/100000");
	auto its = isolate_roots(p, max_interval_width);

	REQUIRE(its.size() == 3);

	for (auto& it : its) {
	  cout << "( " << it.start.value.to_double() << " , " << it.end.value.to_double() << " )" << endl;

	  REQUIRE(is_finite(it));
	  REQUIRE((it.end.value - it.start.value) < max_interval_width);
	}

      }

    }

  }

  TEST_CASE("roots of quadratic -391  + -6 x_0 + 1 x_0^2") {
    monomial m_x(1, {1}, 1);
    polynomial x({m_x}, 1);

    polynomial q = x*x - const_poly(6, 1)*x - const_poly(391, 1);

    rational max_interval_width("1/100000");
    auto its = isolate_roots(q, max_interval_width);

    REQUIRE(its.size() == 2);

    rational twenty_three(23);
    bool contains_23 = false;
    for (auto& it : its) {

      if (it.start.value <= twenty_three &&
	  twenty_three <= it.end.value) {
	contains_23 = true;
      }
    }

    REQUIRE(contains_23);
  }

  TEST_CASE("Isolating roots for 8  + 6 x_0 + 1 x_0^2") {
    monomial m_x(1, {1}, 1);
    polynomial x({m_x}, 1);

    polynomial p = x*x + 6*x + 8;

    rational max_width(0.0001);
    auto its = isolate_roots(p, max_width);

    for (auto& it : its) {
      cout << it << endl;
      cout << "Start = " << it.start.value.to_double() << endl;
      cout << "End   = " << it.end.value.to_double() << endl;

    }
    
    REQUIRE(its.size() == 2);
    
  }

  TEST_CASE("Refine around square root of 2") {
    monomial x_m(1, {1}, 1);

    polynomial x({x_m}, 1);
    polynomial p = x*x - 2;

    interval it{ipt(0), ipt(10)};
    interval i = isolate_root(it, sturm_chain(p), rational(0.0001));

    REQUIRE(is_finite(i));
  }

}

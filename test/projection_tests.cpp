#include "catch.hpp"

#include "projection.h"
#include "root_counting.h"

using namespace std;

namespace ralg {

  TEST_CASE("One polynomial example") {
    monomial x1({"1"}, {1, 0, 0}, 3);
    monomial x2({"1"}, {0, 1, 0}, 3);
    monomial x3({"1"}, {0, 0, 1}, 3);
    monomial one({"1"}, {0, 0, 0}, 3);


    polynomial x1p({x1, -2*one}, 3);
    polynomial x2p({x2, -2*one}, 3);
    polynomial x3p({x3, -2*one}, 3);
    polynomial one_poly({one}, 3);
    
    polynomial f = x1p*x1p + x2p*x2p + x3p*x3p - one_poly;

    vector<polynomial> projection_set =
      project(2, {f});

    REQUIRE(projection_set.size() == 2);
    
    monomial x1_d({"1"}, {1, 0}, 2);
    monomial x2_d({"1"}, {0, 1}, 2);
    monomial one_d({"1"}, {0, 0}, 2);
    polynomial x1p_2({x1_d, -2*one_d}, 2);
    polynomial x2p_2({x2_d, -2*one_d}, 2);
    auto x1sq = x1p_2*x1p_2;
    auto x2sq = x2p_2*x2p_2;
    polynomial result = const_poly(4, 2)*(x1sq + x2sq - const_poly(1, 2));
    
    REQUIRE(elem(result, projection_set));

    vector<polynomial> proj_sq =
      project(1, projection_set);

    REQUIRE(proj_sq.size() == 5);

    monomial xz({"1"}, {1}, 1);
    polynomial xp({xz}, 1);

    polynomial p2_1 =
      const_poly(4, 1)*(xp - const_poly(1, 1))*(xp - const_poly(3, 1))*(xp*xp - const_poly(4, 1)*xp + const_poly(19, 1));

    polynomial p2_2 =
      const_poly(16, 1)*(xp*xp*xp*xp - const_poly(8, 1)*xp*xp*xp + const_poly(30, 1)*xp*xp - const_poly(56, 1)*xp + const_poly(113, 1));

    polynomial p2_3 =
      const_poly(4, 1)*(xp*xp - const_poly(4, 1)*xp + const_poly(7, 1));

    polynomial p2_4 =
      xp*xp - const_poly(4, 1)*xp + const_poly(11, 1);

    polynomial p2_5 =
      const_poly(256, 1)*(xp*xp - const_poly(4, 1)*xp + const_poly(3, 1));

    REQUIRE(elem(p2_1, proj_sq));
    REQUIRE(elem(p2_2, proj_sq));
    REQUIRE(elem(p2_3, proj_sq));
    REQUIRE(elem(p2_4, proj_sq));
    REQUIRE(elem(p2_5, proj_sq));


    // rational max_interval_width("1/100000");
    // for (auto& p : {p2_1, p2_2, p2_3, p2_4, p2_5}) {
    //   cout << "-- Isolating roots of " << p << endl;
    //   auto its = isolate_roots(p, max_interval_width);
    //   for (auto& it : its) {
    // 	cout << "( " << it.start.value.to_double() << " , " << it.end.value.to_double() << " )" << endl;

    //   }
    // }

  }

}

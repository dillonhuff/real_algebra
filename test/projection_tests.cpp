#include "catch.hpp"

#include "projection.h"

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
    cout << "f = " << f << endl;
    // polynomial f({x1*x1, -4*x1, 4*one,
    // 	  x2*x2, -4*x2, 4*one,
    // 	  x3*x3, -4*x3, 4*one, -one}, 3);

    vector<polynomial> projection_set =
      project(2, {f});

    cout << "Projection" << endl;
    for (auto& p : projection_set) {
      cout << p << endl;
    }

    // auto p2 = project(1, {projection_set.front()});
    // cout << "P2" << endl;
    // for (auto& p : p2) {
    //   cout << p << endl;
    // }

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

    cout << "Projection squared" << endl;
    for (auto& p : proj_sq) {
      cout << p << endl;
    }


    REQUIRE(proj_sq.size() == 5);

    monomial xz({"1"}, {1}, 1);
    polynomial xp({xz}, 1);

    polynomial p2_1 =
      const_poly(4, 1)*(xp - const_poly(1, 1))*(xp - const_poly(3, 1))*(xp*xp - const_poly(4, 1)*xp + const_poly(19, 1));

    cout << "p2_1 = " << p2_1 << endl;

    REQUIRE(elem(p2_1, proj_sq));
    
  }

}

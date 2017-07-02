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
    
    polynomial f = x1p*x1p + x2p*x2p + x3p*x3p + one_poly;
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

    REQUIRE(projection_set.size() == 2);
  }

}

#include "catch.hpp"

#include "cad.h"

using namespace std;

namespace ralg {

  TEST_CASE("Sphere CAD") {
    monomial x1({"1"}, {1, 0, 0}, 3);
    monomial x2({"1"}, {0, 1, 0}, 3);
    monomial x3({"1"}, {0, 0, 1}, 3);
    monomial one({"1"}, {0, 0, 0}, 3);


    polynomial x1p({x1, -2*one}, 3);
    polynomial x2p({x2, -2*one}, 3);
    polynomial x3p({x3, -2*one}, 3);
    polynomial one_poly({one}, 3);
    
    polynomial f = x1p*x1p + x2p*x2p + x3p*x3p - one_poly;

    vector<cell> cells =
      build_CAD({f});

    REQUIRE(cells.size() == 25);
  }
  
}

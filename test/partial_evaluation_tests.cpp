#include "catch.hpp"

#include <ralg/projection.h>

using namespace std;

namespace ralg {

  TEST_CASE("Ellipse-Circle Intersection Algorithm") {
    monomial a_m(1, {1, 0, 0, 0, 0, 0, 0, 0, 0}, 9);
    monomial b_m(1, {0, 1, 0, 0, 0, 0, 0, 0, 0}, 9);
    monomial r_m(1, {0, 0, 1, 0, 0, 0, 0, 0, 0}, 9);
    monomial c_m(1, {0, 0, 0, 1, 0, 0, 0, 0, 0}, 9);
    monomial d_m(1, {0, 0, 0, 0, 1, 0, 0, 0, 0}, 9);
    monomial h_m(1, {0, 0, 0, 0, 0, 1, 0, 0, 0}, 9);
    monomial k_m(1, {0, 0, 0, 0, 0, 0, 1, 0, 0}, 9);
    monomial x_m(1, {0, 0, 0, 0, 0, 0, 0, 1, 0}, 9);
    monomial y_m(1, {0, 0, 0, 0, 0, 0, 0, 0, 1}, 9);

    polynomial a({a_m}, 9);
    polynomial b({b_m}, 9);
    polynomial r({r_m}, 9);
    polynomial c({c_m}, 9);
    polynomial d({d_m}, 9);
    polynomial h({h_m}, 9);
    polynomial k({k_m}, 9);
    polynomial x({x_m}, 9);
    polynomial y({y_m}, 9);

    polynomial circle = pow(x - a, 2) + pow(y - b, 2) - pow(r, 2);

    cout << "circle = " << circle << endl;

    polynomial ellipse =
      k*k*pow(x - c, 2) + h*h*pow(y - d, 2) - k*k*h*h;

    cout << "Ellipse = " << ellipse << endl;

    auto ps = mccallum_project(8, {circle, ellipse});

    cout << "McCallum Projection Size = " << ps.size() << endl;
    
    REQUIRE(ps.size() > 0);

    for (auto& p : ps) {
      cout << p << endl;
      REQUIRE(p.num_vars() == 8);
    }

    auto ps2 = mccallum_project(7, ps);

    cout << "McCallum Projection Size 2 = " << ps2.size() << endl;
    for (auto& p : ps2) {
      cout << "# of monomials = " << p.num_monos() << endl;
      for (int i = 0; i < p.num_vars(); i++) {
	cout << "degreee wrt " << i << " = " << degree_wrt(i, p) << endl;
      }
    }
  }

}

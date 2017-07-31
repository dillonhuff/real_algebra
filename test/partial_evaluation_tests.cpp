#include "catch.hpp"

#include <ralg/cad.h>
#include <ralg/projection.h>

using namespace std;

namespace ralg {

  vector<polynomial> ce_polynomials() {
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

    return {circle, ellipse};
  }

  vector<cell> ce_cad(const double a, const double b, const double c, const double d, const double h, const double k, const double l ) {
    vector<polynomial> ps = ce_polynomials();

    vector<rational> rs{{a}, {b}, {c}, {d}, {h}, {k}, {l} };
    vector<polynomial> evaluated;
    for (auto& p : ps) {
      polynomial p_v = evaluate_at(rs, p);

      cout << "evaluated = " << p_v << endl;
      evaluated.push_back(p_v);
    }

    return build_CAD(evaluated);
  }

  void print_cell(std::ostream& out, const cell& c) {
    for (auto& r : c.test_pt) {
      out << r << " ";
    }
  }

  void print_cell_as_double(std::ostream& out, const cell& c) {
    for (auto& r : c.test_pt) {
      out << r.to_double() << " ";
    }
  }

  bool in_ellipse(const double c,
		  const double d,
		  const double h,
		  const double k,
		  const double x,
		  const double y) {
    return k*k*pow(x - c, 2.0) + h*h*pow(y - d, 2.0) - h*h*k*k < 0;
  }
  
  bool in_circle(const double a,
		 const double b,
		 const double r,
		 const double x,
		 const double y) {
    return pow(x - a, 2.0) + pow(y - b, 2.0) - r*r < 0;
  }
  
  TEST_CASE("Ellipse-Circle Intersection Algorithm") {

    double a = -1;
    double b = -1;
    double r = 1000000;

    double c = 3;
    double d = 1;

    double h = 2;
    double k = 4;

    vector<cell> cells = ce_cad( a,  b,  c,  d,  h,  k,  r );

    cout << "Cell dimension = " << cells.front().test_pt.size() << endl;
    cout << "# of cells = " << cells.size() << endl;

    for (auto& cl : cells) {
      print_cell_as_double(cout, cl);
      cout << endl;
      cout << "Point in circle = " << in_circle(a, b, r,
						cl.test_pt[0].to_double(),
						cl.test_pt[1].to_double()) << endl;

      cout << "Point in ellipse = " << in_ellipse(c, d, h, k,
						  cl.test_pt[0].to_double(),
						  cl.test_pt[1].to_double()) << endl;

    }
  }

}

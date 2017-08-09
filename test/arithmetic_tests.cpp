#include "catch.hpp"

#include <ralg/polynomial.h>

using namespace std;

namespace ralg {

  TEST_CASE("Defining rationals with doubles") {
    double d = 1.5;
    rational r(d);

    rational f("3/2");

    REQUIRE(r == f);
  }

  TEST_CASE("Delete if on rationals") {
    vector<rational> rs{1, 3, 2, 4, 6, -4};

    delete_if(rs, [](const rational& r) { return r < 1; });

    REQUIRE(rs.size() == 5);
  }

  TEST_CASE("Constructing polynomial of that should become 8") {
    monomial eight(8, {0}, 1);
    monomial px(1, {1}, 1);
    monomial mx(-1, {1}, 1);

    polynomial p({px, mx, eight}, 1);
    polynomial expected = const_poly(8, 1);

    REQUIRE(p == expected);
  }

  TEST_CASE("Polynomial arithmetic") {

    monomial two_x_sq({"2"}, {2, 0, 0}, 3);
    monomial z_quart({"1"}, {0, 0, 4}, 3);
    monomial four_x_sq({"4"}, {2, 0, 0}, 3);
    monomial mthree_x_y3({"-3"}, {1, 3, 0}, 3);
    monomial m6_x_y3({"-6"}, {1, 3, 0}, 3);
    
    SECTION("Adding polynomials") {

      SECTION("with no common coefficients") {

	polynomial p{{two_x_sq}, 3};
	polynomial q{{mthree_x_y3}, 3};

	polynomial expected{{two_x_sq, mthree_x_y3}, 3};

	polynomial res = p + q;

	REQUIRE(res == expected);
      }

      SECTION("That are the same") {
	polynomial p({mthree_x_y3}, 3);

	polynomial res = p + p;

	polynomial expected({m6_x_y3}, 3);

	REQUIRE(res == expected);
      }
    }

    SECTION("Subtracting polynomials") {

      SECTION("With identical coefficients") {
	polynomial p({two_x_sq, m6_x_y3}, 3);
	polynomial q({two_x_sq, m6_x_y3}, 3);

	REQUIRE((p - q) == zero_polynomial(3));
	REQUIRE((p - q) == (q - p));
      }

    }

    SECTION("Multiplying polynomials") {
      SECTION("One term polynomials") {
	polynomial p({two_x_sq}, 3);
	polynomial q({z_quart}, 3);

	polynomial res = p*q;

	polynomial expected({{{"2"}, {2, 0, 4}, 3}}, 3);

	REQUIRE(res == expected);
      }
    }
    
  }

  TEST_CASE("Rearranging polynomials") {
    monomial x({"1"}, {1, 0, 0}, 3);
    monomial y({"1"}, {0, 1, 0}, 3);
    monomial z({"1"}, {0, 0, 1}, 3);
    monomial one({"1"}, {0, 0, 0}, 3);

    SECTION("x coefficients wrt x") {
      polynomial x_poly({x}, 3);
      vector<polynomial> ps = coefficients_wrt(x_poly, 0);

      REQUIRE(ps.size() == 2);

      monomial one_mono({"1"}, {0, 0}, 2);

      polynomial one_poly({one_mono}, 2);

      REQUIRE(ps.back() == one_poly);
    }

    SECTION("xy^2 + y^2 + y + z + 1 wrt y") {
      monomial one_3({"3"}, {0, 0, 0}, 3);
      polynomial xy2pypzp1({x*y*y, y*y, y, z, one_3}, 3);

      SECTION("Extracting coefficients") {
	auto ps = coefficients_wrt(xy2pypzp1, 1);

	REQUIRE(ps.size() == 3);

	monomial x_2({"1"}, {1, 0}, 2);
	monomial one_2({"1"}, {0, 0}, 2);
	polynomial y_coeff({x_2, one_2}, 2);

	REQUIRE(ps[2] == y_coeff);
      }


      SECTION("Derivatives with respect to different variables") {
	polynomial pp = xy2pypzp1;

	SECTION("Derivative wrt x") {
	  polynomial dx = derivative_wrt(0, pp);

	  polynomial expected({y*y}, 3);

	  REQUIRE(expected == dx);
	}

	// xy^2 + y^2 + y + z + 1 wrt y
	SECTION("Derivative wrt y") {
	  polynomial dy = derivative_wrt(1, pp);

	  polynomial expected({2*x*y, 2*y, one}, 3);

	  REQUIRE(expected == dy);
	}

	SECTION("Derivative wrt z") {
	  polynomial dz = derivative_wrt(2, pp);

	  polynomial expected({one}, 3);

	  REQUIRE(expected == dz);
	}
	
      }
    }

  }

  TEST_CASE("Exponents") {
    monomial m_x(1, {1, 0, 0}, 3);
    polynomial x({m_x}, 3);

    polynomial res = x*x*x;

    REQUIRE(pow(x, 3.0) == res);
  }

  TEST_CASE("Condition numbers") {
    monomial m_x(1, {1}, 1);
    polynomial x({m_x}, 1);

    polynomial res = x*x*x - 3*x*x + 3*x - 1;

    // rational condition_num =
    //   condition_number(1, rational(1.0), res);

    // cout << "Condition number = " << condition_num << endl;
  }

  TEST_CASE("Partially evaluating a polynomial ") {
    monomial m_x(1, {1, 0, 0}, 3);
    monomial m_y(1, {0, 1, 0}, 3);
    monomial m_z(1, {0, 0, 1}, 3);

    polynomial x({m_x}, 3);
    polynomial y({m_y}, 3);
    polynomial z({m_z}, 3);

    polynomial p = x*x + y*x + z;

    cout << "p = " << p << endl;

    polynomial expected = delete_var(0, 2*2 + 2*y + z);

    //cout << "expected = " << expected << endl;

    vector<rational> rs = {2};
    polynomial res = evaluate_at(rs, p);

    REQUIRE(res == expected);

  }
	     

}

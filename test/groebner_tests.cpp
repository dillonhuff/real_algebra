#include "catch.hpp"

#include "groebner.h"

using namespace std;

namespace ralg {

  TEST_CASE("Groebner basis computation") {

    monomial x_cubed({"1"}, {3, 0}, 2);
    monomial x({"1"}, {1, 0}, 2);
    monomial mtwo_xy({"-2"}, {1, 1}, 2);
    monomial x_sq_y({"1"}, {2, 1}, 2);
    monomial m2_y_sq({"-2"}, {0, 2}, 2);

    SECTION("Groebner basis for one polynomial is itself") {
      polynomial p({x_sq_y}, 2);

      vector<polynomial> basis =
	buchberger({p}, lexicographic_order);

      REQUIRE(basis.size() == 1);
    }

    SECTION("Groebner basis for 2 polynomials") {
      polynomial p1({x_cubed, mtwo_xy}, 2);
      polynomial p2({x_sq_y, m2_y_sq, x}, 2);

      vector<polynomial> basis =
	buchberger({p1, p2}, graded_lexicographic_order);


      REQUIRE(basis.size() == 5);

      vector<polynomial> minimal_basis =
	minimize_groebner_basis(basis, graded_lexicographic_order);

      rational one_real("1");
      for (auto& b : minimal_basis) {
	REQUIRE(one_real == b.lt(graded_lexicographic_order).coeff());
      }
      
      REQUIRE(minimal_basis.size() == 3);
    }

    SECTION("Basis for 2 polynomials that are already a basis") {
      monomial x({"1"}, {1, 0, 0}, 3);
      monomial y({"1"}, {0, 1, 0}, 3);

      monomial my({"1"}, {0, 1, 0}, 3);
      monomial mz({"1"}, {0, 0, 1}, 3);
      
      polynomial p1({x, my}, 3);
      polynomial p2({y, mz}, 3);

      vector<polynomial> basis =
	buchberger({p1, p2}, lexicographic_order);

      REQUIRE(basis.size() == 2);
    }

    SECTION("Basis for 4 polynomials in 3 variables") {
      monomial x({"1"}, {1, 0, 0}, 3);
      monomial y({"1"}, {0, 1, 0}, 3);
      monomial z({"1"}, {0, 0, 1}, 3);
      monomial one({"1"}, {0, 0, 0}, 3);

      monomial x2 = x*x;
      monomial y2 = y*y;
      monomial z2 = z*z;

      polynomial p1({x2, y, z, -one}, 3);
      polynomial p2({x, y2, z, -one}, 3);
      polynomial p3({x, y, z2, -one}, 3);

      vector<polynomial> ps{p1, p2, p3};

      auto minimal_basis =
	minimal_groebner_basis(ps, lexicographic_order);

      cout << "Minimal basis in lex order" << endl;
      rational one_real("1");
      for (auto& b : minimal_basis) {
	REQUIRE(one_real == b.lt(lexicographic_order).coeff());
	cout << b << endl;
      }

      REQUIRE(minimal_basis.size() == 4);
    }

  }

}

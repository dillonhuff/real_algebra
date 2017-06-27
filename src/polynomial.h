#pragma once

#include <cassert>
#include <vector>

#include "rational.h"

namespace ralg {

  class monomial {
    rational c;
    std::vector<int> powers;
    int nvars;

  public:
    monomial(const rational& p_coeff,
	     const std::vector<int> p_powers,
	     const int p_num_vars) :
      c(p_coeff),
      powers(p_powers),
      nvars(p_num_vars) {}

    rational coeff() const { return c; }
    int num_vars() const { return nvars; }
    int power(const int i) const { return powers[i]; }

    template<typename Comparator>
    bool equals(Comparator eq, const monomial& g) const {
      assert(g.num_vars() == num_vars());

      if (!eq(g.coeff(), coeff())) { return false; }

      for (int i = 0; i < g.num_vars(); i++) {
	if (g.power(i) != power(i)) {
	  return false;
	}
      }

      return true;
    }
    

  };

  inline bool lexicographic_order(const monomial& x,
				  const monomial& y) {
    assert(x.num_vars() == y.num_vars());

    for (int i = 0; i < x.num_vars(); i++) {
      if (x.power(i) < y.power(i)) {
	return true;
      } if (x.power(i) > y.power(i)) {
	return false;
      }
    }

    // x == y
    return false;
  }


  class polynomial {

    std::vector<monomial> monos;
    int nvars;

  public:
    polynomial(const std::vector<monomial> p_monos,
	       const int p_num_vars) {
      monos = p_monos;
      std::sort(begin(monos), end(monos), lexicographic_order);
      nvars = p_num_vars;
    }

    int num_monos() const { return monos.size(); }
    int num_vars() const { return nvars; }

    const monomial& monomial(const int i) const { return monos[i]; }

    template<typename Comparator>
    bool equals(Comparator eq, const polynomial& g) const {
      if (g.num_monos() != num_monos()) { return false; }

      for (unsigned i = 0; i < g.num_monos(); i++) {
	if ( !g.monomial(i).equals(eq, monomial(i)) ) {
	  return false;
	}
      }

      return true;
    }

  };

  struct division_result {
    std::vector<polynomial> as;
    polynomial remainder;
  };

  // Add within_eps call
  inline bool double_eq(const double l, const double r) {
    return l == r;
  }

  // template<typename N>
  // class field_impl {};

  // template<>
  // class field_impl<double> {
  // public:
  //   static double zero() { return 0.0; }

  //   static polynomial<double> zero_polynomial(const int num_vars) {
  //     return polynomial<double>({}, num_vars);
  //   }

  // };

  // template<int>
  // polynomial<double> zero_polynomial<double>(const int num_vars) {
  //   std::vector<int> coeffs;
  //   for (int i = 0; i < num_vars; i++) {
  //     coeffs.push_back(0.0);
  //   }

  //   monomial<double> zero_monomial{0.0, coeffs, num_vars};

  //   return {{zero_monomial}, num_vars};
  // }
  

  template<typename MonomialOrder>
  division_result divide(const polynomial& f,
			 const std::vector<polynomial>& gs,
			 MonomialOrder m) {
    // std::vector<polynomial> as;
    // for (unsigned i = 0; i < gs.size(); i++) {
    //   as.push_back(field_impl<N>::zero_polynomial(f.num_vars()));
    // }

    // polynomial<N> zr = field_impl<N>::zero_polynomial(f.num_vars());
    // polynomial<N> p = f;
    // while (!poly_eq(field_impl<N>::equals, zr, p)) {
      
    // }

    return division_result{{}, {{}, 2}};
  }

  // template<typename N, typename Comparator>
  // bool poly_eq(Comparator eq,
  // 	       const polynomial<N>& f,
  // 	       const polynomial<N>& g) {
  //   return f.equals(eq, g);
  // }
  
}

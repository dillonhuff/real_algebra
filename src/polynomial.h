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

    bool equals(const monomial& g) const {
      assert(g.num_vars() == num_vars());

      if (!(g.coeff() == coeff())) { return false; }

      for (int i = 0; i < g.num_vars(); i++) {
	if (g.power(i) != power(i)) {
	  return false;
	}
      }

      return true;
    }
    
    void print(std::ostream& out) const {
      out << coeff();
      for (int i = 0; i < num_vars(); i++) {

	if (power(i) != 0) {

	  out << "x_" << i;
	  if (power(i) > 1) {
	    out << "^" << power(i);
	  }

	  if (i != (num_vars() - 1)) {
	    out << "*";
	  }

	}
      }
    }

  };

  inline monomial operator+(const monomial& x, const monomial& y) {
    assert(x.num_vars() == y.num_vars());

    std::vector<int> vars;
    for (int i = 0; i < x.num_vars(); i++) {
      vars.push_back(x.power(i));
    }
    return monomial(x.coeff() + y.coeff(), vars, vars.size());
  }

  inline bool same_powers(const monomial& x, const monomial& y) {
    assert(x.num_vars() == y.num_vars());

    for (int i = 0; i < x.num_vars(); i++) {
      if (x.power(i) != y.power(i)) {
	return false;
      }
    }
    return true;
  }

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


  inline std::ostream& operator<<(std::ostream& out, const monomial& m) {
    m.print(out);
    return out;
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

    bool equals(const polynomial& g) const {
      if (g.num_monos() != num_monos()) { return false; }

      for (unsigned i = 0; i < g.num_monos(); i++) {
	if ( !g.monomial(i).equals(monomial(i)) ) {
	  return false;
	}
      }

      return true;
    }

    template<typename MonomialOrder>
    class monomial lt(MonomialOrder m) const {
      std::vector<class monomial> ms = monos;
      return *max_element(begin(ms), end(ms), m);
    }

    // Fix this later
    polynomial plus(const polynomial& other) const {
      int this_ind = 0;
      int other_ind = 0;

      std::vector<class monomial> monos;
      while ((this_ind < num_monos()) &&
	     (other_ind < other.num_monos())) {
	// std::cout << "this ind = " << this_ind << std::endl;
	// std::cout << "other ind = " << this_ind << std::endl;

	auto this_mono = monomial(this_ind);
	auto other_mono = other.monomial(other_ind);

	if (same_powers(this_mono, other_mono)) {
	  monos.push_back(this_mono + other_mono);
	  this_ind++;
	  other_ind++;
	} else if (lexicographic_order(this_mono, other_mono)) {
	  other_ind++;
	} else {
	  this_ind++;
	}
      }

      std::cout << "Done with main adding" << std::endl;

      for (; this_ind < num_monos(); this_ind++) {
	monos.push_back(monomial(this_ind));
      }

      for (; other_ind < other.num_monos(); other_ind++) {
	monos.push_back(other.monomial(other_ind));
      }
      
      return polynomial(monos, num_vars());
    }

    polynomial minus(const polynomial& p) const {
      std::vector<class monomial> sum = monos;

      return polynomial(monos, num_vars());
    }

    polynomial times(const polynomial& other) const {

      std::vector<class monomial> sum = monos;

      return polynomial(monos, num_vars());
    }
    
    void print(std::ostream& out) const {
      
      for (int i = 0; i < num_monos(); i++) {
	out << monomial(i);
	if (i != (num_monos() - 1)) {
	  out << " + ";
	}
      }
    }

  };

  inline polynomial operator+(const polynomial& x,
			      const polynomial& y) {
    return x.plus(y);
  }

  inline polynomial operator*(const polynomial& x,
			      const polynomial& y) {
    return x.times(y);
  }
  
  inline polynomial operator-(const polynomial& x,
			      const polynomial& y) {
    return x.minus(y);
  }
  
  struct division_result {
    std::vector<polynomial> as;
    polynomial remainder;
  };

  // template<int>
  // polynomial<double> zero_polynomial<double>(const int num_vars) {
  //   std::vector<int> coeffs;
  //   for (int i = 0; i < num_vars; i++) {
  //     coeffs.push_back(0.0);
  //   }

  //   monomial<double> zero_monomial{0.0, coeffs, num_vars};

  //   return {{zero_monomial}, num_vars};
  // }

  inline monomial zero_monomial(const int num_vars) {
    rational z("0.0");
    std::vector<int> coeffs;
    for (int i = 0; i < num_vars; i++) {
      coeffs.push_back(0);
    }

    return {z, coeffs, num_vars};
  }

  inline polynomial zero_polynomial(const int num_vars) {
    monomial zm = zero_monomial(num_vars);
    return polynomial({zm}, num_vars);
  }


  inline bool operator==(const polynomial& l, const polynomial& r) {
    return l.equals(r);
  }

  inline bool operator!=(const polynomial& l, const polynomial& r) {
    return !(l == r);
  }

  inline std::ostream& operator<<(std::ostream& out, const polynomial& p) {
    p.print(out);
    return out;
  }

  inline bool divides(const monomial& v, const monomial& to_divide) {
    return false;
  }

  inline monomial quotient(const monomial& div, const monomial& dividor) {
    assert(false);
  }

  template<typename MonomialOrder>
  division_result divide(const polynomial& f,
			 const std::vector<polynomial>& gs,
			 MonomialOrder m) {
    std::vector<polynomial> as;
    for (unsigned i = 0; i < gs.size(); i++) {
      as.push_back(zero_polynomial(f.num_vars()));
    }

    polynomial zr = zero_polynomial(f.num_vars());
    polynomial p = f;
    polynomial r = zero_polynomial(f.num_vars());

    while (p != zr) {
      bool divided = false;
      for (int i = 0; i < gs.size(); i++) {
	monomial lt_fi = (gs[i]).lt(m);//, gs[i]);
	monomial lt_p = p.lt(m); //, p);

	if (divides(lt_fi, lt_p)) {
	  polynomial qr({quotient(lt_p, lt_fi)}, f.num_vars());
	  as[i] = as[i] + qr;
	  p = p - qr*gs[i];
	  divided = true;
	  break;
	}
      }

      if (!divided) {
	r = r + polynomial({p.lt(m)}, f.num_vars());
	p = p - polynomial({p.lt(m)}, f.num_vars());
      }
    }
    // polynomial<N> zr = field_impl<N>::zero_polynomial(f.num_vars());
    // polynomial<N> p = f;
    // while (!poly_eq(field_impl<N>::equals, zr, p)) {
      
    // }

    return division_result{as, r};
  }

}

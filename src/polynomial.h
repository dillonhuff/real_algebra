#pragma once

#include <cassert>
#include <vector>

#include "algorithm.h"
#include "rational.h"

namespace ralg {

  class monomial zero_monomial(const int num_vars);

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

    monomial scalar_times(const int i) const {
      monomial copy = *this;
      copy.c = copy.coeff().times({std::to_string(i)});
      return copy;
    }

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
      out << coeff() << " ";
      for (int i = 0; i < num_vars(); i++) {

	if (power(i) != 0) {

	  out << "x_" << i;
	  if (power(i) > 1) {
	    out << "^" << power(i);
	  }

	  if (i != (num_vars() - 1)) {
	    out << " ";
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

  inline monomial operator-(const monomial& x, const monomial& y) {
    assert(x.num_vars() == y.num_vars());

    std::vector<int> vars;
    for (int i = 0; i < x.num_vars(); i++) {
      vars.push_back(x.power(i));
    }
    return monomial(x.coeff() - y.coeff(), vars, vars.size());
  }

  inline monomial operator*(const int i, const monomial& x) {
    rational ir(std::to_string(i));

    std::vector<int> vars;
    for (int i = 0; i < x.num_vars(); i++) {
      vars.push_back(x.power(i));
    }

    return monomial(ir*x.coeff(), vars, x.num_vars());
  }

  inline monomial operator-(const monomial& x) {
    std::vector<int> vars;
    for (int i = 0; i < x.num_vars(); i++) {
      vars.push_back(x.power(i));
    }

    rational minus_one{"-1"};
    return monomial(minus_one*x.coeff(), vars, vars.size());
    
  }

  inline monomial operator*(const monomial& x, const monomial& y) {
    assert(x.num_vars() == y.num_vars());

    std::vector<int> vars;
    for (int i = 0; i < x.num_vars(); i++) {
      vars.push_back(x.power(i) + y.power(i));
    }
    return monomial(x.coeff() * y.coeff(), vars, vars.size());
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

  inline int total_degree(const monomial& x) {
    int tg = 0;

    for (int i = 0; i < x.num_vars(); i++) {
      tg += x.power(i);
    }

    return tg;
  }

  inline bool graded_lexicographic_order(const monomial& x,
					 const monomial& y) {
    assert(x.num_vars() == y.num_vars());

    if (total_degree(x) < total_degree(y)) {
      return true;
    }

    if (total_degree(x) > total_degree(y)) {
      return false;
    }

    return lexicographic_order(x, y);
  }
  
  inline bool graded_reverse_lexicographic_order(const monomial& x,
						 const monomial& y) {
    assert(x.num_vars() == y.num_vars());

    if (total_degree(x) < total_degree(y)) {
      return true;
    }

    if (total_degree(x) > total_degree(y)) {
      return false;
    }

    return !lexicographic_order(x, y);
  }


  inline std::ostream& operator<<(std::ostream& out, const monomial& m) {
    m.print(out);
    return out;
  }

  inline bool is_zero(const class monomial& m) {
    rational zr("0");
    return m.coeff() == zr;
  }

  inline bool is_constant(const class monomial& m) {
    for (int i = 0; i < m.num_vars(); i++) {
      if (m.power(i) != 0) {
  	return false;
      }
    }
    return true;
  }
  
  class polynomial {

    std::vector<monomial> monos;
    int nvars;

  public:
    polynomial(const std::vector<monomial> p_monos,
	       const int p_num_vars) {
      for (auto& mono : p_monos) {
	if (is_zero(mono)) {
	  continue;
	}

	bool added_to_existing = false;
	for (int i = 0; i < monos.size(); i++) {
	  auto& m = monos[i];
	  if (same_powers(m, mono)) {
	    monos[i] = monos[i] + mono;
	    added_to_existing = true;
	    break;
	  }
	}

	if (!added_to_existing) {
	  monos.push_back(mono);
	}

      }

      if (monos.size() == 0) {
	monos.push_back(zero_monomial(p_num_vars));
      }
      //monos = p_monos;
      std::sort(begin(monos), end(monos), lexicographic_order);
      nvars = p_num_vars;
    }

    monomial max_monomial_wrt(const int var_num) const {
      return max_e(monos,
		   [var_num](const class monomial& m) {
		     return m.power(var_num);
		   });
    }

    polynomial reductum(const int var_num, const int k) const {
      std::vector<class monomial> ms = monos;
      delete_if(ms, [var_num, k](const class monomial& m) {
	  return m.power(var_num) > k;
	});
      return polynomial(ms, num_vars());
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
	  monos.push_back(other_mono);
	  other_ind++;
	} else {
	  monos.push_back(this_mono);
	  this_ind++;
	}
      }

      // std::cout << "Done with main adding" << std::endl;
      // std::cout << "other_ind = " << other_ind << std::endl;
      // std::cout << "this_ind = " << this_ind << std::endl;

      for (; this_ind < num_monos(); this_ind++) {
	monos.push_back(monomial(this_ind));
      }

      for (; other_ind < other.num_monos(); other_ind++) {
	monos.push_back(other.monomial(other_ind));
      }
      
      return polynomial(monos, num_vars());
    }

    inline bool is_constant() const {
      return all_of(monos, ralg::is_constant);
    }

    polynomial minus(const polynomial& other) const {
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
	  monos.push_back(this_mono - other_mono);
	  this_ind++;
	  other_ind++;
	} else if (lexicographic_order(this_mono, other_mono)) {
	  monos.push_back(other_mono.scalar_times(-1));
	  other_ind++;
	} else {
	  monos.push_back(this_mono);
	  this_ind++;
	}
      }

      // std::cout << "Done with main adding" << std::endl;
      // std::cout << "other_ind = " << other_ind << std::endl;
      // std::cout << "this_ind = " << this_ind << std::endl;

      for (; this_ind < num_monos(); this_ind++) {
	monos.push_back(monomial(this_ind));
      }

      for (; other_ind < other.num_monos(); other_ind++) {
	monos.push_back(other.monomial(other_ind).scalar_times(-1));
      }
      
      return polynomial(monos, num_vars());

    }

    polynomial times(const polynomial& other) const {
      std::vector<class monomial> prod;
      for (auto& other_mono : other.monos) {
	for (auto& this_mono : monos) {
	  prod.push_back(other_mono * this_mono);
	}
      }

      return polynomial(prod, num_vars());
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

  inline polynomial const_poly(const int coeff, const int num_vars) {
    std::vector<int> zeros;
    for (int i = 0; i < num_vars; i++) {
      zeros.push_back(0);
    }
    monomial cm({coeff}, zeros, num_vars);
    return polynomial({cm}, num_vars);
  }

  inline polynomial operator+(const polynomial& x,
			      const polynomial& y) {
    return x.plus(y);
  }

  inline polynomial operator*(const polynomial& x,
			      const polynomial& y) {
    return x.times(y);
  }

  inline polynomial operator*(const int c,
			      const polynomial& y) {
    return const_poly(c, y.num_vars())*y;
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

  inline bool is_zero(const polynomial& m) {
    return m == zero_polynomial(m.num_vars());
  }
    
  inline bool divides(const monomial& v, const monomial& to_divide) {
    assert(v.num_vars() == to_divide.num_vars());

    rational zr{"0"};
    if (v.coeff() == zr) { return false; }

    for (int i = 0; i < v.num_vars(); i++) {
      if (v.power(i) > to_divide.power(i)) { return false; }
    }
    return true;
  }

  inline monomial quotient(const monomial& div, const monomial& v) {
    assert(v.num_vars() == div.num_vars());

    rational c = div.coeff() / v.coeff();

    std::vector<int> powers;
    for (int i = 0; i < v.num_vars(); i++) {
      powers.push_back(div.power(i) - v.power(i));
    }
    return monomial(c, powers, v.num_vars());

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

      //std::cout << "p = " << p << std::endl;

      bool divided = false;
      for (int i = 0; i < gs.size(); i++) {
	monomial lt_fi = (gs[i]).lt(m);//, gs[i]);
	monomial lt_p = p.lt(m); //, p);

	//std::cout << "Checking divide" << std::endl;

	if (divides(lt_fi, lt_p)) {

	  //std::cout << "Divides" << std::endl;
	  //std::cout << lt_fi << " divides " << lt_p << std::endl;

	  polynomial qr({quotient(lt_p, lt_fi)}, f.num_vars());
	  as[i] = as[i] + qr;
	  p = p - qr*gs[i];

	  //std::cout << "p after divide = " << p << std::endl;

	  divided = true;
	  break;
	}
      }

      if (!divided) {
	r = r + polynomial({p.lt(m)}, f.num_vars());
	p = p - polynomial({p.lt(m)}, f.num_vars());

	//std::cout << "p after reducex = " << p << std::endl;
      }
    }
    // polynomial<N> zr = field_impl<N>::zero_polynomial(f.num_vars());
    // polynomial<N> p = f;
    // while (!poly_eq(field_impl<N>::equals, zr, p)) {
      
    // }

    return division_result{as, r};
  }

  std::vector<polynomial> coefficients_wrt(const polynomial& p,
					   const int var_num);

  polynomial derivative_wrt(const int var_num,
			    const polynomial& p);

  int degree_wrt(const int var_num, const polynomial& p);

  monomial delete_var(const int var_num, const monomial& m);

  rational evaluate_at(const rational& val, const polynomial& p);

}

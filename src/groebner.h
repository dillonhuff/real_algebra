#pragma once

#include "algorithm.h"
#include "polynomial.h"

namespace ralg {

  template<typename MonomialOrder>
  polynomial s_poly(MonomialOrder ord,
		    const polynomial& l,
		    const polynomial& r) {
    monomial lt_l = l.lt(ord);
    monomial lt_r = r.lt(ord);
    // Not actually the LCM
    monomial lcm_lr = lt_l * lt_r;//lcm(lt_l, lt_r);

    monomial cancel_r = quotient(lcm_lr, lt_r);
    monomial cancel_l = quotient(lcm_lr, lt_l);

    polynomial cancel_r_poly({cancel_r} , l.num_vars());
    polynomial cancel_l_poly({cancel_l} , l.num_vars());

    return cancel_r_poly*r + cancel_l_poly*l;
  }

  template<typename MonomialOrder>
  std::vector<polynomial>
  buchberger(const std::vector<polynomial>& polys,
	     MonomialOrder ord) {

    std::vector<polynomial> g = polys;
    std::vector<polynomial> g_prime;

    do {
      g_prime = g;

      for (int i = 0; i < g_prime.size(); i++) {
	for (int j = 0; j < g_prime.size(); j++) {
	  if (i != j) {

	    polynomial sp = s_poly(ord, g_prime[i], g_prime[j]);

	    // Should I divide by g or g_prime here? I feel like g is the
	    // better choice, but Ideals, Varieties and algorithms says
	    // to use g_prime
	    auto res = divide(sp, g, ord);
	    auto& r = res.remainder;

	    if (!is_zero(r) && !elem(r, g)) {
	      g.push_back(r);
	    }
	  }
	}
      }

    } while (g_prime.size() != g.size());
    
    return g;
  }
}

#pragma once

#include <ralg/root_counting.h>
#include <ralg/polynomial.h>

#include <cassert>

namespace ralg {

  class rational_interval {
  protected:
    rational start, end;
    bool start_is_open, end_is_open;

  public:

    rational_interval(const rational& p_start, const bool p_start_is_open,
	     const rational& p_end, const bool p_end_is_open) :
      start(p_start), end(p_end),
      start_is_open(p_start_is_open),
      end_is_open(p_end_is_open) {

      assert(start <= end);

    }

    rational_interval(const int val) :
      start(val), end(val),
      start_is_open(false), end_is_open(false) {}

    rational get_start() const { return start; }
    rational get_end() const { return end; }

    bool is_point() const {
      if (start == end) {
	assert(!start_is_open);
	assert(!end_is_open);

	return true;
      }

      return false;
    }

    bool is_fully_open() const {
      return start_is_open && end_is_open;
    }
    
    bool inside(const rational& l) const {
      if (is_point()) {
	return l == start;
      }

      return false;
    }

    rational midpoint() const {
      rational two(2);
      return (start + end) / two;
    }

    rational width() const {
      return end - start;
    }

  };

  class algebraic_number {
  protected:
    polynomial p;
    rational_interval i;

  public:
    algebraic_number(const int val) :
      p({monomial(1, {1}, 1), monomial(rational(val), {0}, 1)}, 1),
      i(val) {}

    algebraic_number(const polynomial& p_p,
		     const rational_interval& i_p) : p(p_p), i(i_p) {
      assert(p.num_vars() == 1);
    }

    bool in_interval(const int val) const {
      rational r(val);
      return i.inside(r);
    }

    // Not const because the shrunk interval is saved
    double decimal_approximation(const double max_width) {
      if (i.is_point()) {
	return i.midpoint().to_double();
      }

      assert(i.is_fully_open());

      rational max_w(max_width);
      if (i.width() <= max_w) {
	return i.midpoint().to_double();
      }

      interval it{ipt(i.get_start()), ipt(i.get_end())};
      auto chain = sturm_chain(p);

      std::cout << "Original interval = " << it << std::endl;

      auto isolated = isolate_root(it, chain, max_w);

      std::cout << "Isolated interval = " << isolated << std::endl;

      rational_interval fresh(isolated.start.value,
			      false,
			      isolated.end.value,
			      false);

      i = fresh;

      return i.midpoint().to_double();
    }
  };


  typedef algebraic_number algnum;
}

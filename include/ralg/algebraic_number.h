#pragma once

#include <ralg/rational.h>

#include <cassert>

namespace ralg {

  class interval {
  protected:
    rational start, end;
    bool start_is_open, end_is_open;

  public:

    interval(const rational& p_start, const bool p_start_is_open,
	     const rational& p_end, const bool p_end_is_open) :
      start(p_start), end(p_end),
      start_is_open(p_start_is_open),
      end_is_open(p_end_is_open) {}

    interval(const int val) :
      start(val), end(val),
      start_is_open(false), end_is_open(false) {}
    

    bool is_point() const {
      if (start == end) {
	assert(!start_is_open);
	assert(!end_is_open);

	return true;
      }

      return false;
    }

    bool inside(const rational& l) const {
      if (is_point()) {
	return l == start;
      }

      return false;
    }

  };

  class algebraic_number {
  protected:
    interval i;

  public:
    algebraic_number(const int val) : i(val) {}

    bool in_interval(const int val) const {
      rational r(val);
      return i.inside(r);
    }
  };


  typedef algebraic_number algnum;
}

#pragma once

#include "polynomial.h"

namespace ralg {

  struct interval_pt {
    bool is_inf;
    bool is_neg_inf;
    rational value;
  };

  interval_pt neg_inf();

  interval_pt pos_inf();

  interval_pt ipt(const int r);
  
  struct interval {
    interval_pt start;
    interval_pt end;
  };

  int num_roots_in_interval(const interval& it,
			    const std::vector<polynomial>& sturm_chain);

  std::vector<polynomial> sturm_chain(const polynomial& p);
  
  std::vector<interval> isolate_roots(const polynomial& p);

  int num_real_roots(const polynomial& p);

}

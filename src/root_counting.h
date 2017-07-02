#pragma once

#include "polynomial.h"

namespace ralg {

  struct interval_pt {
    bool is_inf;
    bool is_neg_inf;
    rational value;
  };

  struct interval {
    interval_pt start;
    interval_pt end;
  };

  std::vector<interval> isolate_roots(const polynomial& p);

  int num_real_roots(const polynomial& p);

}

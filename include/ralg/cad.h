#pragma once

#include <ralg/polynomial.h>

namespace ralg {

  struct cell {
    std::vector<rational> test_pt;
  };

  std::vector<cell> build_CAD(const std::vector<polynomial>& polys);

}

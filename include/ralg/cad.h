#pragma once

#include <ralg/polynomial.h>

namespace ralg {

  struct cell {
    std::vector<rational> test_pt;
  };

  std::vector<cell> build_CAD(const std::vector<polynomial>& polys);

  std::vector<cell>
  base_and_extend(const std::vector<std::vector<polynomial>>& projection_sets,
		  const std::vector<polynomial>& polys);
  
}

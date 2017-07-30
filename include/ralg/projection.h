#pragma once

#include <ralg/polynomial.h>

namespace ralg {

  std::vector<polynomial> project(const int var_num,
				  const std::vector<polynomial>& polys);

  std::vector<polynomial>
  project_mccallum(const int var_num,
		   const std::vector<polynomial>& polys);
  
}

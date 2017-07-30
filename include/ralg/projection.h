#pragma once

#include <ralg/polynomial.h>

namespace ralg {

  std::vector<polynomial> project(const int var_num,
				  const std::vector<polynomial>& polys);

  std::vector<polynomial>
  mccallum_project(const int var_num,
		   const std::vector<polynomial>& polys);
  
}

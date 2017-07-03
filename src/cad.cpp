#include "cad.h"

using namespace std;

namespace ralg {

  vector<vector<polynomial> >
  build_projection_sets(const std::vector<polynomial>& polys) {
    return {};
  }

  std::vector<cell>
  solve_base_projection_set(const std::vector<polynomial>& polys) {
    return {};
  }
  
  std::vector<cell> extend_cells(const std::vector<cell>& previous_cells,
				 const std::vector<polynomial>& polys) {
    return {};
  }

  std::vector<cell> build_CAD(const std::vector<polynomial>& polys) {

    // Projection phase
    vector<vector<polynomial> > projection_sets =
      build_projection_sets(polys);

    // Base phase
    std::vector<cell> cells =
      solve_base_projection_set(projection_sets[0]);

    // Extension phase
    for (int i = 1; i < projection_sets.size(); i++) {
      cells = extend_cells(cells, projection_sets[i]);
    }

    return cells;
  }

}

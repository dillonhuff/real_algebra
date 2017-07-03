#include "cad.h"

#include "projection.h"

using namespace std;

namespace ralg {

  vector<vector<polynomial> >
  build_projection_sets(const std::vector<polynomial>& polys) {
    assert(polys.size() > 0);

    int n_vars = polys.front().num_vars();

    assert(n_vars > 0);

    vector<vector<polynomial> > projected;
    vector<polynomial> to_project = polys;
    for (int i = n_vars - 1; i >= 0; i--) {
      cout << "To project size = " << to_project.size() << endl;
      projected.push_back(project(i, to_project));
      to_project = projected.back();
    }

    return projected;
  }

  std::vector<cell>
  solve_base_projection_set(const std::vector<polynomial>& polys) {
    assert(polys.size() > 0);

    return {};
  }
  
  std::vector<cell> extend_cells(const std::vector<cell>& previous_cells,
				 const std::vector<polynomial>& polys) {
    assert(polys.size() > 0);

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

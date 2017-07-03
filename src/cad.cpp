#include "cad.h"

#include "projection.h"
#include "root_counting.h"

using namespace std;

namespace ralg {

  vector<vector<polynomial> >
  build_projection_sets(const std::vector<polynomial>& polys) {
    assert(polys.size() > 0);

    int n_vars = polys.front().num_vars();

    assert(n_vars > 0);

    vector<vector<polynomial> > projected;
    vector<polynomial> to_project = polys;
    for (int i = n_vars - 1; i > 0; i--) {
      cout << "To project size = " << to_project.size() << endl;
      projected.push_back(project(i, to_project));
      to_project = projected.back();
    }

    reverse(projected);
    return projected;
  }

  rational midpoint(const interval& r) {
    rational two(2);
    return (r.end.value + r.start.value) / two;
  }

  // Assumes cells are sorted along the last dimension
  std::vector<cell> insert_mid_cells(const std::vector<cell>& base_cells) {
    assert(base_cells.size() > 0);

    vector<cell> mcells;

    cell neg_inf = base_cells.front();
    neg_inf.test_pt[neg_inf.test_pt.size() - 1] =
      neg_inf.test_pt[neg_inf.test_pt.size() - 1] - 1;
    mcells.push_back(neg_inf);

    rational two(2);
    for (int i = 0; i < base_cells.size(); i++) {
      mcells.push_back(base_cells[i]);
      if (i < base_cells.size() - 1) {
	auto ci = base_cells[i].test_pt;
	auto cj = base_cells[i + 1].test_pt;

	auto c_mid = ci;
	c_mid[c_mid.size() - 1] = (ci[ci.size() - 1] + cj[cj.size() - 1]) / two;
	mcells.push_back({c_mid});
      }
    }

    cell pos_inf = base_cells.back();
    pos_inf.test_pt[pos_inf.test_pt.size() - 1] =
      pos_inf.test_pt[pos_inf.test_pt.size() - 1] + 1;
    mcells.push_back(pos_inf);
    
    
    assert(mcells.size() == (2*base_cells.size() + 1));

    return mcells;
  }

  std::vector<cell>
  solve_base_projection_set(const std::vector<polynomial>& polys) {
    assert(polys.size() > 0);

    rational tol("1/10000");
    vector<interval> root_intervals;
    for (auto& p : polys) {
      concat(root_intervals, isolate_roots(p, tol));
    }

    vector<rational> roots;
    for (auto& it : root_intervals) {
      roots.push_back(midpoint(it));
    }

    sort(begin(roots), end(roots));
    auto last =
      unique(begin(roots), end(roots),
	     [tol](const rational& l, const rational& r) {
	       return (r - l).abs() < tol;
	     });
    roots.erase(last, roots.end());

    cout << "roots" << endl;
    for (auto& r : roots) {
      cout << r.to_double() << endl;
    }

    vector<cell> base_cells;
    for (auto& r : roots) {
      base_cells.push_back({{r}});
    }

    return insert_mid_cells(base_cells);
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

    cout << "# of base cells = " << cells.size() << endl;
    for (auto& bc : cells) {
      cout << bc.test_pt.back().to_double() << endl;
    }

    // Extension phase
    for (int i = 1; i < projection_sets.size(); i++) {
      cells = extend_cells(cells, projection_sets[i]);
    }

    return cells;
  }

}
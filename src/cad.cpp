#include <ralg/cad.h>

#include <ralg/projection.h>
#include <ralg/root_counting.h>

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
      //cout << "To project size = " << to_project.size() << endl;
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
    //cout << "Inserting mid cells" << endl;
    //assert(base_cells.size() > 0);
    if (base_cells.size() == 0) { return {}; }

    //cout << "creating mid cells" << endl;

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

    //cout << "Isolating roots" << endl;

    rational tol("1/10000");
    vector<interval> root_intervals;
    for (auto& p : polys) {
      concat(root_intervals, isolate_roots(p, tol));
    }

    //cout << "Done isolating roots" << endl;

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

    // cout << "roots" << endl;
    // for (auto& r : roots) {
    //   cout << r.to_double() << endl;
    // }

    vector<cell> base_cells;
    for (auto& r : roots) {
      base_cells.push_back({{r}});
    }

    //cout << "# of base cells = " << base_cells.size() << endl;

    return insert_mid_cells(base_cells);
  }

  std::ostream& operator<<(std::ostream& out, const cell& c) {
    for (auto& t : c.test_pt) {
      out << t << ", ";
    }
    return out;
  }

  std::vector<cell> extend_cell(const cell& previous_cell,
				const std::vector<polynomial>& polys) {
    assert(polys.size() > 0);
    
    //cout << "Extending cells of " << polys.size() << " polynomials" << endl;

    vector<polynomial> upolys;
    for (auto& p : polys) {
      auto up = evaluate_at(previous_cell.test_pt, p);

      //cout << "p evaluates to " << up << endl;
      for (int i = 0; i < up.num_monos(); i++) {
	auto m = up.monomial(i);
	//cout << "coefficient = " << m.coeff().to_double() << endl;
      }

      upolys.push_back(up);
    }

    auto cell_tops = solve_base_projection_set(upolys);

    // cout << "*********** cell tops " << endl;
    // for (auto& c : cell_tops) {
    //   cout << c << endl;
    // }
    
    vector<cell> cells;
    for (auto& c : cell_tops) {
      cell new_c = previous_cell;
      new_c.test_pt.push_back(c.test_pt.back());
      cells.push_back(new_c);
    }

    return cells;
  }

  std::vector<cell> extend_cells(const std::vector<cell>& previous_cells,
				 const std::vector<polynomial>& polys) {
    assert(polys.size() > 0);

    vector<cell> new_cells;

    for (auto& cell : previous_cells) {
      concat(new_cells, extend_cell(cell, polys));
    }

    //cout << "new cells size = " << new_cells.size() << endl;
    return new_cells;
  }


  std::vector<cell>
  extend_projection_sets(const std::vector<cell>& initial_cells,
			 const std::vector<std::vector<polynomial>>& projection_sets,
			 const std::vector<polynomial>& polys) {
    vector<cell> cells = initial_cells;
    // Extension phase
    for (int i = 1; i < projection_sets.size(); i++) {
      //cout << "Extending i = " << i << endl;
      cells = extend_cells(cells, projection_sets[i]);
    }

    cells = extend_cells(cells, polys);

    return cells;
  }

  std::vector<cell>
  base_and_extend(const std::vector<std::vector<polynomial>>& projection_sets,
		  const std::vector<polynomial>& polys) {
    // Base phase
    std::vector<cell> cells =
      solve_base_projection_set(projection_sets[0]);

    return extend_projection_sets(cells, projection_sets, polys);
  }

  std::vector<cell> build_CAD(const std::vector<polynomial>& polys) {

    // Projection phase
    vector<vector<polynomial> > projection_sets =
      build_projection_sets(polys);

    return base_and_extend(projection_sets, polys);
  }

}

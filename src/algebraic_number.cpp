#include <ralg/algebraic_number.h>
#include <ralg/projection.h>

using namespace std;

namespace ralg {

  void algebraic_number::refine_by_half() {
    rational two(2);
    rational max_width = get_interval().width() / two;

    interval it{ipt(get_interval().get_start()), ipt(get_interval().get_end())};
    auto chain = sturm_chain(p);

    auto isolated = isolate_root(it, chain, max_width);

    rational_interval fresh(isolated.start.value,
			    false,
			    isolated.end.value,
			    false);

    i = fresh;
  }

  bool is_inside(const interval& it,
		 const interval& container) {
    assert(is_finite(it));
    assert(is_finite(container));

    return (container.start.value <= it.start.value) &&
      (it.end.value <= container.end.value);
  }

  interval sum_interval(const algebraic_number& a,
			const algebraic_number& b) {
    rational sum_start = a.get_interval().get_start() +
      b.get_interval().get_start();
    rational sum_end = a.get_interval().get_end() +
      b.get_interval().get_end();

    interval sum_it{ipt(sum_start), ipt(sum_end)};

    return sum_it;
  }

  std::vector<interval>
  possible_sum_intervals(const algebraic_number& a,
			 const algebraic_number& b,
			 const vector<interval>& its) {
    interval sum_it = sum_interval(a, b);

    vector<interval> possible;
    for (auto& it : its) {
      if (is_inside(sum_it, it)) {
	possible.push_back(it);
      }
    }

    return possible;
  }

  algebraic_number algebraic_number::plus(algebraic_number& other) {

    monomial p_x(1, {1, 0}, 2);
    monomial p_y(1, {0, 1}, 2);

    polynomial x({p_x}, 2);
    polynomial y({p_y}, 2);

    polynomial xmy = x - y;
    polynomial y_minus_x_sub = const_poly(0, 2);
    for (int i = 0; i < p.num_monos(); i++) {
      int x_pow = p.monomial(i).power(0);
      rational x_coeff = p.monomial(i).coeff();

      polynomial pm = x_coeff * pow(xmy, x_pow);

      y_minus_x_sub = y_minus_x_sub + pm;
    }

    polynomial y_sub = const_poly(0, 2);
    for (int i = 0; i < p.num_monos(); i++) {
      int x_pow = p.monomial(i).power(0);
      rational x_coeff = p.monomial(i).coeff();

      polynomial pm = x_coeff * pow(y, x_pow);

      y_sub = y_sub + pm;
    }

    cout << y_minus_x_sub << endl;
    cout << y_sub << endl;

    polynomial res =
      delete_var(1, resultant(1, y_minus_x_sub, y_sub));

    cout << "resultant = " << res << endl;

    auto its = isolate_roots(res);

    cout << "# of roots = " << endl;
    for (auto& it : its) {
      cout << it << endl;
    }

    vector<interval> possible_its =
      possible_sum_intervals(*this, other, its);

    while (possible_its.size() != 1) {
      cout << "# of possible intervals = " << possible_its.size() << endl;
      refine_by_half();
      other.refine_by_half();

      possible_its =
	possible_sum_intervals(*this, other, its);

    }

    assert(possible_its.size() == 1);

    rational s = possible_its[0].start.value;
    rational e = possible_its[0].end.value;

    assert(s != e);

    rational_interval sol_interval(s, true, e, true);
    return algebraic_number(res, sol_interval);
  }

}

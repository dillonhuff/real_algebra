#include "root_counting.h"

using namespace std;

namespace ralg {

  int sign_at_minus_infinity(const polynomial& p) {
    monomial m = p.max_monomial_wrt(0);
    if (m.power(0) == 0) {
      return m.coeff().sign();
    }

    if (degree_wrt(0, p) % 2 == 0) {
      return m.coeff().sign();
    }
    return -1*m.coeff().sign();
  }

  int sign_at_infinity(const polynomial& p) {
    monomial m = p.max_monomial_wrt(0);
    return m.coeff().sign();
  }
  
  vector<int>
  signs_at_minus_infinity(const std::vector<polynomial>& sequence) {
    vector<int> sgs;
    for (auto& p : sequence) {
      sgs.push_back(sign_at_minus_infinity(p));
    }
    return sgs;
  }

  vector<int>
  signs_at_infinity(const std::vector<polynomial>& sequence) {
    vector<int> sgs;
    for (auto& p : sequence) {
      sgs.push_back(sign_at_infinity(p));
    }
    return sgs;
  }

  vector<int> signs_at(const interval_pt& pt, const std::vector<polynomial>& p) {
    if (pt.is_inf) { return signs_at_infinity(p); }
    if (pt.is_neg_inf) { return signs_at_minus_infinity(p); }

    vector<int> sgs;
    for (auto& poly : p) {
      sgs.push_back(evaluate_at(pt.value, poly).sign());
    }
    return sgs;
  }

  int num_sign_changes(const std::vector<int>& signs) {
    if (signs.size() < 2) {
      return 0;
    }

    int num_changes = 0;
    for (int i = 0; i < signs.size() - 1; i++) {
      if (signs[i] != signs[i + 1]) {
	num_changes++;
      }
    }
    return num_changes;
  }

  vector<polynomial> sturm_chain(const polynomial& p) {
    polynomial rp = p;
    polynomial p2 = derivative_wrt(0, rp);
    vector<polynomial> sequence{rp, p2};
    while (degree_wrt(0, sequence.back()) > 0) {
      auto div_res =
	divide(sequence[sequence.size() - 2],
	       {sequence.back()},
	       lexicographic_order);

      if (is_zero(div_res.remainder)) {
	break;
      }

      sequence.push_back(const_poly(-1, 1)*div_res.remainder);
    }

    return sequence;
  }

  int num_real_roots(const polynomial& p) {
    assert(p.num_vars() == 1);

    if (degree_wrt(0, p) == 0) { return 0; }

    auto sequence = sturm_chain(p);

    // cout << "Sequence" << endl;
    // for (auto& p : sequence) {
    //   cout << p << endl;
    // }
    vector<int> neg_signs = signs_at_minus_infinity(sequence);
    // cout << "Neg signs" << endl;
    // for (auto s : neg_signs) {
    //   cout << s << endl;
    // }

    vector<int> pos_signs = signs_at_infinity(sequence);
    // cout << "Pos signs" << endl;
    // for (auto s : pos_signs) {
    //   cout << s << endl;
    // }

    int neg_changes = num_sign_changes(neg_signs);
    int pos_changes = num_sign_changes(pos_signs);

    // cout << "neg changes = " << neg_changes << endl;
    // cout << "pos changes = " << pos_changes << endl;

    return neg_changes - pos_changes;
  }

  interval_pt neg_inf() {
    return {false, true, {0}};
  }

  interval_pt pos_inf() {
    return {true, false, {0}};
  }

  interval_pt ipt(const int r) {
    return {false, false, {r}};
  }

  interval_pt ipt(const rational r) {
    return {false, false, r};
  }
  
  vector<interval> split_interval(const interval& it, const polynomial& p) {
    interval_pt left_mid = ipt(0); // Dummy value
    interval_pt right_mid = ipt(0); // Dummy value
    std::vector<interval> fresh_intervals;

    rational two(2);
    
    if (it.end.is_inf) {
      left_mid = ipt(2*it.start.value);
      right_mid = ipt(2*it.start.value);
    } else if (it.end.is_neg_inf) {
      left_mid = ipt(2*it.end.value);
      right_mid = ipt(2*it.end.value);
      assert(false);
    } else {
      left_mid = ipt((it.start.value + it.end.value) / two);
      right_mid = left_mid;
    }
    //    return {{it.start, mid}, {mid, it.end}};

    // if (evaluate_at(left_mid.value, p) == rational(0)) {
    //   auto lmm = ipt((it.start.value + left_mid.value) / two);
    //   auto rmm = ipt((it.end.value + left_mid.value) / two);
    //   fresh_intervals.push_back({lmm, rmm});
    //   left_mid = lmm;
    //   right_mid = rmm;
    // }
    concat(fresh_intervals, {{it.start, left_mid}, {right_mid, it.end}});

    //    assert(fresh_intervals.size() == 2);

    return fresh_intervals;
  }

  int num_roots_in_interval(const interval& it,
			    const std::vector<polynomial>& sturm_chain) {
    vector<int> neg_signs = signs_at(it.start, sturm_chain);
    // cout << "Neg signs" << endl;
    // for (auto s : neg_signs) {
    //   cout << s << endl;
    // }

    vector<int> pos_signs = signs_at(it.end, sturm_chain);
    // cout << "Pos signs" << endl;
    // for (auto s : pos_signs) {
    //   cout << s << endl;
    // }

    int neg_changes = num_sign_changes(neg_signs);
    int pos_changes = num_sign_changes(pos_signs);

    // cout << "neg changes = " << neg_changes << endl;
    // cout << "pos changes = " << pos_changes << endl;

    return neg_changes - pos_changes;

  }

  interval
  finitize_interval(const interval& it,
		    const std::vector<polynomial>& chain) {
    if (is_finite(it)) { return it; }

    rational two(2);
    if (it.end.is_inf) {
      rational mid = two*it.start.value;
      interval finite{it.start, ipt(mid)};

      while (num_roots_in_interval(finite, chain) < 1) {
	mid = two*mid;
	finite = {it.start, ipt(mid)};
      }
      return finite;
    }

    if (it.start.is_neg_inf) {
      rational mid = two*it.end.value;
      interval finite{ipt(mid), it.end};

      while (num_roots_in_interval(finite, chain) < 1) {
	mid = two*mid;
	finite = {ipt(mid), it.end};
      }
      return finite;
    }

    assert(false);
  }

  std::vector<interval>
  finitize_intervals(const std::vector<interval>& its,
		     const std::vector<polynomial>& chain) {
    vector<interval> fts;
    for (auto& it : its) {
      fts.push_back(finitize_interval(it, chain));
    }
    return fts;
  }

  std::vector<interval> isolate_roots(const polynomial& p) {
    assert(p.num_vars() == 1);

    auto chain = sturm_chain(p);

    // cout << "----- Sturm chain" << endl;
    // for (auto& c : chain) {
    //   cout << c << endl;
    // }

    //cout << "----- Signs at inf" << endl;

    auto sgs = signs_at_infinity(chain);

    // for (auto& s : sgs) {
    //   cout << s << endl;
    // }

    // cout << endl;

    rational lend("-11233 / 11230");
    rational lstart("11237 / 11230");
    
    vector<interval> isolated;
    vector<interval> in_progress{{neg_inf(), ipt(lend)},
	{ipt(lend), ipt(lstart)}, {ipt(lstart), pos_inf()}};

    // cout << "Initial intervals" << endl;
    // for (auto& it : in_progress) {
    //   cout << it << endl;
    // }

    while (in_progress.size() > 0) {
      interval it = in_progress.back();
      in_progress.pop_back();

      int nroots = num_roots_in_interval(it, chain);

      //cout << p << " has " << nroots << " roots in " << it << endl;

      if (nroots == 0) {
	//cout << "No roots in " << it << endl;
	continue;
      } else if (nroots == 1) {
	//cout << "1 root in " << it << endl;

	isolated.push_back(it);
      } else {
	vector<interval> split = split_interval(it, p);

	// cout << "Split " << it << " into ";
	// for (auto& it : split) {
	//   cout << it << " ";
	// }
	// cout << endl;

	concat(in_progress, split);
      }
    }

    return finitize_intervals(isolated, chain);
  }

  rational width(const interval& it) {
    assert(is_finite(it));

    return it.end.value - it.start.value;
  }

  interval refine_root_interval(const interval& it,
				const std::vector<polynomial>& chain) {
    rational two(2);
    rational mid = (it.end.value + it.start.value) / two;

    // Note: Add code to refine mid in case of exact match
    assert(evaluate_at(mid, chain[0]) != rational(0));

    interval left{it.start, ipt(mid)};
    interval right{ipt(mid), it.end};

    if (num_roots_in_interval(left, chain) == 1) {
      return left;
    }

    return right;
  }

  interval isolate_root(const interval& it,
			const std::vector<polynomial>& chain,
			const rational& max_interval_width) {
    assert(is_finite(it));

    interval ref_it = it;
    while (width(ref_it) > max_interval_width) {
      ref_it = refine_root_interval(ref_it, chain);
    }
    return ref_it;
  }

  std::vector<interval> isolate_roots(const polynomial& p,
				      const rational& max_interval_width) {
    auto its = isolate_roots(p);
    auto chain = sturm_chain(p);
    vector<interval> isolated;
    for (auto& it : its) {
      isolated.push_back(isolate_root(it, chain, max_interval_width));
    }
    return isolated;
  }
  
  std::ostream& operator<<(std::ostream& out, const interval_pt& pt) {
    if (pt.is_inf) {
      out << "inf";
    } else if (pt.is_neg_inf) {
      out << "-inf";
    } else {
      out << pt.value;
    }

    return out;
  }

  std::ostream& operator<<(std::ostream& out, const interval& it) {
    out << "( " << it.start << " , " << it.end << " )";
    return out;
  }

  bool is_finite(const interval_pt& pt) {
    return !pt.is_inf && !pt.is_neg_inf;
  }

  bool is_finite(const interval& it) {
    return is_finite(it.start) && is_finite(it.end);
  }
}

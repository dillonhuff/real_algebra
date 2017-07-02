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

  rational evaluate_at(const rational& val, const polynomial& p) {
    assert(p.num_vars() == 1);

    rational sum(0);
    for (int i = 0; i < p.num_monos(); i++) {
      auto m = p.monomial(i);

      rational res(1);
      for (int j = 0; j < m.power(0); j++) {
	res = res*val;
      }

      sum = sum + res;
    }
    return sum;
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

      sequence.push_back(const_poly(-1, 1)*div_res.remainder);
    }

    return sequence;
  }

  int num_real_roots(const polynomial& p) {
    assert(p.num_vars() == 1);

    if (degree_wrt(0, p) == 0) { return 0; }

    auto sequence = sturm_chain(p);
    // polynomial rp = p;
    // polynomial p2 = derivative_wrt(0, rp);
    // vector<polynomial> sequence{rp, p2};
    // while (degree_wrt(0, sequence.back()) > 0) {
    //   auto div_res =
    // 	divide(sequence[sequence.size() - 2],
    // 	       {sequence.back()},
    // 	       lexicographic_order);

    //   sequence.push_back(const_poly(-1, 1)*div_res.remainder);
    // }

    // cout << "Sequence" << endl;
    // for (auto& p : sequence) {
    //   cout << p << endl;
    // }
    vector<int> neg_signs = signs_at_minus_infinity(sequence);
    cout << "Neg signs" << endl;
    for (auto s : neg_signs) {
      cout << s << endl;
    }

    vector<int> pos_signs = signs_at_infinity(sequence);
    cout << "Pos signs" << endl;
    for (auto s : pos_signs) {
      cout << s << endl;
    }

    int neg_changes = num_sign_changes(neg_signs);
    int pos_changes = num_sign_changes(pos_signs);

    cout << "neg changes = " << neg_changes << endl;
    cout << "pos changes = " << pos_changes << endl;

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

  vector<interval> split_interval(const interval& it) {
    return {};
  }

  int num_roots_in_interval(const interval& it,
			    const std::vector<polynomial>& sturm_chain) {
    vector<int> neg_signs = signs_at(it.start, sturm_chain);
    cout << "Neg signs" << endl;
    for (auto s : neg_signs) {
      cout << s << endl;
    }

    vector<int> pos_signs = signs_at(it.end, sturm_chain);
    cout << "Pos signs" << endl;
    for (auto s : pos_signs) {
      cout << s << endl;
    }

    int neg_changes = num_sign_changes(neg_signs);
    int pos_changes = num_sign_changes(pos_signs);

    cout << "neg changes = " << neg_changes << endl;
    cout << "pos changes = " << pos_changes << endl;

    return neg_changes - pos_changes;

  }

  std::vector<interval> isolate_roots(const polynomial& p) {
    assert(p.num_vars() == 1);

    auto chain = sturm_chain(p);
    
    vector<interval> isolated;
    vector<interval> in_progress{{neg_inf(), ipt(-1)},
	{ipt(-1), ipt(1)}, {ipt(1), pos_inf()}};


    while (in_progress.size() > 0) {
      interval it = in_progress.back();
      in_progress.pop_back();

      int nroots = num_roots_in_interval(it, chain);
      if (nroots == 0) {
	continue;
      } else if (nroots == 1) {
	isolated.push_back(it);
      } else {
	vector<interval> split = split_interval(it);
	concat(in_progress, split);
      }
    }
    
    return isolated;
  }

}

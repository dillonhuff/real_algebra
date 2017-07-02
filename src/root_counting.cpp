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
    if (degree_wrt(0, p) % 2 == 0) {
      return 1;
    }

    return 1;
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
  
  int num_real_roots(const polynomial& p) {
    assert(p.num_vars() == 1);

    if (degree_wrt(0, p) == 0) { return 0; }

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

    cout << "Sequence" << endl;
    for (auto& p : sequence) {
      cout << p << endl;
    }
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

}

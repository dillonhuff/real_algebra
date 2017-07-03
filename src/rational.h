#pragma once

#include <gmp.h>
#include <iostream>
#include <string>

namespace ralg {

  class rational {
  protected:
    mpq_t val;

  public:
    rational(mpq_t tmp) {
      mpq_init(val);
      mpq_swap(val, tmp);
      mpq_canonicalize(val);
    }

    rational(const int value) {
      mpq_init(val);
      mpq_set_str(val, std::to_string(value).c_str(), 10);
      mpq_canonicalize(val);
    }
    
    rational(const std::string& value) {
      mpq_init(val);
      mpq_set_str(val, value.c_str(), 10);
      mpq_canonicalize(val);
    }

    int sign() const { return mpq_sgn(val); }

    bool equals(const rational& l) const {
      if (mpq_equal(val, l.val)) {
	return true;
      }
      return false;
    }

    rational plus(const rational& l) const {
      mpq_t sum;
      mpq_init(sum);
      mpq_add(sum, val, l.val);
      return rational(sum);
    }

    rational times(const rational& l) const {
      mpq_t prod;
      mpq_init(prod);
      mpq_mul(prod, val, l.val);
      return rational(prod);
    }

    // rational lcm(const rational& l) const {
    //   mpq_t prod;
    //   mpq_init(prod);
    //   mpq_lcm(prod, val, l.val);
    //   return rational(prod);
    // }

    rational divide(const rational& l) const {
      mpq_t prod;
      mpq_init(prod);
      mpq_div(prod, val, l.val);
      return rational(prod);
    }

    int cmp(const rational& other) const {
      return mpq_cmp(val, other.val);
    }

    void print(std::ostream& out) const {
      out << val;
    }

    double to_double() const {
      return mpq_get_d(val);
    }

    rational abs() const {
      mpq_t prod;
      mpq_init(prod);
      mpq_abs(prod, val);
      return rational(prod);
    }
    
  };

  inline std::ostream& operator<<(std::ostream& out, const rational& r) {
    r.print(out);
    return out;
  }

  inline rational operator+(const rational& l, const rational& r) {
    return l.plus(r);
  }

  inline bool operator<(const rational& l, const rational& r) {
    int v = l.cmp(r);
    return v < 0;
  }

  inline bool operator>(const rational& l, const rational& r) {
    int v = l.cmp(r);
    return v > 0;
  }
  
  inline rational operator*(const rational& l, const rational& r) {
    return l.times(r);
  }
  
  inline rational operator/(const rational& l, const rational& r) {
    return l.divide(r);
  }
  
  inline rational operator-(const rational& l) {
    return l.times({"-1"});
  }

  inline rational operator-(const rational& l, const rational& r) {
    return l.plus(-r);
  }
  
  inline bool operator==(const rational& l, const rational& r) {
    return l.equals(r);
  }

  inline bool operator!=(const rational& l, const rational& r) {
    return !(l == r);
  }
  
}

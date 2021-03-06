#pragma once

#include <gmp.h>
#include <iostream>
#include <string>

namespace ralg {

  class rational {
  protected:
    mpq_t val;

  public:

    rational() {
      mpq_init(val);
    }

    rational(const mpq_t& tmp) {
      mpq_init(val);
      //mpq_swap(val, tmp);
      mpq_set(val, tmp);
      mpq_canonicalize(val);

      //mpq_clear(tmp);
    }

    rational(const rational& other) {
      mpq_init(val);
      //mpq_swap(val, tmp);
      mpq_set(val, other.val);
      mpq_canonicalize(val);

      //mpq_clear(tmp);
    }
    
    rational(const int value) {
      mpq_init(val);
      mpq_set_str(val, std::to_string(value).c_str(), 10);
      mpq_canonicalize(val);
    }

    rational(const double value) {
      mpq_init(val);
      mpq_set_d(val, value);
      mpq_canonicalize(val);
    }
    
    rational(const std::string& value) {
      mpq_init(val);
      mpq_set_str(val, value.c_str(), 10);
      mpq_canonicalize(val);
    }

    rational& operator=(const rational& other) {
      if (&other == this) {
	return *this;
      }

      mpq_init(val);
      mpq_set(val, other.val);
      mpq_canonicalize(val);
      // std::cout << "copy assignment of A\n";
      // std::swap(n, other.n);
      // std::swap(s1, other.s1);
      return *this;
    }    

    int sign() const { return mpq_sgn(val); }

    bool equals(const rational& l) const {
      if (mpq_equal(val, l.val)) {
	return true;
      }
      return false;
    }

    void clear() {
      mpq_clear(val);
    }

    rational plus(const rational& l) const {
      mpq_t sum;
      mpq_init(sum);
      mpq_add(sum, val, l.val);
      rational s(sum);

      mpq_clear(sum);

      return s;
    }

    rational times(const rational& l) const {
      mpq_t prod;
      mpq_init(prod);
      mpq_mul(prod, val, l.val);
      rational p(prod);

      mpq_clear(prod);

      return p;
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

      
      rational p(prod);
      mpq_clear(prod);

      return p;
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
      rational p(prod);

      mpq_clear(prod);

      return p;
    }

    ~rational() {
      mpq_clear(val);
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

  inline bool operator>=(const rational& l, const rational& r) {
    int v = l.cmp(r);
    return v >= 0;
  }

  inline bool operator<=(const rational& l, const rational& r) {
    int v = l.cmp(r);
    return v <= 0;
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

  inline rational pow(const rational& l, const int exp) {
    rational r(1);
    for (int i = 0; i < exp; i++) {
      r = r*l;
    }

    return r;
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

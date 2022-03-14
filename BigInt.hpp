//
// Created by yeloki on 14.03.2022.
//

#ifndef CORE_BIGINT_HPP_
#define CORE_BIGINT_HPP_
#ifndef NDEBUG
#include <sstream>
#endif
#include <cmath>
#include <cstring>
#include <iostream>
#include <cassert>
#include <iomanip>
class BigInt {
 private:
  const size_t data_size_ = 10000;
 public:
  bool sign_;
  const size_t base_ = size_t(1e9);
  size_t size_;
  uint64_t *data_ = nullptr;
 public:
  // constructors
  BigInt();
  template<typename T, typename Void=void>
  explicit BigInt(T num);
  template<typename Void=void>
  explicit BigInt(const char *str);
  template<typename Void=void>
  explicit BigInt(char *str);
  BigInt(const BigInt &other);
  BigInt(BigInt &&other) noexcept;
  BigInt &operator=(const BigInt &right);

  // destructor
  ~BigInt();

  // unary
  BigInt &operator+();
  BigInt &operator-();
  explicit operator bool() const;
};
BigInt abs(const BigInt &num) {
  BigInt res = num;
  res.sign_ = false;
  return res;
}

// CIO operators
std::ostream &operator<<(std::ostream &out, const BigInt &num);
std::istream &operator>>(std::istream &in, BigInt &num);

// logic
bool operator==(const BigInt &left, const BigInt &right);
bool operator<(const BigInt &left, const BigInt &right);
bool operator!=(const BigInt &left, const BigInt &right);
bool operator<=(const BigInt &left, const BigInt &right);
bool operator>(const BigInt &left, const BigInt &right);
bool operator>=(const BigInt &left, const BigInt &right);
template<typename T>
bool operator==(const BigInt &left, const T &right);
template<typename T>
bool operator<(const BigInt &left, const T &right);
template<typename T>
bool operator!=(const BigInt &left, const T &right);
template<typename T>
bool operator<=(const BigInt &left, const T &right);
template<typename T>
bool operator>(const BigInt &left, const T &right);
template<typename T>
bool operator>=(const BigInt &left, const T &right);

// binary class/class
BigInt operator+(const BigInt &left, const BigInt &right);
template<typename T>
BigInt operator+(const BigInt &left, const T &right);
BigInt operator-(const BigInt &left, const BigInt &right);
template<typename T>
BigInt operator0(const BigInt &left, const T &right);
BigInt operator*(const BigInt &left, const BigInt &right);
template<typename T>
BigInt operator*(const BigInt &left, const T &right);
BigInt operator/(const BigInt &left, const BigInt &right);
template<typename T>
BigInt operator/(const BigInt &left, const T &right);
BigInt operator%(const BigInt &left, const BigInt &right);
template<typename T>
BigInt operator%(const BigInt &left, const T &right);

// complex class/class
BigInt &operator+=(BigInt &left, const BigInt &right);
template<typename T>
BigInt &operator+=(BigInt &left, const T &right);
BigInt &operator-=(BigInt &left, const BigInt &right);
template<typename T>
BigInt &operator-=(BigInt &left, const T &right);
BigInt &operator*=(BigInt &left, const BigInt &right);
template<typename T>
BigInt &operator*=(BigInt &left, const T &right);
BigInt &operator/=(BigInt &left, const BigInt &right);
template<typename T>
BigInt &operator/=(BigInt &left, const T &right);
BigInt &operator%=(BigInt &left, const BigInt &right);
template<typename T>
BigInt &operator%=(BigInt &left, const T &right);

BigInt &operator++(BigInt &left);
const BigInt operator++(BigInt &left, int);
BigInt &operator--(BigInt &left);
const BigInt operator--(BigInt &left, int);

//_                   start constructors                    _//
template<typename T, typename Void>
BigInt::BigInt(T num) {
  free(data_);
  data_ = new uint64_t[data_size_]{0};
  size_ = 0;
  sign_ = (num < 0);
  if (num <= 0)
    num *= -1;
  if (num == 0) {
    size_ = 1;
    return;
  }
  int64_t index = 0;
  while (num) {
    data_[index] = num % base_;
    num /= base_;
    size_++;
    index++;
  }
}
template<typename Void>
BigInt::BigInt(const char *str) {
  if (str == nullptr) throw std::exception();
  if (std::strlen(str) == 0) {
    delete[] data_;
    data_ = new uint64_t[data_size_]{0};
    size_ = 1;
    sign_ = false;
    return;
  }
  sign_ = false;
  free(data_);
  data_ = new uint64_t[data_size_]{0};
  size_ = (std::strlen(str) + 8 - (str[0] == '-' || str[0] == '+')) / 9;
  if (str[0] == '-' || str[0] == '+') sign_ = (str[0] == '-');
  int64_t len = std::strlen(str);
  for (int64_t i(std::strlen(str) - 1); i >= 0 && std::isdigit(str[i]); i--) {
    data_[(len - i - 1) / 9] += pow(10, (len - i - 1) % 9) * (str[i] - '0');
  }
}
template<typename Void>
BigInt::BigInt(char *str) {
  if (str == nullptr) throw std::exception();
  if (std::strlen(str) == 0) {
    delete[] data_;
    data_ = new uint64_t[data_size_]{0};
    size_ = 1;
    sign_ = false;
    return;
  }
  sign_ = false;
  free(data_);
  data_ = new uint64_t[data_size_]{0};
  size_ = (std::strlen(str) + 8 - (str[0] == '-' || str[0] == '+')) / 9;
  if (str[0] == '-' || str[0] == '+') sign_ = (str[0] == '-');
  int64_t len = std::strlen(str);
  for (int64_t i(std::strlen(str) - 1); i >= 0 && std::isdigit(str[i]); i--) {
    data_[(len - i - 1) / 9] += pow(10, (len - i - 1) % 9) * (str[i] - '0');
  }
}
BigInt::BigInt() {
  free(data_);
  data_ = new uint64_t[data_size_]{0};
  size_ = 1;
  sign_ = false;
}
BigInt::BigInt(const BigInt &other) {
  free(data_);
  data_ = new uint64_t[data_size_]{0};
  size_ = other.size_;
  sign_ = other.sign_;
  for (int64_t i = 0; i < size_; ++i) {
    data_[i] = other.data_[i];
  }
};
BigInt::BigInt(BigInt &&other) noexcept {
  sign_ = other.sign_;
  size_ = other.size_;
  data_ = other.data_;
  other.data_ = nullptr;
};
BigInt::~BigInt() {
  free(data_);
}
BigInt &BigInt::operator=(const BigInt &right) {
  if (this == &right) return *this;
  free(data_);
  data_ = new uint64_t[data_size_]{0};
  size_ = right.size_;
  sign_ = right.sign_;
  for (int64_t i = 0; i < size_; ++i) {
    data_[i] = right.data_[i];
  }
  return *this;
}
//_                  end constructors                    _//

//_                     start logic                      _//
bool operator<(const BigInt &left, const BigInt &right) {
  if (!left.sign_ && right.sign_) return false;
  if (left.sign_ && !right.sign_) return true;
  if (left.size_ < right.size_) return (!left.sign_ && !right.sign_);
  if (left.size_ > right.size_) return !(!left.sign_ && !right.sign_);
  for (int64_t i(left.size_ - 1); i >= 0; --i)
    if (left.data_[i] != right.data_[i]) {
      if (left.sign_ && right.sign_) return (left.data_[i] > right.data_[i]);
      else return (left.data_[i] < right.data_[i]);
    }
  return false;
}
bool operator==(const BigInt &left, const BigInt &right) {
  return !(left < right) && !(right < left);
}
bool operator!=(const BigInt &left, const BigInt &right) {
  return !(left == right);
}
bool operator<=(const BigInt &left, const BigInt &right) {
  return left == right || left < right;
}
bool operator>(const BigInt &left, const BigInt &right) {
  return right < left;
}
bool operator>=(const BigInt &left, const BigInt &right) {
  return right <= left;
}
template<typename T>
bool operator==(const BigInt &left, const T &right) {
  return left == BigInt(right);
}
template<typename T>
bool operator<(const BigInt &left, const T &right) {
  return left < BigInt(right);
}
template<typename T>
bool operator!=(const BigInt &left, const T &right) {
  return left != BigInt(right);
}
template<typename T>
bool operator<=(const BigInt &left, const T &right) {
  return left <= BigInt(right);
}
template<typename T>
bool operator>(const BigInt &left, const T &right) {
  return left > BigInt(right);
}
template<typename T>
bool operator>=(const BigInt &left, const T &right) {
  return left >= BigInt(right);
}
//_                       end logic                       _//

//_                      start unary                      _//
BigInt &BigInt::operator+() {
  return (*this);
}
BigInt &BigInt::operator-() {
  this->sign_ = !this->sign_;
  return (*this);
}
BigInt::operator bool() const {
  return (*this) != BigInt();
}
//_                       end unary                       _//

//_                      start binary                     _//
BigInt operator+(const BigInt &left, const BigInt &right) {

  if (left.sign_ && right.sign_) return (-(abs(left) + abs(right)));
  if (left.sign_ && !right.sign_) return (right - abs(left));
  if (!left.sign_ && right.sign_) return (left - abs(right));
  BigInt res;
  res.size_ = 0;
  if (left.size_ > right.size_) {
    res.size_ = left.size_ + 1;
  } else {
    res.size_ = right.size_ + 1;
  }
  for (int ix = 0; ix < res.size_ - 1; ix++) {
    res.data_[ix] += left.data_[ix] + right.data_[ix];
    res.data_[ix + 1] = res.data_[ix] / left.base_;
    res.data_[ix] %= left.base_;
  }
  if (res.data_[res.size_ - 1] == 0)
    --res.size_;

  return res;
}
BigInt operator-(const BigInt &left, const BigInt &right) {
  BigInt final;
  BigInt left_(left);

  if (left_.sign_ && right.sign_)
    return abs(right) - abs(left_);
  if (!left_.sign_ && right.sign_)
    return abs(left_) + abs(right);
  if (left_.sign_ && !right.sign_)
    return -(abs(left_) + abs(right));
  BigInt right_(right);
  if (left_ < right) std::swap(left_, right_);
  final.size_ = left_.size_;
  final.sign_ = (left < right);
  for (int64_t ix = 0; ix < final.size_; ix++) {
    if (ix < final.size_ - 1)
      final.data_[ix] += left_.base_ + (--left_.data_[ix + 1]);
    else
      final.data_[ix] += left_.data_[ix];
    final.data_[ix] -= right_.data_[ix];
    if (final.data_[ix] / left_.base_ == 1) {
      ++left_.data_[ix + 1];
      final.data_[ix] %= left_.base_;
    }
  }
  for (int64_t x(final.size_ - 1); x > 0; x--) {
    if (final.data_[x] == 0)
      final.size_--;
    break;
  }
  return final;
}
BigInt operator*(const BigInt &left, const BigInt &right) {
  BigInt final;
  final.sign_ = (left.sign_ != right.sign_);
  final.size_ = left.size_ + right.size_;
  for (int64_t ix = 0; ix < left.size_; ix++) {
    uint64_t surplus = 0;
    for (int64_t jx = 0; jx < right.size_ || surplus; jx++) {
      final.data_[ix + jx] += left.data_[ix] * right.data_[jx] + surplus;
      surplus = final.data_[ix + jx] / final.base_;
      final.data_[ix + jx] -= surplus * final.base_;
    }
  }
  for (int64_t fx = final.size_ - 1; fx > 0; fx--) {
    if (final.data_[fx] == 0)
      --final.size_;
    break;
  }
  return final;
}
BigInt operator/(const BigInt &left, const BigInt &right) {
  BigInt final;
  final.sign_ = (left.sign_ != right.sign_);
  final.size_ = left.size_;
  BigInt divisible;
  divisible.size_ = 0;
  for (int64_t ix = left.size_ - 1; ix >= 0; ix--) {
    int64_t x = 0;
    int64_t start = 0, end = left.base_;
    for (size_t i = divisible.size_; i >= 1; i--) {
      divisible.data_[i] = divisible.data_[i - 1];
      if (divisible.data_[left.size_]) {
        divisible.size_++;
      }
    }
    divisible.data_[0] = left.data_[ix];
    divisible.size_++;
    while (start <= end) {
      int64_t middle = (start + end) / 2;
      if (abs(right) * middle <= divisible) {
        x = middle;
        start = middle + 1;
      } else {
        end = middle - 1;
      }
    }
    final.data_[ix] = x;
    BigInt temp;
    temp = right * x;
    divisible = divisible - temp;
  }
  int64_t position = left.size_;
  while (--position > 0 && !final.data_[position]);
  final.size_ = position + 1;
  return final;
}
BigInt operator%(const BigInt &left, const BigInt &right) {
  return left - right * (left / right);
}
template<typename T>
BigInt operator+(const BigInt &left, const T &right) {
  return left + BigInt(right);
}
template<typename T>
BigInt operator-(const BigInt &left, const T &right) {
  return left - BigInt(right);
}
template<typename T>
BigInt operator*(const BigInt &left, const T &right) {
  return left * BigInt(right);
}
template<typename T>
BigInt operator/(const BigInt &left, const T &right) {
  return left / BigInt(right);
}
template<typename T>
BigInt operator%(const BigInt &left, const T &right) {
  return left % BigInt(right);
}
//_                       end binary                      _//

//_              start class/class operators              _//
BigInt &operator+=(BigInt &left, const BigInt &right) {
  left = left + right;
  return left;
}
BigInt &operator-=(BigInt &left, const BigInt &right) {
  left = left - right;
  return left;
}
BigInt &operator*=(BigInt &left, const BigInt &right) {
  left = left * right;
  return left;
}
BigInt &operator/=(BigInt &left, const BigInt &right) {
  left = left / right;
  return left;
}
BigInt &operator%=(BigInt &left, const BigInt &right) {
  left = left % right;
  return left;
}
template<typename T>
BigInt &operator+=(BigInt &left, const T &right) {
  left = left + BigInt(right);
  return left;
}
template<typename T>
BigInt &operator-=(BigInt &left, const T &right) {
  left = left - BigInt(right);
  return left;
}
template<typename T>
BigInt &operator*=(BigInt &left, const T &right) {
  left = left * BigInt(right);
  return left;
}
template<typename T>
BigInt &operator/=(BigInt &left, const T &right) {
  left = left / BigInt(right);
  return left;
}
template<typename T>
BigInt &operator%=(BigInt &left, const T &right) {
  left = left % BigInt(right);
  return left;
}
//_              end class/class operators                _//

//_                start inc/dec operators                _//
BigInt &operator++(BigInt &left) {
  left += 1;
  return left;
}
const BigInt operator++(BigInt &left, int) {
  left += 1;
  return left - 1;
}
BigInt &operator--(BigInt &left) {
  left -= 1;
  return left;
}
const BigInt operator--(BigInt &left, int) {
  left -= 1;
  return left + 1;
}
//_                  end inc/dec operators                _//

//_                  start CIO operators                  _//
std::ostream &operator<<(std::ostream &out, const BigInt &num) {
  if (num.sign_) {
    out << '-';
  }
  for (int64_t x = num.size_ - 1; x >= 0; x--) {
    if (x != num.size_ - 1)
      out << std::setw(9) << std::setfill('0') << num.data_[x];
    else
      out << num.data_[x];
  }
  return out;
}
std::istream &operator>>(std::istream &in, BigInt &num) {
  char str[100000];
  in >> str;
  num = BigInt(str);
  return in;
}
//_                   end CIO operators                   _//

//_                      start tests                      _//
namespace BigInt_tests {
void abs() {
  std::cout << "\nSTART TESTING ABS" << std::endl;
  assert(abs(BigInt(32)) == BigInt(32));
  assert(abs(BigInt(-32)) == BigInt(32));
  std::cout << "ABS OK" << std::endl;
}
void logic_operators() {
  std::cout << "\nSTART TESTING LOGIC OPERATORS" << std::endl;
  assert(BigInt(3) < BigInt(4));
  assert(BigInt(-4) < BigInt(-3));
  assert(BigInt(-3) < BigInt(4));
  assert(!(BigInt(3) < BigInt(-4)));
  assert(!(BigInt(3) < BigInt(3)));
  assert(BigInt(3) < 4);
  assert(BigInt(-4) < -3);
  assert(BigInt(-3) < 4);
  assert(!(BigInt(3) < -4));
  assert(!(BigInt(3) < 3));

  assert(BigInt(3) != BigInt(4));
  assert(BigInt(-4) != BigInt(-3));
  assert(BigInt(-3) != BigInt(4));
  assert(BigInt(3) != BigInt(-4));
  assert(!(BigInt(3) != BigInt(3)));
  assert(BigInt(3) != 4);
  assert(BigInt(-4) != -3);
  assert(BigInt(-3) != 4);
  assert(BigInt(3) != -4);
  assert(!(BigInt(3) != 3));

  assert(!(BigInt(3) == BigInt(4)));
  assert(!(BigInt(-4) == BigInt(-3)));
  assert(!(BigInt(-3) == BigInt(4)));
  assert(!(BigInt(3) == BigInt(-4)));
  assert((BigInt(3) == BigInt(3)));
  assert(!(BigInt(3) == 4));
  assert(!(BigInt(-4) == -3));
  assert(!(BigInt(-3) == 4));
  assert(!(BigInt(3) == -4));
  assert((BigInt(3) == 3));

  assert((BigInt(3) <= BigInt(4)));
  assert((BigInt(-4) <= BigInt(-3)));
  assert((BigInt(-3) <= BigInt(4)));
  assert(!(BigInt(3) <= BigInt(-4)));
  assert((BigInt(3) <= BigInt(3)));
  assert((BigInt(3) <= 4));
  assert((BigInt(-4) <= -3));
  assert((BigInt(-3) <= 4));
  assert(!(BigInt(3) <= -4));
  assert((BigInt(3) <= 3));

  assert(!(BigInt(3) > BigInt(4)));
  assert(!(BigInt(-4) > BigInt(-3)));
  assert(!(BigInt(-3) > BigInt(4)));
  assert((BigInt(3) > BigInt(-4)));
  assert(!(BigInt(3) > BigInt(3)));
  assert(!(BigInt(3) > 4));
  assert(!(BigInt(-4) > -3));
  assert(!(BigInt(-3) > 4));
  assert((BigInt(3) > -4));
  assert(!(BigInt(3) > 3));

  assert(!(BigInt(3) >= BigInt(4)));
  assert(!(BigInt(-4) >= BigInt(-3)));
  assert(!(BigInt(-3) >= BigInt(4)));
  assert((BigInt(3) >= BigInt(-4)));
  assert((BigInt(3) >= BigInt(3)));
  assert(!(BigInt(3) >= 4));
  assert(!(BigInt(-4) >= -3));
  assert(!(BigInt(-3) >= 4));
  assert((BigInt(3) >= -4));
  assert((BigInt(3) >= 3));
  std::cout << "LOGIC OPERATORS OK" << std::endl;
}
void binary_operators() {
  std::cout << "\nSTART TESTING BINARY OPERATORS" << std::endl;
  assert((BigInt(2) + BigInt(3)) == BigInt(5));
  assert((BigInt(-2) + BigInt(-3)) == BigInt(-5));
  assert((BigInt(2) + BigInt(-3)) == BigInt(-1));
  assert((BigInt(-2) + BigInt(3)) == BigInt(1));

  assert((BigInt(2) - BigInt(3)) == BigInt(-1));
  assert((BigInt(-2) - BigInt(-3)) == BigInt(1));
  assert((BigInt(2) - BigInt(-3)) == BigInt(5));
  assert((BigInt(-2) - BigInt(3)) == BigInt(-5));

  assert((BigInt(0) * BigInt(3)) == BigInt(0));
  assert((BigInt(2) * BigInt(3)) == BigInt(6));
  assert((BigInt(-2) * BigInt(-3)) == BigInt(6));
  assert((BigInt(2) * BigInt(-3)) == BigInt(-6));
  assert((BigInt(-2) * BigInt(3)) == BigInt(-6));

  assert((BigInt(0) * 3) == BigInt(0));
  assert((BigInt(2) * 3) == BigInt(6));
  assert((BigInt(-2) * -3) == BigInt(6));
  assert((BigInt(2) * -3) == BigInt(-6));
  assert((BigInt(-2) * 3) == BigInt(-6));

  assert((BigInt(0) / BigInt(3)) == BigInt(0));
  assert((BigInt(2) / BigInt(2)) == BigInt(1));
  assert((BigInt(10) / BigInt(5)) == BigInt(2));
  assert((BigInt(85000) / BigInt(1000)) == BigInt(85));
  assert((BigInt(14) / BigInt(-1)) == BigInt(-14));

  assert((BigInt(0) % BigInt(3)) == BigInt(0));
  assert((BigInt(2) % BigInt(1)) == BigInt(0));
  assert((BigInt(10) % BigInt(3)) == BigInt(1));
  assert((BigInt(5) % BigInt(1)) == BigInt(0));
  assert((BigInt(21) % BigInt(2)) == BigInt(1));
  std::cout << "BINARY OPERATORS OK" << std::endl;
}
void constructors() {
  std::cout << "\nSTART TESTING CONSTRUCTORS" << std::endl;
  assert(BigInt("") == BigInt());
  assert(BigInt("2") == BigInt(2));
  assert(BigInt("1234543210") == BigInt(1234543210));
  assert(BigInt("12345678900987654321") == BigInt(12345678900987654321ULL));
  std::cout << "CONSTRUCTORS OK" << std::endl;
}
void class_class_operators() {
  std::cout << "\nSTART TESTING CLASS/CLASS OPERATORS" << std::endl;
  auto a = BigInt(20);
  a += 10;
  assert(a == BigInt(30));
  a += BigInt(10);
  assert(a == BigInt(40));
  a = BigInt(20);
  a -= 10;
  assert(a == BigInt(10));
  a -= BigInt(10);
  assert(a == BigInt(0));
  a = BigInt(20);
  a /= 2;
  assert(a == BigInt(10));
  a /= BigInt(2);
  assert(a == BigInt(5));
  a = BigInt(20);
  a *= 2;
  assert(a == BigInt(40));
  a *= BigInt(2);
  assert(a == BigInt(80));
  a = BigInt(20);
  a %= 9;
  assert(a == BigInt(2));
  a = BigInt(20);
  a %= BigInt(10);
  assert(a == BigInt(0));
  std::cout << "CLASS/CLASS OPERATORS OK" << std::endl;
}
void CIO_operators() {
  std::cout << "\nSTART TESTING CIO OPERATORS" << std::endl;
  std::stringstream sin("12345678900987654321");
  BigInt a;
  sin >> a;
  assert(a == BigInt(12345678900987654321ULL));
  std::stringstream sout;
  sout << a;
  assert(sout.str() == "12345678900987654321");
  std::cout << "CIO OPERATORS OK\n" << std::endl;
}
} // end namespace BigInt_tests
void run_all_tests() {
  BigInt_tests::abs();
  BigInt_tests::logic_operators();
  BigInt_tests::constructors();
  BigInt_tests::binary_operators();
  BigInt_tests::class_class_operators();
  BigInt_tests::CIO_operators();
}
//_                       end tests                       _//
#endif //CORE_BIGINT_HPP_

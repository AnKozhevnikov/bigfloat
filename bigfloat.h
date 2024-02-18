#pragma once

#include "bigarithm.h"
#include "bigint.h"
#include <string>
#include <vector>
#include <memory>

const u_int64_t DEFAULT = 21;

namespace aak {
// class represents signed float value of any length
//   - supports arithmetical operations and comparisons
//   - implemented through vector of bytes
    class bigarithm;
    class bigfloat {
    public:
        // constructors
        bigfloat(); // default, value is assigned to 0
        bigfloat(const bigfloat &other); // from another bigfloat
        bigfloat(double value, u_int64_t precision = DEFAULT); // from a number
        bigfloat(const std::string &str, u_int64_t precision = DEFAULT); // from string presentation of a number
    private:
        bigfloat(const bigarithm &other, u_int64_t precision = DEFAULT); // from raw bigarithm data
    public:
        // destructor
        ~bigfloat();

    public:
        // value getting
        std::string as_string() const; // gets a number in string presentation
        // gets a number as specified float, if cannot convert, then exception
        float as_float() const;
        double as_double() const;
        // true if number fits the specified float presentation
        bool fits_float() const;
        bool fits_double() const;

        // value setting
        bigfloat &operator=(const bigfloat &a); // assignment
        void set(const bigfloat &other); // from another bigfloat
        void set(double value); // from a number
        void set(const std::string &str); // from string presentation of a number

        // operations
        bigfloat operator+() const; // +a
        bigfloat operator-() const; // -a

        bigfloat operator+(const bigfloat &b) const; // a + b
        bigfloat operator-(const bigfloat &b) const; // a - b
        bigfloat operator*(const bigfloat &b) const; // a * b
        bigfloat operator/(const bigfloat &b) const; // a / b

        bigfloat &operator+=(const bigfloat &b); // a += b;
        bigfloat &operator-=(const bigfloat &b); // a -= b;
        bigfloat &operator*=(const bigfloat &b); // a *= b;
        bigfloat &operator/=(const bigfloat &b); // a /= b;

        // comparisons
        bool operator==(const bigfloat &b) const; // a == b
        bool operator!=(const bigfloat &b) const; // a != b
        bool operator>(const bigfloat &b) const; // a > b
        bool operator<(const bigfloat &b) const; // a < b
        bool operator>=(const bigfloat &b) const; // a >= b
        bool operator<=(const bigfloat &b) const; // a <= b

        //state checks
        bool is_zero() const; // check for null
        bool is_nan() const; // check for nan
        bool is_infinity() const; // check for inf
        bool is_undef() const; // check for undef
        bool is_positive() const; // check for positive
        bool is_negative() const; // check for negative
        bool is_normal() const; // check if float
        bool is_abnormal() const; // check for abnormal

        //other
        static int8_t get_sign(const bigfloat &a); // get sign, -1 if negative, 0 if zero, 1 if positive
        static bigfloat abs(const bigfloat &a); // gets abs(a)
        static bigint cut(const bigfloat &bf); // cut fractional part
        std::string decimal(u_int64_t c); // decimal presentation with precision
    private:
        void _set_precision(u_int64_t p);
        static std::pair<bigfloat, bigfloat> _normalize(const bigfloat &a, const bigfloat &b);

    private:
        std::unique_ptr<bigarithm> _data;
        u_int64_t _precision;
    };
}

// streaming
std::ostream &operator<<(std::ostream &os, const aak::bigfloat &a);
std::istream &operator>>(std::istream &is, aak::bigfloat &a);

// string literal
aak::bigfloat operator "" _bf(const char *s);
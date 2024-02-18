#pragma once

#include "bigarithm.h"
#include <string>
#include <vector>
#include <memory>

namespace aak {
// class represents signed integer value of any length
//   - supports arithmetical operations and comparisons
//   - implemented through vector of bytes
    class bigarithm;

    class bigint {
    public:
        // constructors
        bigint(); // default, value is assigned to 0
        bigint(const bigint &other); // from another bigint
        bigint(long long value); // from a number
        bigint(const std::string &str); // from string presentation of a number
    private:
        bigint(const bigarithm &other); // from raw bigarithm data
    public:
        // destructor
        ~bigint();

    public:
        // value getting
        std::string as_string() const; // gets a number in string presentation
        // gets a number as specified integer, if overflows then throws exception
        u_int64_t as_uint64() const;
        u_int32_t as_uint32() const;
        int64_t as_int64() const;
        int as_int32() const;
        // true if number fits the specified integer presentation
        bool fits_uint64() const;
        bool fits_uint32() const;
        bool fits_int64() const;
        bool fits_int32() const;

        // value setting
        bigint &operator=(const bigint &a); // assignment
        void set(const bigint &other); // from another bigint
        void set(long long value); // from a number
        void set(const std::string &str); // from string presentation of a number

        // operations
        bigint operator+() const; // +a
        bigint operator-() const; // -a

        friend bigint operator+(bigint a, const bigint &b); // a + b
        friend bigint operator-(bigint a, const bigint &b); // a - b
        friend bigint operator*(bigint a, const bigint &b); // a * b
        friend bigint operator/(bigint a, const bigint &b); // a / b
        friend bigint operator%(bigint a, const bigint &b); // a % b

        bigint &operator+=(const bigint &b); // a += b;
        bigint &operator-=(const bigint &b); // a -= b;
        bigint &operator*=(const bigint &b); // a *= b;
        bigint &operator/=(const bigint &b); // a /= b;
        bigint &operator%=(const bigint &b); // a %= b;

        // comparisons
        bool operator==(const bigint &b) const; // a == b
        bool operator!=(const bigint &b) const; // a != b
        bool operator>(const bigint &b) const; // a > b
        bool operator<(const bigint &b) const; // a < b
        bool operator>=(const bigint &b) const; // a >= b
        bool operator<=(const bigint &b) const; // a <= b

        // binary shifts
        friend bigint operator<<(bigint a, const bigint &shift); // a << shift
        friend bigint operator>>(bigint b, const bigint &shift); // a >> shift
        bigint &operator<<=(const bigint &shift); // a <<= shift;
        bigint &operator>>=(const bigint &shift); // a >>= shift;

        //state checks
        bool is_zero() const; // check for null
        bool is_nan() const; // check for nan
        bool is_infinity() const; // check for inf
        bool is_undef() const; // check for undef
        bool is_positive() const; // check for positive
        bool is_negative() const; // check for negative
        bool is_normal() const; // check if integer
        bool is_abnormal() const; // check for abnormal

        //other
        static int8_t get_sign(const bigint &a); // get sign, -1 if negative, 0 if zero, 1 if positive
        static bigint abs(const bigint &a); // gets abs(a)

    private:
        std::unique_ptr<bigarithm> _data;

        friend class bigfloat;
    };
}

// streaming
std::ostream &operator<<(std::ostream &os, const aak::bigint &a);
std::istream &operator>>(std::istream &is, aak::bigint &a);

// string literal
aak::bigint operator "" _bi(const char *s);
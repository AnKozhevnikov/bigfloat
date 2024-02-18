#pragma once

#include <string>
#include <vector>

namespace aak {
    class bigarithm {
    public:
        enum State : u_int8_t {
            positive,
            negative,
            infinity,
            undefined,
            nan
        };

        bigarithm();

        bigarithm(const bigarithm &other);

        bigarithm(const std::vector<u_int32_t> other_bits, State other_state);

        ~bigarithm();

        bigarithm &operator=(const bigarithm &a); // assignment

        // operations
        bigarithm operator+() const; // +a
        bigarithm operator-() const; // -a

        friend bigarithm operator+(bigarithm a, const bigarithm &b); // a + b
        friend bigarithm operator-(bigarithm a, const bigarithm &b); // a - b
        friend bigarithm operator*(bigarithm a, const bigarithm &b); // a * b
        friend bigarithm operator/(bigarithm a, const bigarithm &b); // a / b
        friend bigarithm operator%(bigarithm a, const bigarithm &b); // a % b

        bigarithm &operator+=(const bigarithm &b); // a += b;
        bigarithm &operator-=(const bigarithm &b); // a -= b;
        bigarithm &operator*=(const bigarithm &b); // a *= b;
        bigarithm &operator/=(const bigarithm &b); // a /= b;
        bigarithm &operator%=(const bigarithm &b); // a %= b;

        // comparisons
        bool operator==(const bigarithm &b) const; // a == b
        bool operator!=(const bigarithm &b) const; // a != b
        bool operator>(const bigarithm &b) const; // a > b
        bool operator<(const bigarithm &b) const; // a < b
        bool operator>=(const bigarithm &b) const; // a >= b
        bool operator<=(const bigarithm &b) const; // a <= b

        // binary shifts
        bigarithm operator<<(const long long shift) const; // a << b
        bigarithm operator>>(const long long shift) const; // a >> b
        bigarithm &operator<<=(const long long shift); // a <<= b;
        bigarithm &operator>>=(const long long shift); // a >>= b;

        //state checks
        bool is_zero() const; // check for null
        bool is_nan() const; // check for nan
        bool is_infinity() const; // check for inf
        bool is_undef() const; // check for undef
        bool is_positive() const; // check for positive
        bool is_negative() const; // check for negative
        bool is_normal() const; // check if integer
        bool is_abnormal() const; // check for abnormal(all of above except null)

        //other
        static int8_t get_sign(const bigarithm &a); // get sign, -1 if negative, 0 if zero, 1 if positive
        static bigarithm abs(const bigarithm &a); // gets abs(a)

        // get uncommon
        static bigarithm get_nan(); // gets nan in bigarithm representation
        static bigarithm get_infinity(); // gets oo in bigarithm representation
        static bigarithm get_undef(); // gets undefined in bigarithm representation
        static bigarithm get_zero(); // gets null in bigarithm representation

        //other
        void add32(); // add one uinteger at the back
        void delete_leading(); // delete all leading zeros

        std::vector<u_int32_t> get_bits() const; // get raw bits
        u_int32_t get_bits(u_int64_t pos) const; // get bits from precise index
        u_int64_t get_sz() const; // get size of _bits
        u_int8_t get_state() const; // get state

        void set(const bigarithm &other); // set as other bigarithm
        void set_bits(const std::vector<u_int32_t> &other); // set full bits
        void set_bits(u_int64_t pos, u_int32_t value); // set one bit
        void set_state(State other); // set state

    private:
        std::vector<u_int32_t> _bits; // raw bits
        State _state; // state
    };
}
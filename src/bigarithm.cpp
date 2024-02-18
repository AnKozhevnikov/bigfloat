#include "bigarithm.h"

#include <algorithm>

using namespace aak;

const int64_t POW32 = static_cast<int64_t>(1) << 32;
const bigarithm ZERO({0}, bigarithm::State::positive);

namespace aak {
    bigarithm::bigarithm() {
        _bits = {0};
        _state = positive;
    }

    bigarithm::bigarithm(const bigarithm &other) {
        _bits = other._bits;
        _state = other._state;
    }

    bigarithm::bigarithm(const std::vector<u_int32_t> other_bits, State other_state) {
        _bits = other_bits;
        _state = other_state;
    }

    bigarithm::~bigarithm() = default;

    bigarithm &bigarithm::operator=(const bigarithm &a) {
        _bits = a._bits;
        _state = a._state;
        return *this;
    }

    bigarithm bigarithm::operator+() const {
        return *this;
    }

    bigarithm bigarithm::operator-() const {
        if (is_nan()) return get_nan();
        if (is_undef()) return get_undef();
        if (is_infinity()) return get_infinity();
        if (is_zero()) return get_zero();

        bigarithm ret = *this;
        if (ret._state == positive) ret._state = negative;
        else ret._state = positive;

        return ret;
    }

    bigarithm operator+(bigarithm a, const bigarithm &b) {
        a+=b;
        return a;
    }

    bigarithm operator-(bigarithm a, const bigarithm &b) {
        a-=b;
        return a;
    }

    bigarithm operator*(bigarithm a, const bigarithm &b) {
        a*=b;
        return a;
    }

    bigarithm operator/(bigarithm a, const bigarithm &b) {
        a/=b;
        return a;
    }

    bigarithm operator%(bigarithm a, const bigarithm &b) {
        a%=b;
        return a;
    }

    bigarithm &bigarithm::operator+=(const bigarithm &b) {
        if (is_nan() || b.is_nan()) {*this=get_nan(); return *this;}
        if (is_undef() || b.is_undef()) {*this=get_undef(); return *this;}
        if (is_infinity() && b.is_infinity()) {*this=get_undef(); return *this;}
        if (is_infinity() || b.is_infinity()) {*this=get_infinity(); return *this;}

        if (_state == b._state) {
            bigarithm ret(*this);
            u_int64_t carry = 0;
            for (int i = 0; i < b._bits.size(); i++) {
                if (ret._bits.size() == i) ret.add32();
                u_int64_t k = static_cast<u_int64_t>(ret._bits[i]) + static_cast<u_int64_t>(b._bits[i]) + carry;
                ret._bits[i] = k % POW32;
                carry = k / POW32;
            }
            int pos = b._bits.size();
            while (carry != 0) {
                if (ret._bits.size() == pos) ret.add32();
                u_int64_t k = static_cast<u_int64_t>(ret._bits[pos]) + carry;
                ret._bits[pos] = k % POW32;
                carry = k / POW32;
                ++pos;
            }
            if (ret.is_zero()) ret._state = positive;
            ret.delete_leading();
            *this=ret;
        } else {
            bigarithm big = abs(*this);
            bigarithm small = abs(b);
            if (big < small)
                std::swap(big, small);

            for (int i = 0; i < big._bits.size(); i++) {
                if (small._bits.size() <= i) break;
                if (big._bits[i] >= small._bits[i]) {
                    big._bits[i] -= small._bits[i];
                } else {
                    int pos = i + 1;
                    while (big._bits[pos] == 0) ++pos;
                    --big._bits[pos];
                    --pos;
                    while (pos != i) {
                        big._bits[pos] = POW32 - 1;
                        --pos;
                    }

                    big._bits[i] = static_cast<u_int64_t>(big._bits[i]) + POW32 - small._bits[i];
                }
            }

            if ((get_sign(*this) == -1 && abs(*this) > b) || (get_sign(b) == -1 && abs(b) > *this)) {
                big._state = negative;
            }

            if (big.is_zero()) big._state = positive;
            big.delete_leading();
            *this=big;
        }
        return *this;
    }

    bigarithm &bigarithm::operator-=(const bigarithm &b) {
        return *this += -b;
    }

    std::vector<u_int32_t> karatsuba(std::vector<u_int32_t> a, std::vector<u_int32_t> b) {
        while (a.size() < b.size()) a.emplace_back(0);
        while (a.size() > b.size()) b.emplace_back(0);

        if (a.size() == 1) {
            std::vector<u_int32_t> ret = {
                    static_cast<u_int32_t>((static_cast<u_int64_t>(a[0]) * static_cast<u_int64_t>(b[0])) % POW32),
                    static_cast<u_int32_t>((static_cast<u_int64_t>(a[0]) * static_cast<u_int64_t>(b[0])) / POW32)};
            if (ret.back() == 0) ret.pop_back();
            return ret;
        }

        if (a.size() % 2 != 0) {
            a.emplace_back(0);
            b.emplace_back(0);
        }

        int sz = a.size();
        std::vector<u_int32_t> a1(sz / 2), a2(sz / 2), b1(sz / 2), b2(sz / 2);
        for (int i = 0; i < sz / 2; i++) {
            a1[i] = a[i];
            a2[i] = a[i + sz / 2];
            b1[i] = b[i];
            b2[i] = b[i + sz / 2];
        }

        std::vector<u_int32_t> p1 = karatsuba(a1, b1);
        std::vector<u_int32_t> p2 = karatsuba(a2, b2);

        std::vector<u_int32_t> a12(sz / 2, 0), b12(sz / 2, 0);
        u_int64_t carry_a = 0;
        u_int64_t carry_b = 0;
        for (int i = 0; i < sz / 2; i++) {
            a12[i] = (static_cast<u_int64_t>(a1[i]) + static_cast<u_int64_t>(a2[i]) + carry_a) % POW32;
            b12[i] = (static_cast<u_int64_t>(b1[i]) + static_cast<u_int64_t>(b2[i]) + carry_b) % POW32;
            carry_a = (static_cast<u_int64_t>(a1[i]) + static_cast<u_int64_t>(a2[i]) + carry_a) / POW32;
            carry_b = (static_cast<u_int64_t>(b1[i]) + static_cast<u_int64_t>(b2[i]) + carry_b) / POW32;
        }
        if (carry_a != 0) a12.emplace_back(carry_a);
        if (carry_b != 0) b12.emplace_back(carry_b);

        std::vector<u_int32_t> t = karatsuba(a12, b12);

        std::vector<u_int32_t> ret(sz * 2, 0);
        u_int64_t carry = 0;
        for (int i = 0; i < ret.size(); i++) {
            u_int64_t k = carry;
            if (i >= 0 && i < p1.size()) k += p1[i];
            if (i - sz >= 0 && i - sz < p2.size()) k += p2[i - sz];
            if (i - sz / 2 >= 0 && i - sz / 2 < t.size()) k += t[i - sz / 2];
            ret[i] = k % POW32;
            carry = k / POW32;
        }

        while (p1.size() < p2.size()) p1.emplace_back(0);
        while (p1.size() > p2.size()) p2.emplace_back(0);
        std::vector<u_int32_t> psum(p1.size() + sz / 2, 0);
        carry = 0;
        for (int i = 0; i < p1.size(); i++) {
            psum[i + sz / 2] = (carry + static_cast<u_int64_t>(p1[i]) + static_cast<u_int64_t>(p2[i])) % POW32;
            carry = (carry + static_cast<u_int64_t>(p1[i]) + static_cast<u_int64_t>(p2[i])) / POW32;
        }
        if (carry != 0) {
            psum.emplace_back(carry);
        }

        for (int i = 0; i < ret.size(); i++) {
            if (i == psum.size()) break;
            if (ret[i] >= psum[i]) ret[i] -= psum[i];
            else {
                int pos = i + 1;
                while (ret[pos] == 0) ++pos;
                --ret[pos];
                --pos;
                while (pos != i) {
                    ret[pos] = POW32 - 1;
                    --pos;
                }
                ret[i] = (POW32 - psum[i]) + ret[i];
            }
        }

        while (ret.size() > 1 && ret.back() == 0) ret.pop_back();

        return ret;
    }

    bigarithm &bigarithm::operator*=(const bigarithm &b) {
        if (is_nan() || b.is_nan()) {*this=get_nan(); return *this;}
        if (is_undef() || b.is_undef()) {*this=get_undef(); return *this;}
        if (is_infinity() || b.is_infinity()) {
            if (is_zero() || b.is_zero()) {*this=get_undef(); return *this;}
            *this=get_infinity(); return *this;
        }

        bigarithm c;
        if (is_positive()) c._state = b._state;
        else {
            if (b.is_positive()) c._state = negative;
            else c._state = positive;
        }
        c._bits = karatsuba(_bits, b._bits);

        if (is_zero()) c._state = positive;
        c.delete_leading();
        *this=c;
        return *this;
    }

    bigarithm &bigarithm::operator/=(const bigarithm &b) {
        if (is_nan() || b.is_nan()) {*this=get_nan(); return *this;}
        if (is_undef() || b.is_undef()) {*this=get_undef(); return *this;}
        if (is_zero() && b.is_zero()) {*this=get_undef(); return *this;}
        if (is_infinity() && b.is_infinity()) {*this=get_undef(); return *this;}
        if (b.is_infinity()) {*this=get_zero(); return *this;}
        if (is_infinity()) return *this;
        if (is_zero()) return *this;
        if (b.is_zero()) {*this=get_infinity(); return *this;}

        if (abs(*this) < abs(b)) {*this=get_zero(); return *this;}

        int pos = 0;
        bigarithm c = abs(b);
        while (c <= abs(*this)) {
            ++pos;
            c <<= 1;
        }
        --pos;
        c >>= 1;
        bigarithm ret = get_zero();
        ret._bits.resize(pos / 32 + 1);

        bigarithm cur;
        bigarithm mult = ZERO;
        while (pos >= 0) {
            if (mult + c <= abs(*this)) {
                mult += c;
                ret._bits[pos / 32] += static_cast<u_int32_t>(1) << (pos % 32);
            }
            c >>= 1;
            --pos;
        }

        if ((_state == negative && b._state == positive) || (_state == positive && b._state == negative))
            ret._state = negative;
        ret.delete_leading();
        *this=ret;
        return *this;
    }

    bigarithm &bigarithm::operator%=(const bigarithm &b) {
        if (is_nan() || b.is_nan()) {*this=get_nan(); return *this;}
        if ((*this).is_normal() && b.is_infinity()) return *this;
        if (is_abnormal() || b.is_abnormal() || b.is_zero()) {*this=get_undef(); return *this;}
        bigarithm ret = abs(*this) - (abs(*this) / abs(b) * abs(b));
        if (_state == negative) ret._state = negative;
        ret.delete_leading();
        *this=ret;
        return *this;
    }

    bool bigarithm::operator==(const bigarithm &b) const {
        if (is_abnormal() || b.is_abnormal()) throw;
        return (_bits == b._bits) && (_state == b._state);
    }

    bool bigarithm::operator!=(const bigarithm &b) const {
        if (is_abnormal() || b.is_abnormal()) throw;
        return (_bits != b._bits) || (_state != b._state);
    }

    bool bigarithm::operator>(const bigarithm &b) const {
        if (is_abnormal() || b.is_abnormal()) throw;

        if (_state == b._state && _bits == b._bits) return false;

        if (get_sign(*this) > get_sign(b)) return true;
        if (get_sign(*this) < get_sign(b)) return false;

        if (_bits.size() > b._bits.size()) return get_sign(*this) == 1;
        else if (_bits.size() < b._bits.size()) return get_sign(*this) == -1;

        for (int i = _bits.size() - 1; i >= 0; i--) {
            if (_bits[i] != b._bits[i]) {
                return (_bits[i] > b._bits[i] && get_sign(*this) == 1) ||
                       (_bits[i] < b._bits[i] && get_sign(*this) == -1);
            }
        }
        return false;
    }

    bool bigarithm::operator<(const bigarithm &b) const {
        return !(*this >= b);
    }

    bool bigarithm::operator>=(const bigarithm &b) const {
        return (*this > b) || (*this == b);
    }

    bool bigarithm::operator<=(const bigarithm &b) const {
        return !(*this > b);
    }

    bigarithm bigarithm::operator<<(const long long shift) const {
        if (is_nan()) return get_nan();
        if (is_undef()) return get_undef();
        if (is_zero()) return get_zero();
        if (is_infinity()) return get_infinity();

        int r = shift % 32;

        bigarithm ret;
        ret._bits = {};
        ret._state = (*this)._state;
        for (long long i = r; i < shift; i += 32) {
            ret.add32();
        }

        int base_size = ret._bits.size();

        for (int i = 0; i < (*this)._bits.size(); i++) {
            while (ret._bits.size() <= base_size + i + 1) ret.add32();
            ret._bits[base_size + i] += ((*this)._bits[i] % ((static_cast<u_int64_t>(1)) << (32 - r))) << r;
            ret._bits[base_size + i + 1] += (*this)._bits[i] / ((static_cast<u_int64_t>(1) << (32 - r)));
        }
        ret.delete_leading();
        return ret;
    }

    bigarithm bigarithm::operator>>(const long long shift) const {
        if (is_nan()) return get_nan();
        if (is_undef()) return get_undef();
        if (is_infinity()) return (*this);

        int r = shift % 32;
        bigarithm ret;
        ret._state = (*this)._state;

        u_int64_t k = shift - r;

        if (k >= (*this)._bits.size()) return get_zero();

        ret._bits.resize(1, 0);
        ret._bits[0] += (*this)._bits[k] / ((static_cast<u_int64_t>(1)) << r);
        for (int i = k + 1; i < (*this)._bits.size(); i++) {
            while (ret._bits.size() <= i - k) ret.add32();
            ret._bits[i - k - 1] += ((*this)._bits[i] % ((static_cast<u_int64_t>(1)) << r)) << (32 - r);
            ret._bits[i - k] += (*this)._bits[i] / ((static_cast<u_int64_t>(1)) << r);
        }
        ret.delete_leading();
        return ret;
    }

    bigarithm &bigarithm::operator<<=(const long long shift) {
        *this = (*this) << shift;
        return *this;
    }

    bigarithm &bigarithm::operator>>=(const long long shift) {
        *this = (*this) >> shift;
        return *this;
    }

    bool bigarithm::is_zero() const {
        if (is_nan() || is_undef() || is_infinity()) return false;
        for (int i = 0; i < _bits.size(); i++) {
            if (_bits[i] != 0) return false;
        }
        return true;
    }

    bool bigarithm::is_nan() const {
        return _state == nan;
    }

    bool bigarithm::is_infinity() const {
        return _state == infinity;
    }

    bool bigarithm::is_undef() const {
        return _state == undefined;
    }

    bool bigarithm::is_positive() const {
        return _state == positive;
    }

    bool bigarithm::is_negative() const {
        return _state == negative;
    }

    bool bigarithm::is_normal() const {
        return !is_abnormal();
    }

    bool bigarithm::is_abnormal() const {
        return (is_infinity() || is_undef() || is_nan());
    }

    int8_t bigarithm::get_sign(const bigarithm &a) {
        if (a.is_abnormal()) throw;
        if (a.is_zero()) return 0;
        else if (a.is_positive()) return 1;
        else return -1;
    }

    bigarithm bigarithm::abs(const bigarithm &a) {
        if (a.is_abnormal()) throw;

        bigarithm ret = a;
        ret._state = positive;
        return ret;
    }

    bigarithm bigarithm::get_nan() {
        bigarithm ret;
        ret._bits = {0};
        ret._state = nan;
        return ret;
    }

    bigarithm bigarithm::get_infinity() {
        bigarithm ret;
        ret._bits = {0};
        ret._state = infinity;
        return ret;
    }

    bigarithm bigarithm::get_undef() {
        bigarithm ret;
        ret._bits = {0};
        ret._state = undefined;
        return ret;
    }

    bigarithm bigarithm::get_zero() {
        bigarithm ret;
        ret._bits = {0};
        ret._state = positive;
        return ret;
    }

    void bigarithm::add32() {
        _bits.emplace_back(0);
    }

    void bigarithm::delete_leading() {
        while (_bits.size() > 1 && _bits.back() == 0) _bits.pop_back();
    }

    std::vector<u_int32_t> bigarithm::get_bits() const {
        return _bits;
    }

    u_int32_t bigarithm::get_bits(u_int64_t pos) const {
        return _bits[pos];
    }

    u_int64_t bigarithm::get_sz() const {
        return _bits.size();
    }

    u_int8_t bigarithm::get_state() const {
        return _state;
    }

    void bigarithm::set(const bigarithm &other) {
        _bits = other._bits;
        _state = other._state;
    }

    void bigarithm::set_bits(const std::vector<u_int32_t> &other) {
        _bits = other;
    }

    void bigarithm::set_bits(u_int64_t pos, u_int32_t value) {
        _bits[pos] = value;
    }

    void bigarithm::set_state(State other) {
        _state = other;
    }
}
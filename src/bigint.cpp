#include "bigarithm.h"
#include "bigint.h"

#include <string>
#include <algorithm>
#include <climits>

using namespace aak;

const int64_t POW32 = static_cast<int64_t>(1) << 32;

namespace aak {
    bigint::bigint() {
        _data = std::make_unique<bigarithm>();
    }

    bigint::bigint(const bigint &other) {
        _data = std::make_unique<bigarithm>();
        set(other);
    }

    bigint::bigint(long long value) {
        _data = std::make_unique<bigarithm>();
        set(value);
    }

    bigint::bigint(const std::string &str) {
        _data = std::make_unique<bigarithm>();
        set(str);
    }

    bigint::bigint(const bigarithm &other) {
        _data = std::make_unique<bigarithm>(other);
    }

    bigint::~bigint() = default;

    std::string bigint::as_string() const {
        if (is_nan()) return "NaN";
        if (is_infinity()) return "oo";
        if (is_undef()) return "undefined";

        std::string ret = "0";
        for (int i = _data->get_sz() - 1; i >= 0; i--) {
            for (int j = 31; j >= 0; j--) {
                int carry = 0;
                for (int k = 0; k < ret.size(); k++) {
                    int d = ret[k] - '0';
                    d *= 2;
                    d += carry;
                    ret[k] = '0' + (d % 10);
                    carry = d / 10;
                }
                if (carry != 0) {
                    ret.push_back('0' + carry);
                }

                if ((_data->get_bits(i) & (static_cast<u_int32_t>(1) << j)) != 0) {
                    ++ret[0];
                }

                int pos = 0;
                while (ret[pos] > '9') {
                    ret[pos] = 0;
                    if (ret.size() == pos) ret.push_back('0');
                    ++ret[pos + 1];
                    ++pos;
                }
            }
        }

        std::reverse(ret.begin(), ret.end());
        if (!is_zero() && is_negative()) {
            ret = "-" + ret;
        }

        return ret;
    }

    u_int64_t bigint::as_uint64() const {
        if (!fits_uint64()) throw;
        if (_data->get_sz() == 1) return _data->get_bits(0);
        return (static_cast<u_int64_t>(_data->get_bits(1)) << 32) + _data->get_bits(0);
    }

    u_int32_t bigint::as_uint32() const {
        if (!fits_uint32()) throw;
        return _data->get_bits(0);
    }

    int64_t bigint::as_int64() const {
        if (!fits_int64()) throw;
        if (_data->get_sz() == 1) return get_sign(*this) * static_cast<int64_t>(_data->get_bits(0));
        return get_sign(*this) * ((static_cast<int64_t>(_data->get_bits(1)) << 32) + _data->get_bits(0));
    }

    int bigint::as_int32() const {
        if (!fits_int32()) throw;
        return get_sign(*this) * static_cast<int>(_data->get_bits(0));
    }

    bool bigint::fits_uint64() const {
        if (is_abnormal()) return false;
        for (int i = 2; i < _data->get_sz(); i++) {
            if (_data->get_bits(i) != 0) return false;
        }
        return true;
    }

    bool bigint::fits_uint32() const {
        if (is_abnormal()) return false;
        for (int i = 1; i < _data->get_sz(); i++) {
            if (_data->get_bits(i) != 0) return false;
        }
        return true;
    }

    bool bigint::fits_int64() const {
        if (is_abnormal()) return false;
        if (_data->get_bits(1) > INT_MAX) return false;
        for (int i = 2; i < _data->get_sz(); i++) {
            if (_data->get_bits(i) != 0) return false;
        }
        return true;
    }

    bool bigint::fits_int32() const {
        if (is_abnormal()) return false;
        if (_data->get_bits(0) > INT_MAX) return false;
        for (int i = 1; i < _data->get_sz(); i++) {
            if (_data->get_bits(i) != 0) return false;
        }
        return true;
    }

    bigint &bigint::operator=(const bigint &a) {
        _data = std::make_unique<bigarithm>(*a._data);
        return *this;
    }

    void bigint::set(const bigint &other) {
        _data = std::make_unique<bigarithm>(*other._data);
    }

    void bigint::set(long long value) {
        if (std::abs(value) < POW32) {
            _data->set_bits({static_cast<u_int32_t>(std::abs(value))});
        } else {
            _data->set_bits({static_cast<u_int32_t>(std::abs(value) % POW32),
                             static_cast<u_int32_t>(std::abs(value) / POW32)});
        }
        if (value >= 0) _data->set_state(bigarithm::positive);
        else _data->set_state(bigarithm::negative);
    }

    void bigint::set(const std::string &str) {
        _data->set_bits(std::vector<u_int32_t>());
        _data->set_state(bigarithm::positive);

        if (str[0] == '-') {
            _data->set_state(bigarithm::negative);
        } else if ((str[0] > '9' || str[0] < '0') && str[0] != '+') {
            _data->set(bigarithm::get_nan());
            return;
        }

        int start = 0;
        if (str[0] == '-' || str[0] == '+') {
            start = 1;
            if (str.size() == 1) {
                _data->set(bigarithm::get_nan());
                return;
            }
        }

        for (int i = start; i < str.size(); i++) {
            if (str[i] > '9' || str[i] < '0') {
                _data->set(bigarithm::get_nan());
                return;
            }
        }

        std::string str_cpy = str.substr(start, str.size() - start);

        int pos = 0;
        int first_not_null = 0;
        bool finished = false;
        while (true) {
            if (pos % 32 == 0) {
                _data->add32();
            }

            _data->set_bits(pos / 32, _data->get_bits(pos / 32) + (((str_cpy.back() - '0') % 2) << (pos % 32)));

            bool flag = false;
            int add = 0;
            for (int i = first_not_null; i < str_cpy.size(); i++) {
                if (str_cpy[i] == '0' && !flag) {
                    ++first_not_null;
                    if (first_not_null == str_cpy.size()) {
                        finished = true;
                        break;
                    }
                } else flag = true;
                int k = static_cast<int>(str_cpy[i] - '0');
                str_cpy[i] = static_cast<char>('0' + (add + k) / 2);
                if ((add + k) % 2 == 1) add = 10;
                else add = 0;
            }

            if (finished) break;
            ++pos;
        }

        _data->delete_leading();
    }

    bigint bigint::operator+() const {
        return +(*_data);
    }

    bigint bigint::operator-() const {
        return -(*_data);
    }

    bigint operator+(bigint a, const bigint &b) {
        a+=b;
        return a;
    }

    bigint operator-(bigint a, const bigint &b) {
        a-=b;
        return a;
    }

    bigint operator*(bigint a, const bigint &b) {
        a*=b;
        return a;
    }

    bigint operator/(bigint a, const bigint &b) {
        a/=b;
        return a;
    }

    bigint operator%(bigint a, const bigint &b) {
        a%=b;
        return a;
    }

    bigint &bigint::operator+=(const bigint &b) {
        bigint ret= *(this->_data) + *(b._data);
        *this=ret;
        return *this;
    }

    bigint &bigint::operator-=(const bigint &b) {
        bigint ret= *(this->_data) - *(b._data);
        *this=ret;
        return *this;
    }

    bigint &bigint::operator*=(const bigint &b) {
        bigint ret= *(this->_data) * *(b._data);
        *this=ret;
        return *this;
    }

    bigint &bigint::operator/=(const bigint &b) {
        bigint ret= *(this->_data) / *(b._data);
        *this=ret;
        return *this;
    }

    bigint &bigint::operator%=(const bigint &b) {
        bigint ret= *(this->_data) % *(b._data);
        *this=ret;
        return *this;
    }

    bool bigint::operator==(const bigint &b) const {
        return (*_data) == (*b._data);
    }

    bool bigint::operator!=(const bigint &b) const {
        return (*_data) != (*b._data);
    }

    bool bigint::operator>(const bigint &b) const {
        return (*_data) > (*b._data);
    }

    bool bigint::operator<(const bigint &b) const {
        return !(*this >= b);
    }

    bool bigint::operator>=(const bigint &b) const {
        return (*this > b) || (*this == b);
    }

    bool bigint::operator<=(const bigint &b) const {
        return !(*this > b);
    }

    bigint operator<<(bigint a, const bigint &shift) {
        a<<=shift;
        return a;
    }

    bigint operator>>(bigint a, const bigint &shift) {
        a>>=shift;
        return a;
    }

    bigint &bigint::operator<<=(const bigint &shift) {
        if (is_nan() || shift.is_nan()) {*this = bigarithm::get_nan(); return *this;}
        if (is_undef() || shift.is_undef()) {*this = bigarithm::get_undef(); return *this;}
        if (shift.is_negative() || shift.is_infinity()) {*this = bigarithm::get_undef(); return *this;}
        if (is_zero()) *this;
        if (is_infinity()) *this;

        bigint ret = *this;
        bigint cnt = 0;
        while (cnt != shift) {
            *ret._data <<= std::min(POW32, (shift - cnt).as_int64());
            cnt += std::min(POW32, (shift - cnt).as_int64());
        }
        *this=ret;
        return *this;
    }

    bigint &bigint::operator>>=(const bigint &shift) {
        if (is_nan() || shift.is_nan()) {*this = bigarithm::get_nan(); return *this;}
        if (is_undef() || shift.is_undef()) {*this = bigarithm::get_undef(); return *this;}
        if (shift.is_negative() || shift.is_infinity()) {*this = bigarithm::get_undef(); return *this;}
        if (is_infinity()) return *this;
        if (shift.is_infinity()) {*this = bigarithm::get_zero(); return *this;}

        bigint ret = *this;
        bigint cnt = 0;
        while (cnt != shift) {
            *ret._data >>= std::min(POW32, (shift - cnt).as_int64());
            cnt += std::min(POW32, (shift - cnt).as_int64());
        }

        *this=ret;
        return *this;
    }

    bool bigint::is_zero() const {
        return _data->is_zero();
    }

    bool bigint::is_nan() const {
        return _data->is_nan();
    }

    bool bigint::is_infinity() const {
        return _data->is_infinity();
    }

    bool bigint::is_undef() const {
        return _data->is_undef();
    }

    bool bigint::is_positive() const {
        return _data->is_positive();
    }

    bool bigint::is_negative() const {
        return _data->is_negative();
    }

    bool bigint::is_normal() const {
        return !is_abnormal();
    }

    bool bigint::is_abnormal() const {
        return (is_infinity() || is_undef() || is_nan());
    }

    int8_t bigint::get_sign(const bigint &a) {
        return bigarithm::get_sign(*a._data);
    }

    bigint bigint::abs(const bigint &a) {
        return bigarithm::abs(*a._data);
    }
}

std::ostream &operator<<(std::ostream &os, const bigint &a) {
    os << a.as_string();
    return os;
}

std::istream &operator>>(std::istream &is, bigint &a) {
    std::string num;
    is >> num;
    a = bigint(num);
    return is;
}

bigint operator "" _bi(const char *s) {
    return std::string(s);
}
#include "bigarithm.h"
#include "bigfloat.h"

#include <string>
#include <algorithm>

using namespace aak;

const int64_t POW32 = static_cast<int64_t>(1) << 32;

namespace aak {
    bigfloat::bigfloat() {
        _data = std::make_unique<bigarithm>();
        _precision = DEFAULT;
    }

    bigfloat::bigfloat(const bigfloat &other) {
        _data = std::make_unique<bigarithm>();
        set(other);
    }

    bigfloat::bigfloat(double value, u_int64_t precision) {
        _data = std::make_unique<bigarithm>();
        _precision=precision;
        bigfloat::set(value);
    }

    bigfloat::bigfloat(const std::string &str, u_int64_t precision) {
        _data = std::make_unique<bigarithm>();
        _precision=precision;
        bigfloat::set(str);
    }

    bigfloat::bigfloat(const bigarithm &other, u_int64_t precision) {
        _data = std::make_unique<bigarithm>(other);
        _precision=precision;
    }

    bigfloat::~bigfloat() {
        //nothing to destroy
    }

    std::string bigfloat::as_string() const {
        if (is_nan()) return "NaN";
        if (is_infinity()) return "oo";
        if (is_undef()) return "undefined";
        if (is_zero()) return "0";

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

        for (int i=0; i<_precision*32; ++i) {
            ret.push_back('0');
        }

        int first_not_null = 0;
        for (int t=0; t<_precision*32; ++t) {
            bool flag = false;
            int add = 0;
            for (int i = first_not_null; i < ret.size(); i++) {
                if (ret[i] == '0' && !flag) {
                    ++first_not_null;
                } else flag = true;
                int k = static_cast<int>(ret[i] - '0');
                ret[i] = static_cast<char>('0' + (add + k) / 2);
                if ((add + k) % 2 == 1) add = 10;
                else add = 0;
            }
        }

        for (int i = first_not_null; i < ret.size(); i++) {
            if (ret[i] == '0') {
                ++first_not_null;
            } else break;
        }

        ret=ret.substr(first_not_null, ret.size() - first_not_null);

        std::string add_zeros;
        while (add_zeros.size()+ret.size()<=_precision*32) add_zeros.push_back('0');
        ret=add_zeros+ret;
        ret.insert(ret.size()-_precision*32, ".");

        while (ret.back()=='0') ret.pop_back();
        if (ret.back()=='.') ret.pop_back();

        if (!is_zero() && is_negative()) {
            ret = "-" + ret;
        }

        return ret;
    }

    float bigfloat::as_float() const {
        return std::stof(as_string());
    }

    double bigfloat::as_double() const {
        return std::stod(as_string());
    }

    bool bigfloat::fits_float() const {
        if (is_abnormal()) return false;
        return true;
    }

    bool bigfloat::fits_double() const {
        if (is_abnormal()) return false;
        return true;
    }

    bigfloat &bigfloat::operator=(const bigfloat &a) {
        _data = std::make_unique<bigarithm>(*a._data);
        _precision=a._precision;
        return *this;
    }

    void bigfloat::set(const bigfloat &other) {
        _data = std::make_unique<bigarithm>(*other._data);
        _precision = other._precision;
    }

    void bigfloat::set(double value) {
        set(std::to_string(value));
    }

    void bigfloat::set(const std::string &str) {
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

        std::string str_cpy = str.substr(start, str.size() - start);

        std::vector<std::string> v(1);
        for (int i=0; i<str_cpy.size(); i++) {
            if (str_cpy[i]=='.') {
                v.emplace_back("");
            } else if (str_cpy[i]>='0' && str_cpy[i]<='9') {
                v.back().push_back(str_cpy[i]);
            } else {
                _data->set(bigarithm::get_nan());
                return;
            }
        }

        if (v.size()==0 || v.size()>2) {
            _data->set(bigarithm::get_nan());
            return;
        }

        for (int i=0; i<v.size(); i++) {
            if (v[i].empty()) {
                _data->set(bigarithm::get_nan());
                return;
            }
        }

        std::string comb=v[0];
        if (v.size()!=1) comb+=v[1];
        std::reverse(comb.begin(), comb.end());
        while (comb.size()>1 && comb.back()=='0') comb.pop_back();

        for (u_int64_t t=0; t<_precision*32; ++t) {
            int carry=0;
            for (int i=0; i<comb.size(); i++) {
                int k=(comb[i]-'0')*2+carry;
                carry=0;
                if (k>=10) {
                    k%=10;
                    carry=1;
                }
                comb[i]='0'+k;
            }
            if (carry!=0) comb.push_back('1');
        }

        if (v.size()==2 && comb.size()<=v[1].size()) comb="0";
        else if (v.size()==2) comb = comb.substr(v[1].size(), comb.size() - v[1].size());

        std::reverse(comb.begin(), comb.end());

        int pos = 0;
        int first_not_null = 0;
        bool finished = false;
        while (true) {
            if (pos % 32 == 0) {
                _data->add32();
            }

            _data->set_bits(pos / 32, _data->get_bits(pos / 32) + (((comb.back() - '0') % 2) << (pos % 32)));

            bool flag = false;
            int add = 0;
            for (int i = first_not_null; i < comb.size(); i++) {
                if (comb[i] == '0' && !flag) {
                    ++first_not_null;
                    if (first_not_null == comb.size()) {
                        finished = true;
                        break;
                    }
                } else flag = true;
                int k = static_cast<int>(comb[i] - '0');
                comb[i] = static_cast<char>('0' + (add + k) / 2);
                if ((add + k) % 2 == 1) add = 10;
                else add = 0;
            }

            if (finished) break;
            ++pos;
        }
    }

    bigfloat bigfloat::operator+() const {
        return {+(*_data), _precision};
    }

    bigfloat bigfloat::operator-() const {
        return {-(*_data), _precision};
    }

    bigfloat bigfloat::operator+(const bigfloat &b) const {
        std::pair<bigfloat, bigfloat> p = _normalize(*this, b);
        bigfloat ret(*p.first._data + *p.second._data, p.first._precision);
        return ret;
    }

    bigfloat bigfloat::operator-(const bigfloat &b) const {
        return *this + (-b);
    }

    bigfloat bigfloat::operator*(const bigfloat &b) const {
        bigfloat ret(*_data**b._data, _precision + b._precision);
        ret._set_precision(std::max(_precision, b._precision));
        return ret;
    }

    bigfloat bigfloat::operator/(const bigfloat &b) const {
        std::pair<bigfloat, bigfloat> p = _normalize(*this, b);
        p.first._set_precision(p.first._precision * 2);
        bigfloat ret(*p.first._data / *p.second._data, p.second._precision);
        return ret;
    }

    bigfloat &bigfloat::operator+=(const bigfloat &b) {
        *this = (*this) + b;
        return *this;
    }

    bigfloat &bigfloat::operator-=(const bigfloat &b) {
        *this = (*this) - b;
        return *this;
    }

    bigfloat &bigfloat::operator*=(const bigfloat &b) {
        *this = (*this) * b;
        return *this;
    }

    bigfloat &bigfloat::operator/=(const bigfloat &b) {
        *this = (*this) / b;
        return *this;
    }

    bool bigfloat::operator==(const bigfloat &b) const {
        std::pair<bigfloat, bigfloat> p= _normalize(*this, b);
        return *p.first._data == *p.second._data;
    }

    bool bigfloat::operator!=(const bigfloat &b) const {
        return !(*this==b);
    }

    bool bigfloat::operator>(const bigfloat &b) const {
        std::pair<bigfloat, bigfloat> p= _normalize(*this, b);
        return *p.first._data > *p.second._data;
    }

    bool bigfloat::operator<(const bigfloat &b) const {
        return !(*this >= b);
    }

    bool bigfloat::operator>=(const bigfloat &b) const {
        return (*this > b) || (*this == b);
    }

    bool bigfloat::operator<=(const bigfloat &b) const {
        return !(*this > b);
    }

    bool bigfloat::is_zero() const {
        return _data->is_zero();
    }

    bool bigfloat::is_nan() const {
        return _data->is_nan();
    }

    bool bigfloat::is_infinity() const {
        return _data->is_infinity();
    }

    bool bigfloat::is_undef() const {
        return _data->is_undef();
    }

    bool bigfloat::is_positive() const {
        return _data->is_positive();
    }

    bool bigfloat::is_negative() const {
        return _data->is_negative();
    }

    bool bigfloat::is_normal() const {
        return !is_abnormal();
    }

    bool bigfloat::is_abnormal() const {
        return (is_infinity() || is_undef() || is_nan());
    }

    int8_t bigfloat::get_sign(const bigfloat &a) {
        return bigarithm::get_sign(*a._data);
    }

    bigfloat bigfloat::abs(const bigfloat &a) {
        return {bigarithm::abs(*a._data), a._precision};
    }

    bigint bigfloat::cut(const bigfloat &bf) {
        bigarithm new_data=*(bf._data);
        if (new_data.is_abnormal()) return new_data;
        std::vector<u_int32_t> bits=new_data.get_bits();
        u_int64_t precision=bf._precision;
        if (precision>bits.size()) return bigarithm::get_zero();
        std::vector<u_int32_t> new_bits(bits.size()-precision);
        for (int i=0; i<bits.size()-precision; ++i) {
            new_bits[i]=bits[i+precision];
        }
        new_data.set_bits(new_bits);
        return new_data;
    }

    std::string bigfloat::decimal(u_int64_t c) {
        std::string s=as_string();
        size_t pos=s.find('.');
        if (c==0) {
            if (pos==-1) return s;
            std::string ret=s.substr(0, pos);
            int carry=s[pos+2]>='5';
            std::reverse(ret.begin(), ret.end());
            for (u_int64_t i=0; i<ret.size(); ++i) {
                if (carry) ++ret[i];
                if (ret[i]>'9') ret[i]='0';
                else {
                    carry=0;
                    break;
                }
            }
            if (carry) ret.push_back('1');
            std::reverse(ret.begin(), ret.end());
            return ret;
        }
        if (pos==-1) {
            s.push_back('.');
            for (u_int64_t i=0; i<c; ++i) {
                s.push_back('0');
            }
            return s;
        }
        u_int64_t len_dec=s.size()-pos-1;
        if (len_dec==c) return s;
        if (len_dec<=c) {
            for (u_int64_t i=0; i<c-len_dec; ++i) {
                s.push_back('0');
            }
            return s;
        } else {
            std::string ret=s.substr(0, pos+c+1);
            int carry=s[pos+c+1]>='5';
            std::reverse(ret.begin(), ret.end());
            for (u_int64_t i=0; i<ret.size(); ++i) {
                if (ret[i]=='.') continue;
                if (carry) ++ret[i];
                if (ret[i]>'9') ret[i]='0';
                else {
                    carry=0;
                    break;
                }
            }
            if (carry) ret.push_back('1');
            std::reverse(ret.begin(), ret.end());
            return ret;
        }
    }

    void bigfloat::_set_precision(u_int64_t p) {
        if (_data->is_abnormal()) return;
        if (p == _precision) return;
        if (p > _precision) {
            std::vector<u_int32_t> new_data(p - _precision, 0);
            std::vector<u_int32_t> orig=_data->get_bits();
            new_data.insert(new_data.end(), orig.begin(), orig.end());
            _data->set_bits(new_data);
            _data->delete_leading();
        } else {
            std::vector<u_int32_t> new_data;
            u_int32_t carry = 0;
            if (_data->get_bits(_precision - p-1)/(static_cast<u_int32_t>(1) << 31)==1) {
                carry=1;
            }
            for (u_int64_t i = _precision - p; i < _data->get_sz(); ++i) {
                new_data.emplace_back(_data->get_bits(i));
            }
            if (new_data.empty()) new_data={0};
            if (carry) {
                int pos=0;
                while (carry!=0) {
                    if (pos==new_data.size()) new_data.emplace_back(0);
                    ++new_data[pos];
                    if (new_data[pos]!=0) carry=0;
                    ++pos;
                }
            }
            _data->set_bits(new_data);
        }
        _precision = p;
    }

    std::pair<bigfloat, bigfloat> bigfloat::_normalize(const bigfloat &a, const bigfloat &b) {
        if (a._precision==b._precision) return {a,b};
        u_int64_t p=std::max(a._precision, b._precision);
        bigfloat acpy=a;
        bigfloat bcpy=b;
        acpy._set_precision(p);
        bcpy._set_precision(p);
        return {acpy,bcpy};
    }
}

std::ostream &operator<<(std::ostream &os, const bigfloat &a) {
    os << a.as_string();
    return os;
}

std::istream &operator>>(std::istream &is, bigfloat &a) {
    std::string num;
    is >> num;
    a = bigfloat(num);
    return is;
}

bigfloat operator "" _bf(const char *s) {
    return std::string(s);
}
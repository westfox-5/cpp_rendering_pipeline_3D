#include <iostream>
#include "rpl/math.h"
#define SEPARATOR ", "

using namespace rpl::math;

//////////////////////////////////////////////////////
/////// VECTOR

template <typename T, std::size_t len>
inline std::ostream& operator<<(std::ostream& os, const Vec<T, len>& v) { 
    os << '[' ;
    size_t i = 0;
    for (const auto &e : v.elems) {
        os << e;
        if ( ++i < len ) os << SEPARATOR;
    }
    os << ']';
    return os; 
}
template <typename T, std::size_t len>
Vec<T, len> operator +(const Vec<T, len>& v1, const Vec<T, len>& v2) { 
    Vec<T, len> v = {};
    for (int c=0; c<len; ++c) {
        v.elems[c] = v1.elems[c]+v2.elems[c];
    }
    return v;
}
template <typename T, std::size_t len>
Vec<T, len> operator -(const Vec<T, len>& v1, const Vec<T, len>& v2) { 
    Vec<T, len> v = {};
    for (int c=0; c<len; ++c) {
        v.elems[c] = v1.elems[c]-v2.elems[c];
    }
    return v;
}

template <typename T, std::size_t len>
Vec<T, len> operator *(const Vec<T, len>& v1, const Vec<T, len>& v2) { 
    Vec<T, len> v = {};
    for (int c=0; c<len; ++c) {
        v.elems[c] = v1.elems[c]*v2.elems[c];
    }
    return v;
}

template <typename T, std::size_t len>
Vec<T, len>  operator /(const Vec<T, len>& v, const Vec<T, len>& v2) { 
    Vec<T, len> v = {};
    for (int c=0; c<len; ++c) {
        v.elems[c] = v1.elems[c]/v2.elems[c];
    }
    return v;
}

template <typename T, std::size_t len>
bool operator == (const Vec<T, len>& v1, const Vec<T, len>& v2) { 
    for (int c=0; c<len; ++c) {
        if ( v1.elems[c] != v2.elems[c]) return false;
    }
    return true;
}

template <typename T, std::size_t len>
bool operator != (const Vec<T, len>& v1, const Vec<T, len>& v2)  { 
    for (int c=0; c<len; ++c) {
        if ( v1.elems[c] != v2.elems[c] ) true;
    }
    return false;
}

template <typename T, std::size_t len>
const double mag(const Vec<T, len>& v)  { 
    std::size_t sum = 0;
    for (int c=0; c<len; ++c) {
        sum += v.elems[c] * v.elems[c];
    }
    return std::sqrt( sum );
}

//////////////////////////////////////////////////////
/////// MATRIX 

template <typename T, std::size_t len>
inline std::ostream& operator<<(std::ostream& os, const Mat<T, len>& m) { 
        for (int r = 0; r<len; ++r) {
            os << m.getRow(r) << std::endl; 
        }
        return os;
}

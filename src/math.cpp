#include <iostream>
#include "rpl/math.h"
#define SEPARAtype_tOR ", "

using namespace rpl::Math;

//////////////////////////////////////////////////////
/////// VECtype_tOR

template <typename type_t, std::size_t len>
inline std::ostream& operator<<(std::ostream& os, const Vector<type_t, len>& v) { 
    os << '[' ;
    size_t i = 0;
    for (const auto &e : v.elems) {
        os << e;
        if ( ++i < len ) os << SEPARAtype_tOR;
    }
    os << ']';
    return os; 
}
template <typename type_t, std::size_t len>
Vector<type_t, len> operator +(const Vector<type_t, len>& v1, const Vector<type_t, len>& v2) { 
    Vector<type_t, len> v = {};
    for (size_t c=0; c<len; ++c) {
        v.elems[c] = v1.elems[c]+v2.elems[c];
    }
    return v;
}
template <typename type_t, std::size_t len>
Vector<type_t, len> operator -(const Vector<type_t, len>& v1, const Vector<type_t, len>& v2) { 
    Vector<type_t, len> v = {};
    for (size_t c=0; c<len; ++c) {
        v.elems[c] = v1.elems[c]-v2.elems[c];
    }
    return v;
}

template <typename type_t, std::size_t len>
Vector<type_t, len> operator *(const Vector<type_t, len>& v1, const Vector<type_t, len>& v2) { 
    Vector<type_t, len> v = {};
    for (size_t c=0; c<len; ++c) {
        v.elems[c] = v1.elems[c]*v2.elems[c];
    }
    return v;
}

template <typename type_t, std::size_t len>
Vector<type_t, len>  operator /(const Vector<type_t, len>& v1, const Vector<type_t, len>& v2) { 
    Vector<type_t, len> v = {};
    for (size_t c=0; c<len; ++c) {
        v.elems[c] = v1.elems[c]/v2.elems[c];
    }
    return v;
}

template <typename type_t, std::size_t len>
bool operator == (const Vector<type_t, len>& v1, const Vector<type_t, len>& v2) { 
    for (size_t c=0; c<len; ++c) {
        if ( v1.elems[c] != v2.elems[c]) return false;
    }
    return true;
}

template <typename type_t, std::size_t len>
bool operator != (const Vector<type_t, len>& v1, const Vector<type_t, len>& v2)  { 
    for (size_t c=0; c<len; ++c) {
        if ( v1.elems[c] != v2.elems[c] ) true;
    }
    return false;
}

template <typename type_t, std::size_t len>
const double mag(const Vector<type_t, len>& v)  { 
    std::size_t sum = 0;
    for (size_t c=0; c<len; ++c) {
        sum += v.elems[c] * v.elems[c];
    }
    return std::sqrt( sum );
}

//////////////////////////////////////////////////////
/////// MATRIX 

template <typename type_t, std::size_t len>
inline std::ostream& operator<<(std::ostream& os, const Matrix<type_t, len>& m) { 
    for (size_t r = 0; r<len; ++r) {
        os << m.getRow(r) << std::endl; 
    }
    return os;
}

template <typename type_t, std::size_t len>
Matrix<type_t, len> operator + (const Matrix<type_t, len>& m1, const Matrix<type_t, len>& m2) { 
    Matrix<type_t, len> m;
    for (size_t i = 0; i<len*len; ++i)
        m.elems[i] = m1.elems[i] + m2.elems[i];
    return m;
}

template <typename type_t, std::size_t len>
Matrix<type_t, len> operator - (const Matrix<type_t, len>& m1, const Matrix<type_t, len>& m2) { 
    Matrix<type_t, len> m;
    for (size_t i = 0; i<len*len; ++i)
        m.elems[i] = m1.elems[i] - m2.elems[i];
    return m;
}

template <typename type_t, std::size_t len>
Matrix<type_t, len> operator * (const Matrix<type_t, len>& m1, const Matrix<type_t, len>& m2) { 
    Matrix<type_t, len> m;
    for (size_t i = 0; i<len*len; ++i) {
        for (size_t k = 0; k<len; ++k) {
            m.elems[i] += ( m1.elems[i] * m2.elems[i]);
        }
    }
    return m;
}



#pragma once // include only one time at compile-time

#include <vector>
#include <cmath>


namespace rpl {
    namespace math {

        ////////////////////////////////////////////////
        ////////////////////////////////////////////////
        /// VECTOR

        template <typename T, std::size_t len>
        struct Vec {
            T elems[len];

            //Vec<T, len>() : elems() { elems = new T[len]; }
            //Vec<T, len>(const Vec<T, len> & other): elems(other.elems) {}
            ~Vec() {}

        };

        typedef Vec<int, 2> vi2d;
        typedef Vec<float, 2> vf2d;

        typedef Vec<int, 3> vi3d;
        typedef Vec<float, 3> vf3d;

        typedef Vec<int, 4> vi4d;
        typedef Vec<float, 4> vf4d;


        ////////////////////////////////////////////////
        ////////////////////////////////////////////////
        /// MATRIX

        template <typename T, std::size_t len>
        struct Mat {
            T elems[len*len];

            inline const T getElem(int r, int c) const {
                return elems[r+ (c*len)];
            }

            Vec<T, len> getRow(int r) const { 
                Vec<T, len> v = {};
                for (int c=0; c<len; ++c) {
                    v.elems[c] = getElem(r, c);
                }
                return v;
            }
        };

        typedef Mat<int, 2> mi2d;
        typedef Mat<float, 2> mf2d;
    }
}
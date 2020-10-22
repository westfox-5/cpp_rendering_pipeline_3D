#pragma once // include only one time at compile-time

#include <cmath>


namespace rpl {
    namespace Math {

        ////////////////////////////////////////////////
        ////////////////////////////////////////////////
        /// Vector

        template <typename type_t, std::size_t len>
        struct Vector {
            type_t elems[len];


            // COMMONS
            type_t X() { return elems[0]; }
            type_t Y() { return elems[1]; }
            type_t Z() { return elems[2]; }
            type_t W() { return elems[3]; }
        };

        typedef Vector<float, 2> vf2d;
        typedef Vector<float, 3> vf3d;
        typedef Vector<float, 4> vf4d;

        ////////////////////////////////////////////////
        ////////////////////////////////////////////////
        /// MATRIX

        template <typename type_t, std::size_t len>
        struct Matrix {
            type_t elems[len*len];

            inline const type_t getElem(int r, int c) const {
                //std::cout << std::endl <<r << ' ' << c << " : " << elems[r +(c*len)] << std::endl;
                return elems[(r*len)+c];
            }

            Vector<type_t, len> getRow(int r) const { 
                Vector<type_t, len> v = {};
                for (int c=0; c<len; ++c) {
                    v.elems[c] = getElem(r, c);
                }
                return v;
            }
        };

        // typedef Matrix<float, 2> mf2d;
        // typedef Matrix<float, 3> mf3d;
        // typedef Matrix<float, 4> mf4d;


        //static matrix rotationX(degree),rotationY(degree),rotationZ(degree) 
        //static matrix identity
        
        template <typename type_t, size_t len>
        static void transform(Vector<type_t, len> v, Matrix<type_t, len> m ) {
            Vector<type_t, len> tmp(&v);

        }
    }
}
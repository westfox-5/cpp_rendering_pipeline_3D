#pragma once // include only one time at compile-time

#include <cmath>

namespace rpl {
    namespace Math {

        struct Vector2D {
            float x, y;

            Vector2D() : Vector2D(0, 0) {}
            Vector2D(float x, float y) : x(x), y(y) {}
            Vector2D(const Vector2D & other) : x(other.x), y(other.y) {}

            const Vector2D operator+(const Vector2D &val) const { return Vector2D{x+val.x, y+val.y}; }
            const Vector2D operator-(const Vector2D &val) const { return Vector2D{x-val.x, y-val.y}; }
            const Vector2D operator*(const Vector2D &val) const { return Vector2D{x*val.x, y*val.y}; }
            const Vector2D operator/(const Vector2D &val) const { return Vector2D{x/val.x, y/val.y}; }

            const Vector2D operator*(float val) const { return Vector2D{x*val, y*val}; }
            const Vector2D operator/(float val) const { return Vector2D{x/val, y/val}; }

            static float dotProduct(const Vector2D &a, const Vector2D &b) { return a.x*b.x + a.y*b.y; }


            const float length() const { return sqrtf(x*x + y*y); }
            static inline Vector2D normalize(const Vector2D &v) {
                const float l = v.length();
                return Vector2D{v.x / l, v.y / l};
            }
        };

        std::ostream& operator<<(std::ostream& os, const rpl::Math::Vector2D v) { 
            os << '[' << v.x << ", " << v.y << ']';
            return os; 
        }

        struct Vector3D : public Vector2D {
            float z;

            Vector3D() : Vector3D(0, 0, 0) {}
            Vector3D(float x, float y, float z) : Vector2D(x, y), z(z) {}
            Vector3D(const Vector3D & other) : Vector2D(other.x, other.y), z(other.z) {}

            
            
            const Vector3D operator+(const Vector3D &val) const { return Vector3D{x+val.x, y+val.y, z+val.z}; }
            const Vector3D operator-(const Vector3D &val) const { return Vector3D{x-val.x, y-val.y, z-val.z}; }
            const Vector3D operator*(const Vector3D &val) const { return Vector3D{x*val.x, y*val.y, z*val.z}; }
            const Vector3D operator/(const Vector3D &val) const { return Vector3D{x/val.x, y/val.y, z/val.z}; }

            const Vector3D operator*(float val) const { return Vector3D{x*val, y*val, z*val}; }
            const Vector3D operator/(float val) const { return Vector3D{x/val, y/val, z/val}; }

            static Vector3D cross(const Vector3D &a, const Vector3D &b) {
                return Vector3D {
                    a.y*b.z - a.z*b.y,
                    a.z*b.x - a.x*b.z,
                    a.x*b.y - a.y*b.x,
                };
            }
            static float dotProduct(const Vector3D &a, const Vector3D &b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
            
            

            const float length() const { return sqrtf(x*x + y*y + z*z); }
            static inline Vector3D normalize(const Vector3D &v) {
                const float l = v.length();
                return Vector3D{v.x / l, v.y / l, v.z / l};
            }
        };

        std::ostream& operator<<(std::ostream& os, const rpl::Math::Vector3D v) { 
            os << '[' << v.x << ", " << v.y << ", " << v.z << ']';
            return os; 
        }

        struct Vector4D {
            float x, y, z, w;

            Vector4D() : Vector4D(0, 0, 0, 0) {}
            Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
            Vector4D(const Vector3D &v3D, float w) : x(v3D.x), y(v3D.y), z(v3D.z), w(w) {}
            Vector4D(const Vector4D &other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

            const Vector4D operator+(const Vector4D &val) const { return Vector4D{x+val.x, y+val.y, z+val.z, w+val.w}; }
            const Vector4D operator-(const Vector4D &val) const { return Vector4D{x-val.x, y-val.y, z-val.z, w-val.w}; }
            const Vector4D operator*(const Vector4D &val) const { return Vector4D{x*val.x, y*val.y, z*val.z, w*val.w}; }
            const Vector4D operator/(const Vector4D &val) const { return Vector4D{x/val.x, y/val.y, z/val.z, w/val.w}; }

            const Vector4D operator*(float val) const { return Vector4D{x*val, y*val, z*val, w*val}; }
            const Vector4D operator/(float val) const { return Vector4D{x/val, y/val, z/val, w/val}; }

            inline float operator[](unsigned int p) const { 
                if (p == 0) return x;
                if (p == 1) return y;
                if (p == 2) return z;
                if (p == 3) return w;

                return -1;
            }
            
            static float dotProduct(const Vector4D &a, const Vector4D &b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

            const float length() const { return sqrtf(x*x + y*y + z*z + w*w ); }
            static inline Vector4D normalize(const Vector4D &v) {
                const float l = v.length();
                return Vector4D{v.x / l, v.y / l, v.z / l, v.w / l};
            }
        };

        std::ostream& operator<<(std::ostream& os, const rpl::Math::Vector4D v) { 
            os << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']';
            return os; 
        } 

        struct Matrix4D {
            float m[4][4];

            Matrix4D() : m{0} {}

            inline Vector4D getRow(int y) const { 
                return {
                    m[y][0],
                    m[y][1],
                    m[y][2],
                    m[y][3]
                };
            }
            inline Vector4D getCol(int x) const { 
                return {
                    m[0][x],
                    m[1][x],
                    m[2][x],
                    m[3][x]
                };
            }

            std::ostream& operator<<(std::ostream& os) const { 
                for (int r = 0; r<4; ++r){
                    for (int c = 0; c<4; ++c)
                        os << m[r][c];
                    os << std::endl; 
                }
                return os;
            }


            float Determinant() 
            { //http://www.softwareandfinance.com/CPP/Matrix_Inverse.html
                Matrix4D temp[4];
                for(int k = 0; k < 4; k++)
                {
                    for(int i = 1; i < 4; i++)
                    {
                        int j1 = 0;
                        for(int j = 0; j < 4; j++)
                        {
                            if(k == j)
                                continue;

                            temp[k].m[i-1][j1++] = this->m[i][j];
                        }
                    }
                }

                float det = 
                this->m[0][0] * temp[0].Determinant() -
                this->m[0][1] * temp[1].Determinant() +
                this->m[0][2] * temp[2].Determinant() -
                this->m[0][3] * temp[3].Determinant();

                return det;
            }

            bool Invert(Matrix4D &inv)
            { //https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
                float A2323 = this->m[2][2] * this->m[3][3] - this->m[2][3] * this->m[3][2] ;
                float A1323 = this->m[2][1] * this->m[3][3] - this->m[2][3] * this->m[3][1] ;
                float A1223 = this->m[2][1] * this->m[3][2] - this->m[2][2] * this->m[3][1] ;
                float A0323 = this->m[2][0] * this->m[3][3] - this->m[2][3] * this->m[3][0] ;
                float A0223 = this->m[2][0] * this->m[3][2] - this->m[2][2] * this->m[3][0] ;
                float A0123 = this->m[2][0] * this->m[3][1] - this->m[2][1] * this->m[3][0] ;
                float A2313 = this->m[1][2] * this->m[3][3] - this->m[1][3] * this->m[3][2] ;
                float A1313 = this->m[1][1] * this->m[3][3] - this->m[1][3] * this->m[3][1] ;
                float A1213 = this->m[1][1] * this->m[3][2] - this->m[1][2] * this->m[3][1] ;
                float A2312 = this->m[1][2] * this->m[2][3] - this->m[1][3] * this->m[2][2] ;
                float A1312 = this->m[1][1] * this->m[2][3] - this->m[1][3] * this->m[2][1] ;
                float A1212 = this->m[1][1] * this->m[2][2] - this->m[1][2] * this->m[2][1] ;
                float A0313 = this->m[1][0] * this->m[3][3] - this->m[1][3] * this->m[3][0] ;
                float A0213 = this->m[1][0] * this->m[3][2] - this->m[1][2] * this->m[3][0] ;
                float A0312 = this->m[1][0] * this->m[2][3] - this->m[1][3] * this->m[2][0] ;
                float A0212 = this->m[1][0] * this->m[2][2] - this->m[1][2] * this->m[2][0] ;
                float A0113 = this->m[1][0] * this->m[3][1] - this->m[1][1] * this->m[3][0] ;
                float A0112 = this->m[1][0] * this->m[2][1] - this->m[1][1] * this->m[2][0] ;

                float det = this->m[0][0] * ( this->m[1][1] * A2323 - this->m[1][2] * A1323 + this->m[1][3] * A1223 ) 
                        - this->m[0][1] * ( this->m[1][0] * A2323 - this->m[1][2] * A0323 + this->m[1][3] * A0223 ) 
                        + this->m[0][2] * ( this->m[1][0] * A1323 - this->m[1][1] * A0323 + this->m[1][3] * A0123 ) 
                        - this->m[0][3] * ( this->m[1][0] * A1223 - this->m[1][1] * A0223 + this->m[1][2] * A0123 ) ;
                if (det == 0)
                    return false;
                det = 1 / det;

                inv.m[0][0] = det *   ( this->m[1][1] * A2323 - this->m[1][2] * A1323 + this->m[1][3] * A1223 ),
                inv.m[0][1] = det * - ( this->m[0][1] * A2323 - this->m[0][2] * A1323 + this->m[0][3] * A1223 ),
                inv.m[0][2] = det *   ( this->m[0][1] * A2313 - this->m[0][2] * A1313 + this->m[0][3] * A1213 ),
                inv.m[0][3] = det * - ( this->m[0][1] * A2312 - this->m[0][2] * A1312 + this->m[0][3] * A1212 ),
                inv.m[1][0] = det * - ( this->m[1][0] * A2323 - this->m[1][2] * A0323 + this->m[1][3] * A0223 ),
                inv.m[1][1] = det *   ( this->m[0][0] * A2323 - this->m[0][2] * A0323 + this->m[0][3] * A0223 ),
                inv.m[1][2] = det * - ( this->m[0][0] * A2313 - this->m[0][2] * A0313 + this->m[0][3] * A0213 ),
                inv.m[1][3] = det *   ( this->m[0][0] * A2312 - this->m[0][2] * A0312 + this->m[0][3] * A0212 ),
                inv.m[2][0] = det *   ( this->m[1][0] * A1323 - this->m[1][1] * A0323 + this->m[1][3] * A0123 ),
                inv.m[2][1] = det * - ( this->m[0][0] * A1323 - this->m[0][1] * A0323 + this->m[0][3] * A0123 ),
                inv.m[2][2] = det *   ( this->m[0][0] * A1313 - this->m[0][1] * A0313 + this->m[0][3] * A0113 ),
                inv.m[2][3] = det * - ( this->m[0][0] * A1312 - this->m[0][1] * A0312 + this->m[0][3] * A0112 ),
                inv.m[3][0] = det * - ( this->m[1][0] * A1223 - this->m[1][1] * A0223 + this->m[1][2] * A0123 ),
                inv.m[3][1] = det *   ( this->m[0][0] * A1223 - this->m[0][1] * A0223 + this->m[0][2] * A0123 ),
                inv.m[3][2] = det * - ( this->m[0][0] * A1213 - this->m[0][1] * A0213 + this->m[0][2] * A0113 ),
                inv.m[3][3] = det *   ( this->m[0][0] * A1212 - this->m[0][1] * A0212 + this->m[0][2] * A0112 );

                return true;
            }

            rpl::Math::Matrix4D operator*(const Matrix4D &M) const {
                Matrix4D mult;

                mult.m[0][0] = this->m[0][0] * M.m[0][0] + this->m[0][1] * M.m[1][0] + this->m[0][2] * M.m[2][0] + this->m[0][3] * M.m[3][0];
                mult.m[0][1] = this->m[0][0] * M.m[0][1] + this->m[0][1] * M.m[1][1] + this->m[0][2] * M.m[2][1] + this->m[0][3] * M.m[3][1];
                mult.m[0][2] = this->m[0][0] * M.m[0][2] + this->m[0][1] * M.m[1][2] + this->m[0][2] * M.m[2][2] + this->m[0][3] * M.m[3][2];
                mult.m[0][3] = this->m[0][0] * M.m[0][3] + this->m[0][1] * M.m[1][3] + this->m[0][2] * M.m[2][3] + this->m[0][3] * M.m[3][3];

                mult.m[1][0] = this->m[1][0] * M.m[0][0] + this->m[1][1] * M.m[1][0] + this->m[1][2] * M.m[2][0] + this->m[1][3] * M.m[3][0];
                mult.m[1][1] = this->m[1][0] * M.m[0][1] + this->m[1][1] * M.m[1][1] + this->m[1][2] * M.m[2][1] + this->m[1][3] * M.m[3][1];
                mult.m[1][2] = this->m[1][0] * M.m[0][2] + this->m[1][1] * M.m[1][2] + this->m[1][2] * M.m[2][2] + this->m[1][3] * M.m[3][2];
                mult.m[1][3] = this->m[1][0] * M.m[0][3] + this->m[1][1] * M.m[1][3] + this->m[1][2] * M.m[2][3] + this->m[1][3] * M.m[3][3];

                mult.m[2][0] = this->m[2][0] * M.m[0][0] + this->m[2][1] * M.m[1][0] + this->m[2][2] * M.m[2][0] + this->m[2][3] * M.m[3][0];
                mult.m[2][1] = this->m[2][0] * M.m[0][1] + this->m[2][1] * M.m[1][1] + this->m[2][2] * M.m[2][1] + this->m[2][3] * M.m[3][1];
                mult.m[2][2] = this->m[2][0] * M.m[0][2] + this->m[2][1] * M.m[1][2] + this->m[2][2] * M.m[2][2] + this->m[2][3] * M.m[3][2];
                mult.m[2][3] = this->m[2][0] * M.m[0][3] + this->m[2][1] * M.m[1][3] + this->m[2][2] * M.m[2][3] + this->m[2][3] * M.m[3][3];

                mult.m[3][0] = this->m[3][0] * M.m[0][0] + this->m[3][1] * M.m[1][0] + this->m[3][2] * M.m[2][0] + this->m[3][3] * M.m[3][0];
                mult.m[3][1] = this->m[3][0] * M.m[0][1] + this->m[3][1] * M.m[1][1] + this->m[3][2] * M.m[2][1] + this->m[3][3] * M.m[3][1];
                mult.m[3][2] = this->m[3][0] * M.m[0][2] + this->m[3][1] * M.m[1][2] + this->m[3][2] * M.m[2][2] + this->m[3][3] * M.m[3][2];
                mult.m[3][3] = this->m[3][0] * M.m[0][3] + this->m[3][1] * M.m[1][3] + this->m[3][2] * M.m[2][3] + this->m[3][3] * M.m[3][3];

                return mult;

            }
        };

        static Matrix4D Identity() {
            Matrix4D m;
            m.m[0][0] = 1;
            m.m[1][1] = 1;
            m.m[2][2] = 1;
            m.m[3][3] = 1;
            return m;
        } 

        static void MultPointMatrix(const Vector3D &in, Vector3D &out, Matrix4D &M) {
            out.x = in.x * M.m[0][0] + in.y * M.m[1][0] + in.z * M.m[2][0] + M.m[3][0];
            out.y = in.x * M.m[0][1] + in.y * M.m[1][1] + in.z * M.m[2][1] + M.m[3][1];
            out.z = in.x * M.m[0][2] + in.y * M.m[1][2] + in.z * M.m[2][2] + M.m[3][2];
            const float w = in.x * M.m[0][3] + in.y * M.m[1][3] + in.z * M.m[2][3] + M.m[3][3];
            if (w != 1) {
                out.x /= w;
                out.y /= w;
                out.z /= w;
            }
        }

        static Matrix4D Transpose(const Matrix4D &M)
        {
            Matrix4D transposed;
            for(int r=0;r<4;r++)    
                for(int c=0;c<4;c++)    
                transposed.m[r][c] = M.m[c][r];
            return transposed;
        }
    }

}
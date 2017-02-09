#ifndef VEC4_H
#define VEC4_H

#include <iostream>
#include "math.h"

enum v
{
        x = 0,
        y = 1,
        z = 2,
        w = 3
};

enum c
{
        r = 0,
        g,
        b,
        q
};

//Vertex 1, Vertex 2, Vertex 3 of a triangle
enum triangle
{
        v1 = 0,
        v2,
        v3
};

template<typename T>
class Vec4
{
private:
        static const int count = 4;
        T xyzw[count];

public:
        Vec4<T>()
        {
                xyzw[0] = (T)0.0f;
                xyzw[1] = (T)0.0f;
                xyzw[2] = (T)0.0f;
                xyzw[3] = (T)0.0f;
        }

        Vec4<T>(T x, T y, T z, T w)
        {
                xyzw[0] = x;
                xyzw[1] = y;
                xyzw[2] = z;
                xyzw[3] = w;
        }

        Vec4<T>(const Vec4<T> &in)
        {
                xyzw[0] = in.xyzw[0];
                xyzw[1] = in.xyzw[1];
                xyzw[2] = in.xyzw[2];
                xyzw[3] = in.xyzw[3];
        }

        Vec4<T> operator-()
        {
                return Vec4<T>(-xyzw[x], -xyzw[y], -xyzw[z], -xyzw[w]);
        }
        T* operator&()
        {
                return xyzw;
        }

        Vec4<T> operator*(T scalar)
        {
                return Vec4<T>(scalar * xyzw[x], scalar * xyzw[y], scalar * xyzw[z], scalar * xyzw[w]);
        }

        Vec4<T> operator+(Vec4<T> other)
        {
                return Vec4<T>(xyzw[x] + other[x], xyzw[y] + other[y], xyzw[z] + other[z], xyzw[w] + other[w]);
        }

        Vec4<T> operator+(T scalar)
        {
                return Vec4<T>(xyzw[x] + scalar, xyzw[y] + scalar, xyzw[z] + scalar, xyzw[w] + scalar);
        }

        T &operator[](int index)
        {
                return xyzw[index];
        }

        T length() const
        {
                T sum = 0.0f;
                for(int i = 0; i < count; i++)
                {
                        sum += xyzw[i] * xyzw[i];
                }

                return sqrt(sum);
        }

        Vec4<T> normalise() const
        {
                T len = this->length();
                return Vec4<T>(xyzw[x] / len, xyzw[y] / len, xyzw[z] / len, xyzw[w] / len);
        }

        Vec4<T> crossProduct( Vec4<T> &other)
        {
                Vec4<T> ortho;

                ortho[x] = xyzw[y] * other[z] - xyzw[z] * other[y];
                ortho[y] = xyzw[z] * other[x] - xyzw[x] * other[z];
                ortho[z] = xyzw[x] * other[y] - xyzw[y] * other[x];

                return ortho;
        }

        /*Vec4<T> operator*(Mat4<T> *m)
        {
                Vec4<T> retval;

                retval[x] = xyzw[x] * m[0][0] + xyzw[y] * m[1][0] + xyzw[z] * m[2][0] + xyzw[w] * m[3][0];
                retval[y] = xyzw[x] * m[0][1] + xyzw[y] * m[1][1] + xyzw[z] * m[2][1] + xyzw[w] * m[3][1];
                retval[z] = xyzw[x] * m[0][2] + xyzw[y] * m[1][2] + xyzw[z] * m[2][2] + xyzw[w] * m[3][2];
                retval[w] = xyzw[x] * m[0][3] + xyzw[y] * m[1][3] + xyzw[z] * m[2][3] + xyzw[w] * m[3][3];

                return retval;
        }*/

        T *getData()
        {
                return xyzw;
        }

        void print()
        {
                std::cout << "x:" << xyzw[x] << " y:" << xyzw[y] << " z:" << xyzw[z] << " w:" << xyzw[w] << std::endl;
        }
};
#endif // VEC4_H

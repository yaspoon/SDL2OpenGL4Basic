#ifndef MAT4_H
#define MAT4_H

#include "Vec4.h"
#include <iostream>
#include "string.h"

template<typename T>
class Mat4
{
private:
        static const int cols = 4;
        static const int rows = 4;
        static const int count = cols * rows;
        T mat[count];
public:
        Mat4<T>()
        {
                for(int i = 0; i < count; i++)
                {
                        mat[i] = (T)0.0f;
                }
        }

        Mat4<T>(const Mat4<T> &copy)
        {
                memcpy(mat, copy.mat, sizeof(T) * count);
        }

        /*This initialises the diagonal row to whatever d is*/
        Mat4<T>(T d)
        {
                memset(mat, 0, sizeof(T) * count);
                for(int i = 0; i < cols; i++)
                {
                        mat[i * cols + i] = d;
                }
        }

        Mat4<T>(Vec4<T> a, Vec4<T> b, Vec4<T> c, Vec4<T> d)
                //mat[0](a), mat[1](b), mat[2](c), mat[3](d)
        {
                for(int i = 0; i < rows; i++)
                {
                        mat[0 * cols + i] = a[i];
                        mat[1 * cols + i] = b[i];
                        mat[2 * cols + i] = c[i];
                        mat[3 * cols + i] = d[i];
                }
        }

        Mat4<T>(T a, T b, T c, T d, //Col1
                        T e, T f, T g, T h, //Col2
                        T i, T j, T k, T l, //Col3
                        T x, T y, T z, T w //Col4
                        )
        {
                int col1 = 0 * cols;
                int col2 = 1 * cols;
                int col3 = 2 * cols;
                int col4 = 3 * cols;

                mat[col1 + 0] = a;
                mat[col1 + 1] = b;
                mat[col1 + 2] = c;
                mat[col1 + 3] = d;

                mat[col2 + 0] = e;
                mat[col2 + 1] = f;
                mat[col2 + 2] = g;
                mat[col2 + 3] = h;

                mat[col3 + 0] = i;
                mat[col3 + 1] = j;
                mat[col3 + 2] = k;
                mat[col3 + 3] = l;

                mat[col4 + 0] = x;
                mat[col4 + 1] = y;
                mat[col4 + 2] = z;
                mat[col4 + 3] = w;
        }

        Mat4<T> operator=(const Mat4<T> &other)
        {
                memcpy(mat, other.mat, sizeof(T) * count);
                return *this;
        }

        T *operator[](int index)
        {
                int offset = index * cols;
                return &(mat[offset]);
        }

        T *operator&()
        {
                return mat;
        }

        /*
        Wow this is fucked and took so long
        to write because I to retarded for this shit
        */
        Mat4 operator*(Mat4<T> other)
        {
                Mat4<T> retval;

                for(int row = 0; row < rows; row++)
                {
                        for(int col = 0; col < cols; col++)
                        {
                                T accum = (T)0.0f;
                                /*i is to get the 4 elements from each row and
                                   col we currently have selected*/
                                for(int i = 0; i < cols; i++)
                                {
                                        accum += (*this)[i][row] * other[col][i];
                                }
                                retval[col][row] = accum;
                        }
                }

                return retval;
        }

        void print()
        {
                for(int i = 0; i < rows; i++)
                {
                        for(int j = 0; j < cols; j++)
                        {
                                std::cout << (*this)[j][i] << " ";
                        }
                        std::cout << std::endl;
                }
        }
};
#endif // MAT4_H

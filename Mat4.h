#ifndef MAT4_H
#define MAT4_H

#include <iostream>
#include "string.h"
#include "Vec4.h"

// [fx ux rx tx]        [fx fy fz fw]
// [fy uy ry ty]        [ux uy uz uw]
// [fz uz rz tz]         [rx ry rz rw]
// [fw uw rw tw]    [tx ty tz tw]

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
                for(int i = 0; i < (cols - 1); i++)
                {
                        mat[i * cols + i] = d;
                }

                mat[15] = 1.0f;
        }

        Mat4<T>(Vec4<T> a, Vec4<T> b, Vec4<T> c, Vec4<T> d)
        {
                for(int i = 0; i < rows; i++)
                {
                        mat[0 * rows + i] = a[i];
                        mat[1 * rows + i] = b[i];
                        mat[2 * rows + i] = c[i];
                        mat[3 * rows + i] = d[i];
                }
        }

        Mat4<T>(T a, T b, T c, T d, //Row1
                        T e, T f, T g, T h, //Row2
                        T i, T j, T k, T l, //Row3
                        T x, T y, T z, T w //Row4
                        )
        {
                /*These are actually the rows.. But I didn't know wtf I was  doing and though this was the columns being set
                    hurrdurr*/
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
                int offset = index * rows;
                return &(mat[offset]);
        }

        T *operator&()
        {
                return mat;
        }

        Vec4<T> operator*(Vec4<T> v)
        {
                Vec4<T> retval;

                int row0 = 0 * rows;
                int row1 = 1 * rows;
                int row2 = 2 * rows;
                int row3 = 3 * rows;

                retval[x] = v[x] * mat[row0 + 0] + v[y] * mat[row1 + 0] + v[z] * mat[row2 + 0] + v[w] * mat[row3 + 0];
                retval[y] = v[x] * mat[row0 + 1] + v[y] * mat[row1 + 1] + v[z] * mat[row2 + 1] + v[w] * mat[row3 + 1];
                retval[z] = v[x] * mat[row0 + 2] + v[y] * mat[row1 + 2] + v[z] * mat[row2 + 2] + v[w] * mat[row3 + 2];
                retval[w] = v[x] * mat[row0 + 3] + v[y] * mat[row1 + 3] + v[z] * mat[row2 + 3] + v[w] * mat[row3 + 3];

                return retval;
        }

        /*
        Wow this is fucked and took so long
        to write because I'm to retarded for this shit
        */
        Mat4<T> operator*(Mat4<T> other)
        {
                Mat4<T> retval;

                for(int row = 0; row < rows; row++)
                {
                        for(int col = 0; col < cols; col++)
                        {
                                T accum = (T)0.0f;
                                //i is to get the 4 elements from each row and
                                   //col we currently have selected
                                for(int i = 0; i < cols; i++)
                                {
                                        accum += (*this)[row][i] * other[i][col];
                                }
                                retval[row][col] = accum;
                        }
                }

                return retval;
        }

        Mat4<T> setForward(Vec4<T> f)
        {
                Mat4<T> retval(*this);

                retval[0][x] = f[x];
                retval[0][y] = f[y];
                retval[0][z] = f[z];
                retval[0][w] = f[w];

                return retval;
        }

        Mat4<T> setUp(Vec4<T> up)
        {
                Mat4<T> retval(*this);

                retval[1][x] = up[x];
                retval[1][y] = up[y];
                retval[1][z] = up[z];
                retval[1][w] = up[w];

                return retval;
        }

        Mat4<T> setRight(Vec4<T> right)
        {
                Mat4<T> retval(*this);

                retval[2][x] = right[x];
                retval[2][y] = right[y];
                retval[2][z] = right[z];
                retval[2][w] = right[w];

                return retval;
        }

        Mat4<T> setTranslation(Vec4<T> trans)
        {
                Mat4<T> retval(*this);

                retval[3][x] = trans[x];
                retval[3][y] = trans[y];
                retval[3][z] = trans[z];

                return retval;
        }

	/* transpose
	 * input: none
	 * output: transpose of this matrix
	 * Transposings a matrix is to make the rows of the
	 * matrixs the column, or the columns the rows
	 */
	Mat4<T> transpose()
	{
		return Mat4<T>(mat[0 * rows +  x], mat[1 * rows +  x], mat[2 * rows +  x], mat[3 * rows +  x], //Top row of new matrix is the first column of the this matrix
			mat[0 * rows +  y], mat[1 * rows +  y], mat[2 * rows +  y], mat[3 * rows +  y], //Second row of new matrix is the second column of the this matrix
			mat[0 * rows +  z], mat[1 * rows +  z], mat[2 * rows +  z], mat[3 * rows +  z], //Third row of new matrix is the third column of the this matrix
			mat[0 * rows +  w], mat[1 * rows +  w], mat[2 * rows +  w], mat[3 * rows +  w]); //Fourth row of new matrix is the fourth column of the this matrix

	}

        void print()
        {
                for(int i = 0; i < rows; i++)
                {
                        for(int j = 0; j < cols; j++)
                        {
                                std::cout << (*this)[i][j] << " ";
                        }
                        std::cout << std::endl;
                }
        }
};
#endif // MAT4_H

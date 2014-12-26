#ifndef VEC4_H
#define VEC4_H

enum v
{
        x = 0,
        y = 1,
        z = 2
};

template<typename T>
class Vec4
{
private:
        T xyzw[4];

public:
        Vec4<T>()
        {
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

        T* operator&()
        {
                return xyzw;
        }

        T &operator[](int index)
        {
                return xyzw[index];
        }
};
#endif // VEC4_H

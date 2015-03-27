#ifndef BOX_H
#define BOX_H

#include "Mat4.h"
#include "Vec4.h"
#include <cstdlib>

class Box
{
private:
        static const int vertexCount = 4;
        static const int NUM_FACES = 6;
        static const int NUM_TRIANGLES = NUM_FACES * 2;
        float vertices[NUM_TRIANGLES][3][3];
        float colours[NUM_TRIANGLES][3][3];
        float normals[NUM_TRIANGLES][3][3];
        unsigned short indices[36];
public:
        Box();
        Box(Vec4<float> minPoint, Vec4<float> maxPoint);

        float *Vertices();
        size_t vsize();
        int vcount();
        unsigned short *getIndices();
        size_t isize();

        float *Colours();
        size_t csize();

        float *Normals();
        size_t nsize();

        void print(); //Print out box coordinates
};
#endif // BOX_H

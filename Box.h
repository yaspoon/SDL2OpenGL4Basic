#ifndef BOX_H
#define BOX_H

#include "Mat4.h"
#include "Vec4.h"
#include <cstdlib>

class Box
{
private:
        static const int vertexCount = 8;
        float vertices[3 * vertexCount];
        short indices[36];
public:
        Box();
        Box(Vec4<float> minPoint, Vec4<float> maxPoint);

        float *Vertices();
        size_t vsize();
        int vcount();
        short *getIndices();

        void print(); //Print out box coordinates
};
#endif // BOX_H

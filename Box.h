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
        unsigned short indices[36];
        float texCoords[2 * 4 * 6]; //2 floats per tex coord, 4 tex coords per face and 6 faces
        float normals[6];
public:
        Box();
        Box(Vec4<float> minPoint, Vec4<float> maxPoint);

        float *Vertices();
        size_t vsize();
        int vcount();
        unsigned short *getIndices();
        float *getTextureCoords();
        float *getNormals();
        size_t isize();
        size_t tsize();
        size_t nsize();

        void print(); //Print out box coordinates
};
#endif // BOX_H

#include "Box.h"
#include <string.h>

static const unsigned short indexs[] = {0, 1, 2, //Front face
                             2, 3, 0,
                             5, 4, 7, //Back Face
                             7, 6, 5,
                             1, 5, 6, //Right side
                             6, 2, 1,
                             0, 3, 7, //left side
                             7, 4, 0,
                             3,2, 6, //Top
                             6, 7 ,3,
                             4, 5, 1,//Bottom
                             1, 0, 4};

static const float coords[] = {0, 0.5, 0.33, 0.5, 0.33, 0, 0, 0, //0, 1, 0.33, 1, 0.33, 0.5, 0, 0.5, //Front
                                                  1, 0.5, 0.66, 0.5, 0.66, 0, 1, 0,//Back
                                                  0.33, 0.5, 0.66, 0.5, 0.66, 0, 0.33, 0,//Right
                                                  0.33, 1, 0.33, 0.5, 0, 0.5, 0, 1,//Left
                                                  0, 1, 1, 1, 1, 0, 0, 0,//Top
                                                  0, 1, 1, 1, 1, 0, 0, 0, //Bottom
                                                  };

static const float normals[] = {0.0f, 0.0f, -1.0f,
                                                    0.0f, 0.0f, -1.0f,
                                                    0.0f, 0.0f, -1.0f,
                                                    0.0f, 0.0f, -1.0f, //Front
                                                    0.0f, 0.0f, 1.0f,
                                                    0.0f, 0.0f, 1.0f,
                                                    0.0f, 0.0f, 1.0f,
                                                    0.0f, 0.0f, 1.0f,
                                                    1.0f, 0.0f, 0.0f,
                                                    1.0f, 0.0f, 0.0f,
                                                    1.0f, 0.0f, 0.0f,
                                                    1.0f, 0.0f, 0.0f,
                                                    -1.0f, 0.0f, 0.0f,
                                                    -1.0f, 0.0f, 0.0f,
                                                    -1.0f, 0.0f, 0.0f,
                                                    -1.0f, 0.0f, 0.0f,
                                                    0.0f, 1.0f, 0.0f,
                                                    0.0f, 1.0f, 0.0f,
                                                    0.0f, 1.0f, 0.0f,
                                                    0.0f, 1.0f, 0.0f,
                                                    0.0f, -1.0f, 0.0f,
                                                    0.0f, -1.0f, 0.0f,
                                                    0.0f, -1.0f, 0.0f,
                                                    0.0f, -1.0f, 0.0f};

Box::Box()
{
        memcpy(this->indices, indexs, sizeof(unsigned short) * 12 * 3);

        float vertexs[] = {-0.5, -0.5, -0.5, //Front Vertcices
                                            0.5, -0.5, -0.5,
                                            0.5, 0.5, -0.5,
                                           -0.5, 0.5, -0.5,

                                           -0.5, -0.5, 0.5, //Back Vertcices
                                            0.5, -0.5, 0.5,
                                            0.5, 0.5, 0.5,
                                           -0.5, 0.5, 0.5};

        memcpy(vertices, vertexs, sizeof(float) * (sizeof(vertexs) / sizeof(*vertexs)));
        memcpy(texCoords, coords, sizeof(float) * (sizeof(coords) / sizeof(*coords)));
        memcpy(this->normals, normals, sizeof(normals));
}

Box::Box(Vec4<float> min, Vec4<float> max)
{
        memcpy(this->indices, indexs, sizeof(unsigned short) * 12 * 3);

        vertices[0] = min[x]; //Front Vertex 0
        vertices[1] = min[y];
        vertices[2] = min[z];

        vertices[3] = max[x]; //Front Vertex 1
        vertices[4] = min[y];
        vertices[5] = min[z];

        vertices[6] = max[x]; //Front Vertex 2
        vertices[7] = max[y];
        vertices[8] = min[z];

        vertices[9] = min[x]; //Front Vertex 3
        vertices[10] = max[y];
        vertices[11] = min[z];

        vertices[12] = min[x]; //Back Vertex 4
        vertices[13] = min[y];
        vertices[14] = max[z];

        vertices[15] = max[x]; //Back Vertex 5
        vertices[16] = min[y];
        vertices[17] = max[z];

        vertices[18] = max[x]; //Back Vertex 6
        vertices[19] = max[y];
        vertices[20] = max[z];

        vertices[21] = min[x];
        vertices[22] = max[y];
        vertices[23] = max[z];

        memcpy(texCoords, coords, sizeof(coords));
        memcpy(this->normals, normals, sizeof(normals));
}

float *Box::Vertices()
{
        return vertices;
}

size_t Box::vsize()
{
        return sizeof(vertices);
}

int Box::vcount()
{
        return vertexCount;
}

size_t Box::isize()
{
        return sizeof(indices);
}

unsigned short *Box::getIndices()
{
        return indices;
}

void Box::print()
{
        for(int i = 0; i < 8; i++)
        {
                for(int j = 0; j < 3; j++)
                {
                        std::cout << vertices[j * vertexCount + i] << " ";
                }
                std::cout << std::endl;
        }
}

size_t Box::tsize()
{
        return sizeof(texCoords);
}

float *Box::getTextureCoords()
{
        return texCoords;
}

float *Box::getNormals()
{
    return normals;
}

size_t Box::nsize()
{
    return sizeof(normals);
}

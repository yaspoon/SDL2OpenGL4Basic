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

        std::cout << "This is fucked" << std::endl;
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

        vertices[0][v1][x] = min[x]; //Front Tri 0
        vertices[0][v1][y] = min[y];
        vertices[0][v1][z] = min[z];

        vertices[0][v2][x] = max[x];
        vertices[0][v2][y] = min[y];
        vertices[0][v2][z] = min[z];

        vertices[0][v3][x] = max[x];
        vertices[0][v3][y] = max[y];
        vertices[0][v3][z] = min[z];

        vertices[1][v1][x] = max[x]; //Front Tri 1
        vertices[1][v1][y] = max[y];
        vertices[1][v1][z] = min[z];

        vertices[1][v2][x] = min[x];
        vertices[1][v2][y] = max[y];
        vertices[1][v2][z] = min[z];

        vertices[1][v3][x] = min[x];
        vertices[1][v3][y] = min[y];
        vertices[1][v3][z] = min[z];

        vertices[2][v1][x] = max[x]; //Back Tri 0
        vertices[2][v1][y] = min[y];
        vertices[2][v1][z] = max[z];

        vertices[2][v2][x] = min[x];
        vertices[2][v2][y] = min[y];
        vertices[2][v2][z] = max[z];

        vertices[2][v3][x] = max[x];
        vertices[2][v3][y] = max[y];
        vertices[2][v3][z] = max[z];

        vertices[3][v1][x] = min[x]; //Back Tri 1
        vertices[3][v1][y] = min[y];
        vertices[3][v1][z] = max[z];

        vertices[3][v2][x] = min[x];
        vertices[3][v2][y] = max[y];
        vertices[3][v2][z] = max[z];

        vertices[3][v3][x] = max[x];
        vertices[3][v3][y] = max[y];
        vertices[3][v3][z] = max[z];

        vertices[4][v1][x] = max[x]; //Right Tri 0
        vertices[4][v1][y] = min[y];
        vertices[4][v1][z] = min[z];

        vertices[4][v2][x] = max[x];
        vertices[4][v2][y] = min[y];
        vertices[4][v2][z] = max[z];

        vertices[4][v3][x] = max[x];
        vertices[4][v3][y] = max[y];
        vertices[4][v3][z] = max[z];

        vertices[5][v1][x] = max[x]; //Right Tri 1
        vertices[5][v1][y] = min[y];
        vertices[5][v1][z] = min[z];

        vertices[5][v2][x] = max[x];
        vertices[5][v2][y] = max[y];
        vertices[5][v2][z] = max[z];

        vertices[5][v3][x] = max[x];
        vertices[5][v3][y] = max[y];
        vertices[5][v3][z] = min[z];

        vertices[6][v1][x] = min[x]; //Left Tri 0
        vertices[6][v1][y] = min[y];
        vertices[6][v1][z] = min[z];

        vertices[6][v2][x] = min[x];
        vertices[6][v2][y] = max[y];
        vertices[6][v2][z] = max[z];

        vertices[6][v3][x] = min[x];
        vertices[6][v3][y] = min[y];
        vertices[6][v3][z] = max[z];

        vertices[7][v1][x] = min[x]; //Left Tri 1
        vertices[7][v1][y] = min[y];
        vertices[7][v1][z] = min[z];

        vertices[7][v2][x] = min[x];
        vertices[7][v2][y] = max[y];
        vertices[7][v2][z] = min[z];

        vertices[7][v3][x] = min[x];
        vertices[7][v3][y] = max[y];
        vertices[7][v3][z] = max[z];

        vertices[8][v1][x] = min[x]; //Top Tri 0
        vertices[8][v1][y] = max[y];
        vertices[8][v1][z] = min[z];

        vertices[8][v2][x] = max[x];
        vertices[8][v2][y] = max[y];
        vertices[8][v2][z] = min[z];

        vertices[8][v3][x] = max[x];
        vertices[8][v3][y] = max[y];
        vertices[8][v3][z] = max[z];

        vertices[9][v1][x] = min[x]; //Top Tri 1
        vertices[9][v1][y] = max[y];
        vertices[9][v1][z] = min[z];

        vertices[9][v2][x] = max[x];
        vertices[9][v2][y] = max[y];
        vertices[9][v2][z] = max[z];

        vertices[9][v3][x] = min[x];
        vertices[9][v3][y] = max[y];
        vertices[9][v3][z] = max[z];

        vertices[10][v1][x] = min[x]; //Bottom Tri 0
        vertices[10][v1][y] = min[y];
        vertices[10][v1][z] = min[z];

        vertices[10][v2][x] = max[x];
        vertices[10][v2][y] = min[y];
        vertices[10][v2][z] = max[z];

        vertices[10][v3][x] = max[x];
        vertices[10][v3][y] = min[y];
        vertices[10][v3][z] = min[z];

        vertices[11][v1][x] = min[x]; //Bottom Tri 1
        vertices[11][v1][y] = min[y];
        vertices[11][v1][z] = min[z];

        vertices[11][v2][x] = min[x];
        vertices[11][v2][y] = min[y];
        vertices[11][v2][z] = max[z];

        vertices[11][v3][x] = max[x];
        vertices[11][v3][y] = min[y];
        vertices[11][v3][z] = max[z];

        colours[0][v1][r] = 1.0f; //Front Tri 1
        colours[0][v1][g] = 0.0f;
        colours[0][v1][b] = 0.0f;

        colours[0][v2][r] = 0.0f;
        colours[0][v2][g] = 1.0f;
        colours[0][v2][b] = 0.0f;

        colours[0][v3][r] = 0.0f;
        colours[0][v3][g] = 0.0f;
        colours[0][v3][b] = 1.0f;

        colours[1][v1][r] = 0.0f; //Front Tri 2
        colours[1][v1][g] = 0.0f;
        colours[1][v1][b] = 1.0f;

        colours[1][v2][r] = 0.0f;
        colours[1][v2][g] = 1.0f;
        colours[1][v2][b] = 0.0f;

        colours[1][v3][r] = 1.0f;
        colours[1][v3][g] = 0.0f;
        colours[1][v3][b] = 0.0f;

        colours[2][v1][r] = 0.0f; //Back Tri 1
        colours[2][v1][g] = 0.0f;
        colours[2][v1][b] = 1.0f;

        colours[2][v2][r] = 0.0f;
        colours[2][v2][g] = 1.0f;
        colours[2][v2][b] = 0.0f;

        colours[2][v3][r] = 1.0f;
        colours[2][v3][g] = 0.0f;
        colours[2][v3][b] = 0.0f;

        colours[3][v1][r] = 0.0f; //Back Tri 2
        colours[3][v1][g] = 1.0f;
        colours[3][v1][b] = 0.0f;

        colours[3][v2][r] = 0.0f;
        colours[3][v2][g] = 0.0f;
        colours[3][v2][b] = 1.0f;

        colours[3][v3][r] = 1.0f;
        colours[3][v3][g] = 0.0f;
        colours[3][v3][b] = 0.0f;

        colours[4][v1][r] = 0.0f; //Right Tri 1
        colours[4][v1][g] = 1.0f;
        colours[4][v1][b] = 0.0f;

        colours[4][v2][r] = 0.0f;
        colours[4][v2][g] = 0.0f;
        colours[4][v2][b] = 1.0f;

        colours[4][v3][r] = 1.0f;
        colours[4][v3][g] = 0.0f;
        colours[4][v3][b] = 0.0f;

        colours[5][v1][r] = 0.0f; //Right Tri 2
        colours[5][v1][g] = 1.0f;
        colours[5][v1][b] = 0.0f;

        colours[5][v2][r] = 1.0f;
        colours[5][v2][g] = 0.0f;
        colours[5][v2][b] = 0.0f;

        colours[5][v3][r] = 0.0f;
        colours[5][v3][g] = 0.0f;
        colours[5][v3][b] = 1.0f;

        colours[6][v1][r] = 1.0f; //Left Tri 1
        colours[6][v1][g] = 0.0f;
        colours[6][v1][b] = 0.0f;

        colours[6][v2][r] = 0.0f;
        colours[6][v2][g] = 0.0f;
        colours[6][v2][b] = 1.0f;

        colours[6][v3][r] = 0.0f;
        colours[6][v3][g] = 1.0f;
        colours[6][v3][b] = 0.0f;

        colours[7][v1][r] = 1.0f; //Left Tri 2
        colours[7][v1][g] = 0.0f;
        colours[7][v1][b] = 0.0f;

        colours[7][v2][r] = 0.0f;
        colours[7][v2][g] = 1.0f;
        colours[7][v2][b] = 0.0f;

        colours[7][v3][r] = 0.0f;
        colours[7][v3][g] = 0.0f;
        colours[7][v3][b] = 1.0f;

        colours[8][v1][r] = 0.0f; //Top Tri 1
        colours[8][v1][g] = 1.0f;
        colours[8][v1][b] = 0.0f;

        colours[8][v2][r] = 0.0f;
        colours[8][v2][g] = 0.0f;
        colours[8][v2][b] = 1.0f;

        colours[8][v3][r] = 1.0f;
        colours[8][v3][g] = 0.0f;
        colours[8][v3][b] = 0.0f;

        colours[9][v1][r] = 0.0f; //Top Tri 2
        colours[9][v1][g] = 1.0f;
        colours[9][v1][b] = 0.0f;

        colours[9][v2][r] = 1.0f;
        colours[9][v2][g] = 0.0f;
        colours[9][v2][b] = 0.0f;

        colours[9][v3][r] = 0.0f;
        colours[9][v3][g] = 0.0f;
        colours[9][v3][b] = 1.0f;

        colours[10][v1][r] = 1.0f; //Bottom Tri 1
        colours[10][v1][g] = 0.0f;
        colours[10][v1][b] = 0.0f;

        colours[10][v2][r] = 0.0f;
        colours[10][v2][g] = 0.0f;
        colours[10][v2][b] = 1.0f;

        colours[10][v3][r] = 0.0f;
        colours[10][v3][g] = 1.0f;
        colours[10][v3][b] = 0.0f;

        colours[11][v1][r] = 1.0f; //Bottom Tri 2
        colours[11][v1][g] = 0.0f;
        colours[11][v1][b] = 0.0f;

        colours[11][v2][r] = 0.0f;
        colours[11][v2][g] = 1.0f;
        colours[11][v2][b] = 0.0f;

        colours[11][v3][r] = 0.0f;
        colours[11][v3][g] = 0.0f;
        colours[11][v3][b] = 1.0f;


        normals[0][v1][x] = 0; //Front Tri 0
        normals[0][v1][y] = 0;
        normals[0][v1][z] = -1;

        normals[0][v2][x] = 0;
        normals[0][v2][y] = 0;
        normals[0][v2][z] = -1;

        normals[0][v3][x] = 0;
        normals[0][v3][y] = 0;
        normals[0][v3][z] = -1;

        normals[1][v1][x] = 0; //Front Tri 1
        normals[1][v1][y] = 0;
        normals[1][v1][z] = -1;

        normals[1][v2][x] = 0;
        normals[1][v2][y] = 0;
        normals[1][v2][z] = -1;

        normals[1][v3][x] = 0;
        normals[1][v3][y] = 0;
        normals[1][v3][z] = -1;

        normals[2][v1][x] = 0; //Back Tri 0
        normals[2][v1][y] = 0;
        normals[2][v1][z] = 1;

        normals[2][v2][x] = 0;
        normals[2][v2][y] = 0;
        normals[2][v2][z] = 1;

        normals[2][v3][x] = 0;
        normals[2][v3][y] = 0;
        normals[2][v3][z] = 1;

        normals[3][v1][x] = 0; //Back Tri 1
        normals[3][v1][y] = 0;
        normals[3][v1][z] = 1;

        normals[3][v2][x] = 0;
        normals[3][v2][y] = 0;
        normals[3][v2][z] = 1;

        normals[3][v3][x] = 0;
        normals[3][v3][y] = 0;
        normals[3][v3][z] = 1;

        normals[4][v1][x] = 1; //Right Tri 0
        normals[4][v1][y] = 0;
        normals[4][v1][z] = 0;

        normals[4][v2][x] = 1;
        normals[4][v2][y] = 0;
        normals[4][v2][z] = 0;

        normals[4][v3][x] = 1;
        normals[4][v3][y] = 0;
        normals[4][v3][z] = 0;

        normals[5][v1][x] = 1; //Right Tri 1
        normals[5][v1][y] = 0;
        normals[5][v1][z] = 0;

        normals[5][v2][x] = 1;
        normals[5][v2][y] = 0;
        normals[5][v2][z] = 0;

        normals[5][v3][x] = 1;
        normals[5][v3][y] = 0;
        normals[5][v3][z] = 0;

        normals[6][v1][x] = -1; //Left Tri 0
        normals[6][v1][y] = 0;
        normals[6][v1][z] = 0;

        normals[6][v2][x] = -1;
        normals[6][v2][y] = 0;
        normals[6][v2][z] = 0;

        normals[6][v3][x] = -1;
        normals[6][v3][y] = 0;
        normals[6][v3][z] = 0;

        normals[7][v1][x] = -1; //left Tri 1
        normals[7][v1][y] = 0;
        normals[7][v1][z] = 0;

        normals[7][v2][x] = -1;
        normals[7][v2][y] = 0;
        normals[7][v2][z] = 0;

        normals[7][v3][x] = -1;
        normals[7][v3][y] = 0;
        normals[7][v3][z] = 0;

        normals[8][v1][x] = 0; //Top Tri 1
        normals[8][v1][y] = 1;
        normals[8][v1][z] = 0;

        normals[8][v2][x] = 0;
        normals[8][v2][y] = 1;
        normals[8][v2][z] = 0;

        normals[8][v3][x] = 0;
        normals[8][v3][y] = 1;
        normals[8][v3][z] = 0;

        normals[9][v1][x] = 0; //Top Tri 2
        normals[9][v1][y] = 1;
        normals[9][v1][z] = 0;

        normals[9][v2][x] = 0;
        normals[9][v2][y] = 1;
        normals[9][v2][z] = 0;

        normals[9][v3][x] = 0;
        normals[9][v3][y] = 1;
        normals[9][v3][z] = 0;

        normals[10][v1][x] = 0; //Bottom Tri 1
        normals[10][v1][y] = -1;
        normals[10][v1][z] = 0;

        normals[10][v2][x] = 0;
        normals[10][v2][y] = -1;
        normals[10][v2][z] = 0;

        normals[10][v3][x] = 0;
        normals[10][v3][y] = -1;
        normals[10][v3][z] = 0;

        normals[11][v1][x] = 0; //Bottom Tri 2
        normals[11][v1][y] = -1;
        normals[11][v1][z] = 0;

        normals[11][v2][x] = 0;
        normals[11][v2][y] = -1;
        normals[11][v2][z] = 0;

        normals[11][v3][x] = 0;
        normals[11][v3][y] = -1;
        normals[11][v3][z] = 0;
}

float *Box::Vertices()
{
        return (float*)vertices;
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

float *Box::Colours()
{
        return (float*)colours;
}

size_t Box::csize()
{
        return sizeof(colours);
}

float *Box::Normals()
{
        return (float*)normals;
}

size_t Box::nsize()
{
        return sizeof(normals);
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
    return (float*)normals;
}

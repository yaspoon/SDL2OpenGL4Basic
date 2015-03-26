#include "Face.h"

Face::Face()
{
        //ctor
}

Face::~Face()
{
        //dtor
}

void Face::addVertex(int vertex)
{
        vertices.push_back(vertex);
}

void Face::addNormal(int normal)
{
        normals.push_back(normal);
}

void Face::addTexCoord(int texCoord)
{
        texCoords.push_back(texCoord);
}

int Face::getVertex(int index)
{
        return vertices[index];
}

int Face::getNormal(int index)
{
        return normals[index];
}

std::vector<int>::iterator Face::beginNormals()
{
        return normals.begin();
}

std::vector<int>::iterator Face::endNormals()
{
        return normals.end();
}

int Face::getTexCoord(int index)
{
 return texCoords[index];
}

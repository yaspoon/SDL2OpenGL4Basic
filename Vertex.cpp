#include "Vertex.h"
#include <cmath>

Vertex::Vertex()
{
        xyz.first = 0.0f;
        xyz.second = 0.0f;
        xyz.third = 0.0f;

        normals.push_back(triplet<float, float, float>(0.0f, 0.0f, 0.0f));
}

Vertex::Vertex(float x, float y, float z)
{
        xyz.first = x;
        xyz.second = y;
        xyz.third = z;
}

Vertex::~Vertex()
{
        //dtor
}

void Vertex::addNormal(float x, float y, float z)
{
        bool found = false;
        for(std::vector<triplet<float, float, float> >::iterator it = normals.begin(); it != normals.end(); ++it)
        {
                triplet<float, float, float> normal = *it;
                if(normal.first == x && normal.second == y && normal.third == z)
                {
                        found = true;
                }
        }

        if(!found)
        {
                normals.push_back(triplet<float, float, float>(x, y, z));
        }
}

void Vertex::setXyz(float x, float y, float z)
{
        xyz.first = 0.0f;
        xyz.second = 0.0f;
        xyz.third = 0.0f;
}

triplet<float, float, float> Vertex::getXyz()
{
        return xyz;
}

triplet<float, float, float> Vertex::getNormal()
{
        triplet<float, float, float> normal;

        for(std::vector<triplet<float, float, float> >::iterator it = normals.begin(); it != normals.end(); ++it)
        {
                triplet<float, float, float> tmpNormal = *it;
                normal.first += tmpNormal.first;
                normal.second += tmpNormal.second;
                normal.third += tmpNormal.third;
        }

        float length = sqrt((normal.first * normal. first) + (normal.second * normal.second) + (normal.third * normal.third));

        normal.first /= length;
        normal.second /= length;
        normal.third /= length;

        return normal;
}

#include "Renderable.h"

Renderable::Renderable()
{
        //ctor
}

Renderable::Renderable(GLuint vao, int triangleCount)
{
        this->vao = vao;
        this->triangleCount = triangleCount;
}

Renderable::Renderable(GLuint vao, std::vector<int> mats, int triangleCount)
{
        this->vao = vao;
        this->materials = mats;
        this->triangleCount = triangleCount;

}

Renderable::~Renderable()
{
        //dtor
}

GLuint Renderable::getVao()
{
    return vao;
}

void Renderable::addMaterial(int index)
{
    materials.push_back(index);
}

/*
Return all the mats so we can turn them on
and off
*/
std::vector<int> Renderable::getMaterials()
{
    return materials;
}

void Renderable::addTexture(GLuint texture)
{
    textures.push_back(texture);
}

std::vector<GLuint> Renderable::getTextures()
{
    return textures;
}

int Renderable::getTriangleCount()
{
    return triangleCount;
}


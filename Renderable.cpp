#include "Renderable.h"

Renderable::Renderable()
{
        //ctor
}

Renderable::Renderable(GLuint vao, int triangleCount)
	:modelMatrix(1.0f)
{
        this->vao = vao;
        this->triangleCount = triangleCount;
}

Renderable::Renderable(GLuint vao, std::vector<Material> mats, int triangleCount)
	:modelMatrix(1.0f)
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

void Renderable::addMaterial(Material mat)
{
    materials.push_back(mat);
}

/*
Return all the mats so we can turn them on
and off
*/
std::vector<Material> Renderable::getMaterials()
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

Mat4<float> Renderable::getModelMatrix()
{
	return modelMatrix;
}

void Renderable::setModelMatrix(Mat4<float> newModelMatrix)
{
	modelMatrix = newModelMatrix;
}

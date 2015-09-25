#include "Renderable.h"

Renderable::Renderable()
{
        //ctor
}

Renderable::Renderable(GLuint vao, std::vector<ObjMaterial> mats)
{
        this->vao = vao;

}

Renderable::~Renderable()
{
        //dtor
}

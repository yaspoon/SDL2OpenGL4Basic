#include "UBOUniform.h"

UBOUniform::UBOUniform()
{
        index = 0;
        size = 0;
        offset = 0;
        type = 0;
}

UBOUniform::UBOUniform(GLuint index, GLint size, GLint offset, GLint type)
{
        this->index = index;
        this->size = size;
        this->offset = offset;
        this->type = type;
}

UBOUniform::~UBOUniform()
{
        //dtor
}

GLuint UBOUniform::getIndex()
{
        return index;
}

GLint UBOUniform::getSize()
{
        return size;
}

GLint UBOUniform::getOffset()
{
        return offset;
}

GLint UBOUniform::getType()
{
        return type;
}

#include "Material.h"
#include "Light.h"
#include <cstring>

Material::Material(int index, size_t bufSize, int numElements, const char *uniformNames[], GLuint *indices, GLint *sizes, GLint *offsets, GLint *types)
{
        this->index = index;
        this->dataSize = bufSize;
        this->numUniforms = numElements;
        this->materialData = new char[this->dataSize];
        memset(this->materialData, 0, dataSize);

        for(int i = 0; i < numElements; i++)
        {
                std::string stringUniform(uniformNames[i]);
                size_t pos = stringUniform.find('.');
                pos++;
                std::string uniformName = stringUniform.substr(pos);
                UBOUniform uniformData(indices[i], sizes[i], offsets[i], types[i]);
                uniforms[uniformName] = uniformData;
        }
}

Material::Material(const Material &copy)
{
        this->index = copy.index;
        this->dataSize = copy.dataSize;
        this->numUniforms = copy.numUniforms;
        this->materialData = new char[dataSize];
        memset(materialData, 0, dataSize);
        memcpy(materialData, copy.materialData, dataSize);

        for(std::map<std::string, UBOUniform >::const_iterator it = copy.uniforms.begin(); it != copy.uniforms.end(); ++it)
        {
                uniforms[it->first] = it->second;
        }
}

Material::~Material()
{
        delete[] materialData;
}

Material &Material::operator=(const Material &copy)
{
        this->index = copy.index;
        this->dataSize = copy.dataSize;
        this->numUniforms = copy.numUniforms;
        this->materialData = new char[dataSize];
        memset(materialData, 0, dataSize);
        memcpy(materialData, copy.materialData, dataSize);

        for(std::map<std::string, UBOUniform >::const_iterator it = copy.uniforms.begin(); it != copy.uniforms.end(); ++it)
        {
                uniforms[it->first] = it->second;
        }

        return *this;
}

void Material::setEmission(Vec4<GLfloat> emission)
{
        UBOUniform emUbo = uniforms[std::string("emission")];

        size_t size = emUbo.getSize() * typeSize(emUbo.getType());
        memcpy(materialData, emission.getData(), size);
}

void Material::setAmbient(Vec4<GLfloat> ambient)
{
        UBOUniform amUbo = uniforms[std::string("ambient")];

        size_t size = amUbo.getSize() * typeSize(amUbo.getType());
        memcpy(materialData, ambient.getData(), size);
}

void Material::setDiffuse(Vec4<GLfloat> diffuse)
{
        UBOUniform diffUbo = uniforms[std::string("diffuse")];

        size_t size = diffUbo.getSize() * typeSize(diffUbo.getType());
        memcpy(materialData, diffuse.getData(), size);
}

void Material::setSpecular(Vec4<GLfloat> specular)
{
        UBOUniform specUbo = uniforms[std::string("specular")];

        size_t size = specUbo.getSize() * typeSize(specUbo.getType());
        memcpy(materialData, specular.getData(), size);
}

void Material::setShininess(float shininess)
{
        UBOUniform shinUbo = uniforms[std::string("shininess")];

        size_t size = shinUbo.getSize() * typeSize(shinUbo.getType());
        memcpy(materialData, &shininess, size);
}

Vec4<GLfloat> Material::getEmission()
{

}

Vec4<GLfloat> Material::getDiffuse()
{

}

Vec4<GLfloat> Material::getSpecular()
{

}

char *Material::getMaterialData()
{
        return materialData;
}

int Material::getIndex()
{
        return index;
}

size_t Material::getDataSize()
{
        return dataSize;
}

int Material::getNumUniforms()
{
        return numUniforms;
}

float Material::getShininess()
{

}

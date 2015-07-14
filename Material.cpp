#include "Material.h"
#include <cstring>

Material::Material(int index, size_t bufSize, int numElements, char *uniformNames[], GLuint *indices, GLint *sizes, GLint *offsets, GLint *types)
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

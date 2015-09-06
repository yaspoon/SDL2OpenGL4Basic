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
        UBOUniform uniform = uniforms["emission"];
        float emissionArray[4];
        memset(emissionArray, 0, sizeof(emissionArray));
        size_t size = uniform.getSize() * typeSize(uniform.getType());
        memcpy(emissionArray, materialData, size);

        Vec4<GLfloat> emission(emissionArray[0], emissionArray[1], emissionArray[2], emissionArray[3]);

        return emission;
}

Vec4<GLfloat> Material::getDiffuse()
{
        UBOUniform uniform = uniforms["diffuse"];
        float diffuseArray[4];
        memset(diffuseArray, 0 , sizeof(diffuseArray));
        size_t size = uniform.getSize() * typeSize(uniform.getType());

        memcpy(diffuseArray, materialData, size);

        Vec4<GLfloat> diffuse(diffuseArray[0], diffuseArray[1], diffuseArray[2], diffuseArray[3]);

        return diffuse;
}

Vec4<GLfloat> Material::getSpecular()
{
        UBOUniform uniform = uniforms["specular"];
        float specularArray[4];
        memset(specularArray, 0, sizeof(specularArray));
        size_t size = uniform.getSize() * typeSize(uniform.getType());

        memcpy(specularArray, materialData, size);

        Vec4<GLfloat> specular(specularArray[0], specularArray[1], specularArray[2], specular[3]);

        return specular;
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
        float shinness = 0;
        UBOUniform uniform = uniforms["shinness"];
        size_t size = uniform.getSize() * typeSize(uniform.getType());
        memcpy(&shinness, materialData + uniform.getOffset(), size);

        return shinness;
}

void Material::loadObjMaterial(ObjMaterial newMat)
{
        setAmbient(newMat.getAmbient());
        setDiffuse(newMat.getDiffuse());
        setSpecular(newMat.getSpecular());
        setShininess(16.0f);
        setEmission(1.0f, 1.0f, 1.0f, 0.0f);
}

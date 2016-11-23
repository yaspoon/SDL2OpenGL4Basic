#ifndef MATERIAL_H
#define MATERIAL_H

#include "UBOUniform.h"
#include "ModelMaterial.h"
#include "Vec4.h"

#include <SDL2/SDL_opengl.h>
#include <map>
#include <string>

class Material
{
private:
        size_t dataSize;
        int numUniforms;
        char *materialData;
        int index;
        //GLuint matUboStride;
        std::map<std::string, UBOUniform > uniforms;
        Vec4<GLfloat> colour;
        GLboolean hasTextures;
        GLboolean enabled;

public:
        Material(int index, size_t bufSize, int numElements, const char *uniformNames[], GLuint *indices, GLint *sizes, GLint *offsets, GLint *types, GLuint matUboOffset);
        Material(const Material &copy);
        ~Material();

        Material &operator=(const Material &copy);
        Material &operator=(Material &copy);
        //Material operator=(const Material &copy);

        void setEmission(Vec4<GLfloat> emission);
        void setAmbient(Vec4<GLfloat> ambient);
        void setDiffuse(Vec4<GLfloat> diffuse);
        void setSpecular(Vec4<GLfloat> specular);
        void setShininess(GLfloat shininess);
        void setColour(Vec4<GLfloat> colour);
        void setHasTextures(GLboolean hasTextures);
        void setEnabled(GLboolean enabled);

        Vec4<GLfloat> getEmission();
        Vec4<GLfloat> getAmbient();
        Vec4<GLfloat> getDiffuse();
        Vec4<GLfloat> getSpecular();
        GLfloat getShininess();

        char *getMaterialData();
        int getIndex();
        size_t getDataSize();
        int getNumUniforms();

        void loadModelMaterial(ModelMaterial *newMat);
};

#endif // MATERIAL_H

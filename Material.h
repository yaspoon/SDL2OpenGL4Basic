#ifndef MATERIAL_H
#define MATERIAL_H

#include "UBOUniform.h"
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
        std::map<std::string, UBOUniform > uniforms;

public:
        Material(int index, size_t bufSize, int numElements, char *uniformNames[], GLuint *indices, GLint *sizes, GLint *offsets, GLint *types);
        Material(const Material &copy);
        ~Material();

        Material &operator=(const Material &copy);

        void setEmission(Vec4<GLfloat> emission);
        void setAmbient(Vec4<GLfloat> ambient);
        void setDiffuse(Vec4<GLfloat> diffuse);
        void setSpecular(Vec4<GLfloat> specular);

        Vec4<GLfloat> getEmission();
        Vec4<GLfloat> getAmbient();
        Vec4<GLfloat> getDiffuse();
        Vec4<GLfloat> getSpecular();

        char *getMaterialData();
        int getIndex();
        size_t getDataSize();
        int getNumUniforms();
};

#endif // MATERIAL_H

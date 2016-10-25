#ifndef LIGHT_H
#define LIGHT_H

#include <SDL2/SDL_opengl.h>
#include "Vec4.h"
#include "UBOUniform.h"
#include <map>
#include <string>

size_t typeSize(GLenum type);

class Light
{
        public:
                Light(int index, GLint bufferSize, GLint numElements, const char *uniformNames[], GLuint *indices, GLint *sizes, GLint *offsets, GLint *types);
                Light(const Light &copy);
                ~Light();

                Light &operator=(const Light &other);

                void setIsEnabled(GLboolean enabled);
                void enableDiffuse(GLboolean enabled);
                void enableSpecular(GLboolean enabled);
                void setIsSpotLight(GLboolean enabled);
                void setIsPointLight(GLboolean enabled);
                void setSpecularMode(GLboolean specularMode);
                void setAmbientLight(Vec4<GLfloat> light);
                void setDiffuseLight(Vec4<GLfloat> light);
                void setSpecularLight(Vec4<GLfloat> light);
                void setStrength(GLfloat strength);
                void setPosition(Vec4<GLfloat> pos);
                void setNormal(Vec4<GLfloat> normal);
                void setAngle(GLfloat angle);
                void setLinearAtten(GLfloat linearAtten);
                void setQuadAtten(GLfloat quagAtten);
                void setConstAttenuation(GLfloat constAtten);
                void setSpotponent(GLfloat spotponent);
                char *getLightData();

                GLboolean getIsEnabled();
                GLboolean getIsDiffuseEnabled();
                GLboolean getIsSpecularEnabled();
                GLboolean getIsSpotLight();
                GLboolean getIsPointLight();
                GLboolean getSpecularMode();
                Vec4<GLfloat> getAmbientLight();
                Vec4<GLfloat> getDiffuseLight();
                Vec4<GLfloat> getSpecularLight();
                GLfloat getStrength();
                GLfloat getLinearAtten();
                GLfloat getQuadAtten();
                GLfloat getConstAttenuation();
                GLfloat getSpotponent();
                int getIndex();
                size_t getDataSize();

                template <class T>
                void set(std::string uboname, T *data)
                {
                        UBOUniform ubo = uniforms[uboname];
                        size_t size = ubo.getSize() * typeSize(ubo.getType());
                        memcpy(lightData + ubo.getOffset(), data, size);
                }

                template <class T> T get(std::string uboname)
                {
                        T data;
                        UBOUniform ubo = uniforms[uboname];
                        size_t size = ubo.getSize() * typeSize(ubo.getType());
                        memcpy(&data, lightData + ubo.getOffset(), size);

                        return data;
                }

        protected:
        private:
        size_t dataSize;
        char *lightData;
        std::map<std::string, UBOUniform> uniforms;
        int index;
};

#endif // LIGHT_H

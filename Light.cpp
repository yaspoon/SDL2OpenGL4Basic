#include "Light.h"
#include <cstring>
#include "Math.h"

struct LightProperties
{
	bool isEnabled;
	bool pad1[3];
	bool enableDiffuse;
	bool pad2[3];
	bool enableSpecular;
	bool pad3[3];
	bool isSpotlight;
	bool pad4[3];
	bool isPointlight;
	bool pad5[3];
	bool specularMode;
	bool pad6[11];
	float ambientLight[3];
	bool pad7;
        float diffuseLight[3];
        bool pad8;
	float specularLight[3];
	bool pad9;
	float shininess;
	float strength;

	float position[3]; //For directional lights this is instead the direction the light points, yup terrible naming I know.
	bool pad10;
	float normal[3]; //Direction spotlights are pointing
	bool pad11;
	float angle; //How many degrees left and right of the light normal does this light `shine` it's in cos(theta) form

	float linearAtten; //= 1.0;
	float quadAtten; // = 1.0;
	float constAtten; // = 0.1;
	float spotponent; // = 2.0;
};

Light::Light(int index, GLsizei offset, GLint bufferSize, GLint numElements, const char *uniformNames[numUniforms], GLuint *indices, GLint *sizes, GLint *offsets, GLint *types)
{
        this->index = index;
        this->dataSize = bufferSize;
        lightData = new char[bufferSize];
        memset(lightData, 0, bufferSize);

        for(int i = 0; i < numElements; i++)
        {
                std::string lightUniformName(uniformNames[i]);
                std::size_t pos = lightUniformName.find('.');
                pos++;
                std::string uniformName = lightUniformName.substr(pos);
                UBOUniform uniformData(indices[i], sizes[i], offsets[i] - offset, types[i]);
                uniforms[uniformName] = uniformData;
        }
}

Light::Light(const Light &copy)
{
        dataSize = copy.dataSize;
        lightData = new char[dataSize];

        memset(lightData, 0, dataSize);
        memcpy(lightData, copy.lightData, dataSize);

        for(std::map<std::string, UBOUniform>::const_iterator it = copy.uniforms.begin(); it != copy.uniforms.end(); it++)
        {
                uniforms[it->first] = it->second;
        }
}

Light::~Light()
{
        if(lightData)
        {
                delete[] lightData;
        }
}

Light &Light::operator=(Light &other)
{
        std::cout << "Light::operator= this isn't implemented BTW" << std::endl;
}

void Light::setIsEnabled(GLboolean enabled)
{
        UBOUniform isEnabled = uniforms[std::string("isEnabled")];

        memcpy(lightData + isEnabled.getOffset(), &enabled, sizeof(GLboolean));
}


void Light::enableDiffuse(GLboolean enabled)
{
        UBOUniform diffuse = uniforms[std::string("enableDiffuse")];

        size_t size = diffuse.getSize() * typeSize(diffuse.getType());
        memcpy(lightData + diffuse.getOffset(), &enabled, sizeof(enabled));
}

void Light::enableSpecular(GLboolean enabled)
{
        UBOUniform specular = uniforms[std::string("enableSpecular")];

        size_t size = specular.getSize() * typeSize(specular.getType());
        memcpy(lightData + specular.getOffset(), &enabled, sizeof(enabled));
}

void Light::setIsSpotLight(GLboolean enabled)
{
        UBOUniform spotLight = uniforms[std::string("isSpotlight")];
        memcpy(lightData + spotLight.getOffset(), &enabled, sizeof(enabled));
}

void Light::setIsPointLight(GLboolean enabled)
{
        UBOUniform pointLightUbo = uniforms[std::string("isPointlight")];

        memcpy(lightData + pointLightUbo.getOffset(), &enabled, sizeof(enabled));
}

void Light::setSpecularMode(GLboolean specularMode)
{
        UBOUniform specularModeUbo = uniforms[std::string("specularMode")];
        memcpy(lightData + specularModeUbo.getOffset(), &specularMode, sizeof(specularMode));
}

void Light::setAmbientLight(Vec4<GLfloat> light)
{
        UBOUniform ambientLight = uniforms[std::string("ambientLight")];

        size_t size =  ambientLight.getSize() * typeSize(ambientLight.getType());
        memcpy(lightData + ambientLight.getOffset(), light.getData(), size);
}

void Light::setDiffuseLight(Vec4<GLfloat> light)
{
        UBOUniform diffuseLight = uniforms[std::string("diffuseLight")];

        struct LightProperties* sss = (struct LightProperties*)0;
        int *asdf = (int*)&sss->diffuseLight;

        size_t size =  diffuseLight.getSize() * typeSize(diffuseLight.getType());
        struct LightProperties *blah = (struct LightProperties*)lightData;
        size_t test = sizeof(struct LightProperties);
        memcpy(lightData + diffuseLight.getOffset(), light.getData(), size);
}

void Light::setSpecularLight(Vec4<GLfloat> light)
{
        UBOUniform specularLight = uniforms[std::string("specularLight")];

        size_t size =  specularLight.getSize() * typeSize(specularLight.getType());
        memcpy(lightData + specularLight.getOffset(), light.getData(), size);
}

void Light::setShininess(GLfloat shininess)
{
        UBOUniform shininessUBO = uniforms[std::string("shininess")];

        memcpy(lightData + shininessUBO.getOffset(), &shininess, sizeof(GLfloat));
}

void Light::setStrength(GLfloat strength)
{
        UBOUniform strengthUbo = uniforms[std::string("strength")];
        memcpy(lightData + strengthUbo.getOffset(), &strength, sizeof(strength));
}

void Light::setPosition(Vec4<GLfloat> pos)
{
        UBOUniform position = uniforms[std::string("position")];

        size_t size = position.getSize() * typeSize(position.getType());
        memcpy(lightData + position.getOffset(), pos.getData(), size);
}

void Light::setNormal(Vec4<GLfloat> normal)
{
        UBOUniform normalUbo = uniforms[std::string("normal")];
        size_t size = normalUbo.getSize() * typeSize(normalUbo.getType());
        memcpy(lightData + normalUbo.getOffset(), normal.getData(), size);
}

void Light::setAngle(GLfloat angle)
{
        UBOUniform angleUbo = uniforms[std::string("angle")];
        memcpy(lightData + angleUbo.getOffset(), &angle, sizeof(angle));
}

void Light::setLinearAtten(GLfloat linearAtten)
{
        UBOUniform linearAttenUbo = uniforms[std::string("linearAtten")];
        memcpy(lightData + linearAttenUbo.getOffset(), &linearAtten, sizeof(linearAtten));
}

void Light::setQuadAtten(GLfloat quadAtten)
{
        UBOUniform quadAttenUbo = uniforms[std::string("quadAtten")];
        memcpy(lightData + quadAttenUbo.getOffset(), &quadAtten, sizeof(quadAtten));
}

void Light::setConstAttenuation(GLfloat constAtten)
{
        UBOUniform constAttenUbo = uniforms[std::string("constAtten")];

        memcpy(lightData + constAttenUbo.getOffset(), &constAtten, sizeof(constAtten));
}

void Light::setSpotponent(GLfloat spotponent)
{
        UBOUniform spotponentUbo = uniforms[std::string("spotponent")];
        memcpy(lightData + spotponentUbo.getOffset(), &spotponent, sizeof(spotponent));
}

char *Light::getLightData()
{
        return lightData;
}

size_t Light::typeSize(GLenum type)
{
        size_t size;

        #define CASE(Enum, Count,Type) case Enum: size = Count * sizeof(Type); break

        switch(type)
        {
                CASE(GL_FLOAT,                          1, GLfloat);
                CASE(GL_BOOL,                           1, GLboolean);
                CASE(GL_FLOAT_VEC3,               3, GLfloat);
        }

        return size;
}

GLboolean Light::getIsEnabled()
{
        GLboolean enabled;

        UBOUniform enabledUBO = uniforms[std::string("isEnabled")];
        memcpy(&enabled, lightData + enabledUBO.getOffset(), sizeof(enabled));

        return enabled;
}

GLboolean Light::getIsDiffuseEnabled()
{
        GLboolean diffuse;
        UBOUniform diffuseUbo = uniforms[std::string("enableDiffuse")];
        memcpy(&diffuse, lightData + diffuseUbo.getOffset(), sizeof(diffuse));

        return diffuse;
}

GLboolean Light::getIsSpecularEnabled()
{
        GLboolean specular;

        UBOUniform specularUBO = uniforms[std::string("enableSpecular")];
        memcpy(&specular, lightData + specularUBO.getOffset(), sizeof(GLboolean));
        return specular;
}

GLboolean Light::getIsSpotLight()
{
        GLboolean spotLight;
        UBOUniform spotLightUbo = uniforms[std::string("isSpotlight")];
        memcpy(&spotLight, lightData + spotLightUbo.getOffset(), sizeof(spotLight));
}

GLboolean Light::getIsPointLight()
{
        GLboolean pointLight;
        UBOUniform pointLightUbo = uniforms[std::string("isPointlight")];
        memcpy(&pointLight, lightData + pointLightUbo.getOffset(), sizeof(pointLight));

        return pointLight;
}

GLboolean Light::getSpecularMode()
{
        GLboolean specularMode;
        UBOUniform specularModeUbo = uniforms[std::string("specularMode")];
        memcpy(&specularMode, lightData + specularModeUbo.getOffset(), sizeof(specularMode));

        return specularMode;
}

Vec4<GLfloat> Light::getAmbientLight()
{
        GLfloat ambientLight[3];
        UBOUniform ambientLightUbo = uniforms[std::string("ambientLight")];
        size_t size = ambientLightUbo.getSize() * typeSize(ambientLightUbo.getType());
        memcpy(ambientLight, lightData + ambientLightUbo.getOffset(), size);

        return Vec4<GLfloat>(ambientLight[0], ambientLight[1], ambientLight[2], 0.0f);
}

Vec4<GLfloat> Light::getDiffuseLight()
{
        GLfloat diffuseLight[4];
        UBOUniform diffuseLightUbo = uniforms[std::string("diffuseLight")];
        size_t size = diffuseLightUbo.getSize() * typeSize(diffuseLightUbo.getType());
        memcpy(diffuseLight, lightData + diffuseLightUbo.getOffset(), size);

        return Vec4<GLfloat>(diffuseLight[0], diffuseLight[1], diffuseLight[2], 0.0f);
}

Vec4<GLfloat> Light::getSpecularLight()
{
        GLfloat specularLight[4];
        UBOUniform specularLightUbo = uniforms[std::string("specularLight")];
        size_t size = specularLightUbo.getSize() * typeSize(specularLightUbo.getType());
        memcpy(specularLight, lightData + specularLightUbo.getOffset(), size);

        return Vec4<GLfloat>(specularLight[0], specularLight[1], specularLight[2], 0.0f);
}

GLfloat Light::getShininess()
{
        GLfloat shininess;
        UBOUniform shininessUBO = uniforms[std::string("shininess")];
        memcpy(&shininess, lightData + shininessUBO.getOffset(), sizeof(shininess));

        return shininess;

}

GLfloat Light::getStrength()
{
        GLfloat strength;
        UBOUniform strengthUBO = uniforms[std::string("strength")];
        memcpy(&strength, lightData + strengthUBO.getOffset(), sizeof(strength));

        return strength;

}

GLfloat Light::getLinearAtten()
{
        GLfloat linearAtten;
        UBOUniform linearAttenUBO = uniforms[std::string("linearAtten")];
        memcpy(&linearAtten, lightData + linearAttenUBO.getOffset(), sizeof(linearAtten));

        return linearAtten;
}

GLfloat Light::getQuadAtten()
{
        GLfloat quadAtten;
        UBOUniform quadAttenUbo = uniforms[std::string("quadAtten")];
        memcpy(&quadAtten, lightData + quadAttenUbo.getOffset(), sizeof(quadAtten));

        return quadAtten;
}

GLfloat Light::getSpotponent()
{
        GLfloat spotponent;
        UBOUniform spotponentUbo = uniforms[std::string("spotponent")];
        memcpy(&spotponent, lightData + spotponentUbo.getOffset(), sizeof(spotponent));

        return spotponent;
}

GLfloat Light::getConstAttenuation()
{
        GLfloat constAtten;
        UBOUniform constAttenUbo = uniforms[std::string("constAtten")];
        memcpy(&constAtten, lightData + constAttenUbo.getOffset(), sizeof(constAtten));

        return constAtten;
}

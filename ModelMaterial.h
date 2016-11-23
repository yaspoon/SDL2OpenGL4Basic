#ifndef MODEL_MATERIAL
#define MODEL_MATERIAL

#include "Vec4.h"
#include <vector>
#include <string>

class ModelMaterial
{
private:
public:
	virtual ~ModelMaterial() {};
        virtual Vec4<float> getAmbient() = 0;
        virtual Vec4<float> getDiffuse() = 0;
        virtual Vec4<float> getSpecular() = 0;
	virtual bool hasTextures() = 0;
	virtual std::vector<std::string> getTextures() = 0; 
};
#endif

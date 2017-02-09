#ifndef PMDL_MATERIAL
#define PMDL_MATERIAL

#include "ModelMaterial.h"

class PMDLMaterial : public ModelMaterial
{
private:
	Vec4<float> ambient_colour;
	Vec4<float> diffuse_colour;
	Vec4<float> specular_colour;
	float specular_intensity;
	float specular_hardness;
	float roughness;
	std::vector<std::string> textures;
public:
	PMDLMaterial(Vec4<float> ambient_colour, Vec4<float> diffuse_colour, Vec4<float> specular_colour, float specular_intensity, float specular_hardnes, 
			float roughness, std::vector<std::string> textures);
	~PMDLMaterial();
        Vec4<float> getAmbient();
        Vec4<float> getDiffuse();
        Vec4<float> getSpecular();
	bool hasTextures();
	std::vector<std::string> getTextures();
};
#endif

#include "PMDLMaterial.h"

PMDLMaterial::PMDLMaterial(Vec4<float> ambient_colour, Vec4<float> diffuse_colour, Vec4<float> specular_colour, float specular_intensity, float specular_hardness,
			float roughness, std::vector<std::string> textures)
{
	this->ambient_colour = ambient_colour;
	this->diffuse_colour = diffuse_colour;
	this->specular_colour = specular_colour;
	this->specular_intensity = specular_intensity;
	this->specular_hardness = specular_hardness;
	this->roughness = roughness;
	this->textures.insert(this->textures.begin(), textures.begin(), textures.end());
}

PMDLMaterial::~PMDLMaterial()
{
}

Vec4<float> PMDLMaterial::getAmbient()
{
	return ambient_colour;
}

Vec4<float> PMDLMaterial::getDiffuse()
{
	return diffuse_colour;
}

Vec4<float> PMDLMaterial::getSpecular()
{
	return specular_colour;
}

bool PMDLMaterial::hasTextures()
{
	return textures.size() > 0;
}

std::vector<std::string> PMDLMaterial::getTextures()
{
	return textures;
}

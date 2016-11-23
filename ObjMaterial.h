#ifndef OBJ_MATERIAL_H
#define OBJ_MATERIAL_H

#include <string>
#include <vector>
#include "Vec4.h"
#include "ModelMaterial.h"

class ObjMaterial : public ModelMaterial
{
private:
        std::string name;
        float ns;
        float d;
        int illum;
        Vec4<float> ka;
        Vec4<float> kd;
        Vec4<float> ks;
	std::vector<std::string> map_kds;
        bool texture;

public:

        ObjMaterial(std::string filepath);
        ObjMaterial(const ObjMaterial &copy);
        ObjMaterial operator=(const ObjMaterial& copy);
	std::vector<std::string> getTextures();
        Vec4<float> getAmbient();
        Vec4<float> getDiffuse();
        Vec4<float> getSpecular();
        float getSpecularExponent();
        float getDisolved();
        int getIllumination();
        bool hasTextures();
};
#endif // OBJ_MATERIAL_H

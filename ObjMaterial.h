#ifndef OBJ_MATERIAL_H
#define OBJ_MATERIAL_H

#include <string>
#include <vector>

class ObjMaterial
{
private:
        std::string name;
        float ns;
        float d;
        int illum;
        std::vector<float> ka;
        std::vector<float> kd;
        std::vector<float> ks;
        std::string map_kd;

public:

        ObjMaterial(std::string filepath);
        std::string getMapkd();
        std::vector<float> getAmbient();
        std::vector<float> getDiffuse();
        std::vector<float> getSpecular();
        float getSpecularExponent();
        float getDisolved();
        int getIllumination();
};
#endif // OBJ_MATERIAL_H

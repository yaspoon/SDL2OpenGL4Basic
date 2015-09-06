#ifndef OBJ_MATERIAL_H
#define OBJ_MATERIAL_H

#include <string>
#include <vector>

class ObjMaterial
{
private:
        std::string name;
        int ns;
        int d;
        int illum;
        std::vector<float> ka;
        std::vector<float> kd;
        std::vector<float> ks;
        std::string map_kd;

public:

        ObjMaterial(std::string filepath);
        std::string getMapkd();
};
#endif // OBJ_MATERIAL_H

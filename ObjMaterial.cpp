#include "ObjMaterial.h"

#include <iostream>
#include <fstream>
#include <sstream>

ObjMaterial::ObjMaterial(std::string filepath)
{
        std::ifstream matFile;
        matFile.open(filepath.c_str(), std::ios::in);

        map_kd = std::string();
        texture = false;

        for(std::string line; std::getline(matFile, line); /*void*/)
        {
                std::stringstream ss(line);
                std::string identifier;

                ss >> identifier;
                if(identifier.compare("#") == 0)
                {
                        std::cout << "Found comment" << std::endl;
                }
                else if(identifier.compare("newmtl") == 0)
                {
                        ss >> name;
                        std::cout << "Found name:" << name << std::endl;
                }
                else if(identifier.compare("Ns") == 0)
                {
                        ss >> ns;
                        std::cout << "Ns:" << ns << std::endl;
                }
                else if(identifier.compare("Ka") == 0)
                {
                        std::vector<float> kaVec;
                        for(int i = 0; i < 3; i++)
                        {
                                float tmp;
                                ss >> tmp;
                                kaVec.push_back(tmp);
                        }
                        ka = Vec4<float>(kaVec[0], kaVec[1], kaVec[2], 1.0f);
                }
                else if(identifier.compare("Kd") == 0)
                {
                        std::vector<float> kdVec;
                        for(int i = 0; i < 3; i++)
                        {
                                float tmp;
                                ss >> tmp;
                                kdVec.push_back(tmp);
                        }
                        kd = Vec4<float>(kdVec[0], kdVec[1], kdVec[2], 1.0f);
                }
                else if(identifier.compare("Ks") == 0)
                {
                        std::vector<float> ksVec;
                        for(int i = 0; i < 3; i++)
                        {
                                float tmp;
                                ss >> tmp;
                                ksVec.push_back(tmp);
                        }
                        ks = Vec4<float>(ksVec[0], ksVec[1], ksVec[2], 1.0f);
                }
                else if(identifier.compare("d") == 0)
                {
                        ss >> d;
                }
                else if(identifier.compare("illum") == 0)
                {
                        ss >> illum;
                }
                else if(identifier.compare("map_Kd") == 0)
                {
                        ss >> map_kd;
                        texture = true;
                }
        }
}

ObjMaterial::ObjMaterial(const ObjMaterial &copy)
{
        this->name = copy.name;
        this->ns = copy.ns;
        this->d = copy.d;
        this->illum = copy.illum;
        this->ka = copy.ka;
        this->kd = copy.kd;
        this->ks = copy.ks;
        this->map_kd = copy.map_kd;
        this->texture = copy.texture;
}

ObjMaterial& ObjMaterial::operator=(const ObjMaterial &copy)
{
        ObjMaterial mat(copy);
        return mat;
}

std::string ObjMaterial::getMapkd()
{
        return map_kd;
}

Vec4<float> ObjMaterial::getAmbient()
{
        return ka;
}

Vec4<float> ObjMaterial::getDiffuse()
{
        return kd;
}

Vec4<float> ObjMaterial::getSpecular()
{
        return ks;
}

float ObjMaterial::getSpecularExponent()
{
        return ns;
}

float ObjMaterial::getDisolved()
{
        return d;
}

int ObjMaterial::getIllumination()
{
        return illum;
}

bool ObjMaterial::hasTexture()
{
    return texture;
}

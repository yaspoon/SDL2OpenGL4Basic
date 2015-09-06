#include "ObjMaterial.h"

#include <iostream>
#include <fstream>
#include <sstream>

ObjMaterial::ObjMaterial(std::string filepath)
{
        std::ifstream matFile;
        matFile.open(filepath.c_str(), std::ios::in);

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
                        for(int i = 0; i < 3; i++)
                        {
                                float tmp;
                                ss >> tmp;
                                ka.push_back(tmp);
                        }
                }
                else if(identifier.compare("Kd") == 0)
                {
                        for(int i = 0; i < 3; i++)
                        {
                                float tmp;
                                ss >> tmp;
                                kd.push_back(tmp);
                        }
                }
                else if(identifier.compare("Ks") == 0)
                {
                        for(int i = 0; i < 3; i++)
                        {
                                float tmp;
                                ss >> tmp;
                                ks.push_back(tmp);
                        }
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
                }
        }
}

std::string ObjMaterial::getMapkd()
{
        return map_kd;
}

std::vector<float> ObjMaterial::getAmbient()
{
        return ka;
}

std::vector<float> ObjMaterial::getDiffuse()
{
        return kd;
}

std::vector<float> ObjMaterial::getSpecular()
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

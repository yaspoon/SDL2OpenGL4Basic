#include "ObjLoader.h"

#include <iostream>
#include <fstream>

ObjLoader::ObjLoader(std::string filepath)
{
        std::ifstream file;
        file.open(filepath.c_str(), std::ios::in);


       for(std::string line; std::getline(file, line);)
       {
               std::stringstream ss(line);
               std::string identifier;

               ss >> identifier;

               if(identifier.compare("#") ==0)
               {
                       std::cout << "Found comment" << std::endl;
               }
               else if(identifier.compare("v") == 0)
               {
                        std::vector<float> vec3 = loadNumbers(ss);
                        vertices.push_back(Vertex(vec3[0], vec3[1], vec3[2]));
               }
               else if(identifier.compare("vn") == 0)
               {
                               std::vector<float> vec3 = loadNumbers(ss);
                                normals.push_back(triplet<float, float, float>(vec3[0], vec3[1], vec3[2]));
               }
               else if(identifier.compare("f") == 0)
               {
                       std::string face;

                       ss >> face;

                       Face newFace;
                       while(!ss.fail())
                       {
                               std::vector<std::pair<int, bool> > vec3 = loadFace(face);
                               newFace.addVertex(vec3[0].first);
                               if(vec3[1].second)
                               {
                                       newFace.addTexCoord(vec3[1].first);
                               }
                               if(vec3[2].second)
                               {
                                       newFace.addNormal(vec3[2].first);
                               }
                               ss >> face;
                       }

                       for(int i = 0; i < 3; i ++)
                       {
                               int vertexIdx = newFace.getVertex(i);
                               Vertex vert = vertices[vertexIdx];
                               triplet<float, float, float> normal = normals[newFace.getNormal(i)];
                               vert.addNormal(normal.first, normal.second, normal.third);
                               vertices[vertexIdx] = vert;
                       }

                       faces.push_back(newFace);
               }
       }

       /*for(std::vector<Face>::iterator it = faces.begin(); it != faces.end(); ++it)
       {
                Face face = *it;
                std::cout << "Face:" << std::endl;
                std::cout << "<" << vertices[face.getVertex(0)].first << "," << vertices[face.getVertex(0)].second << "," << vertices[face.getVertex(0)].third << ">"   << std::endl;
                std::cout << "<" << vertices[face.getVertex(1)].first << "," << vertices[face.getVertex(1)].second << "," << vertices[face.getVertex(1)].third << ">"   << std::endl;
                std::cout << "<" << vertices[face.getVertex(2)].first << "," << vertices[face.getVertex(2)].second << "," << vertices[face.getVertex(2)].third << ">"   << std::endl;
       }*/

        file.close();
}

ObjLoader::~ObjLoader()
{
        //dtor
}

std::vector<float> ObjLoader::loadNumbers(std::stringstream &ss)
{
        std::vector<float> vec3;

        float number;
        ss >> number;
        do
        {
                vec3.push_back(number);
                ss >> number;
        }
        while(!ss.fail());

        return vec3;
}

std::vector<std::pair<int, bool> > ObjLoader::loadFace(std::string&ss)
{
        std::vector<std::pair<int, bool> > face(3);

        std::string::iterator start = ss.begin();
        std::string::iterator end = ss.begin();

        while((end != ss.end()) && (*end != '/'))
        {
                ++end;
        }

        {
                std::string num(start, end);
                std::istringstream convert(num);
                int vertex;
                convert >> vertex;
                face.insert(face.begin(), std::pair<int, bool>((vertex - 1), true));
        }

        start = end + 1;
        end = end + 1;
        while((end != ss.end()) && (*end != '/'))
        {
                ++end;
        }

        if(start != end)
        {
                std::string texCoord(start, end);

        }

        start = end + 1;
        end = end + 1;

        while((end != ss.end()))
        {
                ++end;
        }

        if(start != end)
        {
                std::string normalNum(start, end);
                std::istringstream convert(normalNum);
                int normal;
                convert >> normal;
                face.insert(face.begin() + 2, std::pair<int, bool>((normal - 1), true));
        }

        return face;
}

float *ObjLoader::getVertices()
{
        int count = faces.size() * 3 * 3;
        float *vertices = new float[faces.size() * 3 * 3];
        int i = 0;

        for(std::vector<Face>::iterator it = faces.begin(); it != faces.end(); ++it, i += 9)
        {
                Face face = *it;

                triplet<float, float, float> vertex1= (this->vertices[face.getVertex(2)]).getXyz();
                vertices[i + 0] = vertex1.first;
                vertices[i + 1] = vertex1.second;
                vertices[i + 2] = vertex1.third;

                triplet<float, float, float> vertex2 = (this->vertices[face.getVertex(1)]).getXyz();
                vertices[i + 3] = vertex2.first;
                vertices[i + 4] = vertex2.second;
                vertices[i + 5] = vertex2.third;

                triplet<float, float, float> vertex3 = (this->vertices[face.getVertex(0)]).getXyz();
                vertices[i + 6] = vertex3.first;
                vertices[i + 7] = vertex3.second;
                vertices[i + 8] = vertex3.third;
        }

        return vertices;
}

size_t ObjLoader::vsize()
{
        return sizeof(float) * faces.size() * 3 * 3;
}

size_t ObjLoader::nsize()
{
        return sizeof(float) * faces.size() * 3 * 3;
}

float *ObjLoader::getNormals()
{
        float *normals = new float[faces.size() * 3 * 3];
        int i = 0;

        for(std::vector<Face>::iterator it = faces.begin(); it != faces.end(); ++it)
        {
                Face face = *it;

                for(std::vector<int>::iterator itNormals = face.beginNormals(); itNormals != face.endNormals(); ++itNormals, i += 3)
                {
                        int normalIndex = *itNormals;
                        triplet<float, float, float> normal = this->normals[normalIndex];

                        normals[i + 0] = normal.first;
                        normals[i + 1] = normal.second;
                        normals[i + 2] = normal.third;
                }
        }

        return normals;
}

float *ObjLoader::getAvgNormals()
{
        float *normals = new float[faces.size() * 3 * 3];
        int i = 0;

        for(std::vector<Face>::iterator it = faces.begin(); it != faces.end(); ++it)
        {
                Face face = *it;

                //Reverse normals because the vertices are in the wrong order so we had to reverse them
                //so we have to reverse the order of normals as well!
                for(int j = 2; j >= 0; j--, i += 3)
                {
                        int vertIdx = face.getVertex(j);
                        Vertex vert = vertices[vertIdx];
                        triplet<float, float, float> normal = vert.getNormal();

                        normals[i + 0] = normal.first;
                        normals[i + 1] = normal.second;
                        normals[i + 2] = normal.third;
                }
        }

        return normals;
}

float *ObjLoader::getColours()
{
        float *colours = new float[faces.size() * 3 * 3];

        for(int i = 0; i < faces.size() * 3 * 3; i += 3)
        {
                colours[i + 0] = 1.0f;
                colours[i + 1] = 0.0f;
                colours[i + 2] = 0.0f;
        }

        return colours;
}

size_t ObjLoader::csize()
{
        return sizeof(float) * faces.size() * 3 * 3;
}

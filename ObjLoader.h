#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <vector>
#include <sstream>
#include "triplet.h"
#include "Face.h"
#include "Vertex.h"

class ObjLoader
{
        public:
                ObjLoader(std::string filepath);

                float *getVertices();
                size_t vsize();
                float *getNormals();
                size_t nsize();

                float *getAvgNormals();

                float *getColours();
                size_t csize();

                virtual ~ObjLoader();
        protected:
        private:
                std::vector<Vertex> vertices;
                std::vector<triplet<float, float, float> > normals;
                std::vector<Face> faces;

                std::vector<float> loadNumbers(std::stringstream &ss);
                std::vector<std::pair<int, bool> > loadFace(std::string &ss);
};

#endif // OBJLOADER_H

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "ModelLoader.h"

#include <string>
#include <vector>
#include <sstream>
#include "triplet.h"
#include "Face.h"
#include "Vertex.h"
#include "ObjMaterial.h"

class ObjLoader: public ModelLoader
{
        public:
                ObjLoader(std::string filepath);

                float *getVertices();
                size_t vsize();
                float *getNormals();
                size_t nsize();
                float *getTexCoords();
                size_t tsize();

                float *getAvgNormals();

                float *getColours();
                size_t csize();

		bool hasTextures();

                std::vector<ModelMaterial*> getMaterials();

                ~ObjLoader();
        protected:
        private:
                std::vector<Vertex> vertices;
                std::vector<triplet<float, float, float> > normals;
                std::vector<std::pair<float, float> > texCoords;
                std::vector<Face> faces;

                std::vector<float> loadNumbers(std::stringstream &ss);
                std::vector<std::pair<int, bool> > loadFace(std::string &ss);
                std::pair<float, float> loadTexCoords(std::stringstream &ss);

                std::vector<ObjMaterial*> materials;
};

#endif // OBJLOADER_H

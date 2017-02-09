#ifndef FACE_H
#define FACE_H

#include <vector>

class Face
{
        public:
                Face();
                virtual ~Face();

                void addVertex(int vertex);
                void addNormal(int normal);
                std::vector<int>::iterator beginNormals();
                std::vector<int>::iterator endNormals();
                void addTexCoord(int texCoord);

                int getVertex(int index);
                int getNormal(int index);
                int getTexCoord(int index);
        protected:
        private:
        std::vector<int> vertices;
        std::vector<int> normals;
        std::vector<int> texCoords;
};

#endif // FACE_H

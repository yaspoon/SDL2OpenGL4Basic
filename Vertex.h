#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include "triplet.h"

class Vertex
{
        public:
                Vertex();
                Vertex(float x, float y, float z);
                virtual ~Vertex();

                triplet<float, float, float> getXyz();
                triplet<float, float, float> getNormal();

                void addNormal(float x, float y, float z);
                void setXyz(float x, float y, float z);
        protected:
        private:
                triplet<float, float, float> xyz;
                std::vector<triplet<float, float, float> > normals;
};

#endif // VERTEX_H

#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <stddef.h>
#include "ModelMaterial.h"
class ModelLoader
{
        public:
                virtual ~ModelLoader() {}
                virtual float *getVertices() = 0;
                virtual size_t vsize() = 0;
                virtual float *getNormals() = 0;
                virtual size_t nsize() = 0;
                virtual float *getTexCoords() = 0;
                virtual size_t tsize() = 0;
                virtual float *getColours() = 0;
                virtual size_t csize() = 0;
                virtual std::vector<ModelMaterial*> getMaterials() = 0;
                //virtual int *getIndices();
                //virtual size_t isize();

        protected:
        private:
};

#endif // MODELLOADER_H

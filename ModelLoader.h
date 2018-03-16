#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <stddef.h>
#include "ModelMaterial.h"
#include "Mat4.h"
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
		virtual int getTriangleCount() = 0;
		Mat4<float> getModelMatrix() { return modelMatrix; }
		void setModelMatrix(Mat4<float> newModelMat) { modelMatrix = newModelMat; }

        protected:
		Mat4<float> modelMatrix;
        private:
};

#endif // MODELLOADER_H

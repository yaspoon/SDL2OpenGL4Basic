#ifndef DAELOADER_H
#define DAELOADER_H

#include "ModelLoader.h"
#include <collada-dom2.4/dae.h>
#include <collada-dom2.4/1.4/dom/domFloat_array.h>


class DaeLoader: public ModelLoader
{
private:
        DAE dae;
        //daeTArray<domFloat> vertices;
        float *vertices;
        size_t nvertices;
        float *normals;
        size_t nnormals;
        //daeTArray<domFloat> normals;
        size_t ntexCoords;
        float *texCoords;
        //daeTArray<domFloat> texCoords;
        daeTArray<domFloat> colors;

        std::vector<ObjMaterial> materials;
public:
        DaeLoader(std::string daeFile);

        float *getVertices();
        size_t vsize();
        float *getColours();
        size_t csize();
        std::vector<ObjMaterial> getMaterials();
        float *getNormals();
        size_t nsize();
        float *getTexCoords();
        size_t tsize();
        ~DaeLoader();
};

#endif // DAELOADER_H

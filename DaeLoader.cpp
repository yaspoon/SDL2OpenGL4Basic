#include "DaeLoader.h"
#include <iostream>

#include <collada-dom2.4/1.4/dom/domGeometry.h>
#include <collada-dom2.4/1.4/dom/domSource.h>

DaeLoader::DaeLoader(std::string daeFile)
{
        std::cout << "Loading: " << daeFile << std::endl;
        std::string path = "file:///" + daeFile;
        daeElement *root = dae.open(daeFile.c_str());

        std::vector<daeElement*> geometry = dae.getDatabase()->typeLookup(domGeometry::ID());

        std::cout << "Found some geometry:" << geometry.size() << std::endl;

        if(geometry.size() > 0)
        {
                for(std::vector<daeElement*>::iterator it = geometry.begin(); it != geometry.end(); ++it)
                {
                        daeElement *mesh = *it;

                        std::string name = mesh->getAttribute("id");
                        std::cout << "Geometry name is:" << name << std::endl;

                        std::string positions = name + "-positions-array";

                        daeElement *points = dae.getDatabase()->idLookup(positions, mesh->getDocument());

                        if(points != NULL)
                        {
                                std::cout << "Found points" << std::endl;

                                if(points->typeID() == domFloat_array::ID())
                                {
                                        std::cout << "Found float array" << std::endl;
                                        domFloat_array *point_array = (domFloat_array*)points;
                                        daeTArray<domFloat> verts = point_array->getValue();
                                        //std::cout << "There are " << point_array->getCount() << " points" << std::endl;

                                        //vertices = point_array->getValue();
                                        nvertices = point_array->getCount();
                                        std::cout << "nvertices:" << nvertices << std::endl;
                                        vertices = new float[nvertices];

                                        for(int i = 0; i < nvertices; i++)
                                        {
                                                vertices[i] = verts.get(i);
                                        }
                                        //daeTArray<domFloat> values = point_array->getValue();

                                        /*for(size_t i = 0; i < values.getCount(); i++)
                                        {
                                                std::cout << values.get(i) << "\n" << std::endl;
                                        }*/

                                }
                                else
                                {
                                        std::cout << "Couldn't find the float array for the vertices" << std::endl;
                                }
                        }
                        else
                        {
                                std::cout << "Couldn't find any points with a name" << positions << std::endl;
                        }

                        std::string normalName = name + "-normals-array";
                        daeElement *norms = dae.getDatabase()->idLookup(normalName, mesh->getDocument());

                        if(norms)
                        {
                                std::cout << "Found normals!" << std::endl;

                                if(norms->typeID() == domFloat_array::ID())
                                {
                                        domFloat_array *normalsArray = (domFloat_array*)norms;

                                        //normals = normalsArray->getValue();
                                        nnormals = normalsArray->getCount();
                                        normals = new float[nnormals];

                                        daeTArray<domFloat> norms = normalsArray->getValue();

                                        for(int i = 0; i < nnormals; i++)
                                        {
                                                normals[i] = norms.get(i);
                                        }

                                }
                        }
                        else
                        {
                                std::cout << "Failed to find normals :(" << std::endl;
                        }

                        std::string texName = name + "-mesh-map-0-array";
                        daeElement *texs = dae.getDatabase()->idLookup(texName, mesh->getDocument());

                        if(texs)
                        {
                                std::cout << "Found texture coords" << std::endl;

                                if(texs->typeID() == domFloat_array::ID())
                                {
                                        domFloat_array *texArray = (domFloat_array*)texs;
                                        //texCoords = texArray->getValue();

                                        ntexCoords = texArray->getCount();
                                        texCoords = new float[ntexCoords];

                                        daeTArray<domFloat> texs = texArray->getValue();

                                        for(int i = 0; i < ntexCoords; i++)
                                        {
                                                texCoords[i] = texs.get(i);
                                        }
                                }
                        }

                        materials.push_back(ObjMaterial("./" + name + ".mtl"));
                        //std::vector<daeElement*> sources = dae.getDatabase()->typeLookup(domSource::ID());

                        //std::cout << "There are " << sources.size() << " sources" << std::endl;

                        /*for(std::vector<daeElement*>::iterator sourceIt = sources.begin(); sourceIt != sources.end(); ++sourceIt)
                        {
                                daeElement *source = *sourceIt;
                                std::string positions = name + "-positions-array";
                                daeElement *points = dae.getDatabase()->idLookup(positions, source->getDocument());

                                if(points != NULL)
                                {
                                        std::cout << "Found points" << std::endl;

                                        if(points->typeID() == domFloat_array::ID())
                                        {
                                                std::cout << "Found float array" << std::endl;
                                                domFloat_array *point_array = (domFloat_array*)points;
                                                std::cout << "There are " << point_array->getCount() << " points" << std::endl;

                                                vertices = point_array->getValue();
                                                daeTArray<domFloat> values = point_array->getValue();

                                                for(size_t i = 0; i < values.getCount(); i++)
                                                {
                                                        std::cout << values.get(i) << "\n" << std::endl;
                                                }

                                        }
                                        else
                                        {
                                                std::cout << "Couldn't find the float array for the vertices" << std::endl;
                                        }
                                }
                                else
                                {
                                        std::cout << "Couldn't find any points with a name" << positions << std::endl;
                                }
                        }*/
                }
        }

        dae.close(daeFile.c_str());
}

float *DaeLoader::getVertices()
{
        return (float*)vertices;
}

size_t DaeLoader::vsize()
{
        return nvertices;
}

float *DaeLoader::getColours()
{
        return NULL;
}

size_t DaeLoader::csize()
{
        return 0;
}

float *DaeLoader::getNormals()
{
        return (float*)normals;
}

size_t DaeLoader::nsize()
{
        return nnormals;
}

float *DaeLoader::getTexCoords()
{
        return (float*)texCoords;
}

size_t DaeLoader::tsize()
{
        return ntexCoords;
}

std::vector<ObjMaterial> DaeLoader::getMaterials()
{
        return materials;
}

DaeLoader::~DaeLoader()
{
        delete vertices;
}

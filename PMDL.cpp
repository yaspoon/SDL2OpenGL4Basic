#include "PMDL.h"

#include <iostream>
#include <fstream>
#include <string.h>

PMDL::PMDL(std::string filepath)
{
        std::cout << "PMDL_HEADER_LEN:" << PMDL_HEADER_LEN << std::endl;
        vertices = NULL;
        normals = NULL;
        colours = NULL;

        vertices_len = 0;
        normals_len = 0;
        colours_len = 0;

        std::ifstream file;
        file.open(filepath.c_str(), std::ios::in | std::ios::binary);

        if(file.is_open())
        {
                struct PMDL_header header;
                file.read((char*)&header, PMDL_HEADER_LEN);

                std::cout << "PMDL header\n{\n\tmagic:" << header.magic << "\n\tmajor.minor:" << header.major_version << "." << header.minor_version << \
                                "\n\tchecksum:" << header.checksum << "\n\tvertices_offset:" << header.vertices_offset << "\n\tvertices_len:" << \
                                header.vertices_len << "\n\tfaces_offset:" << header.faces_offset << "\n\tfaces_len:" << header.faces_len << \
                                "\n\tuvcoords_offset:" << header.uvcoords_offset << "\n\tuvcoords_len:" << header.uvcoords_len << \
                                "\n\tnormals_offset:" << header.normals_offset << "\n\tnormals_len:" << header.normals_len << \
                                "\n\tcolour:" << header.colour[0] << " " << header.colour[1] << " " << header.colour[2] << " " << header.colour[3] << \
                                "\n}" << std::endl;

                vertices_len = header.vertices_len * sizeof(float);
                vertices = new float[header.vertices_len];
                file.seekg(header.vertices_offset);
                file.read((char*)vertices, vertices_len);

                uvcoords_len = header.uvcoords_len * sizeof(float);
                uvcoords = new float[header.uvcoords_len];
                file.seekg(header.uvcoords_offset);
                file.read((char*)uvcoords, uvcoords_len);

                normals_len = header.normals_len * sizeof(float);
                normals = new float[header.normals_len];
                file.seekg(header.normals_offset);
                file.read((char*)normals, normals_len);

                colours_len = header.vertices_len * sizeof(float);
                colours = new float[header.vertices_len];

                for(int i = 0; i < header.vertices_len; i++)
                {
                        int channel = i % 3;
                        colours[i] = header.colour[channel];
                }

                for(int i = 0; i < header.uvcoords_len; i+=2)
                {
                        std::cout << "[" << uvcoords[i] << "," << uvcoords[i+1] << "]" << std::endl;
                }

                file.close();
        }
        else
        {
                std::cout << "Failed to open file:" << filepath << std::endl;
        }
}

PMDL::~PMDL()
{
        if(vertices)
        {
                delete[] vertices;
        }

        if(normals)
        {
                delete[] normals;
        }

        if(colours)
        {
                delete[] colours;
        }

        if(uvcoords)
        {
                delete[] uvcoords;
        }
}

float *PMDL::getVertices()
{
        int buffer_len = vertices_len / sizeof(float); /*so gross*/
        float *retval = new float[buffer_len];
        memcpy(retval, vertices, vertices_len);
        return retval;
}

size_t PMDL::vsize()
{
        return vertices_len;
}

float *PMDL::getNormals()
{
        int buffer_len = normals_len / sizeof(float);
        float *retval = new float[buffer_len];
        memcpy(retval, normals, normals_len);
        return retval;
}

size_t PMDL::nsize()
{
        return normals_len;
}

float *PMDL::getTexCoords()
{
        int buffer_len = uvcoords_len / sizeof(float);
        float *retval = new float[buffer_len];
        memcpy(retval, uvcoords, uvcoords_len);
        return retval;
}

size_t PMDL::tsize()
{
        return uvcoords_len;
}

float *PMDL::getColours()
{
        int buffer_len = colours_len / sizeof(float);
        float * retval = new float[buffer_len];
        memcpy(retval, colours, colours_len);
        return retval;
}

size_t PMDL::csize()
{
        return colours_len;
}

std::vector<ObjMaterial> PMDL::getMaterials()
{
        return std::vector<ObjMaterial>();
}

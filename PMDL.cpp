#include "PMDL.h"

#include <iostream>
#include <fstream>
#include <string.h>

PMDL::PMDL(std::string filepath)
{
        //std::cout << "PMDL_HEADER_LEN:" << PMDL_HEADER_LEN << std::endl;
        std::cout << "PMDL_HEADER_LEN:" << sizeof(PMDL_header) << std::endl;
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
                //file.read((char*)&header, PMDL_HEADER_LEN);
                file.read((char*)&header, sizeof(PMDL_header));

                std::cout << "PMDL header\n{\n\tmagic:" << header.magic << "\n\tmajor.minor:" << (int)header.major_version << "." << (int)header.minor_version << \
                                "\n\tchecksum:" << header.checksum << "\n\tvertices_offset:" << header.vertices_offset << "\n\tvertices_len:" << \
                                header.vertices_len << "\n\tfaces_offset:" << header.faces_offset << "\n\tfaces_len:" << header.faces_len << \
                                "\n\tuvcoords_offset:" << header.uvcoords_offset << "\n\tuvcoords_len:" << header.uvcoords_len << \
                                "\n\tnormals_offset:" << header.normals_offset << "\n\tnormals_len:" << header.normals_len << \
				"\n\ttextures_offset:" << header.textures_offset << "\n\ttextures_len:" << header.textures_len << \
				"\n\tmaterials_offset:" << header.materials_offset << "\n\tmaterials_len:" << header.materials_len << \
                                "\n\tcolour:" << header.colour[0] << " " << header.colour[1] << " " << header.colour[2] << " " << header.colour[3] << \
                                "\n}" << std::endl;

                vertices_len = header.vertices_len * sizeof(float);
                vertices = new float[header.vertices_len];
                file.seekg(header.vertices_offset);
                file.read((char*)vertices, vertices_len);

		std::cout << "Vertex count:" << header.vertices_len / sizeof(float) << std::endl;
		for(int i = 0; i < header.vertices_len / sizeof(float); i++)
		{
			std::cout << "vertex[" << vertices[i*3] << "," << vertices[i*3+1] << "," << vertices[i*3+1] << "]" << std::endl;
		}

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

		std::cout << "uvcoords:" << std::endl;
                for(int i = 0; i < header.uvcoords_len; i+=2)
                {
                        std::cout << "[" << uvcoords[i] << "," << uvcoords[i+1] << "]" << std::endl;
                }

		file.seekg(header.textures_offset);
		std::vector<std::string> materials_textures;
		for(int i = 0; i < header.textures_len; i++)
		{
			int texture_len;
			file.read((char*)&texture_len, sizeof(texture_len));
			printf("texture_len:%d\n", texture_len);
			char *buf = new char[texture_len+1];
			file.read(buf, texture_len);
			buf[texture_len] = '\0';
			std::cout << "texture:" << buf << std::endl;
			std::string new_texture(buf);
			materials_textures.push_back(new_texture);
			delete[] buf;
		}

		file.seekg(header.materials_offset);
		float *alphas = new float[header.materials_len];
		file.read((char*)alphas, header.materials_len * sizeof(float));

		float *diffuse_colours = new float[header.materials_len * 3];
		file.read((char*)diffuse_colours, header.materials_len * 3 * sizeof(float));

		float *specular_colours = new float[header.materials_len * 3];
		file.read((char*)specular_colours, header.materials_len * 3 * sizeof(float));

		float *specular_intensities = new float[header.materials_len];
		file.read((char*)specular_intensities, header.materials_len * sizeof(float));

		float *specular_hardnesses = new float[header.materials_len];
		file.read((char*)specular_hardnesses, header.materials_len * sizeof(float));

		float *roughnesses = new float[header.materials_len];
		file.read((char*)roughnesses, header.materials_len * sizeof(float));

		int *materials_texture_counts = new int[header.materials_len];
		file.read((char*)materials_texture_counts, header.materials_len * sizeof(int));

		int total_materials_texture_indexs = 0;
		for(int i = 0; i < header.materials_len; i++)
		{
			total_materials_texture_indexs += materials_texture_counts[i];
		}

		std::cout << "total_material_indexs:" << total_materials_texture_indexs << std::endl;

		int *materials_texture_indexs = new int[total_materials_texture_indexs];
		file.read((char*)materials_texture_indexs, sizeof(int) * total_materials_texture_indexs);

		for(int i = 0; i < total_materials_texture_indexs; i++)
		{
			std::cout << "texture_index[" << i << "]:" << materials_texture_indexs[i] << std::endl;
		}

		for(int i = 0; i < header.materials_len; i++)
		{
			Vec4<float> ambient_colour(1.0f, 1.0f, 1.0f, alphas[i]);
			Vec4<float> diffuse_colour(diffuse_colours[i*3], diffuse_colours[i*3 + 1], diffuse_colours[i*3 + 2], alphas[i]);
			Vec4<float> specular_colour(specular_colours[i*3], specular_colours[i*3 + 1], specular_colours[i*3 + 2], alphas[i]);
			std::vector<std::string> newMattextures;
			for(int j = 0; j < materials_texture_counts[i]; j++)
			{
				newMattextures.push_back(materials_textures.at(j));
			}
			PMDLMaterial *newMat = new PMDLMaterial(ambient_colour, diffuse_colour, specular_colour, specular_intensities[i], specular_hardnesses[i], roughnesses[i], newMattextures);
			materials.push_back(newMat);
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

std::vector<ModelMaterial*> PMDL::getMaterials()
{
	std::vector<ModelMaterial*> mdlMaterials(materials.begin(), materials.end());
        return mdlMaterials;
}

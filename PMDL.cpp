#include "PMDL.h"

#include <iostream>
#include <fstream>
#include <string.h>

PMDL::PMDL(std::string filepath)
{
	ModelLoader::modelMatrix = Mat4<float>(1.0f);
        //std::cout << "PMDL_HEADER_LEN:" << PMDL_HEADER_LEN << std::endl;
        std::cout << "PMDL_HEADER_LEN:" << sizeof(PMDL_header) << std::endl;
        vertices = NULL;
        normals = NULL;
        colours = NULL;
	faces = NULL;

        vertices_len = 0;
	vertices_count = 0;
        normals_len = 0;
        colours_len = 0;
	faces_len = 0;
	faces_count = 0;

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
		vertices_count = header.vertices_len;
                vertices = new float[header.vertices_len];
                file.seekg(header.vertices_offset);
                file.read((char*)vertices, vertices_len);

		std::cout << "Vertex count:" << header.vertices_len << std::endl;
		/*for(int i = 0; i < header.vertices_len / 3; i++)
		{
			std::cout << "vertex[" << vertices[i*3] << "," << vertices[i*3+1] << "," << vertices[i*3+2] << "]" << std::endl;
		}*/

		faces_len = header.faces_len * sizeof(int);
		faces_count = header.faces_len;
		faces = new int[header.faces_len];
		file.seekg(header.faces_offset);
		file.read((char*)faces, faces_len);

		std::cout << "Faces count:" << header.faces_len << std::endl;
		/*for(int i = 0; i < header.faces_len; i+=3)
		{
			std::cout << "face[" << faces[i] << "," << faces[i+1] << "," << faces[i+2] << "]" << std::endl;
		}*/

                uvcoords_len = header.uvcoords_len * sizeof(float);
		uvcoords_count = header.uvcoords_len;
                uvcoords = new float[header.uvcoords_len];
                file.seekg(header.uvcoords_offset);
                file.read((char*)uvcoords, uvcoords_len);

                normals_len = header.normals_len * sizeof(float); //Buffer length in bytes
		normals_count = header.normals_len / normal_component_count;
		std::cout << "normals_count:" << normals_count << std::endl;
		/*Because my modeling format sucks normals_len is actualy the number of floats in the normal array, aka 4 * no_normals(aka number of vertices)*/
                normals = new float[header.normals_len];
                file.seekg(header.normals_offset);
                file.read((char*)normals, normals_len);

		/*for(int i = 0; i < header.normals_len; i+=3)
		{
			std::cout << "normal[" << normals[i] << "," << normals[i+1] << "," << normals[i+2] << "]" << std::endl;
		}*/

                colours_len = faces_count * sizeof(float);
		colours_count = faces_count;
                colours = new float[faces_count];

                for(int i = 0; i < faces_count; i++)
                {
                        int channel = i % 3;
                        colours[i] = header.colour[channel];
                }

		/*std::cout << "uvcoords:" << std::endl;
                for(int i = 0; i < header.uvcoords_len; i+=2)
                {
                        std::cout << "[" << uvcoords[i] << "," << uvcoords[i+1] << "]" << std::endl;
                }*/

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
		delete[] alphas;
		delete[] diffuse_colours;
		delete[] specular_colours;
		delete[] specular_intensities;
		delete[] specular_hardnesses;
		delete[] roughnesses;
		delete[] materials_texture_counts;
		delete[] materials_texture_indexs;
        }
        else
        {
                std::cout << "Failed to open file:" << filepath << std::endl;
        }
}

PMDL::~PMDL()
{
	if(faces) {
		delete[] faces;
	}

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

	for(std::vector<PMDLMaterial*>::iterator it = materials.begin(); it != materials.end(); ++it) {
		delete *it;
	}
}

float *PMDL::getVertices()
{
        int buffer_len = faces_count * vertices_component_count;
        float *retval = new float[buffer_len];
	int index = 0;

	for(int i = 0; i < faces_count; i++)
	{
		int vertexIndex = faces[i];
		retval[index] = vertices[vertexIndex * 3];
		retval[index + 1] = vertices[(vertexIndex * 3) + 1];
		retval[index + 2] = vertices[(vertexIndex * 3) + 2];
		retval[index + 3] = 1.0f; //This is a point, so set the W to 1 so translations work!
		index += 4;
	}

	/*for(int i = 0; i < faces_count; i++)
	{
		std::cout << "vertex:[" << retval[i*3] << "," << retval[i*3+1] << "," << retval[i*3+2] << "]" << std::endl;
	}*/

        return retval;
}

size_t PMDL::vsize()
{
        return faces_count * vertices_component_count * sizeof(float);
}

/*
 * Like vertices there is only 4 normals in 2 triangles
 * We need to "unpack" the normals so we end up with 3 per per face
 */
float *PMDL::getNormals()
{
	int buffer_len = faces_count * normal_component_count;
        float *retval = new float[buffer_len];

	for(int i = 0; i < faces_count; i++)
	{
		std::cout << "getNormals Normal[" << i << "]=" << normals[i] << std::endl;
		int normalIndex = faces[i] * 4;
		retval[i * normal_component_count] = normals[normalIndex];
		retval[i * normal_component_count + 1] = normals[normalIndex + 1];
		retval[i * normal_component_count + 2] = normals[normalIndex + 2];
		retval[i * normal_component_count + 3] = 0.0f; //This is a vector, therefore the w component is 0 because we don't want translation to do stuff!
	}

        return retval;
}

size_t PMDL::nsize()
{
        return faces_count * normal_component_count * sizeof(float);
}

void PMDL::dumpNormals()
{
	std::cout << "normals_count:" << normals_count << std::endl << "normal_component_count:" << normal_component_count << std::endl;
	int count = normals_count * normal_component_count;
	std::cout << "count:" << count << std::endl;
	std::cout << "<";
	for(int i = 0; i < count; i++) {
		if(i != 0 && i % 4 == 0) {
			std::cout << ">\n<";
		}
		std::cout << normals[i] << ",";
	}
	std::cout << ">" << std::endl;
}

float *PMDL::getTexCoords()
{
        int buffer_len = uvcoords_count;
        float *retval = new float[buffer_len];

	for(int i = 0; i < buffer_len; i++)
	{
		retval[i] = uvcoords[i];
		retval[i] = uvcoords[i];
	}

	/*for(int i = 0; i < faces_count; i++)
	{
		std::cout << "uv[" << retval[i*2] * 1024.0f << "," << retval[i*2 + 1] * 1024.0f << "]" << std::endl;
	}*/

        return retval;
}

size_t PMDL::tsize()
{
        return faces_count * 2 * sizeof(float);
}

float *PMDL::getColours()
{
        int buffer_len = colours_count;
        float *retval = new float[buffer_len];
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

int PMDL::getTriangleCount()
{
	return faces_count;
}

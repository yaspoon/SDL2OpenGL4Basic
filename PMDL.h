#ifndef PMDL_H
#define PMDL_H

#include "ModelLoader.h"
#include "PMDLMaterial.h"
#include <cstdint>
#include <string>
#include <vector>

struct __attribute__((packed)) PMDL_header
{
    char magic[4];
    uint8_t major_version;
    uint8_t minor_version;
    int32_t checksum;
    int32_t vertices_offset;
    int32_t vertices_len;
    int32_t faces_offset;
    int32_t faces_len;
    int32_t uvcoords_offset;
    int32_t uvcoords_len;
    int32_t normals_offset;
    int32_t normals_len;
    int32_t textures_offset;
    int32_t textures_len;
    int32_t materials_offset;
    int32_t materials_len;
    float colour[4];
};

/*struct __attribute__((packed)) PMDL_material
{
	float alpha;
	float diffuse_colour[3];
	float specular_colour[3];
	float specular_intensity;
	float specular_hardness;
	float roughness;
	int texture_count;
}*/

struct PMDL_data
{
    float *vertices;
    float *faces;
    float *uvcoords;
    float *normals;
};
/*class PMDL_material
{
	public:
		PMDL_material(float alpha, float diffuse_colour[3], float specular_colour[3], float specular_intensity, float specular_hardness, float roughness, 
				std::vector<std::string> textures);

	private:
	float alpha;
	float diffuse_colour[3];
	float specular_colour[3];
	float specular_intensity;
	float specular_hardness;
	float roughness;
	std::vector<std::string> textures;
};*/

class PMDL: public ModelLoader
{
public:
        PMDL(std::string filepath);
        ~PMDL();

        float* getVertices();
        size_t vsize();
        float *getNormals();
        size_t nsize();
        float *getTexCoords();
        size_t tsize();
        float *getColours();
        size_t csize();

        std::vector<ModelMaterial*> getMaterials();
private:
        float *vertices;
        float *normals;
        float *colours;
        float *uvcoords;
	std::vector<PMDLMaterial*> materials;

        size_t vertices_len;
        size_t uvcoords_len;
        size_t normals_len;
        size_t colours_len;

        const int PMDL_HEADER_LEN = sizeof(struct PMDL_header);
};
#endif // PMDL_H

#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
/*We need this weirdo define for the core profile gl3 stuff to work*/
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>

#include "Mat4.h"
#include <vector>
#include "Shader.h"
#include "Light.h"
#include "Material.h"
#include "ModelLoader.h"
#include "Renderable.h"

class Renderer
{
private:
        enum vaoIds {TRIANGLES, NUM_VAOS};
        enum bufferIds {ARRAY_BUFFER, NUM_BUFFERS};
        enum indexIds {INDEXS, NUM_INDEXS};
        enum texIds {TEXTURE, NUM_TEXTURES};

        GLint vPosition;
        GLint vColour;
        GLint vNormal;

        //GLuint vertArrays[NUM_VAOS];
        //GLuint buffers[NUM_BUFFERS];
        //GLuint ibo[NUM_INDEXS];
        //GLuint textures[NUM_TEXTURES];

        float mf_width;
        float mf_height;
        float mf_aspectRatio;
        float mf_fov;
        float mf_near;
        float mf_far;

        GLint projMatLocation;
        GLint modelMatLocation;
        GLint cameraMatLocation;
        GLint camPositionLocation;
        GLint textureLocation;
        GLint normalMapLocation;
        GLint numEnabledLightsLocation;
        GLint program;

        GLint maxTextureUnits;

        const static int numUniforms = 17;
        const char *shaderUniforms[numUniforms] = {
                "lights[0].isEnabled",
                "lights[0].enableDiffuse",
                "lights[0].enableSpecular",
                "lights[0].isSpotlight",
                "lights[0].isPointlight",
                "lights[0].specularMode",
                "lights[0].ambientLight",
                "lights[0].diffuseLight",
                "lights[0].specularLight",
                "lights[0].strength",
                "lights[0].position",
                "lights[0].normal",
                "lights[0].angle",
                "lights[0].linearAtten",
                "lights[0].quadAtten",
                "lights[0].constAtten",
                "lights[0].spotponent",
        };

        const static int MAX_LIGHTS = 10;
        const static int MAX_MATERIALS = 10;

        const static int numMatUniforms = 7;
        const char *materialUniforms[numMatUniforms] = {
                "materials[0].emission",
                "materials[0].ambient",
                "materials[0].diffuse",
                "materials[0].specular",
                "materials[0].shininess",
                "materials[0].colour",
                "materials[0].hasTexture"
                "materials[0].enabled"
        };


        GLuint uboIndex;
        GLint uboSize;
        GLuint ubo;
        GLuint uboStride;
        GLuint matUboOffset;

        GLuint matBlockIndex;
        GLint matBlockSize;
        GLuint matUbo;
        GLuint matUboStride;
        //GLvoid *buffer; //We need to store the uniform structure in our application then upload it to gfx....

        GLuint uniformIndices[numUniforms];
        GLint uniformSizes[numUniforms];
        GLint uniformOffsets[numUniforms];
        GLint uniformType[numUniforms];
        GLint uniformStrides[numUniforms];

        GLuint matUniformIndices[numMatUniforms];
        GLint matUniformSizes[numMatUniforms];
        GLint matUniformOffsets[numMatUniforms];
        GLint matUniformType[numMatUniforms];
        GLint matUniformStrides[numMatUniforms];

        //std::vector<int> triangleCounts;
        //std::vector<GLuint> vertexArrays;
        std::vector<GLuint> buffers;
        std::vector<GLuint> ibos;
        int materialIndex;

        Mat4<float> modelMatrix;
        Mat4<float> projectionMatrix;
        Mat4<float> cameraMatrix;

        Vec4<float> camPosition;

        Mat4<float> frustumProjection(float left, float right, float top, float bottom, float near, float far);
        Mat4<float> perspectiveProjection(float FOV, float aspectRatio, float near, float far);
        std::pair<GLuint, int> loadPrimitiveData(float *vertices, size_t vcount, unsigned short *indices, size_t icount, float *colour, size_t ccount, size_t tsize, float *texCoords, float *normals, size_t nsize);
        std::string title;

        SDL_Window *window = NULL;
        SDL_GLContext context = NULL;

        std::vector<GLuint> textures;
        std::vector<Renderable> renderables;

        Shader shader;
public:
        Renderer();
        Renderer(float width, float height);
        Renderer(float width, float height, float near, float far, float fov);

        bool initSDL();
        bool initGL(std::vector<struct ShaderList> list);
        void cleanup();

        void setWindowTitle(std::string title);

        std::pair<GLint, bool> loadProgram(std::vector<struct ShaderList> list);
        void useProgram(GLint program);
        void setProjectionMatrix(Mat4<float> projMat);
        void setNumEnabledLights(int count);

        void setTriangleCount(float triangles);

        Light newLight(int i);
        Material newMaterial(int index);

        void updateCameraMatrix(Mat4<float> camMat);
        void updateModelMatrix(Mat4<float> modMat);
        void updateCameraPosition(Vec4<float> camDir);
        void updateLight(Light updatedLight);
        void updateLights(std::vector<Light> lights);
        void updateMaterials(std::vector<Material> materials);
        void updateMaterial(Material material);

        void draw();

        void loadModel(ModelLoader &model);
        GLuint loadTexture(const char *name);

        void loadTest();

        const int getMaxLights();
};
#endif

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

        GLuint vertArrays[NUM_VAOS];
        GLuint buffers[NUM_BUFFERS];
        GLuint ibo[NUM_INDEXS];
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

        const static int numUniforms = 180;
        /*const char *shaderUniforms[numUniforms] = {
                "isEnabled",
                "enableDiffuse",
                "enableSpecular",
                "isSpotlight",
                "isPointlight",
                "specularMode",
                "ambientLight",
                "diffuseLight",
                "specularLight",
                "shininess",
                "strength",
                "position",
                "normal",
                "angle",
                "linearAtten",
                "quadAtten",
                "constAtten",
                "spotponent"
        };*/

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
                "lights[0].shininess",
                "lights[0].strength",
                "lights[0].position",
                "lights[0].normal",
                "lights[0].angle",
                "lights[0].linearAtten",
                "lights[0].quadAtten",
                "lights[0].constAtten",
                "lights[0].spotponent",
                "lights[1].isEnabled",
                "lights[1].enableDiffuse",
                "lights[1].enableSpecular",
                "lights[1].isSpotlight",
                "lights[1].isPointlight",
                "lights[1].specularMode",
                "lights[1].ambientLight",
                "lights[1].diffuseLight",
                "lights[1].specularLight",
                "lights[1].shininess",
                "lights[1].strength",
                "lights[1].position",
                "lights[1].normal",
                "lights[1].angle",
                "lights[1].linearAtten",
                "lights[1].quadAtten",
                "lights[1].constAtten",
                "lights[1].spotponent",
                "lights[2].isEnabled",
                "lights[2].enableDiffuse",
                "lights[2].enableSpecular",
                "lights[2].isSpotlight",
                "lights[2].isPointlight",
                "lights[2].specularMode",
                "lights[2].ambientLight",
                "lights[2].diffuseLight",
                "lights[2].specularLight",
                "lights[2].shininess",
                "lights[2].strength",
                "lights[2].position",
                "lights[2].normal",
                "lights[2].angle",
                "lights[2].linearAtten",
                "lights[2].quadAtten",
                "lights[2].constAtten",
                "lights[2].spotponent",
                "lights[3].isEnabled",
                "lights[3].enableDiffuse",
                "lights[3].enableSpecular",
                "lights[3].isSpotlight",
                "lights[3].isPointlight",
                "lights[3].specularMode",
                "lights[3].ambientLight",
                "lights[3].diffuseLight",
                "lights[3].specularLight",
                "lights[3].shininess",
                "lights[3].strength",
                "lights[3].position",
                "lights[3].normal",
                "lights[3].angle",
                "lights[3].linearAtten",
                "lights[3].quadAtten",
                "lights[3].constAtten",
                "lights[3].spotponent",
                "lights[4].isEnabled",
                "lights[4].enableDiffuse",
                "lights[4].enableSpecular",
                "lights[4].isSpotlight",
                "lights[4].isPointlight",
                "lights[4].specularMode",
                "lights[4].ambientLight",
                "lights[4].diffuseLight",
                "lights[4].specularLight",
                "lights[4].shininess",
                "lights[4].strength",
                "lights[4].position",
                "lights[4].normal",
                "lights[4].angle",
                "lights[4].linearAtten",
                "lights[4].quadAtten",
                "lights[4].constAtten",
                "lights[4].spotponent",
                "lights[5].isEnabled",
                "lights[5].enableDiffuse",
                "lights[5].enableSpecular",
                "lights[5].isSpotlight",
                "lights[5].isPointlight",
                "lights[5].specularMode",
                "lights[5].ambientLight",
                "lights[5].diffuseLight",
                "lights[5].specularLight",
                "lights[5].shininess",
                "lights[5].strength",
                "lights[5].position",
                "lights[5].normal",
                "lights[5].angle",
                "lights[5].linearAtten",
                "lights[5].quadAtten",
                "lights[5].constAtten",
                "lights[5].spotponent",
                "lights[6].isEnabled",
                "lights[6].enableDiffuse",
                "lights[6].enableSpecular",
                "lights[6].isSpotlight",
                "lights[6].isPointlight",
                "lights[6].specularMode",
                "lights[6].ambientLight",
                "lights[6].diffuseLight",
                "lights[6].specularLight",
                "lights[6].shininess",
                "lights[6].strength",
                "lights[6].position",
                "lights[6].normal",
                "lights[6].angle",
                "lights[6].linearAtten",
                "lights[6].quadAtten",
                "lights[6].constAtten",
                "lights[6].spotponent",
                "lights[7].isEnabled",
                "lights[7].enableDiffuse",
                "lights[7].enableSpecular",
                "lights[7].isSpotlight",
                "lights[7].isPointlight",
                "lights[7].specularMode",
                "lights[7].ambientLight",
                "lights[7].diffuseLight",
                "lights[7].specularLight",
                "lights[7].shininess",
                "lights[7].strength",
                "lights[7].position",
                "lights[7].normal",
                "lights[7].angle",
                "lights[7].linearAtten",
                "lights[7].quadAtten",
                "lights[7].constAtten",
                "lights[7].spotponent",
                "lights[8].isEnabled",
                "lights[8].enableDiffuse",
                "lights[8].enableSpecular",
                "lights[8].isSpotlight",
                "lights[8].isPointlight",
                "lights[8].specularMode",
                "lights[8].ambientLight",
                "lights[8].diffuseLight",
                "lights[8].specularLight",
                "lights[8].shininess",
                "lights[8].strength",
                "lights[8].position",
                "lights[8].normal",
                "lights[8].angle",
                "lights[8].linearAtten",
                "lights[8].quadAtten",
                "lights[8].constAtten",
                "lights[8].spotponent",
                "lights[9].isEnabled",
                "lights[9].enableDiffuse",
                "lights[9].enableSpecular",
                "lights[9].isSpotlight",
                "lights[9].isPointlight",
                "lights[9].specularMode",
                "lights[9].ambientLight",
                "lights[9].diffuseLight",
                "lights[9].specularLight",
                "lights[9].shininess",
                "lights[9].strength",
                "lights[9].position",
                "lights[9].normal",
                "lights[9].angle",
                "lights[9].linearAtten",
                "lights[9].quadAtten",
                "lights[9].constAtten",
                "lights[9].spotponent",
                };

        const static int MAX_LIGHTS = 10;


        GLuint uboIndex;
        GLint uboSize;
        GLuint ubo;
        GLuint uboStride;
        //GLvoid *buffer; //We need to store the uniform structure in our application then upload it to gfx....

        GLuint uniformIndices[numUniforms];
        GLint uniformSizes[numUniforms];
        GLint uniformOffsets[numUniforms];
        GLint uniformType[numUniforms];
        GLint uniformStrides[numUniforms];


        int triangleCount;

        Mat4<float> modelMatrix;
        Mat4<float> projectionMatrix;
        Mat4<float> cameraMatrix;

        Vec4<float> camPosition;

        Mat4<float> frustumProjection(float left, float right, float top, float bottom, float near, float far);
        Mat4<float> perspectiveProjection(float FOV, float aspectRatio, float near, float far);

        std::string title;

        SDL_Window *window = NULL;
        SDL_GLContext context = NULL;

        std::vector<GLuint> textures;
public:
        Renderer();
        Renderer(float width, float height);
        Renderer(float width, float height, float near, float far, float fov);

        bool initSDL();
        void initGL(std::vector<struct ShaderList> list);
        void cleanup();

        void setWindowTitle(std::string title);

        GLint loadProgram(std::vector<struct ShaderList> list);
        void useProgram(GLint program);
        void setProjectionMatrix(Mat4<float> projMat);

        void setTriangleCount(float triangles);

        Light newLight();

        void updateCameraMatrix(Mat4<float> camMat);
        void updateModelMatrix(Mat4<float> modMat);
        void updateCameraPosition(Vec4<float> camDir);
        void updateLights(Light updatedLight);

        void draw();

        void loadPrimitiveData(float *vertices, size_t vcount, unsigned short *indices, size_t icount, float *colour, size_t ccount, size_t tsize, float *texCoords, float *normals, size_t nsize);
        void loadTexture(char *name);

        void loadTest();
};
#endif

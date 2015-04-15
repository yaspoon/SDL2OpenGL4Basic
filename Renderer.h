#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
/*We need this weirdo define for the core profile gl3 stuff to work*/
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>

#include "Mat4.h"
#include <vector>
#include "Shader.h"

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
        GLuint textures[NUM_TEXTURES];

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
        GLint lightPosLocation;
        GLint enableDiffuseLocation;
        GLint enableSpecularLocation;
        GLint program;

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

        void updateCameraMatrix(Mat4<float> camMat);
        void updateModelMatrix(Mat4<float> modMat);
        void updateCameraPosition(Vec4<float> camDir);
        void updateLightPosition(Vec4<float> lightPos);

        void draw();

        void loadPrimitiveData(float *vertices, size_t vcount, unsigned short *indices, size_t icount, float *colour, size_t ccount, size_t tsize, float *texCoords, float *normals, size_t nsize);
        void loadTexture(char *name);

        void loadTest();

        bool diffuse;
        bool specular;
};
#endif

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
        enum indexIds{INDEXS, NUM_INDEXS};

        GLint vPosition;
        GLint vColour;

        GLuint vertArrays[NUM_VAOS];
        GLuint buffers[NUM_BUFFERS];
        GLuint ibo[NUM_INDEXS];

        float mf_width;
        float mf_height;
        float mf_aspectRatio;
        float mf_fov;
        float mf_near;
        float mf_far;

        GLint projMatLocation;
        GLint modelMatLocation;
        GLint cameraMatLocation;

        GLint program;

        Mat4<float> modelMatrix;
        Mat4<float> projectionMatrix;
        Mat4<float> cameraMatrix;

        Mat4<float> frustumProjection(float left, float right, float top, float bottom, float near, float far);
        Mat4<float> perspectiveProjection(float FOV, float aspectRatio, float near, float far);

        SDL_Window *window = NULL;
        SDL_GLContext context = NULL;
public:
        Renderer();
        Renderer(float width, float height);
        Renderer(float width, float height, float near, float far, float fov);

        bool initSDL();
        void initGL(std::vector<struct ShaderList> list);
        void cleanup();

        GLint loadProgram(std::vector<struct ShaderList> list);
        void useProgram(GLint program);
        void setProjectionMatrix(Mat4<float> projMat);

        void updateCameraMatrix(Mat4<float> camMat);
        void updateModelMatrix(Mat4<float> modMat);

        void draw();

        void loadPrimitiveData(float *vertices, size_t vcount, unsigned short *indices, size_t icount, float *colour, size_t ccount);
};
#endif

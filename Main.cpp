#include <SDL2/SDL.h>
/*We need this weirdo define for the core profile gl3 stuff to work*/
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include <iostream>

#include "Shader.h"
#include "MD2Model.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Camera.h"
#include "Timer.h"

#include "Box.h"
#include "Renderer.h"

const GLfloat PI = 3.14;
enum vaoIds {TRIANGLES, NUM_VAOS};
enum bufferIds {ARRAY_BUFFER, NUM_BUFFERS};
enum indexIds{INDEXS, NUM_INDEXS};

GLint vPosition;
GLint vColour;

GLuint vertArrays[NUM_VAOS];
GLuint buffers[NUM_BUFFERS];
GLuint ibo[NUM_INDEXS];
const GLuint NUM_VERTICES = 8;

Renderer renderer;

void init()
{
        renderer.initSDL();

        Vec4<float> min(-10.0, -10.0, -10.0, 1.0f);
        Vec4<float> max(10.0, 10.0, 10.0, 1.0f);
        Box b(min, max);

        GLfloat colours[NUM_VERTICES*3] = {1.0, 0.0, 0.0,
                                                                          0.0, 1.0, 0.0,
                                                                          0.0, 0.0, 1.0,
                                                                          1.0, 1.0, 1.0,
                                                                          1.0, 0.0, 0.0,
                                                                          0.0, 1.0, 0.0,
                                                                          0.0, 0.0, 1.0,
                                                                          1.0, 1.0, 1.0};


        std::vector<struct ShaderList> list;
        list.push_back((struct ShaderList){GL_VERTEX_SHADER, "./shader.vert"});
        list.push_back((struct ShaderList){GL_FRAGMENT_SHADER, "./shader.frag"});

        renderer.initGL(list);
        renderer.loadPrimitiveData(b.Vertices(), b.vsize(), b.getIndices(), b.isize(), colours, sizeof(colours));
}

int main(int argc, char *argv[])
{
        SDL_bool relativeMouse = SDL_FALSE;
        Timer frameTimer;
        Camera cam;

        Mat4<float> transMatrix(1.0f);
        transMatrix[3][2] = 10.0f;

        MD2Model skel;
        skel.loadModel("./hueteotl/tris.md2");
        SDL_Window *window = NULL;
        SDL_GLContext context = NULL;
        bool quit = false;

        SDL_Event event;

        init();

        int rotStep = 0;

        frameTimer.start();
        float previousTime = frameTimer.getTicks();
        float currentTime = previousTime;

        while(!quit)
        {
                while(SDL_PollEvent(&event))
                {
                        if(event.type == SDL_QUIT)
                        {
                                quit = true;
                        }
                        else if(event.type == SDL_KEYDOWN)
                        {
                                switch(event.key.keysym.scancode)
                                {
                                case SDL_SCANCODE_Q:
                                        quit = true;
                                        break;
                                case SDL_SCANCODE_W:
                                        cam.dirz(1.0f);
                                        break;
                                case SDL_SCANCODE_S:
                                        cam.dirz(-1.0f);
                                        break;
                                case SDL_SCANCODE_A:
                                        cam.dirx(-1.0f);
                                        break;
                                case SDL_SCANCODE_D:
                                        cam.dirx(1.0f);
                                        break;
                                case SDL_SCANCODE_F:
                                        relativeMouse = (SDL_bool)!relativeMouse;
                                        SDL_SetRelativeMouseMode(relativeMouse);
                                        break;
                                }
                        }
                        else if(event.type == SDL_KEYUP)
                        {
                                switch(event.key.keysym.scancode)
                                {
                                case SDL_SCANCODE_W:
                                        cam.dirz(0.0f);
                                        break;
                                case SDL_SCANCODE_S:
                                        cam.dirz(0.0f);
                                        break;
                                case SDL_SCANCODE_A:
                                        cam.dirx(0.0f);
                                        break;
                                case SDL_SCANCODE_D:
                                        cam.dirx(0.0f);
                                        break;
                                case SDL_SCANCODE_F1:
                                {
                                        /*SDL_DisplayMode dm;
                                        SDL_GetDesktopDisplayMode(0, &dm);

                                        width = dm.w;
                                        height = dm.h;
                                        mf_aspectRatio = width / height;

                                        SDL_SetWindowDisplayMode(window, &dm);
                                        glViewport(0, 0, width, height);
                                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

                                        projectionMatrix = perspectiveProjection(mf_fov, mf_aspectRatio, mf_near, mf_far);
                                        glUniformMatrix4fv(projMatLocation, 1, false, (float*)&projectionMatrix);*/
                                }
                                }
                        }
                        else if(event.type == SDL_MOUSEMOTION)
                        {
                                //cam.updatePitch((float)event.motion.yrel);
                                //cam.updateYaw((float)event.motion.xrel);
                        }
                }

                currentTime = frameTimer.getTicks();
                float dt = (float)(currentTime - previousTime) / 1000.0f;
                cam.update(dt);
                previousTime = currentTime;


                renderer.updateCameraMatrix(cam.cameraMatrix());

                Mat4<float> rotMatrix(1.0f);
                renderer.updateModelMatrix(rotMatrix * transMatrix);

                renderer.draw();

                rotStep += 1;
                rotStep %= 360;

                SDL_GL_SwapWindow(window);
        }

        renderer.cleanup();

        return 0;
}

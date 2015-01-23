#include <SDL2/SDL.h>
/*We need this weirdo define for the core profile gl3 stuff to work*/
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <sstream>

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

static const float MAX_FPS = 60.0f;

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
        Timer GameTimer; //Keep track of time between game frames
        Timer FrameTimer; //Keep track of time between actual drawn frames
        Camera cam;

        Mat4<float> transMatrix(1.0f);
        transMatrix[3][2] = 10.0f;

        MD2Model skel;
        skel.loadModel("./hueteotl/tris.md2");
        bool quit = false;

        SDL_Event event;

        init();

        int rotStep = 0;

        GameTimer.start();
        float previousTime = GameTimer.getTicks();
        float currentTime = previousTime;

        const float FRAME_TIME = 1000.0f / MAX_FPS; //Amount of time each frame should take
        int frame = 0;
        int ticks = 0;
        float timeSinceLastFrame = 0;

        bool uncappedFps = false;

        FrameTimer.start();

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
                                case SDL_SCANCODE_F2:
                                        uncappedFps = !uncappedFps;
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
                                        /*Implement fullscreen switching*/
                                }
                                }
                        }
                        else if(event.type == SDL_MOUSEMOTION)
                        {
                                cam.updatePitch((float)event.motion.yrel);
                                cam.updateYaw((float)event.motion.xrel);
                        }
                }

                currentTime = GameTimer.getTicks();
                float dt = (float)(currentTime - previousTime) / 1000.0f;
                timeSinceLastFrame += currentTime - previousTime;
                cam.update(dt);
                previousTime = currentTime;

                /*Frame limiting*/
                if(timeSinceLastFrame > FRAME_TIME || uncappedFps)
                {
                        timeSinceLastFrame = 0;
                        //SDL_Delay(FRAME_TIME - dt);

                        renderer.updateCameraMatrix(cam.cameraMatrix());

                        Mat4<float> rotMatrix(1.0f);
                        renderer.updateModelMatrix(rotMatrix * transMatrix);

                        renderer.draw();

                        rotStep += 1;
                        rotStep %= 360;

                        GLenum error = glGetError();

                        switch(error)
                        {
                        case GL_INVALID_ENUM:
                                std::cout << "ERROR: GL Invalid Enum" << std::endl;
                        case GL_INVALID_VALUE:
                                std::cout << "ERROR: GL Invalid Value" << std::endl;
                                break;
                        case GL_INVALID_OPERATION:
                                std::cout << "ERROR: GL Invalid Operation" << std::endl;
                                break;
                        case GL_INVALID_FRAMEBUFFER_OPERATION:
                                std::cout << "ERROR: GL Invalid Framebuffer Operation" << std::endl;
                                break;
                        case GL_OUT_OF_MEMORY:
                                std::cout << "ERROR: GL Out of Memory" << std::endl;
                                break;
                        case GL_STACK_UNDERFLOW:
                                std::cout << "ERROR:GL Stack Underflow" << std::endl;
                                break;
                        case GL_STACK_OVERFLOW:
                                std::cout << "ERROR:GL Stack Overflow" << std::endl;
                                break;
                        case GL_NO_ERROR:
                                break;
                        default:
                                break;
                        }

                        frame += 1;
                }

                float frameTime = FrameTimer.getTicks();
                if(frameTime > 1000.0f)
                {
                        std::stringstream convert;
                        convert << frame;
                        renderer.setWindowTitle(convert.str());

                        frame = 0;
                        FrameTimer.stop();
                        FrameTimer.start();
                }

        }

        renderer.cleanup();

        return 0;
}

#include <SDL2/SDL.h>
/*We need this weirdo define for the core profile gl3 stuff to work*/
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <sstream>

#include "Shader.h"
#include "MD2Model.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Camera.h"
#include "Timer.h"
#include "ObjLoader.h"
#include "Math.h"


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


        std::vector<struct ShaderList> list;
        list.push_back((struct ShaderList){GL_VERTEX_SHADER, "resources/shaders/shader_multilight.vert"});
        list.push_back((struct ShaderList){GL_FRAGMENT_SHADER, "resources/shaders/shader_multilight.frag"});

        renderer.initGL(list);

        //renderer.loadPrimitiveData(b.Vertices(), b.vsize(), b.getIndices(), b.isize(), colours, sizeof(colours), b.tsize(), b.getTextureCoords(), b.getNormals(), b.nsize());
        ObjLoader loader("resources/models/invCubeRev.obj");
        renderer.loadPrimitiveData(loader.getVertices(), loader.vsize(), NULL, 0, loader.getColours(), loader.csize(), loader.tsize(), loader.getTexCoords(), loader.getAvgNormals(), loader.nsize());
        renderer.loadTexture("resources/textures/cube_texture.png");
        renderer.loadTexture("resources/textures/normalmap2.png");

        //renderer.loadPrimitiveData(b.Vertices(), b.vsize(), b.getIndices(), b.isize(), b.Colours(), b.csize(), b.Normals(), b.nsize());
}

int main(int argc, char *argv[])
{
        renderer = Renderer(1280, 720);
        SDL_bool relativeMouse = SDL_FALSE;
        Timer GameTimer; //Keep track of time between game frames
        Timer FrameTimer; //Keep track of time between actual drawn frames
        Camera cam;

        Mat4<float> transMatrix(10.0f);
        transMatrix[3][2] = 0.0f;

        MD2Model skel;
        skel.loadModel("./hueteotl/tris.md2");
        bool quit = false;

        SDL_Event event;

        init();

        float rotStep = 0;

        GameTimer.start();
        float previousTime = GameTimer.getTicks();
        float currentTime = previousTime;

        const float FRAME_TIME = 1000.0f / MAX_FPS; //Amount of time each frame should take
        int frame = 0;
        int ticks = 0;
        float timeSinceLastFrame = 0;

        bool uncappedFps = false;

        FrameTimer.start();

        Light light = renderer.newLight();
        light.setAmbientLight(Vec4<GLfloat>(0.4f, 0.4f, 0.4f, 1.0f));
        light.setDiffuseLight(Vec4<GLfloat>(0.4f, 0.4f, 0.4f, 1.0f));
        light.setSpecularLight(Vec4<GLfloat>(1.0f, 1.0f, 1.0f, 1.0f));
        light.setIsEnabled(true);
        light.setPosition(Vec4<GLfloat>(0.0f, 1.0f, 0.0f, 0.0f));
        light.setShininess(1.0f);
        light.setConstAttenuation(0.5f);
        light.setIsPointLight(true);

        //light.enableDiffuse(true);
        //light.enableSpecular(true);

        renderer.updateLights(light);

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
                                case SDL_SCANCODE_O:
                                {
                                        renderer.diffuse = !renderer.diffuse;
                                        std::string diffuseState =  renderer.diffuse ? "On" : "Off";
                                        std::cout << "Toggled Diffuse:" << diffuseState << std::endl;
                                }
                                        break;
                                case SDL_SCANCODE_P:
                                {
                                        renderer.specular = !renderer.specular;
                                        std::string specularState = renderer.specular ? "On" : "Off";
                                        std::cout << "Toggled Specular:" << specularState << std::endl;
                                }
                                        break;
                                case SDL_SCANCODE_U:
                                {
                                        renderer.specularMode = !renderer.specularMode;
                                        std::string specularModeState = renderer.specularMode ? "Blin-Phong" : "Phong";
                                        std::cout << "Toggled Specular Mode:" << specularModeState << std::endl;
                                }
                                        break;
                                case SDL_SCANCODE_F2:
                                {
                                        uncappedFps = !uncappedFps;
                                        std::string fpsMode = uncappedFps ? "off" : "on";
                                        std::cout << "Toggled FPS cap:" << fpsMode << std::endl;
                                }
                                        break;
                                case SDL_SCANCODE_KP_PLUS:
                                {
                                        lightPos = lightPos + Vec4<float>(0.0f, 0.1f, 0.0f, 0.0f);
                                }
                                        break;
                                case SDL_SCANCODE_KP_MINUS:
                                {
                                        lightPos = lightPos + Vec4<float>(0.0f, -0.1f, 0.0f, 0.0f);
                                }
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

                        float radians = Math::toRadians(rotStep);

                        Vec4<float> newLightPos;
                        newLightPos[x] = lightPos[x];// * cos(radians) + lightPos[z] * sin(radians);
                        newLightPos[y] = lightPos[y];
                        newLightPos[z] = lightPos[z];// * (-sin(radians)) + lightPos[z] * cos(radians);

                        Vec4<float> newLightNormal;
                        newLightNormal[x] = lightNormal[x];// * cos(radians) + lightNormal[z] * sin(radians);
                        newLightNormal[y] = lightNormal[y] * cos(radians) + lightNormal[z] * sin(radians);
                        newLightNormal[z] = lightNormal[y] * (-sin(radians)) + lightNormal[z] * cos(radians);

                        //std::cout << "LightPos[z]=" << lightPos[z] << std::endl;
                        renderer.updateLightNormal(newLightNormal);
                        renderer.updateLightPosition(newLightPos);
                        renderer.updateCameraMatrix(cam.cameraMatrix());
                        renderer.updateCameraPosition(cam.getPosition());

                        Mat4<float> rotMatrix(1.0f);
                        Mat4<float> modelMatrix = rotMatrix * transMatrix;
                        renderer.updateModelMatrix(modelMatrix);

                        renderer.draw();

                        rotStep += 1.0f;
                        rotStep = rotStep < 360 ? rotStep : 0;

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

void checkGL()
{
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
}

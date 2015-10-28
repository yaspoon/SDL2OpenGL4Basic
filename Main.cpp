#include <SDL2/SDL.h>
/*We need this weirdo define for the core profile gl3 stuff to work*/
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <sstream>

#include "Shader.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Camera.h"
#include "Timer.h"
#include "ObjLoader.h"
#include "ObjMaterial.h"
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

bool init()
{
        bool retval = true;
        renderer.initSDL();

        Vec4<float> min(-10.0, -10.0, -10.0, 1.0f);
        Vec4<float> max(10.0, 10.0, 10.0, 1.0f);
        Box b(min, max);


        std::vector<struct ShaderList> list;
        list.push_back((struct ShaderList){GL_VERTEX_SHADER, "resources/shaders/shader_multilight.vert"});
        list.push_back((struct ShaderList){GL_FRAGMENT_SHADER, "resources/shaders/shader_multilight.frag"});

        if(renderer.initGL(list))
        {
                std::cout << "Initialised GL successfully" << std::endl;
        }
        else
        {
                retval = false;
                std::cout << "ERROR:Failed to initialise OpenGL" << std::endl;
        }

        return retval;
}

int main(int argc, char *argv[])
{
        renderer = Renderer(1280, 720);
        SDL_bool relativeMouse = SDL_FALSE;
        Timer GameTimer; //Keep track of time between game frames
        Timer FrameTimer; //Keep track of time between actual drawn frames
        Camera cam;

        Mat4<float> transMatrix(30.0f);
        transMatrix[3][2] = 0.0f;

        bool quit = false;

        SDL_Event event;

        if(init())
        {

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
                int lightIndex = 0;

                std::vector<Light> lights;
                std::vector<Material> materials;

                lights.insert(lights.end(), renderer.newLight(lights.size()));
                lights[0].setAmbientLight(Vec4<GLfloat>(0.3f, 0.3f, 0.3f, 1.0f));
                lights[0].setDiffuseLight(Vec4<GLfloat>(0.4f, 0.4f, 0.4f, 1.0f));
                lights[0].setSpecularLight(Vec4<GLfloat>(0.6f, 0.6f, 0.6f, 1.0f));
                lights[0].setIsEnabled(true);
                lights[0].enableDiffuse(true);
                lights[0].enableSpecular(true);
                lights[0].setPosition(Vec4<GLfloat>(0.0f, 10.0f, 0.0f, 0.0f));
                lights[0].setNormal(Vec4<GLfloat>(0.0f, 1.0f, 0.0f, 0.0f));
                lights[0].setConstAttenuation(1.0f);
                lights[0].setLinearAtten(0.0f);
                lights[0].setQuadAtten(1.0f);
                //lights[0].setIsPointLight(true);
                lights[0].setIsSpotLight(true);
                lights[0].setAngle(90.0f);
                lights[0].setSpotponent(16.0f);

                /*lights.insert(lights.end(), renderer.newLight(lights.size()));
                lights[1].setAmbientLight(Vec4<GLfloat>(0.4f, 0.4f, 0.4f, 1.0f));
                lights[1].setDiffuseLight(Vec4<GLfloat>(0.4f, 0.4f, 0.4f, 1.0f));
                lights[1].setSpecularLight(Vec4<GLfloat>(1.0f, 1.0f, 1.0f, 1.0f));
                lights[1].setIsEnabled(true);
                lights[1].enableDiffuse(true);
                lights[1].enableSpecular(true);
                lights[1].setPosition(Vec4<GLfloat>(5.0f, 5.0f, 0.0f, 0.0f));
                lights[1].setNormal(Vec4<GLfloat>(0.0f, 1.0f, 0.0f, 0.0f));
                //lights[1].setShininess(1.0f);
                lights[1].setConstAttenuation(1.0f);
                lights[1].setIsSpotLight(true);
                lights[1].setLinearAtten(1.0f);
                lights[1].setAngle(90.0f);
                lights[1].setSpotponent(16.0f);*/

                //materials.insert(materials.end(), renderer.newMaterial(materials.size()));
                /*materials[0].setEmission(Vec4<GLfloat>(1.0f, 0.0f, 0.0f, 0.0f));
                materials[0].setAmbient(Vec4<GLfloat>(1.0f, 1.0f, 1.0f, 1.0f));
                materials[0].setDiffuse(Vec4<GLfloat>(1.0f, 1.0f, 1.0f, 1.0f));
                materials[0].setSpecular(Vec4<GLfloat>(1.0f, 1.0f, 1.0f, 1.0f));
                materials[0].setShininess(16.0f);*/

                ObjLoader loader("resources/models/plane.obj");
                //renderer.loadPrimitiveData(loader.getVertices(), loader.vsize(), NULL, 0, loader.getColours(), loader.csize(), loader.tsize(), loader.getTexCoords(), loader.getAvgNormals(), loader.nsize());
                renderer.loadModel(loader);
                ObjLoader monkey("resources/models/monkey.obj");
                //renderer.loadPrimitiveData(sphere.getVertices(), sphere.vsize(), NULL, 0, sphere.getColours(), sphere.csize(), sphere.tsize(), sphere.getTexCoords(), sphere.getAvgNormals(), sphere.nsize());
                renderer.loadModel(monkey);

                ObjLoader texCube("resources/models/texCube.obj");
                renderer.loadModel(texCube);

                /*std::vector<ObjMaterial> mats = loader.getMaterials();
                //mats.insert(mats.end(), sphere.getMaterials().begin(), sphere.getMaterials().end());
                for(std::vector<ObjMaterial>::iterator it = mats.begin(); it != mats.end(); ++it)
                {
                        ObjMaterial mat = *it;
                        int i = materials.size();
                        materials.insert(materials.end(), renderer.newMaterial(materials.size()));
                        materials[i].loadObjMaterial(mat);

                        if(mat.getMapkd().length() != 0) //There is a texture string
                        {
                                std::cout << "Loading texture:" << mat.getMapkd() << std::endl;
                                renderer.loadTexture(mat.getMapkd().c_str());
                                materials[i].setHasTexture(true);
                        }
                        else
                        {
                                materials[i].setColour(Vec4<GLfloat>(1.0f, 1.0f, 1.0f, 1.0f));
                                materials[i].setHasTexture(false);
                        }

                }
                //renderer.loadTexture("resources/textures/normalmap2.png");
                renderer.updateMaterials(materials);*/
                renderer.updateLights(lights);
                renderer.setNumEnabledLights(lights.size());

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
                                                case SDL_SCANCODE_L:
                                                {
                                                        Light tmpLight = lights[lightIndex];
                                                        GLboolean isEnabled = !tmpLight.getIsEnabled();
                                                        lights[lightIndex].setIsEnabled(isEnabled);
                                                        renderer.updateLight(tmpLight);
                                                        std::string lightState = isEnabled ? "On" : "Off";
                                                        std::cout << "Toggled light:" << lightState << std::endl;
                                                }
                                                break;
                                                case SDL_SCANCODE_O:
                                                {
                                                        GLboolean diffuse =  !lights[lightIndex].getIsDiffuseEnabled();
                                                        lights[lightIndex].enableDiffuse(diffuse);
                                                        renderer.updateLight(lights[lightIndex]);
                                                        std::string diffuseState =  diffuse ? "On" : "Off";
                                                        std::cout << "Toggled Diffuse:" << diffuseState << std::endl;
                                                }
                                                break;
                                                case SDL_SCANCODE_P:
                                                {
                                                        GLboolean specular = !lights[lightIndex].getIsSpecularEnabled();
                                                        lights[lightIndex].enableSpecular(specular);
                                                        renderer.updateLight(lights[lightIndex]);
                                                        std::string specularState = specular ? "On" : "Off";
                                                        std::cout << "Toggled Specular:" << specularState << std::endl;
                                                }
                                                break;
                                                case SDL_SCANCODE_U:
                                                {
                                                        GLboolean specularMode = !lights[lightIndex].getSpecularMode();
                                                        lights[lightIndex].setSpecularMode(specularMode);
                                                        renderer.updateLight(lights[lightIndex]);
                                                        std::string specularModeState = specularMode ? "Blin-Phong" : "Phong";
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
                                                case SDL_SCANCODE_1:
                                                {
                                                        std::cout << "REMOVED: lights arn't shiny anymore...." << std::endl;
                                                }
                                                break;
                                                case SDL_SCANCODE_2:
                                                {
                                                        std::cout << "REMOVED: lights arn't shiny anymore...." << std::endl;
                                                }
                                                break;
                                                case SDL_SCANCODE_MINUS:
                                                {
                                                        lightIndex = lightIndex > 0 ? --lightIndex : lightIndex;
                                                        std::cout << "light[" << lightIndex << "] selected" << std::endl;
                                                }
                                                break;
                                                case SDL_SCANCODE_EQUALS:
                                                {
                                                        /*Yeah I'm using the ternery operator like an ass hat*/
                                                        lightIndex = lightIndex < (lights.size() - 1) ? ++lightIndex : lightIndex;
                                                        std::cout << "light[" << lightIndex << "] selected" << std::endl;
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

                                //std::cout << "LightPos[z]=" << lightPos[z] << std::endl;
                                //renderer.updateLights(lights);
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

        }
        else
        {
                std::cout << "ERROR:Failed to initialise exiting" << std::endl;
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

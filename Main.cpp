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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Box.h"

const GLfloat PI = 3.14;
enum vaoIds {TRIANGLES, NUM_VAOS};
enum bufferIds {ARRAY_BUFFER, NUM_BUFFERS};
enum indexIds{INDEXS, NUM_INDEXS};
//enum attribIds {vPosition = 0};
GLint vPosition;
GLint vColour;

GLuint vertArrays[NUM_VAOS];
GLuint buffers[NUM_BUFFERS];
GLuint ibo[NUM_INDEXS];
const GLuint NUM_VERTICES = 8;

GLfloat width = 640.0f;
GLfloat height = 640.0f;
GLfloat mf_aspectRatio = width / height;
const GLfloat mf_fov = 90.0f;
const GLfloat mf_near = 1.0f;
const GLfloat mf_far = 500.0f;
GLint projMatLocation;
GLint modelMatLocation;
GLint cameraMatLocation;

Mat4<float> modelMatrix;
Mat4<float> cameraMatrix;
Mat4<float> projectionMatrix;

float toRadians(float deg)
{
        return deg * (PI / 180.0f);
}

Mat4<float> frustumProjection(float left, float right, float top, float bottom, float near, float far)
{
        Mat4<float> m(1.0f);

        float A = (right + left) / (right - left);
        float B = (top + bottom) / (top - bottom);
        float C = -((far + near) / (far - near));
        float D = -((2 * far * near) / (far - near));

        m[0][0] = (2 * near) / (right - left);
        m[1][1] = (2 * near) / (top - bottom);
        m[2][0] = A;
        m[2][1] = B;
        m[2][2] = C;
        m[2][3] = -1.0f;
        m[3][2] = D;

        m[3][3] = 0.0f;

        return m;
}

Mat4<float> perspectiveProjection(float FOV, float aspectRatio, float near, float far)
{
        float right = near * tan(toRadians(FOV / 2));
        float left = -right;
        float top = right / aspectRatio;
        float bottom = left / aspectRatio;

        return frustumProjection(left, right, top, bottom, near, far);
}

void init()
{
        Vec4<float> min(-10.0, -10.0, -10.0, 1.0f);
        Vec4<float> max(10.0, 10.0, 10.0, 1.0f);
        Box b(min, max);

        glEnable(GL_MULTISAMPLE);

        projectionMatrix = perspectiveProjection(mf_fov, mf_aspectRatio, mf_near, mf_far);

        GLfloat colours[NUM_VERTICES][3] = {{1.0, 0.0, 0.0},
                                                                          {0.0, 1.0, 0.0},
                                                                          {0.0, 0.0, 1.0},
                                                                          {1.0, 1.0, 1.0},
                                                                          {1.0, 0.0, 0.0},
                                                                          {0.0, 1.0, 0.0},
                                                                          {0.0, 0.0, 1.0},
                                                                          {1.0, 1.0, 1.0}};

        glGenBuffers(NUM_INDEXS, ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[INDEXS]);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, b.isize(), b.getIndices(), GL_STATIC_DRAW);

        glGenVertexArrays(NUM_VAOS, vertArrays);
        glBindVertexArray(vertArrays[TRIANGLES]);

        glGenBuffers(NUM_BUFFERS, buffers);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[ARRAY_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, b.vsize() + sizeof(colours), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, b.vsize(), b.Vertices());
        glBufferSubData(GL_ARRAY_BUFFER, b.vsize(), sizeof(colours), colours);

        std::vector<struct ShaderList> list;
        list.push_back((struct ShaderList){GL_VERTEX_SHADER, "./shader.vert"});
        list.push_back((struct ShaderList){GL_FRAGMENT_SHADER, "./shader.frag"});

        GLuint program = Shader::LoadShaders(list);
        glUseProgram(program);

        vPosition = glGetAttribLocation(program, "vPosition");
        if(vPosition == -1)
        {
                std::cout << "Couldn't find vPosition in shader" << std::endl;
        }

        glVertexAttribPointer(vPosition, 3, GL_FLOAT,  GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(vPosition);

        vColour = glGetAttribLocation(program, "vColour");
        if(vColour == -1)
        {
                std::cout << "Couldn't find Colour  n in shader" << std::endl;
        }

        glVertexAttribPointer(vColour, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)b.vsize());
        glEnableVertexAttribArray(vColour);

        projMatLocation = glGetUniformLocation(program, "vprojectionMat");
        if(projMatLocation == -1)
        {
                std::cout << "Couldn't find vprojectionMat in shader" << std::endl;
        }

        modelMatLocation = glGetUniformLocation(program, "modelMatrix");
        if(modelMatLocation == -1)
        {
                std::cout << "Couldn't find modelMatrix in shader" << std::endl;
        }

        cameraMatLocation = glGetUniformLocation(program, "cameraMatrix");
        if(cameraMatLocation == -1)
        {
                std::cout << "Couldn't find modelMatrix in shader" << std::endl;
        }

        glUniformMatrix4fv(projMatLocation, 1, false, (float*)&projectionMatrix);

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
}

void draw()
{
        glUniformMatrix4fv(modelMatLocation, 1, false, &modelMatrix);

        glUniformMatrix4fv(cameraMatLocation, 1, false, &cameraMatrix);

        //std::cout << rotStep << std::endl;
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vertArrays[TRIANGLES]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[INDEXS]);

        //glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
}

int main(int argc, char *argv[])
{
        SDL_bool relativeMouse = SDL_FALSE;
        Timer frameTimer;
        Camera cam;
        modelMatrix = Mat4<float>(1.0f);

        Mat4<float> transMatrix(1.0f);
        transMatrix[3][2] = 10.0f;

        cameraMatrix = cam.cameraMatrix();

        MD2Model skel;
        skel.loadModel("./hueteotl/tris.md2");
        SDL_Window *window = NULL;
        SDL_GLContext context = NULL;
        bool quit = false;

        if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
        {
                window = SDL_CreateWindow("OpenGL 4.3 Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
                if(window != NULL)
                {
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
                        context = SDL_GL_CreateContext(window);
                        if(context != NULL)
                        {
                                int major;
                                int minor;
                                SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
                                SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
                                printf("Opengl context version %d.%d\n", major, minor);

                                int samples;
                                SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &samples);
                                printf("Multisamples %d\n", samples);

                                glViewport(0, 0, width, height);

                                SDL_Event event;

                                init();

                                int rotStep = 0;

                                frameTimer.start();
                                float previousTime = frameTimer.getTicks();
                                float currentTime = previousTime;

                                Vec4<float> b1(1.0, 1.0, 0.0, 1.0f);

                                projectionMatrix.print();

                                std::cout << "Camera:" << std::endl;
                                cameraMatrix.print();

                                Vec4<float> test = (transMatrix * b1);
                                Vec4<float> test2 = cameraMatrix * test;
                                Vec4<float> test3 = projectionMatrix * test2;
                                test.print();
                                test2.print();
                                test3.print();

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
                                                                SDL_DisplayMode dm;
                                                                SDL_GetDesktopDisplayMode(0, &dm);

                                                                width = dm.w;
                                                                height = dm.h;
                                                                mf_aspectRatio = width / height;

                                                                SDL_SetWindowDisplayMode(window, &dm);
                                                                glViewport(0, 0, width, height);
                                                                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

                                                                projectionMatrix = perspectiveProjection(mf_fov, mf_aspectRatio, mf_near, mf_far);
                                                                glUniformMatrix4fv(projMatLocation, 1, false, (float*)&projectionMatrix);
                                                        }
                                                        }
                                                }
                                                else if(event.type == SDL_MOUSEMOTION)
                                                {
                                                        cam.updatePitch((float)event.motion.yrel);
                                                        cam.updateYaw((float)event.motion.xrel);
                                                }
                                        }

                                        currentTime = frameTimer.getTicks();
                                        float dt = (float)(currentTime - previousTime) / 1000.0f;
                                        cam.update(dt);
                                        previousTime = currentTime;


                                        cameraMatrix = cam.cameraMatrix();

                                        Mat4<float> rotMatrix(1.0f);
                                        modelMatrix = rotMatrix * transMatrix;

                                        draw();

                                        rotStep += 1;
                                        rotStep %= 360;

                                        SDL_GL_SwapWindow(window);
                                }

                                SDL_GL_DeleteContext(context);
                        }
                        else
                        {
                                fprintf(stderr, "Failed to Create GL context:%s\n", SDL_GetError());
                        }

                        SDL_DestroyWindow(window);
                }
                else
                {
                        fprintf(stderr, "Failed to create SDL window%s\n", SDL_GetError());
                }
        }
        else
        {
                fprintf(stderr,"Failed to init SDL %s\n", SDL_GetError());
        }

        return 0;
}

#include <SDL2/SDL.h>
/*We need this weirdo define for the core profile gl3 stuff to work*/
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include <iostream>

#include "Shader.h"
#include "MD2Model.h"
#include "Vec4.h"
#include "Mat4.h"

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

const GLfloat mf_aspectRatio = 1.0f / 1.0f;
const GLfloat mf_fov = 90.0f;
const GLfloat mf_near = 1;
const GLfloat mf_far = 500;
GLint projMatLocation;
GLint modelMatLocation;

Mat4<float> modelMatrix;

float toRadians(float deg)
{
        return deg * (PI / 180.0f);
}

void init()
{
        Vec4<float> min(-0.5, -0.5, -0.5, 1.0f);
        Vec4<float> max(0.5, 0.5, 0.5, 1.0f);
        Box b(min, max);

        b.print();

        glEnable(GL_MULTISAMPLE);
        GLfloat tanHalfAngleRad = tan(toRadians(mf_fov / 2));
        GLfloat nf = (mf_near + mf_far) / (mf_near - mf_far);
        Vec4<GLfloat> col1(1.0f / tanHalfAngleRad, 0.0f, 0.0f, 0.0f);
        Vec4<GLfloat> col2(0.0f, mf_aspectRatio / tanHalfAngleRad, 0.0f, 0.0f);
        Vec4<GLfloat> col3(0.0f, 0.0f, nf, -1.0f);
        Vec4<GLfloat> col4(0.0f, 0.0f, nf, 0.0f);

        Mat4<GLfloat> projectionMatrix(col1, col2, col3, col4);

        GLfloat colours[NUM_VERTICES][3] = {{1.0, 0.0, 0.0},
                                                                          {0.0, 1.0, 0.0},
                                                                          {0.0, 0.0, 1.0},
                                                                          {1.0, 1.0, 1.0},
                                                                          {1.0, 0.0, 0.0},
                                                                          {0.0, 1.0, 0.0},
                                                                          {0.0, 0.0, 1.0},
                                                                          {1.0, 1.0, 1.0}};

        GLushort indices[] = {0, 1, 2, //Front face
                                     2, 3, 0,
                                     5, 4, 7, //Back Face
                                     7, 6, 5,
                                     1, 5, 6, //Right side
                                     6, 2, 1,
                                     4, 0, 3, //left side
                                     3, 7, 4,
                                     3,2, 6, //Top
                                     6, 7 ,3,
                                     4, 5, 1,//Bottom
                                     1, 0, 4};

        glGenBuffers(NUM_INDEXS, ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[INDEXS]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

        glUniformMatrix4fv(projMatLocation, 1, false, &projectionMatrix);

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);

}

void draw()
{
        glUniformMatrix4fv(modelMatLocation, 1, false, &modelMatrix);

        //std::cout << rotStep << std::endl;
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vertArrays[TRIANGLES]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[INDEXS]);

        //glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
}

int main(int argc, char *argv[])
{
        modelMatrix = Mat4<float>(1.0f);

        Mat4<float> transMatrix(1.0f);
        transMatrix[2][3] = -2.5f;

        MD2Model skel;
        skel.loadModel("./hueteotl/tris.md2");
        SDL_Window *window = NULL;
        SDL_GLContext context = NULL;
        bool quit = false;

        if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
        {
                window = SDL_CreateWindow("OpenGL 4.3 Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
                if(window != NULL)
                {
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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

                                glViewport(0, 0, 640, 480);

                                SDL_Event event;

                                init();

                                int rotStep = 0;

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
                                                        if(event.key.keysym.scancode == SDL_SCANCODE_Q)
                                                        {
                                                                quit = true;
                                                        }
                                                }
                                        }

                                        //modelMatrix = Mat4<float>(1.0f); //Reset model matrix to the identity matrix
                                        Mat4<float> rotMatrix(1.0f);
                                        float radians = (float)rotStep * (PI / 180.0f);
                                        rotMatrix[1][1] = cos(radians);
                                        rotMatrix[1][2] = -sin(radians);
                                        rotMatrix[2][1] = sin(radians);
                                        rotMatrix[2][2] = cos(radians);

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

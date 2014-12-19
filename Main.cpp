#include <SDL2/SDL.h>
/*We need this weirdo define for the core profile gl3 stuff to work*/
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include <iostream>

#include "Shader.h"
#include "MD2Model.h"

enum vaoIds {TRIANGLES, NUM_VAOS};
enum bufferIds {ARRAY_BUFFER, NUM_BUFFERS};
enum attribIds {vPosition = 0};

GLuint vertArrays[NUM_VAOS];
GLuint buffers[NUM_BUFFERS];
const GLuint NUM_VERTICES = 6;

GLint rotStepLocation;

void init()
{
        GLfloat vertices[NUM_VERTICES][2] = {{-0.50, -0.50}, //Triangle 1
                                                                           {0.50, -0.50},
                                                                           {-0.50, 0.50},
                                                                           {0.50, -0.50}, //Triangle 2
                                                                           {0.50, 0.50},
                                                                           {-0.50, 0.50}};

        glGenVertexArrays(NUM_VAOS, vertArrays);
        glBindVertexArray(vertArrays[TRIANGLES]);

        glGenBuffers(NUM_BUFFERS, buffers);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[ARRAY_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        std::vector<struct ShaderList> list;
        list.push_back((struct ShaderList){GL_VERTEX_SHADER, "./shader.vert"});
        list.push_back((struct ShaderList){GL_FRAGMENT_SHADER, "./shader.frag"});

        GLuint program = Shader::LoadShaders(list);
        glUseProgram(program);

        glVertexAttribPointer(vPosition, 2, GL_FLOAT,  GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(vPosition);

        rotStepLocation = glGetUniformLocation(program, "rotStep");

        GLfloat pointSize = 0.0f;
        glGetFloatv(GL_POINT_SIZE, &pointSize);
        std::cout << "Point Size:" << pointSize << std::endl;

        GLfloat pointRange[2];
        glGetFloatv(GL_POINT_SIZE_RANGE, pointRange);
        std::cout << "Point Range from " << pointRange[0] << " to " << pointRange[1] << std::endl;

        glPointSize(20.0f);

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);

}

void draw(int rotStep)
{
        glUniform1i(rotStepLocation, rotStep);

        //std::cout << rotStep << std::endl;
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vertArrays[TRIANGLES]);

        glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
}

int main(int argc, char *argv[])
{
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
                        context = SDL_GL_CreateContext(window);
                        if(context != NULL)
                        {
                                int major;
                                int minor;
                                SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
                                SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
                                printf("Opengl context version %d.%d\n", major, minor);

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

                                        draw(rotStep);

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

bool glInit()
{
        bool retval = false;

}

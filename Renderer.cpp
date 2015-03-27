#include "Renderer.h"
#include "Common.h"
#include "Math.h"

Renderer::Renderer():
modelMatrix(1.0f), projectionMatrix(1.0f), cameraMatrix(1.0f)
{
        title = "OpenGL 4.3 Test";

        mf_width = 640.0f;
        mf_height = 480.0f;
        mf_aspectRatio = mf_width / mf_height;
        mf_fov = 90.0f;
        mf_near = 1.0f;
        mf_far = 500.0f;

        projMatLocation = -1;
        cameraMatLocation = -1;
        modelMatLocation = -1;
        angle = 0.0f;

        projectionMatrix = perspectiveProjection(mf_fov, mf_aspectRatio, mf_near, mf_far);
}

Renderer::Renderer(float width, float height):
modelMatrix(1.0f), projectionMatrix(1.0f), cameraMatrix(1.0f)
{
        title = "OpenGL 4.3 Test";

        mf_width = width;
        mf_height = height;
        mf_aspectRatio = mf_width / mf_height;
        mf_fov = 90.0f;
        mf_near = 1.0f;
        mf_far = 500.0f;

        projMatLocation = -1;
        cameraMatLocation = -1;
        modelMatLocation = -1;
        angle = 0.0f;

        projectionMatrix = perspectiveProjection(mf_fov, mf_aspectRatio, mf_near, mf_far);
}

Renderer::Renderer(float width, float height, float near, float far, float fov):
modelMatrix(1.0f), projectionMatrix(1.0f), cameraMatrix(1.0f)
{
        title = "OpenGL 4.3 Test";

        mf_width = width;
        mf_height = height;
        mf_aspectRatio = mf_width / mf_height;
        mf_fov = fov;
        mf_near = near;
        mf_far = far;

        projMatLocation = -1;
        cameraMatLocation = -1;
        modelMatLocation = -1;
        angle = 0.0f;

        projectionMatrix = perspectiveProjection(mf_fov, mf_aspectRatio, mf_near, mf_far);
}

Mat4<float> Renderer::frustumProjection(float left, float right, float top, float bottom, float near, float far)
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

Mat4<float> Renderer::perspectiveProjection(float FOV, float aspectRatio, float near, float far)
{
        float right = near * tan(Math::toRadians(FOV / 2));
        float left = -right;
        float top = right / aspectRatio;
        float bottom = left / aspectRatio;

        return frustumProjection(left, right, top, bottom, near, far);
}

bool Renderer::initSDL()
{
        if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
        {
                window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mf_width, mf_height, SDL_WINDOW_OPENGL);
                if(window != NULL)
                {
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
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
                        }
                }
                else
                {
                        STUB("Implement proper error handling");
                }
        }
        else
        {
                STUB("Implement proper error stuff");
        }
}

void Renderer::initGL(std::vector<struct ShaderList> list)
{
        glViewport(0, 0, mf_width, mf_height);

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);

        program = loadProgram(list);
        glUseProgram(program);

        glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, &projectionMatrix);

        glGenBuffers(NUM_INDEXS, ibo);
        glGenVertexArrays(NUM_VAOS, vertArrays);
        glGenBuffers(NUM_BUFFERS, buffers);
}

void Renderer::cleanup()
{
        if(context != NULL)
        {
                SDL_GL_DeleteContext(context);
        }

        if(window != NULL)
        {
                SDL_DestroyWindow(window);
        }

        SDL_Quit();
}

void Renderer::setWindowTitle(std::string title)
{
        std::string newTitle = this->title + " FPS:" + title;
        SDL_SetWindowTitle(window, newTitle.c_str());
}

GLint Renderer::loadProgram(std::vector<struct ShaderList> list)
{
        GLint program = Shader::LoadShaders(list);

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

        camPositionLocation = glGetUniformLocation(program, "camPosition");
        if(camPositionLocation == -1)
        {
                std::cout << "Couldn't find modelMatrix in shader" << std::endl;
        }

        angleLocation = glGetUniformLocation(program, "angle");
        if(angle == -1)
        {
                std::cout << "Failed to find angle in shader" << std::endl;
        }

        return program;
}

void Renderer::useProgram(GLint program)
{
        glUseProgram(program);
}

void Renderer::setProjectionMatrix(Mat4<float> projMat)
{
        projectionMatrix = projMat;
}

void Renderer::updateCameraMatrix(Mat4<float> camMat)
{
        cameraMatrix = camMat;
}

void Renderer::updateModelMatrix(Mat4<float> modMat)
{
        modelMatrix = modMat;
}

void Renderer::updateCameraPosition(Vec4<float> camDir)
{
        camPosition = camDir;
}

void Renderer::draw()
{
        glUniformMatrix4fv(modelMatLocation, 1, false, &modelMatrix);

        glUniformMatrix4fv(cameraMatLocation, 1, false, &cameraMatrix);

        glUniform1f(angleLocation, angle);

        glUniform3f(camPositionLocation, camPosition[x], camPosition[y], camPosition[z]);

        angle = (angle += 1) <= 360 ? angle : 0.0f;

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vertArrays[TRIANGLES]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[INDEXS]);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        SDL_GL_SwapWindow(window);
}

void Renderer::loadPrimitiveData(float *vertices, size_t vsize, unsigned short *indices, size_t icount, float *colours, size_t csize, float *normals, size_t nsize)
{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[INDEXS]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount, indices, GL_STATIC_DRAW);


        glBindVertexArray(vertArrays[TRIANGLES]);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[ARRAY_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER,  vsize + csize + nsize, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, vertices);
        glBufferSubData(GL_ARRAY_BUFFER, vsize, csize, colours);
        glBufferSubData(GL_ARRAY_BUFFER, vsize + csize, nsize, normals);

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

        glVertexAttribPointer(vColour, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(vsize));
        glEnableVertexAttribArray(vColour);

        vNormal = glGetAttribLocation(program, "vNormal");
        if(vNormal == -1)
        {
                std::cout << "Failed to find vNormal in shader" << std::endl;
        }

        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(vsize + csize));
        glEnableVertexAttribArray(vNormal);
}

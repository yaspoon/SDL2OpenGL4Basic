#include "Renderer.h"
#include "Common.h"
#include "Math.h"
#include <SDL2/SDL_image.h>
#include <iomanip>

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
        bool retval = false;

        if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
        {
                window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mf_width, mf_height, SDL_WINDOW_OPENGL);
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

                                retval = true;
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

        return retval;
}

bool Renderer::initGL(std::vector<struct ShaderList> list)
{
        bool retval = true;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
        std::cout << "Max texture units:" << maxTextureUnits << std::endl;
        glViewport(0, 0, mf_width, mf_height);

        glDepthRange(mf_near, mf_far); /*Tell opengl where the near and far planes sit*/

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);

        //glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LESS);

        std::pair<GLuint, bool> programPair = loadProgram(list);

        if(programPair.second)
        {
                program = programPair.first;
                glUseProgram(program);

                glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, &projectionMatrix);

                //glGenBuffers(NUM_INDEXS, ibo);
                //glGenVertexArrays(NUM_VAOS, vertArrays);
                //glGenBuffers(NUM_BUFFERS, buffers);
        }
        else
        {
                retval = false;
        }

        return retval;
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

std::pair<GLint, bool> Renderer::loadProgram(std::vector<struct ShaderList> list)
{
        std::pair<GLuint, bool> programLink = shader.LoadShaders(list);
        program = programLink.first;
        bool retval = false;

        if(!programLink.second)
        {

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
                        std::cout << "Couldn't find cameraMatrix in shader" << std::endl;
                }

                camPositionLocation = glGetUniformLocation(program, "camPosition");
                if(camPositionLocation == -1)
                {
                        std::cout << "Couldn't find camPosition in shader" << std::endl;
                }

                textureLocation = glGetUniformLocation(program, "tex");
                if(textureLocation == -1)
                {
                        std::cout << "Failed to find tex in shader" << std::endl;
                }

                normalMapLocation = glGetUniformLocation(program, "normalMap");
                if(normalMapLocation == -1)
                {
                        std::cout << "Failed to find normalMap in shader" << std::endl;
                }

                GLint activeUniforms;
                glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms);
                std::cout << "Active uniform count:" << activeUniforms << std::endl;

                uboIndex = glGetUniformBlockIndex(program, "UniformBlock");
                if(uboIndex == GL_INVALID_INDEX)
                {
                        std::cout << "invalid index" << std::endl;
                }
                glGetActiveUniformBlockiv(program, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);

                glGetUniformIndices(program, numUniforms, shaderUniforms, uniformIndices);
                glGetActiveUniformsiv(program, numUniforms, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);
                glGetActiveUniformsiv(program, numUniforms, uniformIndices, GL_UNIFORM_SIZE, uniformSizes);
                glGetActiveUniformsiv(program, numUniforms, uniformIndices, GL_UNIFORM_TYPE, uniformType);
                GLint strides[numUniforms];

                glGetActiveUniformsiv(program, numUniforms, uniformIndices, GL_UNIFORM_ARRAY_STRIDE, strides);

                glGetUniformIndices(program, numMatUniforms, materialUniforms, matUniformIndices);
                glGetActiveUniformsiv(program, numMatUniforms, matUniformIndices, GL_UNIFORM_OFFSET, matUniformOffsets);
                glGetActiveUniformsiv(program, numMatUniforms, matUniformIndices, GL_UNIFORM_SIZE, matUniformSizes);
                glGetActiveUniformsiv(program, numMatUniforms, matUniformIndices, GL_UNIFORM_TYPE, matUniformType);

                GLint matStrides[numMatUniforms];
                glGetActiveUniformsiv(program, numMatUniforms, matUniformIndices, GL_UNIFORM_ARRAY_STRIDE, matStrides);

                uboStride = matUniformOffsets[0];
                matUboOffset = uboStride;
                matUboStride = (uboSize - uboStride) / MAX_MATERIALS;
                uboStride = uboStride / MAX_LIGHTS;

                glGenBuffers(1, &ubo);
                glBindBuffer(GL_UNIFORM_BUFFER, ubo);
                glBufferData(GL_UNIFORM_BUFFER, uboSize, NULL, GL_STATIC_DRAW); //We'll init the buffer later?

                numEnabledLightsLocation = glGetUniformLocation(program, "numEnabledLights");
                if(numEnabledLightsLocation == -1)
                {
                        std::cout << "Failed to find numEnabledLights in shader" << std::endl;
                }

                retval = true;
        }

        return std::pair<GLuint, bool>(program, retval);
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

void Renderer::updateLight(Light updatedLight)
{
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        GLintptr offset = updatedLight.getIndex() * uboStride;
        GLsizei size = updatedLight.getDataSize();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, updatedLight.getLightData());
        glBindBufferBase(GL_UNIFORM_BUFFER, uboIndex, ubo);
}

void Renderer::updateLights(std::vector<Light> lights)
{
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        for( std::vector<Light>::iterator it = lights.begin(); it != lights.end(); ++it)
        {
                Light light = *it;
                GLintptr offset = light.getIndex() * uboStride;
                GLsizei size = light.getDataSize();
                char *data = light.getLightData();
                glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);// light.getLightData());
                glBindBufferBase(GL_UNIFORM_BUFFER, uboIndex, ubo);
        }
}

void Renderer::updateMaterials(std::vector<Material> materials)
{
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        for(std::vector<Material>::iterator it = materials.begin(); it != materials.end(); ++it)
        {
                Material material = *it;
                GLintptr offset = (material.getIndex() * matUboStride) + matUboOffset;
                GLsizei size = material.getDataSize();
                char *data = material.getMaterialData();
                glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
                glBindBufferBase(GL_UNIFORM_BUFFER, uboIndex, ubo);
        }
}

void Renderer::draw()
{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glUniform1i(textureLocation, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glUniform1i(normalMapLocation, 1);

        glUniformMatrix4fv(modelMatLocation, 1, false, &modelMatrix);

        glUniformMatrix4fv(cameraMatLocation, 1, false, &cameraMatrix);

        glUniform3f(camPositionLocation, camPosition[x], camPosition[y], camPosition[z]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(int i = 0; i < vertexArrays.size(); i++)
        {
                GLuint vao = vertexArrays.at(i);
                int triangleCount = triangleCounts.at(i);
                glBindVertexArray(vao);
                //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[INDEXS]);

                glDrawArrays(GL_TRIANGLES, 0, triangleCount);
        }

        SDL_GL_SwapWindow(window);
}

void Renderer::loadPrimitiveData(float *vertices, size_t vsize, unsigned short *indices, size_t icount, float *colours, size_t csize, size_t tsize, float *texCoords, float *normals, size_t nsize)
{

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        vertexArrays.push_back(vao);
        int triangleCount = vsize / sizeof(*vertices) / 3;
        triangleCounts.push_back(triangleCount);

        size_t bufSize = vsize + csize + tsize + nsize;

        if(indices)
        {
                GLuint ibo;
                glGenBuffers(1, &ibo);
                ibos.push_back(ibo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount, indices, GL_STATIC_DRAW);
        }

        GLuint buffer;
        glGenBuffers(1, &buffer);
        buffers.push_back(buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER,  bufSize, NULL, GL_STATIC_DRAW);

        size_t total = 0.0f;

        if(vertices)
        {
                glBufferSubData(GL_ARRAY_BUFFER, total, vsize, vertices);

                vPosition = glGetAttribLocation(program, "vPosition");
                if(vPosition == -1)
                {
                        std::cout << "Couldn't find vPosition in shader" << std::endl;
                }
                glVertexAttribPointer(vPosition, 3, GL_FLOAT,  GL_FALSE, 0, (GLvoid*)total);
                glEnableVertexAttribArray(vPosition);
                total += vsize;
        }

        if(colours)
        {
                glBufferSubData(GL_ARRAY_BUFFER, total, csize, colours);
                vColour = glGetAttribLocation(program, "vColour");
                if(vColour == -1)
                {
                        std::cout << "Couldn't find Colour in shader" << std::endl;
                }
                glVertexAttribPointer(vColour, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)total);
                glEnableVertexAttribArray(vColour);
                total += csize;
        }

        if(tsize)
        {
                glBufferSubData(GL_ARRAY_BUFFER, total, tsize, texCoords);

                GLint texAttrib = glGetAttribLocation(program, "vTexCoord");
                if(texAttrib == -1)
                {
                        std::cout << "Couldn't find tCoord in shader" << std::endl;
                }
                glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)total);
                glEnableVertexAttribArray(texAttrib);
                total += tsize;
        }

        if(normals)
        {
                glBufferSubData(GL_ARRAY_BUFFER, total, nsize, normals);
                vNormal = glGetAttribLocation(program, "vNormal");
                if(vNormal == -1)
                {
                        std::cout << "Failed to find vNormal in shader" << std::endl;
                }

                glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(total));
                glEnableVertexAttribArray(vNormal);
                total += nsize;
        }
}

void Renderer::loadTexture(const char *name)
{
        SDL_Surface *image = IMG_Load(name);
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        textures.push_back(texture);
}

void Renderer::loadTest()
{
        unsigned short indexs[] = {0, 1, 2, //Front face
                             2, 3, 0};

        float coords[] = {0, 0, 1, 0, 1, 1, 0, 1};

        float vertices[] = {-5.0f, -5.0f, 0.0f,
                                        5.0f, -5.0f, 0.0f,
                                        5.0f, 5.0f, 0.0f,
                                        -5.0f, 5.0f, 0.0f};

        glBindVertexArray(vertArrays[TRIANGLES]);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[INDEXS]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[ARRAY_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(coords), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(coords), coords);

        GLint vPos = glGetAttribLocation(program, "vPosition");
        glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
        glEnableVertexAttribArray(vPos);

        GLint tCoord = glGetAttribLocation(program, "tCoord");
        glVertexAttribPointer(tCoord, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertices));
        glEnableVertexAttribArray(tCoord);

        GLuint texture;
        glGenTextures(1, &texture);
        SDL_Surface *crate = IMG_Load("basic2.png");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, crate->w, crate->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, crate->pixels);
        glUniform1i(glGetUniformLocation(program, "tex"), 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderer::setTriangleCount(float triangles)
{
        triangleCount = triangles;
}

void Renderer::setNumEnabledLights(int count)
{
        glUniform1i(numEnabledLightsLocation, count);
}

Light Renderer::newLight(int index)
{
        return Light(index, uboStride, numUniforms, shaderUniforms, uniformIndices, uniformSizes, uniformOffsets, uniformType);
}

Material Renderer::newMaterial(int index)
{
        return Material(index, matUboStride, numMatUniforms, materialUniforms, matUniformIndices,
                        matUniformSizes, matUniformOffsets, matUniformType, matUboOffset);
}

const int Renderer::getMaxLights()
{
        return MAX_LIGHTS;
}

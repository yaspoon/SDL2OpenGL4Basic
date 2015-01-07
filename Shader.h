#ifndef SHADER_H
#define SHADER_H

#include <string>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include <vector>

struct ShaderList
{
        GLenum type;
        std::string path;
};

class Shader
{
private:
        static std::string readText(std::string filepath);
        static void validateShader(GLuint shader, const char* file=0);
        static bool validateProgram(GLuint program);
public:
        Shader();
        Shader(std::string filepath);
        static GLuint LoadShaders(std::vector<struct ShaderList> shaders);
};
#endif // SHADER_H

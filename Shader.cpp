#include "Shader.h"
#include <fstream>
#include <iostream>
#include <string.h>

Shader::Shader()
{

}

Shader::Shader(std::string filepath)
{
        std::cout << readText(filepath) << std::endl;
}

std::string Shader::readText(std::string filepath)
{
        std::ifstream input(filepath.c_str(), std::ios::in);
        std::string retval;

        if(input.is_open())
        {
                input.seekg(0, input.end);
                int length = input.tellg();
                input.seekg(0,input.beg);

                char *buffer = new char[length + 1];
                input.read(buffer, length);
                buffer[length] = '\0';

                retval.insert(0, buffer); //Insert buffer string  into retval string at the start position 0

                input.close();
        }
        else
        {
                std::cout << "Failed to open file:" << filepath << std::endl;
        }

        return retval;
}

GLuint Shader::LoadShaders(std::vector<struct ShaderList> shaders)
{
        GLuint program = glCreateProgram();
        std::vector<GLuint> compiled_shaders;
        for(std::vector<struct ShaderList>::iterator it = shaders.begin(); it != shaders.end(); it++)
        {
                struct ShaderList current = *it;
                GLuint shaderid = glCreateShader(current.type);
                std::string shaderText = readText(current.path);
                GLint length = shaderText.size();
                const char *blah = shaderText.c_str();

                glShaderSource(shaderid, 1, &blah, 0);
                glCompileShader(shaderid);
                validateShader(shaderid, current.path.c_str());
                glAttachShader(program, shaderid);
        }

        glLinkProgram(program);

        return program;
}

void Shader::validateShader(GLuint shader, const char* file)
{
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

    if(length > 0)
    {
        std::cerr << "Shader" << shader << (file? file : "") << " compile error: " << buffer << std::endl;
    }
}

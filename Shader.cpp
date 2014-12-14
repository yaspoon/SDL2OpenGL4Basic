#include "Shader.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>

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
        validateProgram(program);

        return program;
}

void Shader::validateShader(GLuint shader, const char* file)
{
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(status == GL_TRUE)
        {
                std::cout << "Loaded \"" << file << "\" successfully" << std::endl;
        }
        else
        {
                GLsizei logSize;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
                GLchar *buffer = new GLchar[logSize];
                GLsizei length = 0;

                glGetShaderInfoLog(shader, logSize, &length, buffer);

                if(length > 0)
                {
                        std::cerr << "Shader:" << shader << (file? file : "") << " compile error: " << buffer << std::endl;
                }
        }
}

bool Shader::validateProgram(GLuint program)
{
        bool retval = true;
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if(status == GL_TRUE)
        {
                retval =- true;
                std::cout << "Successfully linked shaders into program" << std::endl;
        }
        else
        {
                GLsizei logLength;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
                GLsizei length = 0;
                GLchar *buffer = new GLchar[logLength];
                glGetProgramInfoLog(program, logLength, &length, buffer);
                std::cout << "Program failed to link: " << buffer << std::endl;
        }

        return retval;
}

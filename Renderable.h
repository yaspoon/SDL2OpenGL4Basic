#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Renderable
{
        public:
                Renderable();
                virtual ~Renderable();
                GLuint getVao();
        protected:
        private:
                GLuint vao;
                GLuint vbo;
                GLuint tex;
                std::vector<GLfloat> vertices;
                std::vector<GLushort> indices; /*Index drawing is best drawing.*/
                std::vector<GLfloat> colours;
                std::vector<GLfloat> texCoords;
                SDL_Surface *texture;
};

#endif // RENDERABLE_H

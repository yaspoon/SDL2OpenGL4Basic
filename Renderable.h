#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "ObjMaterial.h"

class Renderable
{
        public:
                Renderable();
                Renderable(GLuint vao, std::vector<ObjMaterial> mats);
                virtual ~Renderable();
                GLuint getVao();
        protected:
        private:
                GLuint vao;
                std::vector<GLuint> textures;
};

#endif // RENDERABLE_H

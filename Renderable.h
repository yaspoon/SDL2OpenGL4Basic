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
                Renderable(GLuint vao, int triangleCount);
                Renderable(GLuint vao, std::vector<int> mats, int triangleCount);
                virtual ~Renderable();
                GLuint getVao();
                std::vector<int> getMaterials();
                int getTriangleCount();
                std::vector<GLuint> getTextures();
                void addMaterial(int index);
                void addTexture(GLuint texture);
                void setTriangleCount(int triangleCount);

        protected:
        private:
                GLuint vao;
                int triangleCount;
                std::vector<GLuint> textures;
                std::vector<int> materials;
};

#endif // RENDERABLE_H

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "ObjMaterial.h"
#include "Material.h"

class Renderable
{
        public:
                Renderable();
                Renderable(GLuint vao, int triangleCount);
                Renderable(GLuint vao, std::vector<Material> mats, int triangleCount);
                virtual ~Renderable();
                GLuint getVao();
                std::vector<Material> getMaterials();
                int getTriangleCount();
                std::vector<GLuint> getTextures();
                void addMaterial(Material mat);
                void addTexture(GLuint texture);
                void setTriangleCount(int triangleCount);

        protected:
        private:
                GLuint vao;
                int triangleCount;
                std::vector<GLuint> textures;
                std::vector<Material> materials;
};

#endif // RENDERABLE_H

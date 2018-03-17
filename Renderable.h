#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "ObjMaterial.h"
#include "Material.h"
#include "Mat4.h"

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
		void setModelMatrix(Mat4<float> newModelMatrix);
		Mat4<float> getModelMatrix();

        protected:
        private:
                GLuint vao;
                int triangleCount;
                std::vector<GLuint> textures;
                std::vector<Material> materials;
		Mat4<float> modelMatrix;
};

#endif // RENDERABLE_H

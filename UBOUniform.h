#ifndef UBO_UNIFORM_H
#define UBO_UNIFORM_H

#include <SDL2/SDL_opengl.h>

class UBOUniform
{
        public:
                UBOUniform();
                UBOUniform(GLuint index, GLint size, GLint offset, GLint type);
                virtual ~UBOUniform();
                GLuint getIndex();
                GLint getSize();
                GLint getOffset();
                GLint getType();
        protected:
        private:
                GLuint index;
                GLint size;
                GLint offset;
                GLint type;
};

#endif // LIGHT_H

#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
private:
        GLfloat width = 640.0f;
        GLfloat height = 640.0f;
        GLfloat mf_aspectRatio = width / height;
        const GLfloat mf_fov = 90.0f;
        const GLfloat mf_near = 1.0f;
        const GLfloat mf_far = 500.0f;
        GLint projMatLocation;
        GLint modelMatLocation;
        GLint cameraMatLocation;

        Mat4<float> modelMatrix;
        Mat4<float> projectionMatrix;
        Mat4<float> cameraMatrix;
public:
        Renderer();
        Renderer(float width, float height);
        Renderer(float width, float height, float near, float far);
};
#endif

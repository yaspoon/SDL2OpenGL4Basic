#include "Camera.h"

Camera::Camera()
{
        position[x] = 0.0f;
        position[y] = 0.0f;
        position[z] = 0.0f;

        direction[x] = 0.0f;
        direction[y] = 0.0f;
        direction[z] = 0.0f;
}

void Camera::update(float dt)
{
        position[x] += (direction[x] * speed) * dt; //Direction is 0 when we're not moving that that direction
        position[y] += (direction[y] * speed) * dt; //Direction is 0 when we're not moving that that direction
        position[z] += (direction[z] * speed) * dt; //Direction is 0 when we're not moving that that direction
}

Mat4<float> Camera::cameraMatrix()
{
        Mat4<float> camMatrix(1.0f);
        camMatrix[0][3] = -position[x];
        camMatrix[1][3] = -position[y];
        camMatrix[2][3] = -position[z];
        camMatrix[2][2] = -1.0f;

        return camMatrix;
}

void Camera::dirx(float dir)
{
        direction[x] = dir;
}

void Camera::diry(float dir)
{
        direction[y] = dir;
}

void Camera::dirz(float dir)
{
        direction[z] = dir;
}

#include "Camera.h"
#include "Math.h"
#include "math.h"

float mouseSensitivity = 0.1f;

Camera::Camera()
{
        position[x] = 0.0f;
        position[y] = 0.0f;
        position[z] = 0.0f;
        position[w] = 1.0f;

        direction[x] = 0.0f;
        direction[y] = 0.0f;
        direction[z] = 1.0f;
        direction[w] = 0.0f;

        up[x] = 0.0f;
        up[y] = 1.0f;
        up[z] = 0.0f;
        up[w] = 0.0f;

        pitch = 0.0f;
        yaw = 0.0f;
        roll = 0.0f;
}

void Camera::update(float dt)
{
        direction[x] = sin(Math::toRadians(yaw));
        direction[y] = sin(Math::toRadians(pitch));
        direction[z] = cos(Math::toRadians(yaw));

        float length = direction.length();

        Vec4<float> right = up.crossProduct(direction);
        right = right.normalise();

        Vec4<float> velocity = (direction * movedir[z]) + (right * movedir[x]);

        position = position + velocity * dt;
}

Mat4<float> Camera::cameraMatrix()
{
        Mat4<float> camMatrix(1.0f);

        Vec4<float> camSide = up.crossProduct(direction);
        camSide = camSide.normalise();
        Vec4<float> camUp = direction.crossProduct(camSide);
        camUp = camUp.normalise();

        camMatrix = camMatrix.setForward(Vec4<float>(camSide[x], camUp[x], -direction[x], 1.0f));
        camMatrix = camMatrix.setUp(Vec4<float>(camSide[y], camUp[y], -direction[y], 1.0f));
        camMatrix = camMatrix.setRight(Vec4<float>(camSide[z], camUp[z], -direction[z], 1.0f));

        Mat4<float> trans(1.0f);
        trans = trans.setTranslation(-position);
        trans[3][2] = trans[3][2] + 1.0f;

        camMatrix = trans * camMatrix;

        return camMatrix;
}

void Camera::dirx(float dir)
{
        movedir[x] = dir * speed;
}

void Camera::diry(float dir)
{
        movedir[y] = dir;
}

void Camera::dirz(float dir)
{
        movedir[z] = dir * speed;
}

void Camera::updatePitch(float newMouse)
{
        pitch -= (newMouse * mouseSensitivity);

        if(pitch > 89.0f)
        {
                pitch = 89.0f;
        }
        else if(pitch < -89.0f)
        {
                pitch = -89.0f;
        }
}

void Camera::updateYaw(float newMouse)
{
        yaw += (newMouse * mouseSensitivity);
        if(yaw > 180.0f)
        {
                yaw -= 360.0f;
        }
        else if(yaw < -180.0f)
        {
                yaw += 360.0f;
        }
}

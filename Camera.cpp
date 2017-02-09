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

        up[x] = 0.0f;
        up[y] = 1.0f;
        up[z] = 0.0f;
        up[w] = 0.0f;

}

void Camera::update(float dt)
{
        direction.normalise();
        Vec4<float> tmpDir = direction.toVector().normalise();

        Vec4<float> right = up.crossProduct(tmpDir);
        right = right.normalise();

        Vec4<float> velocity = (tmpDir * movedir[z]) + (right * movedir[x]);

        position = position + velocity * dt;
}

Mat4<float> Camera::cameraMatrix()
{
        Mat4<float> camMatrix(1.0f);
        direction.normalise();
        Vec4<float> tmpDir = direction.toVector().normalise();

        Vec4<float> camSide = up.crossProduct(tmpDir);
        camSide = camSide.normalise();
        Vec4<float> camUp = tmpDir.crossProduct(camSide);
        camUp = camUp.normalise();

        camMatrix = camMatrix.setForward(Vec4<float>(camSide[x], camUp[x], -tmpDir[x], 0.0f));
        camMatrix = camMatrix.setUp(Vec4<float>(camSide[y], camUp[y], -tmpDir[y], 0.0f));
        camMatrix = camMatrix.setRight(Vec4<float>(camSide[z], camUp[z], -tmpDir[z], 0.0f));

        Mat4<float> trans(1.0f);
        trans = trans.setTranslation(-position);
        trans[3][2] = trans[3][2] + 1.0f;

        camMatrix = trans * camMatrix;

        return camMatrix;
}

Vec4<float> Camera::getDirection()
{
        return direction.toVector();
}

Vec4<float> Camera::getPosition()
{
        return position;
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
        direction.add2Pitch(-newMouse * mouseSensitivity);

}

void Camera::updateYaw(float newMouse)
{
        direction.add2Yaw(newMouse * mouseSensitivity);
}

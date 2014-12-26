#ifndef CAMERA_H
#define CAMERA_H

#include "Vec4.h"
#include "Mat4.h"

class Camera
{
private:
        static const int speed = 5;
        Vec4<float> position;
        Vec4<float> direction;
public:
        Camera();
        void update(float dt);
        Mat4<float> cameraMatrix();

        void dirx(float dir);
        void diry(float dir);
        void dirz(float dir);
};
#endif // CAMERA_H

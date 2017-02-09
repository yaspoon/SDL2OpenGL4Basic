#ifndef CAMERA_H
#define CAMERA_H

#include "Vec4.h"
#include "Mat4.h"
#include "EAngle.h"

class Camera
{
private:
        static const int speed = 20;
        Vec4<float> position;
        //Vec4<float> direction;
        Vec4<float> up;
        Vec4<float> movedir;
        //float pitch;
        //float yaw;
        //float roll;
        EAngle direction;


public:
        Camera();
        void update(float dt);
        Mat4<float> cameraMatrix();

        Vec4<float> getDirection();
        Vec4<float> getPosition();

        void dirx(float dir);
        void diry(float dir);
        void dirz(float dir);

        void updatePitch(float newMouse);
        void updateYaw(float newMouse);
        void updateRoll(float newMouse);
};
#endif // CAMERA_H

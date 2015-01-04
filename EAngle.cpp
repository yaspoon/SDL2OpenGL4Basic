#include "EAngle.h"

#include "Math.h"

EAngle::EAngle()
{
        pitch = 0.0f;
        yaw = 0.0f;
        roll = 0.0f;
}

EAngle::EAngle(float pitch, float yaw, float roll)
{
        this->pitch = pitch;
        this->yaw = yaw;
        this->roll = roll;
}

void EAngle::add2Pitch(float y)
{
        pitch += y;
        std::cout << "pitch:" << pitch << std::endl;
}

void EAngle::add2Yaw(float x)
{
        yaw += x;
}

void EAngle::add2Roll(float z)
{
        roll += z;
}

void EAngle::normalise()
{
        if(pitch > 90.0f)
        {
                pitch = 90.0f;
        }
        else if(pitch < -90.0f)
        {
                pitch = -90.0f;
        }

        if(yaw > 180.0f)
        {
                yaw = 180.0f;
        }
        else if(yaw < -180.0f)
        {
                yaw = -180.0f;
        }

        if(roll > 180.0f)
        {
                roll = 180.0f;
        }
        else if(roll < -180.0f)
        {
                roll = -180.0f;
        }
}

Vec4<float> EAngle::toVector()
{
        return Vec4<float>(sin(Math::toRadians(yaw)) * cos(Math::toRadians(pitch)), sin(Math::toRadians(pitch)), cos(Math::toRadians(yaw)) * cos(Math::toRadians(pitch)), 0.0f);
}

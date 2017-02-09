#ifndef EANGLE_H
#define EANGLE_H

#include "Vec4.h"

class EAngle
{
private:
        float pitch;
        float yaw;
        float roll;
public:
        EAngle();
        EAngle(float pitch, float yaw, float roll);

        void add2Pitch(float y);
        void add2Yaw(float x);
        void add2Roll(float z);

        void normalise();

        Vec4<float> toVector();

};
#endif

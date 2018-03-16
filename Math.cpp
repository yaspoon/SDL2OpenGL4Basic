#include "Math.h"
#include <cmath>

float Math::toRadians(float degrees)
{
        return degrees * (PI / 180.0f);
}

float Math::cos(float degrees)
{
	return std::cos(degrees);
}

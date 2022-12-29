#define _USE_MATH_DEFINES
#include <cmath>
#include "angle_conversions.h"

long double toRadians(const long double degrees)
{
	long double one_deg = M_PI / 180;
	return one_deg * degrees;
}

long double toDegrees(const long double radians)
{
	long double one_rad = 180 / M_PI;
	return one_rad * radians;
}
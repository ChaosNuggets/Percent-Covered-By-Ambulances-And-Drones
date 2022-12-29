#ifndef CALCULATE_DISTANCE_H
#define CALCULATE_DISTANCE_H

#include <utility>
#include "Structs/point.h"

const extern long double EARTH_RADIUS;
long double calculateDistance(const Point& coord1, const Point& coord2);

#endif
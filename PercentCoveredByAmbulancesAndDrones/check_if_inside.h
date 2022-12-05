#ifndef CHECK_IF_INSIDE_H
#define CHECK_IF_INSIDE_H

#include "Structs/point.h"
#include "Structs/polygon.h"
#include <vector>

int calculateDirection(const Point a, const Point b, const Point c);
bool checkIfInside(const Polygon& poly, const Point p);

#endif
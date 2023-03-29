#pragma once

#include "Structs/point.h"
#include "Structs/polygon.h"
#include <vector>

int calculateDirection(const Point a, const Point b, const Point c);
bool checkIfInside(const Polygon& poly, const Point p);
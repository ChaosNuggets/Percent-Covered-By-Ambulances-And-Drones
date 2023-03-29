#pragma once

#include <utility>
#include "Structs/point.h"
#include "Structs/index.h"

double calcCoordLong(const Point& coordinate, const double distance);
double calcCoordLat(const Point& coordinate, const double distance);
std::pair<Index, Index> calculateBoundingBox(const Point& stationCoordinate, const double boxRadius);
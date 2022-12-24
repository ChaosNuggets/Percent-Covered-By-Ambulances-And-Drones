#ifndef CALCULATE_BOUNDING_BOX_H
#define CALCULATE_BOUNDING_BOX_H

#include <utility>
#include "Structs/point.h"
#include "Structs/index.h"

double calcCoordLong(const Point& coordinate, const double distance);
constexpr double calcCoordLat(const Point& coordinate, const double distance);
std::pair<Index, Index> calculateBoundingBox(const Point& stationCoordinate, const double boxRadius);

#endif
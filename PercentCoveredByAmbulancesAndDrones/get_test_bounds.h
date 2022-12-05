#ifndef GET_TEST_BOUNDS_H
#define GET_TEST_BOUNDS_H

#include <utility>
#include "Structs/point.h"
#include "Structs/index.h"

double calcCoordLong(const Point& coordinate, const double distance);
constexpr double calcCoordLat(const Point& coordinate, const double distance);
std::pair<Index, Index> getTestIndexBounds(const Point& stationCoordinate, const double boxRadius);

#endif
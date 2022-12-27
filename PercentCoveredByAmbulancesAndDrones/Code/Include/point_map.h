#ifndef POINTS_H
#define POINTS_H

#include <utility>
#include <vector>
#include "Structs/point.h"
#include "Structs/index.h"

typedef std::vector<std::vector<uint8_t>> PointMap;

Index coordToIndex(const Point& coordinate, double (*roundFunc)(double) = round);
Point indexToCoord(const Index& index);
PointMap createPointMap();

// What a value represents in the point map
const int OUTSIDE = 0;
const int NOT_COVERED = 1;
const int COVERED_BY_AMBULANCE = 2;
const int COVERED_BY_DRONE = 3;
const int COVERED_BY_BOTH = 4;

const extern double LAT_IN_1_MILE;
const extern int LAT_SIZE;
const extern int LONG_SIZE;

#endif
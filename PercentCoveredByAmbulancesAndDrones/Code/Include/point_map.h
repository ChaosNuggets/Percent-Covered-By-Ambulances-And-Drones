#ifndef POINTS_H
#define POINTS_H

#include <utility>
#include <vector>
#include "Structs/point.h"
#include "Structs/index.h"

typedef double (*funcPtr)(double);

extern std::vector<std::vector<uint8_t>> pointMap;

Index coordToIndex(const Point& coordinate, funcPtr roundFunc = round);
Point indexToCoord(const Index& index);
void fillPointMap();

const extern int OUTSIDE;
const extern int NOT_COVERED;
const extern int COVERED_BY_AMBULANCE;
const extern int COVERED_BY_DRONE;
const extern int COVERED_BY_BOTH;

#endif
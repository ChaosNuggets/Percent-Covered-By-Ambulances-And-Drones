#define _USE_MATH_DEFINES
#include "point_map.h"
#include "calculate_bounding_box.h"
#include "calculate_distance.h"
#include "check_if_inside.h"
#include <cmath>
#include <stdint.h>
#include "indiana_border.h"

const double LOWEST_LAT = 37.779567; // Lowest latitude that I have in the point map
const double LOWEST_LONG = -88.057587; // Lowest longitude that I have in the point map
const double HIGHEST_LAT = 41.760356; // Highest latitude that I have in the point map
const double HIGHEST_LONG = -84.805966; // Highest longitude that I have in the point map
const double LONG_IN_1_MILE = 0.01836529538; // Longitude per mile at LOWEST_LAT
const double LAT_IN_1_MILE = 0.01447228581; // Latitude per mile
const double MILES_BETWEEN_POINTS = 1; // Controls the density of the point map
const int LAT_SIZE = ceil((HIGHEST_LAT - LOWEST_LAT) / (LAT_IN_1_MILE * MILES_BETWEEN_POINTS)) + 1; // Vertical size of pointMap matrix
const int LONG_SIZE = ceil((HIGHEST_LONG - LOWEST_LONG) / (LONG_IN_1_MILE * MILES_BETWEEN_POINTS)) + 1; // Horizontal size of pointMap matrix

Index coordToIndex(const Point& coordinate, double (*roundFunc)(double))
{
    const auto& [latitude, longitude] = coordinate; // Extract data

    // Calculate the coordinate's difference in miles from LOWEST_LAT and LOWEST_LONG
    const double latMileDifference = (latitude - LOWEST_LAT) / LAT_IN_1_MILE;
    double longMileDifference = calculateDistance({ latitude, LOWEST_LONG }, {latitude, longitude});

    if (longitude < LOWEST_LONG) longMileDifference *= -1;

    // Round the difference so it's an integer
    int latIndex = roundFunc(latMileDifference / MILES_BETWEEN_POINTS); // Also why is round not constexpr aaaaa
    int longIndex = roundFunc(longMileDifference / MILES_BETWEEN_POINTS);

    return { latIndex, longIndex };
}

Point indexToCoord(const Index& index)
{
    const auto [latIndex, longIndex] = index; // Extract data

    // Do the conversion
    const double latitude = calcCoordLat({ LOWEST_LAT, 0 }, latIndex * MILES_BETWEEN_POINTS);
    
    double longitude;
    if (longIndex == 0)
    {
        longitude = LOWEST_LONG;
    }
    else
    {
        longitude = calcCoordLong({ latitude, LOWEST_LONG }, longIndex * MILES_BETWEEN_POINTS);
    }

    return { latitude, longitude };
}

std::vector<std::vector<uint8_t>> createPointMap()
{
    std::vector<std::vector<uint8_t>> pointMap;

    // Resizes pointMap to be a 2d matrix of points of size latSize by longSize
    pointMap.resize(LAT_SIZE, std::vector<uint8_t>(LONG_SIZE, OUTSIDE));
    
    // Fill the points in pointMap to be spaced less than MILES_BETWEEN_POINTS apart
    for (int i = 0; i < LAT_SIZE; i++)
    {
        for (int j = 0; j < LONG_SIZE; j++)
        {
            // If the point is inside the polyline border, set it to true
            const Point p = indexToCoord({i, j});
            if (checkIfInside(indianaBorder, p))
            {
                pointMap[i][j] = NOT_COVERED;
            }
        }
    }

    return pointMap;
}
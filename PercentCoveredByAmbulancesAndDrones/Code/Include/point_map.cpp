#define _USE_MATH_DEFINES
#include "point_map.h"
#include "calculate_bounding_box.h"
#include "calculate_distance.h"
#include "check_if_inside.h"
#include <cmath>
#include <stdint.h>

const double LOWEST_LAT = 37.779567; // Lowest latitude that I have in the point map
const double LOWEST_LONG = -88.057587; // Lowest longitude that I have in the point map
const double HIGHEST_LAT = 41.760356; // Highest latitude that I have in the point map
const double HIGHEST_LONG = -84.805966; // Highest longitude that I have in the point map
const double LONG_IN_1_MILE = 0.01836529538; // Longitude per mile at LOWEST_LAT
const double LAT_IN_1_MILE = 0.01447228581; // Latitude per mile
const double MILES_BETWEEN_POINTS = 0.25; // Controls the density of the point map
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

    // These are the coordinates of vertexes of the polygon that make up the Indiana border
    const Polygon indianaBorder =
    {
        {41.712028, -87.524126},
        {41.690771, -87.400318},
        {41.639333, -87.399395},
        {41.760356, -86.826046},
        {41.760173, -84.805966},
        {39.105683, -84.820450},
        {38.783325, -84.812752},
        {38.716382, -85.451102},
        {38.262720, -85.749565},
        {37.957369, -86.043145},
        {38.195959, -86.364906},
        {37.840126, -86.647414},
        {37.998914, -86.823140},
        {37.779567, -87.111361},
        {37.972504, -87.594365},
        {37.789831, -88.057587},
        {38.266599, -87.977350},
        {38.755578, -87.498621},
        {39.136105, -87.658967},
        {39.371112, -87.531690}
    };
    
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
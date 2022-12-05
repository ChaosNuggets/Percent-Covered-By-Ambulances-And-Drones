// Uses the data in the RawData folder to calculate the coverage
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "station_coordinates.h"
#include "get_test_bounds.h"
#include "constants.h"
#include "point_map.h"
#include "extract_polygons.h"
#include "check_if_inside.h"
#include "remove_concave_corners.h"
#include "calculate_distance.h"
#include <cmath>

static void changeAmbulancePoints(const int stationNum)
{
    // Extract the indexes of the box corners
    const auto [lowCorner, highCorner] = getTestIndexBounds(stationCoordinates[stationNum], AMBULANCE_BOX_RADIUS);
    const auto [lowLatIndex, lowLongIndex] = lowCorner;
    const auto [highLatIndex, highLongIndex] = highCorner;

    for (int i = lowLatIndex; i <= highLatIndex; i++)
    {
        for (int j = lowLongIndex; j <= highLongIndex; j++)
        {
            if (pointMap[i][j] == OUTSIDE)
            {
                continue;
            }

            Point testPoint = indexToCoord({ i, j });

            if (checkIfInside(isochrones[stationNum], testPoint))
            {
                if (pointMap[i][j] == COVERED_BY_DRONE)
                {
                    pointMap[i][j] = COVERED_BY_BOTH;
                }
                else if (pointMap[i][j] == NOT_COVERED)
                {
                    pointMap[i][j] = COVERED_BY_AMBULANCE;
                }
            }
        }
    }
}

static void changeAmbulancePoints()
{
    for (int i = 0; i < stationCoordinates.size(); i++)
    {
        changeAmbulancePoints(i);
    }
}

static void changeDronePoints(const int stationNum, const double droneBoxRadius)
{
    // Extract the indexes of the box corners
    const auto [lowCorner, highCorner] = getTestIndexBounds(stationCoordinates[stationNum], droneBoxRadius);
    const auto [lowLatIndex, lowLongIndex] = lowCorner;
    const auto [highLatIndex, highLongIndex] = highCorner;

    for (int i = lowLatIndex; i <= highLatIndex; i++)
    {
        for (int j = lowLongIndex; j <= highLongIndex; j++)
        {
            if (pointMap[i][j] == OUTSIDE)
            {
                continue;
            }

            Point testPoint = indexToCoord({ i, j });
            const float distance = calculateDistance(testPoint, stationCoordinates[stationNum]);

            if (distance <= droneBoxRadius)
            {
                if (pointMap[i][j] == COVERED_BY_AMBULANCE)
                {
                    pointMap[i][j] = COVERED_BY_BOTH;
                }
                else if (pointMap[i][j] == NOT_COVERED)
                {
                    pointMap[i][j] = COVERED_BY_DRONE;
                }
            }
        }
    }
}

static void changeDronePoints(const double droneSpeed)
{
    double droneBoxRadius = droneSpeed * (MAX_TIME / MINUTES_IN_1_HOUR);

    for (int i = 0; i < stationCoordinates.size(); i++)
    {
        changeDronePoints(i, droneBoxRadius);
    }
}

static std::tuple<long double, int, int> calculateCoverage()
{
    int pointsCovered = 0;
    int pointsTotal = 0;
    for (int i = 0; i < LAT_SIZE; i++)
    {
        for (int j = 0; j < LONG_SIZE; j++)
        {
            if (pointMap[i][j] == NOT_COVERED)
            {
                pointsTotal++;
            }
            else if (pointMap[i][j] == COVERED_BY_DRONE)
            {
                pointsCovered++;
                pointsTotal++;
            }
            else if (pointMap[i][j] == COVERED_BY_BOTH)
            {
                pointsCovered++;
                pointsTotal++;
            }
            else if (pointMap[i][j] == COVERED_BY_AMBULANCE)
            {
                pointsCovered++;
                pointsTotal++;
            }
        }
    }
    const long double coverage = (long double) pointsCovered / pointsTotal;
    return { coverage, pointsCovered, pointsTotal };
}

static void printCoverage(const double droneSpeed, const long double coverage, const int pointsCovered, const int pointsTotal)
{
    std::cout << std::setprecision(10);
    std::cout << droneSpeed << " mph ";
    std::cout << "Coverage: " << coverage * 100 << '%';
    std::cout << " (" << pointsCovered << '/' << pointsTotal << ')';
    std::cout << '\n';
}

static std::string generateFilePath(const int droneSpeed)
{
    return "Points/" + std::to_string(droneSpeed) + "mph.csv";
}

static void savePoints(const double droneSpeed)
{
    std::ofstream fout(generateFilePath(round(droneSpeed)));
    // Write the header
    fout << "Latitude,Longitude,Value\n";
    
    fout << std::setprecision(8);
    for (int i = 0; i < pointMap.size(); i++)
    {
        for (int j = 0; j < pointMap[i].size(); j++)
        {
            if (pointMap[i][j] == OUTSIDE) continue;

            Point point = indexToCoord({ i, j });
            fout << point.lat << ',' << point.lon << ',' << (int) pointMap[i][j] << '\n';
        }
    }

    fout.close();
}

static void printPointMap()
{
    for (int i = pointMap.size() - 1; i >= 0; i--)
    {
        for (uint8_t point : pointMap[i])
        {
            if (point == NOT_COVERED)
            {
                std::cout << 'O';
            }
            else if (point == COVERED_BY_AMBULANCE)
            {
                std::cout << '.';
            }
            else if (point == COVERED_BY_DRONE)
            {
                std::cout << ',';
            }
            else if (point == COVERED_BY_BOTH)
            {
                std::cout << ';';
            }
            else
            {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
}

int main()
{
    extractPolygons();
    removeConcaveCorners(isochrones);
    fillPointMap();

    changeAmbulancePoints();

    // Drone speed is in mph
    for (double droneSpeed = 0; droneSpeed <= 200; droneSpeed += 5)
    {
        changeDronePoints(droneSpeed);
        const auto [coverage, pointsCovered, pointsTotal] = calculateCoverage();
        printCoverage(droneSpeed, coverage, pointsCovered, pointsTotal);
        //savePoints(droneSpeed);
    }

    return 0;
}
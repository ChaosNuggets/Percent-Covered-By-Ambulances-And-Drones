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

static void changePointsAmbulance(const int stationNum)
{
    // Extract the indexes of the box corners
    const auto [lowCorner, highCorner] = getTestIndexBounds(stationCoordinates[stationNum], AMBULANCE_BOX_RADIUS);
    const auto [lowLatIndex, lowLongIndex] = lowCorner;
    const auto [highLatIndex, highLongIndex] = highCorner;

    for (int i = lowLatIndex; i <= highLatIndex; i++)
    {
        for (int j = lowLongIndex; j <= highLongIndex; j++)
        {
            if (points[i][j] == OUTSIDE)
            {
                continue;
            }

            Point testPoint = indexToCoord({ i, j });

            if (checkIfInside(isochrones[stationNum], testPoint))
            {
                if (points[i][j] == COVERED_BY_DRONE)
                {
                    points[i][j] = COVERED_BY_BOTH;
                }
                else if (points[i][j] == NOT_COVERED)
                {
                    points[i][j] = COVERED_BY_AMBULANCE;
                }
            }
        }
    }
}

static void changePointsAmbulance()
{
    for (int i = 0; i < stationCoordinates.size(); i++)
    {
        changePointsAmbulance(i);
    }
}

static void changePointsDrone(const int stationNum, const double droneBoxRadius)
{
    // Extract the indexes of the box corners
    const auto [lowCorner, highCorner] = getTestIndexBounds(stationCoordinates[stationNum], droneBoxRadius);
    const auto [lowLatIndex, lowLongIndex] = lowCorner;
    const auto [highLatIndex, highLongIndex] = highCorner;

    for (int i = lowLatIndex; i <= highLatIndex; i++)
    {
        for (int j = lowLongIndex; j <= highLongIndex; j++)
        {
            if (points[i][j] == OUTSIDE)
            {
                continue;
            }

            Point testPoint = indexToCoord({ i, j });
            const float distance = calculateDistance(testPoint, stationCoordinates[stationNum]);

            if (distance <= droneBoxRadius)
            {
                if (points[i][j] == COVERED_BY_AMBULANCE)
                {
                    points[i][j] = COVERED_BY_BOTH;
                }
                else if (points[i][j] == NOT_COVERED)
                {
                    points[i][j] = COVERED_BY_DRONE;
                }
            }
        }
    }
}

static void changePointsDrone(const double droneSpeed)
{
    double droneBoxRadius = droneSpeed * (MAX_TIME / MINUTES_IN_1_HOUR);

    for (int i = 0; i < stationCoordinates.size(); i++)
    {
        changePointsDrone(i, droneBoxRadius);
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
            if (points[i][j] == NOT_COVERED)
            {
                pointsTotal++;
            }
            else if (points[i][j] == COVERED_BY_DRONE)
            {
                pointsCovered++;
                pointsTotal++;
            }
            else if (points[i][j] == COVERED_BY_BOTH)
            {
                pointsCovered++;
                pointsTotal++;
            }
            else if (points[i][j] == COVERED_BY_AMBULANCE)
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

static void writePoints(const double droneSpeed)
{
    std::ofstream fout(generateFilePath(round(droneSpeed)));
    // Write the header
    fout << "Latitude,Longitude,Value\n";
    
    fout << std::setprecision(8);
    for (int i = 0; i < points.size(); i++)
    {
        for (int j = 0; j < points[i].size(); j++)
        {
            if (points[i][j] == OUTSIDE) continue;

            Point point = indexToCoord({ i, j });
            fout << point.lat << ',' << point.lon << ',' << (int) points[i][j] << '\n';
        }
    }

    fout.close();
}

static void printPointMap()
{
    for (int i = points.size() - 1; i >= 0; i--)
    {
        for (uint8_t point : points[i])
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
    fillPoints();

    changePointsAmbulance();

    // Drone speed is in mph
    for (double droneSpeed = 0; droneSpeed <= 200; droneSpeed += 5)
    {
        changePointsDrone(droneSpeed);
        const auto [coverage, pointsCovered, pointsTotal] = calculateCoverage();
        printCoverage(droneSpeed, coverage, pointsCovered, pointsTotal);
        writePoints(droneSpeed);
    }

    return 0;
}
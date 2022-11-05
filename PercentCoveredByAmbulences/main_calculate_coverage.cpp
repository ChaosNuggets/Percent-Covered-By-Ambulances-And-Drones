// Uses the data in the RawData folder to calculate the coverage
#include <iostream>
#include <iomanip>
#include <string>
#include "station_coordinates.h"
#include "get_test_bounds.h"
#include "constants.h"
#include "point_map.h"
#include "extract_polygons.h"
#include "check_if_inside.h"
#include "remove_concave_corners.h"
#include "calculate_distance.h"

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
                points[i][j] = COVERED_BY_AMBULANCE;
            }
        }
    }
}

static void changePointsDrone(int stationNum, double droneBoxRadius)
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
                else
                {
                    points[i][j] = COVERED_BY_DRONE;
                }
            }
        }
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

int main()
{
    extractPolygons();
    removeConcaveCorners(isochrones);

    double droneSpeed = 70; // Drone speed in mph
    double droneBoxRadius = droneSpeed * (MAX_TIME / MINUTES_IN_1_HOUR);

    fillPoints();

    for (int i = 0; i < stationCoordinates.size(); i++)
    {
        changePointsAmbulance(i);
        changePointsDrone(i, droneBoxRadius);
	}
    // Calculate and print coverage
    auto [coverage, pointsCovered, pointsTotal] = calculateCoverage();
    std::cout << std::setprecision(10);
    std::cout << "Coverage: " << coverage * 100 << "% (" << pointsCovered << '/' << pointsTotal << ")\n";

    // Print Indiana point map
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

    std::cout << "Press enter to exit";
    std::cin.ignore();
    return 0;
}
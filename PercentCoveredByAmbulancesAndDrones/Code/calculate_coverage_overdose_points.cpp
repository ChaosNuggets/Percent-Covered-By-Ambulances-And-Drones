// Uses the data in the RawData folder to calculate the coverage from the overdose points
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <optional>
#include <cmath>
#include "Include/station_coordinates.h"
#include "Include/calculate_bounding_box.h"
#include "Include/overdose_points.h"
#include "Include/point_map.h"
#include "Include/extract_polygons.h"
#include "Include/check_if_inside.h"
#include "Include/remove_concave_corners.h"
#include "Include/calculate_distance.h"

static const long double MAX_TIME = 5; // The longest time that we are allowing our drone/ambulance to travel
static const long double MINUTES_IN_1_HOUR = 60; // Self explanatory

static void changeOneAmbulancePoints(std::vector<uint8_t>& coverageStatuses, const std::vector<Polygon>& isochrones, const int stationNum)
{
    for (int i = 0; i < coverageStatuses.size(); i++)
    {
        if (coverageStatuses[i] == COVERED_BY_AMBULANCE || coverageStatuses[i] == COVERED_BY_BOTH)
        {
            continue;
        }

        if (checkIfInside(isochrones[stationNum], overdosePoints[i]))
        {
            if (coverageStatuses[i] == COVERED_BY_DRONE)
            {
                coverageStatuses[i] = COVERED_BY_BOTH;
            }
            else if (coverageStatuses[i] == NOT_COVERED)
            {
                coverageStatuses[i] = COVERED_BY_AMBULANCE;
            }
        }
    }
}

static void changeAllAmbulancePoints(std::vector<uint8_t>& coverageStatuses, const std::vector<Polygon>& isochrones)
{
    for (int i = 0; i < stationCoordinates.size(); i++)
    {
        changeOneAmbulancePoints(coverageStatuses, isochrones, i);
    }
}

static void changeOneDronePoints(std::vector<uint8_t>& coverageStatuses, const int stationNum, const double droneRadius)
{
    for (int i = 0; i < coverageStatuses.size(); i++)
    {
        if (coverageStatuses[i] == COVERED_BY_DRONE || coverageStatuses[i] == COVERED_BY_BOTH)
        {
            continue;
        }

        const long double distance = calculateDistance(overdosePoints[i], stationCoordinates[stationNum]);

        if (distance <= droneRadius)
        {
            if (coverageStatuses[i] == COVERED_BY_AMBULANCE)
            {
                coverageStatuses[i] = COVERED_BY_BOTH;
            }
            else if (coverageStatuses[i] == NOT_COVERED)
            {
                coverageStatuses[i] = COVERED_BY_DRONE;
            }
        }
    }
}

static void changeAllDronePoints(std::vector<uint8_t>& coverageStatuses, const double droneSpeed)
{
    double droneRadius = droneSpeed * (MAX_TIME / MINUTES_IN_1_HOUR);

    for (int i = 0; i < stationCoordinates.size(); i++)
    {
        changeOneDronePoints(coverageStatuses, i, droneRadius);
    }
}

// Returns the coverage, the number of points covered, and the total number of points
static std::tuple<long double, int, int> calculateCoverage(const std::vector<uint8_t>& coverageStatuses)
{
    int pointsCovered = 0;
    int pointsTotal = 0;
    for (int i = 0; i < coverageStatuses.size(); i++)
    {
        if (coverageStatuses[i] == NOT_COVERED)
        {
            pointsTotal++;
        }
        else if (coverageStatuses[i] == COVERED_BY_DRONE)
        {
            pointsCovered++;
            pointsTotal++;
        }
        else if (coverageStatuses[i] == COVERED_BY_BOTH)
        {
            pointsCovered++;
            pointsTotal++;
        }
        else if (coverageStatuses[i] == COVERED_BY_AMBULANCE)
        {
            pointsCovered++;
            pointsTotal++;
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
    return "Points/" + std::to_string(droneSpeed) + "mph_overdose_points.csv";
}

static void savePoints(const std::vector<uint8_t>& coverageStatuses, const double droneSpeed)
{
    std::ofstream fout(generateFilePath(round(droneSpeed)));
    // Write the header
    fout << "Latitude,Longitude,Value\n";

    fout << std::setprecision(8);
    for (int i = 0; i < coverageStatuses.size(); i++)
    {
        fout << overdosePoints[i].lat << ',' << overdosePoints[i].lon << ',' << (int) coverageStatuses[i] << '\n';
    }

    fout.close();
}

static void printPoints(const std::vector<uint8_t>& coverageStatuses, const std::optional<uint8_t> status = std::nullopt)
{
    for (int i = 0; i < coverageStatuses.size(); i++)
    {
        if (!status.has_value() || coverageStatuses[i] == status)
        {
            std::cout << overdosePoints[i].lat << ", " << overdosePoints[i].lon << '\n';
        }
    }
}

int main()
{
    std::vector<Polygon> isochrones = extractPolygons();
    removeConcaveCorners(isochrones);

    std::vector<uint8_t> coverageStatuses(overdosePoints.size(), NOT_COVERED);

    changeAllAmbulancePoints(coverageStatuses, isochrones);

    // Drone speed is in mph
    for (double droneSpeed = 0; droneSpeed <= 200; droneSpeed += 5)
    {
        changeAllDronePoints(coverageStatuses, droneSpeed);
        const auto [coverage, pointsCovered, pointsTotal] = calculateCoverage(coverageStatuses);
        printCoverage(droneSpeed, coverage, pointsCovered, pointsTotal);
    }

    return 0;
}

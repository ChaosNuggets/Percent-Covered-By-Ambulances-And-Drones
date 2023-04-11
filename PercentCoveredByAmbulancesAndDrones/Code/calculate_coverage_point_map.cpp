//// Uses the data in the RawData folder to calculate the coverage with a point map
//#include <iostream>
//#include <iomanip>
//#include <string>
//#include <fstream>
//#include "Include/station_coordinates.h"
//#include "Include/calculate_bounding_box.h"
//#include "Include/point_map.h"
//#include "Include/extract_polygons.h"
//#include "Include/check_if_inside.h"
//#include "Include/remove_concave_corners.h"
//#include "Include/calculate_distance.h"
//#include <cmath>
//
//static const long double MAX_TIME = 5; // The longest time that we are allowing our drone/ambulance to travel
//static const long double MINUTES_IN_1_HOUR = 60; // Self explanatory
//
//static void changeAmbulancePoints(PointMap& pointMap, const std::vector<Polygon>& isochrones, const int stationNum, const double ambulanceBoxRadius)
//{
//    // Extract the indexes of the box corners
//    const auto [lowCorner, highCorner] = calculateBoundingBox(stationCoordinates[stationNum], ambulanceBoxRadius);
//    const auto [lowLatIndex, lowLongIndex] = lowCorner;
//    const auto [highLatIndex, highLongIndex] = highCorner;
//
//    for (int i = lowLatIndex; i <= highLatIndex; i++)
//    {
//        for (int j = lowLongIndex; j <= highLongIndex; j++)
//        {
//            if (pointMap[i][j] == OUTSIDE || pointMap[i][j] == COVERED_BY_AMBULANCE || pointMap[i][j] == COVERED_BY_BOTH)
//            {
//                continue;
//            }
//
//            Point testPoint = indexToCoord({ i, j });
//
//            if (checkIfInside(isochrones[stationNum], testPoint))
//            {
//                if (pointMap[i][j] == COVERED_BY_DRONE)
//                {
//                    pointMap[i][j] = COVERED_BY_BOTH;
//                }
//                else if (pointMap[i][j] == NOT_COVERED)
//                {
//                    pointMap[i][j] = COVERED_BY_AMBULANCE;
//                }
//            }
//        }
//    }
//}
//
//static void changeAmbulancePoints(PointMap& pointMap, const std::vector<Polygon>& isochrones)
//{
//    const int MAX_SPEED = 70; // Max speed the ambulance can go in mph
//    const double ambulanceBoxRadius = MAX_SPEED * (MAX_TIME / MINUTES_IN_1_HOUR); // 1/2 of the side length of the box around each police station (in miles)
//
//    for (int i = 0; i < stationCoordinates.size(); i++)
//    {
//        changeAmbulancePoints(pointMap, isochrones, i, ambulanceBoxRadius);
//    }
//}
//
//static void changeDronePoints(PointMap& pointMap, const int stationNum, const double droneBoxRadius)
//{
//    // Extract the indexes of the box corners
//    const auto [lowCorner, highCorner] = calculateBoundingBox(stationCoordinates[stationNum], droneBoxRadius);
//    const auto [lowLatIndex, lowLongIndex] = lowCorner;
//    const auto [highLatIndex, highLongIndex] = highCorner;
//
//    for (int i = lowLatIndex; i <= highLatIndex; i++)
//    {
//        for (int j = lowLongIndex; j <= highLongIndex; j++)
//        {
//            if (pointMap[i][j] == OUTSIDE || pointMap[i][j] == COVERED_BY_DRONE || pointMap[i][j] == COVERED_BY_BOTH)
//            {
//                continue;
//            }
//
//            Point testPoint = indexToCoord({ i, j });
//            const long double distance = calculateDistance(testPoint, stationCoordinates[stationNum]);
//
//            if (distance <= droneBoxRadius)
//            {
//                if (pointMap[i][j] == COVERED_BY_AMBULANCE)
//                {
//                    pointMap[i][j] = COVERED_BY_BOTH;
//                }
//                else if (pointMap[i][j] == NOT_COVERED)
//                {
//                    pointMap[i][j] = COVERED_BY_DRONE;
//                }
//            }
//        }
//    }
//}
//
//static void changeDronePoints(PointMap& pointMap, const double droneSpeed)
//{
//    double droneBoxRadius = droneSpeed * (MAX_TIME / MINUTES_IN_1_HOUR);
//
//    for (int i = 0; i < stationCoordinates.size(); i++)
//    {
//        changeDronePoints(pointMap, i, droneBoxRadius);
//    }
//}
//
//// Returns the coverage, the number of points covered, and the total number of points
//static std::tuple<long double, int, int> calculateCoverage(const PointMap& pointMap)
//{
//    int pointsCovered = 0;
//    int pointsTotal = 0;
//    for (int i = 0; i < LAT_SIZE; i++)
//    {
//        for (int j = 0; j < LONG_SIZE; j++)
//        {
//            if (pointMap[i][j] == NOT_COVERED)
//            {
//                pointsTotal++;
//            }
//            else if (pointMap[i][j] == COVERED_BY_DRONE)
//            {
//                pointsCovered++;
//                pointsTotal++;
//            }
//            else if (pointMap[i][j] == COVERED_BY_BOTH)
//            {
//                pointsCovered++;
//                pointsTotal++;
//            }
//            else if (pointMap[i][j] == COVERED_BY_AMBULANCE)
//            {
//                pointsCovered++;
//                pointsTotal++;
//            }
//        }
//    }
//    const long double coverage = (long double) pointsCovered / pointsTotal;
//    return { coverage, pointsCovered, pointsTotal };
//}
//
//static void printCoverage(const double droneSpeed, const long double coverage, const int pointsCovered, const int pointsTotal)
//{
//    std::cout << std::setprecision(10);
//    std::cout << droneSpeed << " mph ";
//    std::cout << "Coverage: " << coverage * 100 << '%';
//    std::cout << " (" << pointsCovered << '/' << pointsTotal << ')';
//    std::cout << '\n';
//}
//
//static std::string generateFilePath(const int droneSpeed)
//{
//    return "Points/" + std::to_string(droneSpeed) + "mph.csv";
//}
//
//static void savePoints(const PointMap& pointMap, const double droneSpeed)
//{
//    std::ofstream fout(generateFilePath(round(droneSpeed)));
//    // Write the header
//    fout << "Latitude,Longitude,Value\n";
//    
//    fout << std::setprecision(8);
//    for (int i = 0; i < pointMap.size(); i++)
//    {
//        for (int j = 0; j < pointMap[i].size(); j++)
//        {
//            if (pointMap[i][j] == OUTSIDE) continue;
//
//            Point point = indexToCoord({ i, j });
//            fout << point.lat << ',' << point.lon << ',' << (int) pointMap[i][j] << '\n';
//        }
//    }
//
//    fout.close();
//}
//
//static void printPointMap(const PointMap& pointMap)
//{
//    for (int i = pointMap.size() - 1; i >= 0; i--)
//    {
//        for (uint8_t point : pointMap[i])
//        {
//            if (point == NOT_COVERED)
//            {
//                std::cout << 'O';
//            }
//            else if (point == COVERED_BY_AMBULANCE)
//            {
//                std::cout << '.';
//            }
//            else if (point == COVERED_BY_DRONE)
//            {
//                std::cout << ',';
//            }
//            else if (point == COVERED_BY_BOTH)
//            {
//                std::cout << ';';
//            }
//            else
//            {
//                std::cout << ' ';
//            }
//        }
//        std::cout << '\n';
//    }
//}
//
//int main()
//{
//    std::vector<Polygon> isochrones = extractPolygons();
//    removeConcaveCorners(isochrones);
//
//    PointMap pointMap = createPointMap();
//
//    changeAmbulancePoints(pointMap, isochrones);
//
//    // Drone speed is in mph
//    for (double droneSpeed = 0; droneSpeed <= 200; droneSpeed += 5)
//    {
//        changeDronePoints(pointMap, droneSpeed);
//        const auto [coverage, pointsCovered, pointsTotal] = calculateCoverage(pointMap);
//        printCoverage(droneSpeed, coverage, pointsCovered, pointsTotal);
//    }
//
//    return 0;
//}

#include "constants.h"
#include <cmath>

const int EARTH_RADIUS = 3959; // Earth's radius in miles
const double LOWEST_LAT = 37.999366; // Lowest latitude that I have in points
const double LOWEST_LONG = -87.910623; // Lowest longitude that I have in points
const double HIGHEST_LAT = 41.759907; // Highest latitude that I have in points
const double HIGHEST_LONG = -84.820157; // Highest longitude that I have in points
const double LONG_IN_1_MILE = 0.01836529538; // Longitude per mile at LOWEST_LAT
const double LAT_IN_1_MILE = 0.01447228581; // Latitude per mile
const double MILES_BETWEEN_POINTS = 2; // Controls the density of the points
const int LAT_SIZE = ceil((HIGHEST_LAT - LOWEST_LAT) / (LAT_IN_1_MILE * MILES_BETWEEN_POINTS)) + 1; // Vertical size of points matrix
const int LONG_SIZE = ceil((HIGHEST_LONG - LOWEST_LONG) / (LONG_IN_1_MILE * MILES_BETWEEN_POINTS)) + 1; // Horizontal size of points matrix
const long double MAX_TIME = 5; // The longest time that we are allowing our drone to fly for
const long double MINUTES_IN_1_HOUR = 60; // Self explanatory
const int LAT_INDEX_STOP = 3; // The lat index at which the second api request starts
const int LONG_INDEX_STOP = 4; // The long index at which the second api request starts
const int MAX_SPEED = 70; // Max speed the ambulence can go in mph
const double BOX_RADIUS = MAX_SPEED * (MAX_TIME / MINUTES_IN_1_HOUR); // 1/2 of the side length of the box around each police station (in miles)
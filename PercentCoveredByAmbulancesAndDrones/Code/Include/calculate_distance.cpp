#include "angle_conversions.h"
#include <cmath>
#include "calculate_distance.h"
#define _USE_MATH_DEFINES

// C++ program to calculate Distance
// Between Two Points on Earth

static long double calculateDistanceL(const std::pair<long double, long double>& coord1, const std::pair<long double, long double>& coord2)
{
	// Extract data
	auto [lat1, long1] = coord1;
	auto [lat2, long2] = coord2;

	// Convert the latitudes
	// and longitudes
	// from degree to radians.
	lat1 = toRadians(lat1);
	long1 = toRadians(long1);
	lat2 = toRadians(lat2);
	long2 = toRadians(long2);

	// Haversine Formula
	const long double dlong = long2 - long1;
	const long double dlat = lat2 - lat1;

	long double ans = pow(sin(dlat / 2), 2) +
		cos(lat1) * cos(lat2) *
		pow(sin(dlong / 2), 2);

	ans = 2 * asin(sqrt(ans));

	// Radius of Earth in
	// Miles, R = 3959
	const long double R = 3959;

	// Calculate the result
	ans = ans * R;

	return ans;
}

long double calculateDistance(const Point& coord1, const Point& coord2)
{
	return calculateDistanceL({ coord1.lat, coord1.lon }, { coord2.lat, coord2.lon });
}

// This code was written
// by Aayush Chaturvedi
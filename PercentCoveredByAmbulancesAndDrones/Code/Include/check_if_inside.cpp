#include "check_if_inside.h"
#include <algorithm>

struct Line {
	Point p1, p2;
};

static bool onLine(const Line l1, const Point p)
{
	// Check whether p is on the line or not
	if (p.lon <= std::max(l1.p1.lon, l1.p2.lon)
		&& p.lon >= std::min(l1.p1.lon, l1.p2.lon)
		&& (p.lat <= std::max(l1.p1.lat, l1.p2.lat)
			&& p.lat >= std::min(l1.p1.lat, l1.p2.lat)))
		return true;

	return false;
}

int calculateDirection(const Point a, const Point b, const Point c)
{
	double val = (b.lat - a.lat) * (c.lon - b.lon)
		- (b.lon - a.lon) * (c.lat - b.lat);

	if (val == 0)

		// Colinear
		return 0;

	else if (val < 0)

		// Anti-clockwise direction
		return 2;

	// Clockwise direction
	return 1;
}

static bool isIntersect(const Line l1, const Line l2)
{
	// Four direction for two lines and points of other line
	int dir1 = calculateDirection(l1.p1, l1.p2, l2.p1);
	int dir2 = calculateDirection(l1.p1, l1.p2, l2.p2);
	int dir3 = calculateDirection(l2.p1, l2.p2, l1.p1);
	int dir4 = calculateDirection(l2.p1, l2.p2, l1.p2);

	// When intersecting
	if (dir1 != dir2 && dir3 != dir4)
		return true;

	// When p2 of line2 are on the line1
	if (dir1 == 0 && onLine(l1, l2.p1))
		return true;

	// When p1 of line2 are on the line1
	if (dir2 == 0 && onLine(l1, l2.p2))
		return true;

	// When p2 of line1 are on the line2
	if (dir3 == 0 && onLine(l2, l1.p1))
		return true;

	// When p1 of line1 are on the line2
	if (dir4 == 0 && onLine(l2, l1.p2))
		return true;

	return false;
}

bool checkIfInside(const Polygon& poly, const Point p)
{
	// When polygon has less than 3 edges, it is not a polygon
	if (poly.size() < 3)
		return false;

	// Create a point at infinity, y is same as point p
	Line exline = { p, { 181, p.lat } };
	int count = 0;
	int i = 0;
	do {
		// Forming a line from two consecutive points of
		// poly
		Line side = { poly[i], poly[(i + 1) % poly.size()]};
		if (isIntersect(side, exline)) {
			// If side is intersects exline
			if (calculateDirection(side.p1, p, side.p2) == 0)
				return onLine(side, p);
			count++;
		}
		i = (i + 1) % poly.size();
	} while (i != 0);

	// When count is odd
	return count & 1;
}

// This code was written
// by Aarti Rathi
#include "remove_concave_corners.h"
#include "check_if_inside.h"
#include <stdexcept>

static constexpr int prevIndex(const int index, const int size)
{
	return index - 1 >= 0 ? index - 1 : size - 1;
}

static constexpr int nextIndex(const int index, const int size)
{
	return (index + 1) % size;
}

static int calculateDirection(const Polygon& polygon)
{
	double sum = 0;
	for (int i = 0; i < polygon.size(); i++)
	{
		const int nextInd = nextIndex(i, polygon.size());
		sum += (polygon[nextInd].lon - polygon[i].lon) * (polygon[nextInd].lat + polygon[i].lat);
	}
	if (sum == 0) return 0; // Colinear
	if (sum < 0) return 2; // CCW
	return 1; // CW
}

static void removeConcaveCorners(Polygon& polygon)
{
	int polygonDirection = calculateDirection(polygon);
	if (polygonDirection == 0)
	{
		throw std::runtime_error("The polygon is colinear????");
	}
	bool isConvex;
	do
	{
		isConvex = true;
		std::vector<int> indexesToRemove;
		for (int i = 0; i < polygon.size(); i++)
		{
			const int startInd = prevIndex(i, polygon.size()); // The index of the point before i
			const int endInd = nextIndex(i, polygon.size()); // The index of the point after i
			const int pointDirection = calculateDirection(polygon[startInd], polygon[i], polygon[endInd]);
			if (pointDirection != polygonDirection && pointDirection != 0)
			{
				indexesToRemove.push_back(i);
				isConvex = false;
			}
		}

		for (int i = indexesToRemove.size() - 1; i >= 0; i--)
		{
			polygon.erase(polygon.begin() + indexesToRemove[i]);
		}
	} while (!isConvex);
}

void removeConcaveCorners(std::vector<Polygon>& polygons)
{
	for (Polygon& polygon : polygons)
	{
		removeConcaveCorners(polygon);
	}
}
// This program extracts data from the json files
#include <fstream>
#include "station_coordinates.h"
#include <vector>
#include <ctype.h>
#include <string>
#include "extract_polygons.h"

static std::string generateReadFilePath(const int stationNum)
{
	std::string filePath = "RawData/";
	filePath.append(std::to_string(stationNum));
	filePath.append(".json");
	return filePath;
}

static std::size_t findStart(const std::string& line)
{
	std::size_t found = line.find("coordinates");
	if (found == std::string::npos)
	{
		throw std::runtime_error("couldn't find the word \"coordinates\"");
	}
	return found + 15;
}

static double getLon(const std::string& line, std::size_t& start)
{
	std::size_t numSize = line.find(',', start) - start;
	double lon = stod(line.substr(start, numSize));
	start += numSize + 1;
	return lon;
}

static double getLat(const std::string& line, std::size_t& start)
{
	std::size_t numSize = line.find(']', start) - start;
	double lat = stod(line.substr(start, numSize));
	start += numSize + 3;
	return lat;
}

static Polygon getVertexes(const std::string& line, std::size_t start)
{
	Polygon vertexes;
	while (isdigit(line[start]) || line[start] == '-')
	{
		Point currentVertex;
		currentVertex.lon = getLon(line, start);
		currentVertex.lat = getLat(line, start);
		vertexes.push_back(currentVertex);
	}
	return vertexes;
}

static Polygon extractPolygon(std::ifstream& fin)
{
	std::string line;
	std::getline(fin, line); // The file should only be one line
	std::size_t start = findStart(line);

	return getVertexes(line, start);
}

static Polygon extractPolygon(const int stationNum)
{
	const std::string readFilePath = generateReadFilePath(stationNum);

	std::ifstream fin(readFilePath.c_str()); // Ifstream stands for input file stream
	if (!fin.is_open())
	{
		throw std::runtime_error("Unable to open " + readFilePath);
	}
	
	Polygon polygon = extractPolygon(fin);

	fin.close();
	return polygon;
}

std::vector<Polygon> extractPolygons()
{
	std::vector<Polygon> isochrones;
	for (int i = 0; i < stationCoordinates.size(); i++)
	{
		isochrones.push_back(extractPolygon(i));
	}
	return isochrones;
}
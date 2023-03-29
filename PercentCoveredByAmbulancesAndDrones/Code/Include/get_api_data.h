#pragma once

#include <string>
#include <vector>
#include "Structs/point.h"

void promptForAPIKey(std::string& apiKey);
std::string generateURL(const Point& coordinate, const int maxTime, const std::string& apiKey);
bool doCurlStuff(const std::string& URL, std::string& fileName);
// Program to get the api data
#include "Include/get_api_data.h"
#include "Include/max_time.h"
#include "Include/station_coordinates.h"
#include <chrono>
#include <thread>
#include <iostream>

static std::string generateFilePath(const int stationNum)
{
    std::string filePath = "RawData/";
    filePath.append(std::to_string(stationNum));
    filePath.append(".json");
    return filePath;
}

bool downloadData(const std::string& apiKey, const int stationNum)
{
    const int ADJUSTED_MAX_TIME = 6; // This accounts for time between 911 call and ambulance leaving, and also ambulance driving faster.
    std::string URL = generateURL(stationCoordinates[stationNum], ADJUSTED_MAX_TIME, apiKey);
    std::string filePath = generateFilePath(stationNum);

    bool curlSuccess = doCurlStuff(URL, filePath);
    if (!curlSuccess)
    {
        return false;
    }
    return true;
}

int main(void)
{
    std::string apiKey;
    promptForAPIKey(apiKey);

    for (int i = 0; i < stationCoordinates.size(); i++)
    {
        std::cout << "Downloading file: " << (i + 1) << " / " << stationCoordinates.size() << '\n';
        const bool downloadSuccess = downloadData(apiKey, i);
        if (!downloadSuccess)
        {
            return 1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    return 0;
}

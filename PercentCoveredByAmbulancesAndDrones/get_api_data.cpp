#include <iostream>
#include "get_api_data.h"

#define CURL_STATICLIB
#include <curl/curl.h>

void promptForAPIKey(std::string& apiKey)
{
    std::cout << "Enter API key: ";
    std::getline(std::cin, apiKey);
    apiKey.insert(0, "&access_token=");
}

std::string generateURL(const Point& coordinate, const int maxTime, const std::string& apiKey)
{
    std::string api = "https://api.mapbox.com/isochrone/v1/mapbox/driving/";
    std::string coordStr = std::to_string(coordinate.lon) + ',' + std::to_string(coordinate.lat);
    std::string time = "?contours_minutes=" + std::to_string(maxTime);
    std::string URL = api + coordStr + time + apiKey;
    return URL;
}

static bool initializeCurl(CURL*& curl)
{
    curl = curl_easy_init();
    if (!curl) // If initialization failed
    {
        std::cout << "Curl initialization failed";
        return false;
    }
    return true;
}

static bool openFile(FILE*& filePtr, const std::string& filePath)
{
    errno_t error = fopen_s(&filePtr, filePath.c_str(), "wb"); // wb stands for write binary
    if (error != 0)
    {
        std::cout << "File opening failed";
        return false;
    }
    return true;
}

// This is called whenever a chunk is downloaded
static size_t writeData(void* contents, size_t itemSize, size_t numItems, FILE* stream) {
    size_t written = fwrite(contents, itemSize, numItems, stream);
    return written;
}

static void setUpOptions(CURL*& curl, const std::string& URL, const FILE* filePtr)
{
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, filePtr);
}

static bool performCurlAction(CURL* curl)
{
    CURLcode result; // Result of curl call, equals CURLE_OK if download succeded
    result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        std::cout << "download problem: " << curl_easy_strerror(result);
        return false;
    }
    return true;
}

bool doCurlStuff(const std::string& URL, std::string& filePath)
{
    CURL* curl;
    FILE* filePtr;

    bool initSuccess = initializeCurl(curl);
    if (!initSuccess)
    {
        return false;
    }

    bool fileOpenSuccess = openFile(filePtr, filePath);
    if (!fileOpenSuccess)
    {
        return false;
    }

    setUpOptions(curl, URL, filePtr);

    bool performSuccess = performCurlAction(curl);
    if (!performSuccess)
    {
        return false;
    }

    if (fclose(filePtr))
    {
        std::cout << "file close error";
        return false;
    }
    curl_easy_cleanup(curl);

    return true;
}
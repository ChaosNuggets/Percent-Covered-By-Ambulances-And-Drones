//// Removes the concave corners from the isochrones and outputs the new isochrones to the ConvexIsochrones folder
//#include "extract_polygons.h"
//#include "remove_concave_corners.h"
//#include <fstream>
//#include <string>
//
//static std::string generateFilePath(const int stationNum)
//{
//    std::string filePath = "ConvexIsochrones/";
//    filePath.append(std::to_string(stationNum));
//    filePath.append(".json");
//    return filePath;
//}
//
//static void writePolygons()
//{
//    for (int i = 0; i < isochrones.size(); i++)
//    {
//        std::ofstream fout(generateFilePath(i));
//        fout << "{\"features\":[{\"properties\":{\"fill\":\"#bf4040\",\"fillOpacity\":0.33,\"fill - opacity\":0.33,\"fillColor\":\"#bf4040\",\"color\":\"#bf4040\",\"contour\":5,\"opacity\":0.33,\"metric\":\"time\"},\"geometry\":{\"coordinates\":[";
//        for (int j = 0; j < isochrones[i].size(); j++)
//        {
//            fout << '[' << isochrones[i][j].lon << ',' << isochrones[i][j].lat << ']';
//            if (j < isochrones[i].size() - 1)
//            {
//                fout << ',';
//            }
//        }
//        fout << "],\"type\":\"LineString\"},\"type\":\"Feature\"}],\"type\":\"FeatureCollection\"}";
//        fout.close();
//    }
//}
//
//int main()
//{
//    extractPolygons();
//    removeConcaveCorners(isochrones);
//    writePolygons();
//}
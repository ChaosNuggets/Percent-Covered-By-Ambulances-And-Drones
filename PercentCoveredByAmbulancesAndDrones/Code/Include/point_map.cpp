#define _USE_MATH_DEFINES
#include "point_map.h"
#include "calculate_bounding_box.h"
#include "calculate_distance.h"
#include "check_if_inside.h"
#include <cmath>
#include <stdint.h>

const double LOWEST_LAT = 37.779567; // Lowest latitude that I have in the point map
const double LOWEST_LONG = -88.057587; // Lowest longitude that I have in the point map
const double HIGHEST_LAT = 41.760356; // Highest latitude that I have in the point map
const double HIGHEST_LONG = -84.805966; // Highest longitude that I have in the point map
const double LONG_IN_1_MILE = 0.01836529538; // Longitude per mile at LOWEST_LAT
const double LAT_IN_1_MILE = 0.01447228581; // Latitude per mile
const double MILES_BETWEEN_POINTS = 1; // Controls the density of the point map
const int LAT_SIZE = ceil((HIGHEST_LAT - LOWEST_LAT) / (LAT_IN_1_MILE * MILES_BETWEEN_POINTS)) + 1; // Vertical size of pointMap matrix
const int LONG_SIZE = ceil((HIGHEST_LONG - LOWEST_LONG) / (LONG_IN_1_MILE * MILES_BETWEEN_POINTS)) + 1; // Horizontal size of pointMap matrix

Index coordToIndex(const Point& coordinate, double (*roundFunc)(double))
{
    const auto& [latitude, longitude] = coordinate; // Extract data

    // Calculate the coordinate's difference in miles from LOWEST_LAT and LOWEST_LONG
    const double latMileDifference = (latitude - LOWEST_LAT) / LAT_IN_1_MILE;
    double longMileDifference = calculateDistance({ latitude, LOWEST_LONG }, {latitude, longitude});

    if (longitude < LOWEST_LONG) longMileDifference *= -1;

    // Round the difference so it's an integer
    int latIndex = roundFunc(latMileDifference / MILES_BETWEEN_POINTS); // Also why is round not constexpr aaaaa
    int longIndex = roundFunc(longMileDifference / MILES_BETWEEN_POINTS);

    return { latIndex, longIndex };
}

Point indexToCoord(const Index& index)
{
    const auto [latIndex, longIndex] = index; // Extract data

    // Do the conversion
    const double latitude = calcCoordLat({ LOWEST_LAT, 0 }, latIndex * MILES_BETWEEN_POINTS);
    
    double longitude;
    if (longIndex == 0)
    {
        longitude = LOWEST_LONG;
    }
    else
    {
        longitude = calcCoordLong({ latitude, LOWEST_LONG }, longIndex * MILES_BETWEEN_POINTS);
    }

    return { latitude, longitude };
}

std::vector<std::vector<uint8_t>> createPointMap()
{
    std::vector<std::vector<uint8_t>> pointMap;

    // Resizes pointMap to be a 2d matrix of points of size latSize by longSize
    pointMap.resize(LAT_SIZE, std::vector<uint8_t>(LONG_SIZE, OUTSIDE));

    // These are the coordinates of vertexes of the polygon that make up the Indiana border
    const Polygon indianaBorder =
    {
        {41.712028, -87.524126}, // Northwest corner, going clockwise
        {41.690771, -87.400318},
        {41.639333, -87.399395},
        {41.760356, -86.826046},
        {41.760173, -84.805966},
        {4.050180E+01, -8.479955E+01}, // This is where we start getting coordinates from http://download.geofabrik.de/north-america/us/indiana.poly
        {3.981420E+01, -8.481118E+01},
        {3.910692E+01, -8.481607E+01},
        {3.909997E+01, -8.481412E+01},
        {3.909997E+01, -8.481412E+01},
        {3.909569E+01, -8.482383E+01},
        {3.905851E+01, -8.488616E+01},
        {3.903368E+01, -8.486521E+01},
        {3.898492E+01, -8.482668E+01},
        {3.895580E+01, -8.482668E+01},
        {3.891977E+01, -8.487087E+01},
        {3.890696E+01, -8.485532E+01},
        {3.889582E+01, -8.479364E+01},
        {3.887524E+01, -8.478042E+01},
        {3.885552E+01, -8.479033E+01},
        {3.883149E+01, -8.482338E+01},
        {3.878400E+01, -8.479774E+01},
        {3.877955E+01, -8.482699E+01},
        {3.878966E+01, -8.488983E+01},
        {3.877208E+01, -8.493529E+01},
        {3.877321E+01, -8.499056E+01},
        {3.869751E+01, -8.512078E+01},
        {3.867430E+01, -8.519678E+01},
        {3.872674E+01, -8.525626E+01},
        {3.872760E+01, -8.539725E+01},
        {3.873208E+01, -8.541767E+01},
        {3.871881E+01, -8.543555E+01},
        {3.869512E+01, -8.545095E+01},
        {3.866793E+01, -8.543585E+01},
        {3.861156E+01, -8.542739E+01},
        {3.855164E+01, -8.540671E+01},
        {3.852325E+01, -8.542049E+01},
        {3.850251E+01, -8.546899E+01},
        {3.845983E+01, -8.549409E+01},
        {3.844865E+01, -8.556092E+01},
        {3.843054E+01, -8.560885E+01},
        {3.836938E+01, -8.562846E+01},
        {3.830885E+01, -8.565503E+01},
        {3.828239E+01, -8.569740E+01},
        {3.826335E+01, -8.575323E+01},
        {3.828416E+01, -8.578650E+01},
        {3.827265E+01, -8.582767E+01},
        {3.823339E+01, -8.583809E+01},
        {3.821507E+01, -8.585530E+01},
        {3.818493E+01, -8.589196E+01},
        {3.814192E+01, -8.590606E+01},
        {3.809533E+01, -8.589929E+01},
        {3.806959E+01, -8.590888E+01},
        {3.804250E+01, -8.591170E+01},
        {3.801193E+01, -8.592927E+01},
        {3.799940E+01, -8.596584E+01},
        {3.799540E+01, -8.600757E+01},
        {3.795102E+01, -8.604291E+01},
        {3.796651E+01, -8.606510E+01},
        {3.799852E+01, -8.608371E+01},
        {3.801229E+01, -8.612713E+01},
        {3.800518E+01, -8.616830E+01},
        {3.803939E+01, -8.625515E+01},
        {3.807314E+01, -8.628391E+01},
        {3.813422E+01, -8.628408E+01},
        {3.811885E+01, -8.638204E+01},
        {3.805671E+01, -8.643449E+01},
        {3.803895E+01, -8.647058E+01},
        {3.803850E+01, -8.651344E+01},
        {3.797273E+01, -8.651908E+01},
        {3.792871E+01, -8.650160E+01},
        {3.791269E+01, -8.652980E+01},
        {3.791670E+01, -8.659071E+01},
        {3.785839E+01, -8.659353E+01},
        {3.783777E+01, -8.663667E+01},
        {3.783880E+01, -8.667135E+01},
        {3.791047E+01, -8.665951E+01},
        {3.788688E+01, -8.671308E+01},
        {3.789934E+01, -8.674636E+01},
        {3.798982E+01, -8.682841E+01},
        {3.793627E+01, -8.690652E+01},
        {3.791397E+01, -8.700589E+01},
        {3.782454E+01, -8.705090E+01},
        {3.778945E+01, -8.707680E+01},
        {3.777761E+01, -8.710959E+01},
        {3.778465E+01, -8.713368E+01},
        {3.783439E+01, -8.716486E+01},
        {3.785012E+01, -8.723734E+01},
        {3.793965E+01, -8.740908E+01},
        {3.794104E+01, -8.744062E+01},
        {3.790309E+01, -8.750909E+01},
        {3.791001E+01, -8.754509E+01},
        {3.796714E+01, -8.759300E+01},
        {3.791804E+01, -8.762007E+01},
        {3.789205E+01, -8.758263E+01},
        {3.786107E+01, -8.758533E+01},
        {3.783043E+01, -8.760699E+01},
        {3.782027E+01, -8.763867E+01},
        {3.782758E+01, -8.768459E+01},
        {3.785751E+01, -8.768369E+01},
        {3.789526E+01, -8.767151E+01},
        {3.789454E+01, -8.770264E+01},
        {3.787531E+01, -8.776715E+01},
        {3.787233E+01, -8.781716E+01},
        {3.787888E+01, -8.784295E+01},
        {3.791804E+01, -8.787769E+01},
        {3.792334E+01, -8.790113E+01},
        {3.788386E+01, -8.793634E+01},
        {3.784468E+01, -8.790656E+01},
        {3.783542E+01, -8.789934E+01},
        {3.780512E+01, -8.790250E+01},
        {3.778701E+01, -8.793316E+01},
        {3.776911E+01, -8.794807E+01},
        {3.777622E+01, -8.797162E+01},
        {3.779407E+01, -8.799725E+01},
        {3.779603E+01, -8.802744E+01},
        {3.779202E+01, -8.802919E+01},
        {3.779822E+01, -8.802899E+01},
        {3.778858E+01, -8.805789E+01},
        {3.781651E+01, -8.809520E+01},
        {3.790626E+01, -8.810327E+01},
        {3.792839E+01, -8.807251E+01},
        {3.795954E+01, -8.803647E+01},
        {3.797480E+01, -8.801739E+01},
        {3.802473E+01, -8.803098E+01},
        {3.804039E+01, -8.804724E+01},
        {3.808599E+01, -8.802063E+01},
        {3.810370E+01, -8.801637E+01},
        {3.813127E+01, -8.797298E+01},
        {3.813762E+01, -8.794561E+01},
        {3.816014E+01, -8.793530E+01},
        {3.819181E+01, -8.797243E+01},
        {3.821225E+01, -8.798595E+01},
        {3.822034E+01, -8.798606E+01},
        {3.823103E+01, -8.798812E+01},
        {3.826321E+01, -8.798874E+01},
        {3.826247E+01, -8.794757E+01},
        {3.827328E+01, -8.795410E+01},
        {3.829760E+01, -8.793700E+01},
        {3.830480E+01, -8.792134E+01},
        {3.829687E+01, -8.790476E+01},
        {3.827718E+01, -8.790786E+01},
        {3.828728E+01, -8.789016E+01},
        {3.831116E+01, -8.788046E+01},
        {3.831438E+01, -8.786701E+01},
        {3.829384E+01, -8.784077E+01},
        {3.832471E+01, -8.783420E+01},
        {3.834730E+01, -8.782413E+01},
        {3.836387E+01, -8.780723E+01},
        {3.837198E+01, -8.778284E+01},
        {3.841011E+01, -8.774648E+01},
        {3.843562E+01, -8.774257E+01},
        {3.845827E+01, -8.774607E+01},
        {3.847706E+01, -8.774176E+01},
        {3.849815E+01, -8.768090E+01},
        {3.854120E+01, -8.766169E+01},
        {3.856825E+01, -8.765398E+01},
        {3.859206E+01, -8.764045E+01},
        {3.860580E+01, -8.762929E+01},
        {3.864024E+01, -8.762261E+01},
        {3.866580E+01, -8.760417E+01},
        {3.867594E+01, -8.757855E+01},
        {3.868486E+01, -8.753289E+01},
        {3.869629E+01, -8.752329E+01},
        {3.871267E+01, -8.752201E+01},
        {3.873952E+01, -8.750576E+01},
        {3.874623E+01, -8.750423E+01},
        {3.877281E+01, -8.751673E+01},
        {3.878136E+01, -8.750017E+01},
        {3.881820E+01, -8.752913E+01},
        {3.882787E+01, -8.752424E+01},
        {3.884541E+01, -8.753302E+01},
        {3.886258E+01, -8.755548E+01},
        {3.889545E+01, -8.754419E+01},
        {3.892412E+01, -8.752066E+01},
        {3.897081E+01, -8.752989E+01},
        {3.898751E+01, -8.757929E+01},
        {3.900127E+01, -8.758066E+01},
        {3.901830E+01, -8.757200E+01},
        {3.903362E+01, -8.757583E+01},
        {3.905622E+01, -8.757394E+01},
        {3.907753E+01, -8.759478E+01},
        {3.908390E+01, -8.761280E+01},
        {3.910381E+01, -8.763089E+01},
        {3.912780E+01, -8.764500E+01},
        {3.914451E+01, -8.764743E+01},
        {3.916768E+01, -8.764118E+01},
        {3.919989E+01, -8.758964E+01},
        {3.925951E+01, -8.760779E+01},
        {3.928142E+01, -8.761271E+01},
        {3.931352E+01, -8.760302E+01},
        {3.933508E+01, -8.759145E+01},
        {3.934311E+01, -8.757861E+01},
        {3.935564E+01, -8.754447E+01},
        {3.935263E+01, -8.753259E+01},
        {3.988296E+01, -8.753596E+01},
        {4.049141E+01, -8.752911E+01},
        {4.147453E+01, -8.752660E+01},
        {4.156283E+01, -8.752560E+01},
        {4.156769E+01, -8.752613E+01},
        {4.168917E+01, -8.752496E+01}
    };
    
    // Fill the points in pointMap to be spaced less than MILES_BETWEEN_POINTS apart
    for (int i = 0; i < LAT_SIZE; i++)
    {
        for (int j = 0; j < LONG_SIZE; j++)
        {
            // If the point is inside the polyline border, set it to true
            const Point p = indexToCoord({i, j});
            if (checkIfInside(indianaBorder, p))
            {
                pointMap[i][j] = NOT_COVERED;
            }
        }
    }

    return pointMap;
}
#include <limits.h>

#include <DBASEV/GPS_functions.h>
#include <DBASEV/Map.h>

using namespace std;

#define WAYPOINT_DISTANCE 5 // unit: Meter

void vehicle_control()
{
    //==================vehicle main init====================
    unordered_map<int, Vertex> graph = creatingMap();

    int road_id = 1;
    printf("now road: %d\n", road_id);

    //==========================================

    //=========== should get two parameter of gps value============ //
    //============ and also get graph parameter================= //
    // =========== must talk with jaesung ======================= //  
    string pre_gps = "$GPGGA,114455.532,3592.1011,N,12860.2372,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F";
    string current_gps = "$GPGGA,114456.532,3591.9281,N,12861.3479,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F";
    cout << "speed: " << getSpeed(getDistance(pre_gps, current_gps), pre_gps, current_gps) << " m/s \n\n";

    float current_latitude, current_longitude;
    current_latitude = extract_gps_data(current_gps).latitude;
    current_longitude = extract_gps_data(current_gps).longitude;

    int pre_waypoint = 0, now_waypoint;
    now_waypoint = calculateClosestWaypoint(road_id, pre_waypoint, current_latitude, current_longitude, graph);
    pre_waypoint = now_waypoint;
    printf("now_waypoint: %d\n\n", now_waypoint);

    int remain_waypoints = graph[road_id].waypoints.back() - now_waypoint;
    if (remain_waypoints < 2)
    {
        road_id = findNextRoadId(road_id, current_latitude, current_longitude, graph);
        printf("now road: %d\n", road_id);
    }

    pre_gps = current_gps;
}

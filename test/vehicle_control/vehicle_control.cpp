#include <limits.h>
#include <windows.h>

// ==============temp==============
#include <queue>
// ==============================

#include "GPS_functions.h"
#include "Map.h"
#include "MyIMU.h"

using namespace std;

#define WAYPOINT_DISTANCE 5 // unit: Meter


int main()
{
    //==================init====================
    unordered_map<int, Vertex> graph = creatingMap();

    int road_id = 1;
    printf("now road: %d\n", road_id);

    //==========================================
    
    
    // ============temp================
    queue<string> gps;

    gps.push("$GPGGA,114452.532,3592.1011,N,12860.2372,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F");
    gps.push("$GPGGA,114453.542,3592.0511,N,12860.4058,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F");
    gps.push("$GPGGA,114454.568,3592.0413,N,12860.6813,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F");
    gps.push("$GPGGA,114455.512,3592.0011,N,12861.0185,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F");
    gps.push("$GPGGA,114456.532,3591.9281,N,12861.3479,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F");
    gps.push("$GPGGA,114457.666,3591.9529,N,12861.3521,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F");
    gps.push("$GPGGA,114458.666,3592.0608,N,12861.3729,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F");
    gps.push("$GPGGA,114459.158,3592.2285,N,12861.4110,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F");

    // ============================
    string pre_gps="";
    float current_latitude, current_longitude;
    int pre_waypoint = 0, now_waypoint;

    while (1) {
        // =========temp====================
        if (gps.empty()) {
            // if queue empty, go next step
            continue;
        }
        

        string current_gps = gps.front();
        gps.pop();
        if (pre_gps!=""){
            cout << "speed: " << getSpeed(getDistance(pre_gps, current_gps), pre_gps, current_gps) << " m/s \n\n";
        }

        // ==================================

        current_latitude = extract_gps_data(current_gps).latitude;
        current_longitude = extract_gps_data(current_gps).longitude;

        now_waypoint = calculateClosestWaypoint(road_id, pre_waypoint, current_latitude, current_longitude, graph);
        pre_waypoint = now_waypoint;
        printf("now_waypoint: %d\n\n", graph[road_id].waypoints[now_waypoint]);

        int remain_waypoints = graph[road_id].waypoints.back() - now_waypoint;
        
        if (remain_waypoints < 2) {
            int pre_road_id = road_id;

            while(pre_road_id == road_id){
                road_id = findNextRoadId(road_id, current_latitude, current_longitude, graph);
            }
            pre_waypoint = 0;
            
            printf("now road: %d\n", road_id);
        }

        pre_gps = current_gps;

        Sleep(500);
    }

    return 0;
}
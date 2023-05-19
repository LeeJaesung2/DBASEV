#include "GPS_functions.h"
#include "CreatingMap.h"
#include "MyIMU.h"

using namespace std;

#define WAYPOINT_DISTANCE 5 // unit: Meter

vector<int> getWaypoints(const unordered_map<int, Vertex>& graph, int roadId) {
    vector<int> waypoints;

    if (graph.find(roadId) != graph.end()) {
        waypoints = graph.at(roadId).waypoints;
    }

    return waypoints;
}

int main() {
    unordered_map<int, Vertex> graph = creatingMap();

    int road_id = 1;
    float vehicle_velocity = 0.0, remain_distance = 0.0;
    size_t total_passed_waypoint = 0;
    int passed_num_waypoint = 0, sum_remain_distance = 0;

    vector<int> waypoints = getWaypoints(graph, road_id);

    int remain_waypoints = waypoints.size();

    while (remain_waypoints > 0) {
        vehicle_velocity = getVelocity();

        passed_num_waypoint = static_cast<int>(vehicle_velocity / WAYPOINT_DISTANCE);

        remain_distance = vehicle_velocity - (passed_num_waypoint * WAYPOINT_DISTANCE);
        if (remain_distance > 0) {
            sum_remain_distance += remain_distance;
            if (sum_remain_distance >= WAYPOINT_DISTANCE) {
                passed_num_waypoint++;
                sum_remain_distance -= WAYPOINT_DISTANCE;
            }
        }

        total_passed_waypoint += passed_num_waypoint;
        if (total_passed_waypoint > waypoints.size()) {
            total_passed_waypoint = waypoints.size();
        }
        cout << "Currently passed " << total_passed_waypoint << " waypoints.\n";

        remain_waypoints -= passed_num_waypoint;
        cout << "remain_waypoints: " << remain_waypoints << "\n\n";

        if (remain_waypoints < 1) {
            // Condition to wake up GPS thread
            // Need to find road_id in GPS_main.cpp

        }
    }

    return 0;
}
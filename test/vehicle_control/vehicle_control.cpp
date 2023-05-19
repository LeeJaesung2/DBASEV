#include <limits>

#include "GPS_functions.h"
#include "Map.h"
#include "MyIMU.h"

using namespace std;

#define WAYPOINT_DISTANCE 5 // unit: Meter

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

        // 해당 road_id의 남은 웨이포인트가 1개 미만이면
        if (remain_waypoints < 1) {
            // Condition to wake up GPS thread
            // Need to find road_id in GPS_main.cpp

            // 현재 GPS값을 읽어와
            string gps = "$GPGGA,114455.532,3591.9538,N,12861.3522,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F";

            // 현재 GPS값을 gps_function을 이용해서 위도, 경도를 추출한다.
            float current_latitude = extract_gps_data(gps).latitude;
            float current_longitude = extract_gps_data(gps).longitude;

            // 현재 road_id와 연결되어 있는 모든 인접 road_id의 첫 waypoint의 위도 경도를 들고온다.
            vector<int> connected_roads = graph[road_id].connectedRoads;
            float min_distance = numeric_limits<float>::max(); // 최솟값 초기화
            int nearest_road_id = -1;
            vector<float> first_waypoint_coordinates;

            for (int connected_road : connected_roads) {
                vector<int> connected_waypoints = getWaypoints(graph, connected_road);
                if (!connected_waypoints.empty()) {
                    int first_waypoint_id = connected_waypoints[0];

                    cout << "first_waypoint_id: " << first_waypoint_id;

                    float waypoint_latitude = graph[connected_road].latitude[0];
                    float waypoint_longitude = graph[connected_road].longitude[0];

                    // 저장할 웨이포인트인 경우 위도 경도 저장
                    if (first_waypoint_id == 1) {
                        first_waypoint_coordinates = {waypoint_latitude, waypoint_longitude};
                    }

                    // 두 gps 값의 거리를 측정한다.
                    float distance = calc_distance(current_latitude, current_longitude, waypoint_latitude, waypoint_longitude);
                    if (distance < min_distance) {
                        min_distance = distance;
                        nearest_road_id = connected_road;
                    }

                    cout << "     distance: " << distance << "\n";
                }
            }

            // 거리가 유효한 값인지 확인하고, 3m 이내로 들어가면 해당 road_id로 변경한다
            if (nearest_road_id != -1 && min_distance <= 3) {
                road_id = nearest_road_id;
                waypoints = getWaypoints(graph, road_id);
                remain_waypoints = waypoints.size();
                cout << "Switched to road ID: " << road_id << endl;
                cout << "remain_waypoints: " << remain_waypoints << endl << endl;
            }
        }
    }

    return 0;
}

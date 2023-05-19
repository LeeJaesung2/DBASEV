#include <iostream>
#include <vector>

#include "CreatingMap.h"

using namespace std;

#define WAYPOINT_DISTANCE 5 // unit: Meter

double getVelocity() {
    return 12.0;
}

vector<int> getWaypoints() { // 초기 road_id는 1번으로 세팅하기
    vector<int> waypoints;

    //map에서 waypoint 불러오기
    //우선 20개의 waypoint 있다고 가정
    for (int i = 1; i <= 20; i++) {
        waypoints.push_back(i);
    }
    return waypoints;
}

int main() {

    creatingMap();

    double vehicle_velocity = 0.0, remain_distance = 0.0;
    int total_passed_waypoint = 0, passed_num_waypoint = 0, sum_remain_distance = 0;

    vector<int> waypoints = getWaypoints(); // waypoint 가져오는 함수 구현 필요

    int remain_waypoints = waypoints.size();

    while (remain_waypoints > 0) {
        vehicle_velocity = getVelocity();// 단위 : m/s => 속도 가져오는 함수 구현 필요

        passed_num_waypoint = static_cast<int>(vehicle_velocity / WAYPOINT_DISTANCE);  // 웨이포인트를 지나간 개수

        // 웨이포인트가 정확히 나누어 떨어지지 않으면
        remain_distance = vehicle_velocity - (passed_num_waypoint * WAYPOINT_DISTANCE);  // 남은 거리
        if (remain_distance > 0) {
            sum_remain_distance += remain_distance;
            cout << "sum_remain_distance: " << sum_remain_distance << "\n";
            if (sum_remain_distance >= WAYPOINT_DISTANCE) {
                passed_num_waypoint++;
                sum_remain_distance -= WAYPOINT_DISTANCE;
            }
        }

        total_passed_waypoint += passed_num_waypoint;
        if (total_passed_waypoint > waypoints.size()) {
            total_passed_waypoint = waypoints.size();
        }
        cout << "현재 " << total_passed_waypoint << "개의 웨이포인트를 지났습니다.\n";
 

        remain_waypoints -= passed_num_waypoint;
        cout << "remain_waypoints: " << remain_waypoints << "\n\n";

        if (remain_waypoints > 4) {
            // GPS 쓰레드 깨우기용 if문 
            // GPS_main.cpp로 road_id 찾아야함
        }

        vehicle_velocity += 4;
    }

    return 0;
}
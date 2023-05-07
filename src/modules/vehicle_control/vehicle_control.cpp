#include <iostream>

#include "GPSData.h"
#include "GPS_functions.h"

using namespace std;

int main() { // main 함수는 차량의 기능만 있는 함수로 만들기
    // 0. src/drivers/gps에 init 함수(GPS 값 받아오기, (시간,속도,거리)추출 하기)
    // 1. gps Data 받아오는 모듈 만들기
    string gps_data1 = "$GPGGA,114455.532,3591.0291,N,12860.1963,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F";
    string gps_data2 = "$GPGGA,114465.532,3590.6390,N,12860.5123,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F";

    // 2. 받아온 gps Data에서 추출하는 함수 따로 빼기
    GPSData gps_data1_parsed = extract_gps_data(gps_data1);
    GPSData gps_data2_parsed = extract_gps_data(gps_data2);

    // 3. map Data 확인 하기

    // 4. 거리, 속도, 시간 출력 함수 만들기
    if (gps_data1_parsed.latitude != 0.0 && gps_data1_parsed.longitude != 0.0 &&
        gps_data2_parsed.latitude != 0.0 && gps_data2_parsed.longitude != 0.0) {

        double distance = calc_distance(gps_data1_parsed.latitude, gps_data1_parsed.longitude, gps_data2_parsed.latitude, gps_data2_parsed.longitude);
        double time_interval = gps_data2_parsed.time - gps_data1_parsed.time;
        double speed = distance / time_interval;

        std::cout << "Distance: " << distance << " meters" << std::endl;
        std::cout << "Time Interval: " << time_interval << " seconds" << std::endl;
        std::cout << "Speed: " << speed << " meters/second" << std::endl;
    }
    else {
        std::cout << "Failed to extract GPS data from input string" << std::endl;
    }

    return 0;
}
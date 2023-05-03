#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <corecrt_math_defines.h>

#define LATITUDE 1
#define LONGITUDE 2

using namespace std;

// 구조체 선언
struct GPSData {
    double latitude;
    double longitude;
    double time;
};

string rawGps2degGps(int type, string token); 

// GPGGA 데이터에서 위도, 경도, 시간 정보를 추출하는 함수
GPSData extract_gps_data(const string& gps_str);

// 두 점 사이의 거리를 계산하는 함수
double calc_distance(double lat1, double lon1, double lat2, double lon2);

int main() {
    string gps_data1 = "$GPGGA,114455.532,3591.0291,N,12860.1963,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F";
    string gps_data2 = "$GPGGA,114465.532,3590.6390,N,12860.5123,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F";

    GPSData gps_data1_parsed = extract_gps_data(gps_data1);
    GPSData gps_data2_parsed = extract_gps_data(gps_data2);

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

string rawGps2degGps(int type, string token) {
    int degrees;
    double minutes;

    if (type == LATITUDE) { // latitude 
        degrees = stoi(token.substr(0, 2));
        minutes = stod(token.substr(2));
    } 
    else if(type == LONGITUDE) { // longitude
        degrees = stoi(token.substr(0, 3));
        minutes = stod(token.substr(3));
    }
   
    double deg_minutes = minutes / 60;
    string str_minutes = to_string(deg_minutes);
    str_minutes.erase(remove(str_minutes.begin(), str_minutes.end(), '.'), str_minutes.end()); // 공백 제거

    string what3words = to_string(degrees).append(".").append(str_minutes);

    return what3words;
}

GPSData extract_gps_data(const string& gps_str) {
    GPSData gps_data;

    // 문자열을 ','로 구분하여 문자열 스트림에 삽입
    stringstream ss(gps_str);
    string token;

    // GPGGA 태그 제거
    getline(ss, token, ',');

    // 시간 정보 추출
    getline(ss, token, ',');
    gps_data.time = stod(token);

    // 위도 정보 추출
    getline(ss, token, ',');
    string latitude = rawGps2degGps(LATITUDE, token);
    gps_data.latitude = stod(latitude);

    // 북위/남위 정보 추출
    getline(ss, token, ',');

    // 경도 정보 추출
    getline(ss, token, ',');
    string longitude = rawGps2degGps(LONGITUDE, token);
    gps_data.longitude = stod(longitude);

    // 동경/서경 정보 추출
    getline(ss, token, ',');

    return gps_data;
}

double calc_distance(double lat1, double lon1, double lat2, double lon2); {
    const double R = 6371e3; // 지구 반지름
    double phi1 = lat1 * M_PI / 180; // 위도 1
    double phi2 = lat2 * M_PI / 180; // 위도 2
    double delta_phi = (lat2 - lat1) * M_PI / 180; // 위도 차이
    double delta_lambda = (lon2 - lon1) * M_PI / 180; // 경도 차이

    double a = sin(delta_phi / 2) * sin(delta_phi / 2) +
        cos(phi1) * cos(phi2) *
        sin(delta_lambda / 2) * sin(delta_lambda / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c; // in meters
}
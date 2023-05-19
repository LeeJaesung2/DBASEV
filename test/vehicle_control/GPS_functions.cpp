#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "GPS_functions.h"

#define LATITUDE 1
#define LONGITUDE 2

std::string rawGps2degGps(int type, std::string token) {
    int degrees;
    double minutes;

    if (type == LATITUDE) { // latitude 
        degrees = stoi(token.substr(0, 2));
        minutes = stod(token.substr(2));
    }
    else if (type == LONGITUDE) { // longitude
        degrees = stoi(token.substr(0, 3));
        minutes = stod(token.substr(3));
    }

    double deg_minutes = minutes / 60;
    std::string str_minutes = std::to_string(deg_minutes);
    str_minutes.replace(str_minutes.find("."), 1, "");

    std::string what3words = std::to_string(degrees).append(".").append(str_minutes);

    return what3words;
}

GPSData extract_gps_data(const std::string& gps_str) {
    GPSData gps_data;

    // 문자열을 ','로 구분하여 문자열 스트림에 삽입
    std::stringstream ss(gps_str);
    std::string token;

    // GPGGA 태그 제거
    getline(ss, token, ',');

    // 시간 정보 추출
    getline(ss, token, ',');
    try {
        gps_data.time = std::stod(token);
    }
    catch (std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        gps_data.time = 0.0;
    }

    // 위도 정보 추출
    getline(ss, token, ',');
    try {
        std::string latitude = rawGps2degGps(LATITUDE, token);
        gps_data.latitude = std::stod(latitude);
    }
    catch (std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        gps_data.latitude = 0.0;
    }

    // 북위/남위 정보 추출
    getline(ss, token, ',');

    // 경도 정보 추출
    getline(ss, token, ',');
    try {
        std::string longitude = rawGps2degGps(LONGITUDE, token);
        gps_data.longitude = std::stod(longitude);
    }
    catch (std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        gps_data.longitude = 0.0;
    }

    // 동경/서경 정보 추출
    getline(ss, token, ',');

    return gps_data;
}

double calc_distance(double lat1, double lon1, double lat2, double lon2) {
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
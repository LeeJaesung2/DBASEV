#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "GPS_functions.h"

#define LATITUDE 1
#define LONGITUDE 2

using namespace std;

bool isValidGPSData(const string& gpsData) {
    // GPS 데이터 파싱
    GPSData gpsDataParsed = extract_gps_data(gpsData);

    // 위도 또는 경도가 0.0인 경우 유효하지 않은 GPS 데이터로 판단
    if (gpsDataParsed.latitude == 0.0 || gpsDataParsed.longitude == 0.0) {
        return false;
    }

    return true;
}

string rawGps2degGps(int type, string token) {
    int degrees;
    float minutes;

    if (type == LATITUDE) { // latitude 
        degrees = stoi(token.substr(0, 2));
        minutes = stod(token.substr(2));
    }
    else if (type == LONGITUDE) { // longitude
        degrees = stoi(token.substr(0, 3));
        minutes = stod(token.substr(3));
    }

    float deg_minutes = minutes / 60;
    string str_minutes = to_string(deg_minutes);
    str_minutes.replace(str_minutes.find("."), 1, "");

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
    try {
        gps_data.time = stod(token);
    }
    catch (invalid_argument& e) {
        #ifndef DEBUG
            //cerr << "Invalid argument: " << e.what() << endl;
        #endif
        gps_data.time = 0.0;
    }

    // 위도 정보 추출
    getline(ss, token, ',');
    try {
        string latitude = rawGps2degGps(LATITUDE, token);
        gps_data.latitude = stod(latitude);
    }
    catch (invalid_argument& e) {
        #ifndef DEBUG
            //cerr << "Invalid argument: " << e.what() << endl;
        #endif
        gps_data.latitude = 0.0;
    }

    // 북위/남위 정보 추출
    getline(ss, token, ',');

    // 경도 정보 추출
    getline(ss, token, ',');
    try {
        string longitude = rawGps2degGps(LONGITUDE, token);
        gps_data.longitude = stod(longitude);
    }
    catch (invalid_argument& e) {
        #ifndef DEBUG
            //cerr << "Invalid argument: " << e.what() << endl;
        #endif
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

    double distance = R * c; // in meters
    
    return distance;
}

float getDistance(string gps_data1, string gps_data2){
    GPSData gps_data1_parsed = extract_gps_data(gps_data1);
    GPSData gps_data2_parsed = extract_gps_data(gps_data2);

    float distance = calc_distance(gps_data1_parsed.latitude, gps_data1_parsed.longitude, 
        gps_data2_parsed.latitude, gps_data2_parsed.longitude);

    return distance;
}

float getSpeed(float distance, string gps_data1, string gps_data2){
    GPSData gps_data1_parsed = extract_gps_data(gps_data1);
    GPSData gps_data2_parsed = extract_gps_data(gps_data2);

    float speed = 0.0;

    float time_interval = gps_data2_parsed.time - gps_data1_parsed.time;
    speed = distance / time_interval;

    return speed;
}
#ifndef GPS_FUNCTIONS_H
#define GPS_FUNCTIONS_H

#include <DBASEV/visibility.h>
#include <cmath>
#include <sstream>
#include <wiringSerial.h> // g++ -o file file.cpp -lwiringPi

#define LATITUDE 1
#define LONGITUDE 2

struct GPSData {
    double latitude;
    double longitude;
    double time;
};

void *getGPS(void *arg);
bool isValidGPSData(const std::string& gpsData);
std::string rawGps2degGps(int type, std::string token);
GPSData extract_gps_data(const std::string& gps_str);
double calc_distance(double lat1, double lon1, double lat2, double lon2);
float getDistance(string gps_data1, string gps_data2);
float getSpeed(float distance, string gps_data1, string gps_data2);

#endif // GPS_FUNCTIONS_H

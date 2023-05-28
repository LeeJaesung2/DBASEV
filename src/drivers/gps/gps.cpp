#include <DBASEV/gps.h>

void *getGPS(void* arg){
    int fd = serialOpen("/dev/serial0", 9600);
    
    MsgBuf msg;
    key_t key = 1234;
    int key_id = mq_init(key);
    struct msqid_ds buf;
    
    while (1) {
        if (serialDataAvail(fd)) {
            char temp = (char)serialGetchar(fd);
        
            if (temp == '$') {
                string sentence;
                sentence += temp;

                while (temp != '\n') {
                    temp = (char)serialGetchar(fd);
                    sentence += temp;
                }

                if (sentence.find("GPGGA") != string::npos) {
                    cout << "sentence.c_str(): " << sentence.c_str() << endl;
                    strcpy(msg.buf, sentence.c_str());
                    push(key_id,buf, msg);
                }
            }
        }
    }
}

bool isValidGPSData(const string& gpsData) {
    // GPS data parsing
    GPSData gpsDataParsed = extract_gps_data(gpsData);

    if (gpsDataParsed.latitude == 0.0 || gpsDataParsed.longitude == 0.0) {
        return false;
    }

    return true;
}

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
    
    // Split the string by ',' and insert into string stream
    std::stringstream ss(gps_str);
    std::string token;

    // Remove the GPGGA tag
    getline(ss, token, ',');

    // Extract time information
    getline(ss, token, ',');
    try {
        gps_data.time = std::stod(token);
    }
    catch (std::invalid_argument& e) {
        //std::cerr << "Invalid argument: " << e.what() << std::endl;
        gps_data.time = 0.0;
    }

    // Extract latitude information
    getline(ss, token, ',');
    try {
        std::string latitude = rawGps2degGps(LATITUDE, token);
        gps_data.latitude = std::stod(latitude);
    }
    catch (std::invalid_argument& e) {
        //std::cerr << "Invalid argument: " << e.what() << std::endl;
        gps_data.latitude = 0.0;
    }

    // Extract N/S information
    getline(ss, token, ',');

    // Extract longitude information
    getline(ss, token, ',');
    try {
        std::string longitude = rawGps2degGps(LONGITUDE, token);
        gps_data.longitude = std::stod(longitude);
    }
    catch (std::invalid_argument& e) {
        //std::cerr << "Invalid argument: " << e.what() << std::endl;
        gps_data.longitude = 0.0;
    }

    // Extract E/W information
    getline(ss, token, ',');

    return gps_data;
}

double calc_distance(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371e3; // Earth's radius
    double phi1 = lat1 * M_PI / 180; // latitude 1
    double phi2 = lat2 * M_PI / 180; // latitude 2
    double delta_phi = (lat2 - lat1) * M_PI / 180; // difference in latitude
    double delta_lambda = (lon2 - lon1) * M_PI / 180; // difference in longitude

    double a = sin(delta_phi / 2) * sin(delta_phi / 2) +
    cos(phi1) * cos(phi2) *
    sin(delta_lambda / 2) * sin(delta_lambda / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c; // in meters
}

float getDistance(string gps_data1, string gps_data2){
    GPSData gps_data1_parsed = extract_gps_data(gps_data1);
    GPSData gps_data2_parsed = extract_gps_data(gps_data2);

    float distance = 0.0;

    if (gps_data1_parsed.latitude != 0.0 && gps_data1_parsed.longitude != 0.0 &&
        gps_data2_parsed.latitude != 0.0 && gps_data2_parsed.longitude != 0.0) {
        
        float distance = calc_distance(gps_data1_parsed.latitude, gps_data1_parsed.longitude, 
        gps_data2_parsed.latitude, gps_data2_parsed.longitude);
    }

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
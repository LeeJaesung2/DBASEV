#include <DBASEV/accel.h>
float accel_scale_factor = 0;
void set_accel(int device_handle, int scale_val ){

    switch (scale_val)
    {
    case 2:
        accel_scale_factor = 16384.0;    
        break;
    case 4:
        accel_scale_factor = 8192.0;     
        break;
    case 8:
        accel_scale_factor = 4096.0;    
        break;
    case 18:
        accel_scale_factor = 2048.0;    
        break;
    
    default:
        std::cout << "Unable to set accel scale\n";
    }
}


float get_accel_x(int device_handle){
    int raw_accel_x;
    float scaled_accel_x;
    raw_accel_x = read_word(device_handle, ACCEL_XOUT_H_ADDR);
    scaled_accel_x = scale_accel(raw_accel_x);
    return scaled_accel_x;
}

float get_accel_y(int device_handle){
    int raw_accel_y;
    float scaled_accel_y;
    raw_accel_y = read_word(device_handle, ACCEL_YOUT_H_ADDR);
    scaled_accel_y = scale_accel(raw_accel_y);
    return scaled_accel_y;
}

float get_accel_z(int device_handle){
    int raw_accel_z;
    float scaled_accel_z;
    raw_accel_z = read_word(device_handle, ACCEL_ZOUT_H_ADDR);
    scaled_accel_z = scale_accel(raw_accel_z);
    return scaled_accel_z;
}


float scale_accel(int raw_accel){
    float scaled;
    scaled = ((raw_accel / accel_scale_factor) * GRAVITATIONAL_CONST);
    return scaled;
}

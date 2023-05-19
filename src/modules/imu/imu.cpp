#include <DBASEV/visibility.h>
#include <DBASEV/i2c.h>
#include <DBASEV/accel.h>

void getAcceleration()
{
    if (gpioInitialise()<0) //initialises pigpio.h
    {
        //if pigpio initialisation failed
        std::cout<<"pigpio.h initialisation failed\n";
        return -1;
    }

    //variables commonly used in main()
    int device_handle, wake_handle;
    std::string current_time;
    float accel_x

    // gravity calibration
    float gx = 0
    const int SAMPLES = 100; // number of samples
    float samples[SAMPLES]; // sample data
    
    //attempt to open I2C devices
    device_handle = i2cOpen(I2C_DEVICE,I2C_DEVICE_ADDR,I2C_FLAGS);
    if (device_handle < 0)
    {
        std::string err = "Failed to open i2c communication to IMU\n";
        std::cout<<err<<"\n";
        return -1;
    }
    
    //attempt to disable sleep modes
    wake_handle = i2cWriteByteData(device_handle,PWR_MGMT_1_ADDR,PWR_MGMT_1_VAL);
    if (wake_handle<0)
    {
        std::string err = "Failed to wake IMU device\n";
        std::cout<<err<<"\n";
        return -2;
    }

    sleep(1); //to allow device to be ready to take readings
    std::cout << "Device is ready to take readings\n\n";

    //attempt to set scales
    set_accel(device_handle,2);
    
    bool cali_flag = true;
    
    //infinite loop to get readings from device
    while (true)
    {
        if(cali_flag){
        
            // Calculate the value of maintaining gravity
            for (int i = 0; i < SAMPLES; i++) {
                samples[i] = get_accel_x(device_handle);

                gx += samples[i];

                sleep(0.01);
            }

            printf("calibracn finish!!\n");
        
            gx /= SAMPLES;

            cali_flag = false;
        }
        
        //get readings
        accel_x = get_accel_x(device_handle) - gx;
                
        //output readings
        std::cout << "Accel:  x: "<<accel_x<<"  [m/s^2]\n";
        std::cout <<"\n";
        
        sleep(0.1); 
    }
}

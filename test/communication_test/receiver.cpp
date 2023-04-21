#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <chrono>
#include <time.h>
#include <termios.h>
#include <telemetry/common/mavlink.h>
#include <telemetry/mavlink_helpers.h>

using namespace std;
using std::chrono::system_clock;
using std::chrono::duration;

int sender_serialSetting(int serial_port){
	termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Error getting serial port attributes." << std::endl;
        close(serial_port);
        return 1;
    }
    
    cfsetospeed(&tty, B57600);
    cfsetispeed(&tty, B57600);
    
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting serial port attributes." << std::endl;
        close(serial_port);
        return 1;
    }
    return 0;
}

int receiver_serialSetting(int serial_port){   
    struct termios options = {0};
    if (tcgetattr(serial_port, &options) != 0) {
        std::cerr << "Error getting serial port attributes." << std::endl;
        close(serial_port);
        return 1;
    }
    cfsetispeed(&options, B57600);
    cfsetospeed(&options, B57600);
    options.c_cflag = (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag = CS8;
    options.c_cflag &= ~CRTSCTS;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 50;
    if (tcsetattr(serial_port, TCSANOW, &options) != 0) {
        std::cerr << "Error setting serial port attributes." << std::endl;
        close(serial_port);
        return 1;
    }
    return 0;
}

int main()
{
    
    int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        std::cerr << "Error opening serial port." << std::endl;
        return 1;
    }
    sender_serialSetting(fd);
    receiver_serialSetting(fd);

    //Create a Mavlink instance
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_status_t status;
    mavlink_channel_t channel = MAVLINK_COMM_0;
    bool init = false;
    //Wait for messages
    while (true) {
        uint8_t byte;
        bool message_received = false;
        std::string message = "";
        auto start = system_clock::now();
        
        while (read(fd, &byte, 1) == 1) {
            // if(init == false){
                
            // }
            // init = true;
            //Try to parse the message
            if (mavlink_parse_char(channel, byte, &msg, &status)) {           
                //If a message is received, check if it's a string message
                if (msg.msgid == MAVLINK_MSG_ID_STATUSTEXT) {
                  mavlink_statustext_t text;
                  mavlink_msg_statustext_decode(&msg, &text);
                  message += std::string(reinterpret_cast<char*>(text.text));
                  //break; //=> if break exist, faster but "\n" exist in string 
                }
            }
        }
        std::cout << message << std::endl;
        auto end = system_clock::now();
        cout << "elpased " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    }

    //Close the serial port
    close(fd);
    return 0;
}



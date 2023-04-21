#include <iostream>
#include <telemetry/common/mavlink.h>
#include <telemetry/mavlink_helpers.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int serialSetting(int serial_port){
	termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Error getting serial port attributes." << std::endl;
        close(serial_port);
        return 1;
    }
    cfsetospeed(&tty, B57600);
    cfsetispeed(&tty, B57600);
    tty.c_cflag &= ~PARENB; // no parity
    tty.c_cflag &= ~CSTOPB; // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; // 8 data bits
    tty.c_cflag &= ~CRTSCTS; // no hardware flow control
    tty.c_cc[VMIN] = 1; // read() blocks until at least 1 byte is available
    tty.c_cc[VTIME] = 0; // read() blocks indefinitely
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting serial port attributes." << std::endl;
        close(serial_port);
        return 1;
    }
    return 0;
}

int main()
{
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Error opening serial port." << std::endl;
        return 1;
    }

    serialSetting(serial_port);

    // Create a Mavlink instance
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_channel_t channel = MAVLINK_COMM_0;
    mavlink_statustext_t statustext;

    // Set the fields of the statustext message
    statustext.severity = MAV_SEVERITY_INFO; // Set severity to INFO

    const int max_chunk_size = 49;
    char temp_buffer[50];
    std::string temp;
    
    while (true) {
		std::string message = "Enter_new_road nxt velocity 42.6 nxt road_id 5 nxt way_points : nxt 0 35.18413 128.0713 3.3 1 0 nxt 1 35.18413 128.0713 3.3 1 0 nxt 2 35.18513 128.0673 3.3 1 0 nxt 3 35.18603 128.0653 3.3 1 0 nxt 4 35.18413 128.0713 3.3 1 0 nxt 5 35.18413 128.0713 3.3 1 0 nxt 6 35.18413 128.0713 3.3 1 0 nxt 7 35.18413 128.0713 3.3 1 0 nxt 8 35.18413 128.0713 3.3 1 0 nxt 9 35.18413 128.0713 3.3 1 0 nxt 10 35.18413 128.0713 3.3 1 1";
		
		
		for (int i = 0; i < message.length(); i += max_chunk_size) {
				int chunk_size = std::min(max_chunk_size, static_cast<int>(message.length() - i));
				temp = message.substr(i, chunk_size);

				// Copy temp to buffer
				temp.copy(temp_buffer, chunk_size);
				temp_buffer[chunk_size] = '\0';

				strcpy(statustext.text, temp_buffer); // Set the message text

				// Encode the statustext message
				mavlink_msg_statustext_encode(255, 200, &msg, &statustext);

				// Convert the message to a byte buffer
				uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

				// Send the message over the serial port
				ssize_t bytesWritten = write(serial_port, buf, len);
				
				if (bytesWritten < 0 || bytesWritten != len) {
					std::cerr << "Error sending message." << std::endl;
					close(serial_port);
					return 1;
				}
		}

        sleep(20);
    }

    // Close the serial port
    close(serial_port);
    return 0;
}


#include <DBASEV/communication.h>

void * thread_func2(void *arg)
{
    /*send data with queue*/
    tbb::concurrent_queue<const char*>* cq = static_cast<tbb::concurrent_queue<const char*>*>(arg);
    const char* tmp;
    for (int i = 0; i < 10; i++) {
        const char* msg = "Enter_new_road nxt velocity 42.6 nxt road_id 5 nxt way_points : nxt 0 35.18413 128.0713 3.3 1 0 nxt 1 35.18413 128.0713 3.3 1 0 nxt 2 35.18513 128.0673 3.3 1 0 nxt 3 35.18603 128.0653 3.3 1 0 nxt 4 35.18413 128.0713 3.3 1 0 nxt 5 35.18413 128.0713 3.3 1 0 nxt 6 35.18413 128.0713 3.3 1 0 nxt 7 35.18413 128.0713 3.3 1 0 nxt 8 35.18413 128.0713 3.3 1 0 nxt 9 35.18413 128.0713 3.3 1 0 nxt 10 35.18413 128.0713 3.3 1 1";
        while (cq->try_pop(tmp)) {
        // empty queue before push
        }
        cq->push(msg); // push item into the concurrent queue
    }
    return 0;

}

void * sender(void *arg)
{
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Error opening serial port." << std::endl;
        //return 1;
    }

    termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Error getting serial port attributes." << std::endl;
        close(serial_port);
        //return 1;
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
        //return 1;
    }

    // Create a Mavlink instance
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_status_t status;
    mavlink_channel_t channel = MAVLINK_COMM_0;
    mavlink_statustext_t statustext;

    char sending[100];

    // Set the fields of the statustext message
    statustext.severity = MAV_SEVERITY_INFO; // Set severity to INFO

    // Send a message every 1 second
    while (true) {
        printf("Input : ");
        scanf("%s\n", sending);
        strcpy(statustext.text, sending); // Set the message text

        // Encode the statustext message
        mavlink_msg_statustext_encode(255, 200, &msg, &statustext);

        // Convert the message to a byte buffer
        uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

        // Send the message over the serial port
        ssize_t bytesWritten = write(serial_port, buf, len);
        if (bytesWritten < 0 || bytesWritten != len) {
            std::cerr << "Error sending message." << std::endl;
            close(serial_port);
            //return 1;
        }

        // Wait for 1 second before sending the next message
        sleep(1);
    }

    // Close the serial port
    close(serial_port);
    return 0;
}

void *receiver(void *arg)
{
    int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        std::cerr << "Error opening serial port." << std::endl;
        //return 1;
    }

    struct termios options = {0};
    if (tcgetattr(fd, &options) != 0) {
        std::cerr << "Error getting serial port attributes." << std::endl;
        close(fd);
        //return 1;
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
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        std::cerr << "Error setting serial port attributes." << std::endl;
        close(fd);
        //return 1;
    }

    //Create a Mavlink instance
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_status_t status;
    mavlink_channel_t channel = MAVLINK_COMM_0;

    //Wait for messages
    while (true) {
        uint8_t byte;
        std::string message = "";
        while (read(fd, &byte, 1) == 1) {
            //Try to parse the message
            if (mavlink_parse_char(channel, byte, &msg, &status)) {           
                //If a message is received, check if it's a string message
                if (msg.msgid == MAVLINK_MSG_ID_STATUSTEXT) {
                  mavlink_statustext_t text;
                  mavlink_msg_statustext_decode(&msg, &text);
                  message = std::string(reinterpret_cast<char*>(text.text));
                  std::cout << "Received id: " << text.id << std::endl;
                  std::cout << "Received: " << message << std::endl;
                  printf("=======================================================================\n");
                  break;
                }
            }
        }
    }

    //Close the serial port
    close(fd);
    return 0;
}

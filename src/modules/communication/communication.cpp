#include <DBASEV/communication.h>


void * sender(void *arg)
{
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Error opening serial port." << std::endl;
        //return 1;
    }

    serialSetting(serial_port);

    // Create a Mavlink instance
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    //mavlink_channel_t channel = MAVLINK_COMM_0;
    mavlink_statustext_t statustext;

    // Set the fields of the statustext message
    statustext.severity = MAV_SEVERITY_INFO; // Set severity to INFO

    const int max_chunk_size = 49;
    char temp_buffer[50];
    string temp;

    MsgBuf cmd;
    key_t key = 5555;
    int key_id = mq_init(key);
    struct msqid_ds bufs;
    int comp;
    string message;
    
    while (true) {
		
		cmd = pop(key_id, bufs);
        if(comp != cmd.sq){
            cout << "sender : " << cmd.buf  << "msg count : " << cmd.sq << endl;
        }
        comp = cmd.sq;
        message = string(cmd.buf);
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
					//std::cerr << "Error sending message." << std::endl;
					close(serial_port);
					//return 1;
				}
		}
    }

    // Close the serial port
    close(serial_port);

    return 0;
}

void * receiver(void *arg)
{
    int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        std::cerr << "Error opening serial port." << std::endl;
        //return 1;
    }
    sender_serialSetting(fd);
    receiver_serialSetting(fd);

    //Create a Mavlink instance
    mavlink_message_t msg;
    //uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_status_t status;
    mavlink_channel_t channel = MAVLINK_COMM_0;
    string message;

    //ready to send message using message queue
    MsgBuf qmsg;
    qmsg.msgtype = 1;
    key_t key = 5656;
    int key_id = mq_init(key);
    struct msqid_ds buf;

    
    //Wait for messages
    while (true) {
        #ifndef DEBUG
            printf("start while\n");
        #endif
        bool message_received = receiveMessage(fd, msg, status, channel, message);
        
        if (message_received) {
            printf("%s\n",message.c_str());
            #ifndef DEBUG
                cout << "communication.cpp" << endl;
            #endif

            strcpy(qmsg.buf, message.c_str());
            push(key_id,buf, qmsg);
        } else {
            message = "ERR";
            printf("%s\n",message.c_str());
            #ifndef DEBUG
                cout << "communication.cpp" << endl;
            #endif
            printf("timeout occurred\n");
            #ifndef DEBUG
                cout << "communication.cpp" << endl;
            #endif
        }
    }
    return 0;
}

// void * thread_func2(void *arg)
// {
//     int road_id = 1;
//     int waypoint = 0;
//     float speed = 5;
//     char sndmsg[100];


    
//     //int cnt = 0;
//     MsgBuf msg;
//     msg.msgtype = 1;
//     key_t key = 5656;
//     int key_id = mq_init(key);

//     struct msqid_ds buf;
//     // while(1){
//     //     msg.value = ++cnt;
//     //     if (cnt >= 10) {
//     //         cout << "Message Sending Finished!" << endl;
//     //         break;
//     //     }
//     //     strcpy(msg.buf, sndmsg);
//     //     push(key_id,buf, msg);
//     //     //usleep(1);
//     //     //cout << "value: " << msg.value << endl;
//     // }
//     for(int i=0; i<93; i++){
//         waypoint++;
//         if (waypoint<38){
//             road_id = 1;
//         }
//         else if(waypoint<79){
//             road_id = 2;
//         }
//         else{
//             road_id = 3;
//         }
//         for(int x=0; x<20; x++){
            
//             snprintf(sndmsg,sizeof(sndmsg),"%.2f %d %d", speed, waypoint, road_id);
//             strcpy(msg.buf, sndmsg);
//             //printf(sndmsg);
//             //printf("\n");
//             usleep(50000);
//             push(key_id,buf, msg);
            

//         }
//     }

//     return 0;

// }
#include <DBASEV/communication.h>

template<typename T>
void pop_front(std::vector<T> &v)
{
    if (v.size() > 0) {
        v.erase(v.begin());
    }
}

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
         
    vector<string> data = {
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "0/1/1/1.0",
        "1.4/1/1/2.4",
        "1.4/1/1/3.8",
        "1.4/1/1/5.2",
        "1.4/1/1/6.6",
        "1.4/1/1/8.0",
        "1.4/1/1/9.4",
        "1.4/1/1/10.8",
        "1.4/1/1/12.2",
        "1.4/1/1/13.6",
        "1.4/1/1/15.0",
        "1.4/1/1/16.4",
        "1.4/1/1/17.8",
        "1.4/1/1/19.2",
        "1.4/1/1/20.6",
        "1.4/1/1/22.0",
        "1.4/1/1/23.4",
        "1.4/1/1/24.8",
        "1.4/1/1/26.2",
        "1.4/1/1/27.6",
        "1.4/1/1/29.0",
        "1.4/1/1/30.4",
        "2.5/1/1/32.9",
        "2.5/1/1/35.4",
        "2.5/1/1/37.9",
        "2.5/1/1/40.4",
        "2.5/1/1/42.9",
        "2.5/1/1/45.4",
        "2.5/1/1/47.9",
        "2.5/1/1/50.4",
        "2.5/1/1/52.9",
        "2.5/1/1/55.4",
        "2.5/1/1/57.9",
        "2.5/1/1/60.4",
        "2.5/1/1/62.9",
        "2.5/1/1/65.4",
        "2.5/1/1/67.9",
        "2.5/1/1/70.4",
        "2.5/1/1/72.9",
        "2.5/1/1/75.4",
        "2.5/1/1/77.9",
        "2.5/1/1/80.4",
        "2.5/1/1/82.9",
        "2.5/1/1/85.4",
        "2.5/1/1/87.9",
        "2.5/1/1/90.4",
        "2.5/1/1/92.9",
        "2.5/1/1/95.4",
        "2.5/1/1/97.9",
        "2.5/1/1/100.4",
        "2.5/1/1/102.9",
        "2.5/1/1/105.4",
        "2.5/1/1/107.9",
        "2.5/1/1/110.4",
        "2.5/1/1/112.9",
        "2.5/1/1/115.4",
        "2.5/1/1/117.9",
        "2.5/1/1/120.4",
        "2.5/1/1/122.9",
        "2.5/1/1/125.4",
        "2.5/1/1/127.9",
        "2.5/1/1/130.4",
        "2.5/4/1/2.5",
        "2.5/4/1/5.0",
        "2.5/4/1/7.5",
        "2.5/4/1/10.0",
        "2.5/4/1/12.5",
        "2.5/4/1/15.0",
        "2.5/4/1/17.5",
        "2.5/4/1/20.0",
        "2.5/4/1/22.5",
        "2.5/4/1/25.0",
        "2.5/4/1/27.5",
        "2.5/4/1/30.0",
        "2.5/4/1/32.5",
        "2.5/4/1/35.0",
        "2.5/4/1/37.5",
        "2.5/4/1/40.0"
    };    

    while (true) {
		//cmd = pop(key_id, bufs);
        // if(comp != cmd.sq){
        //     cout << "sender : " << cmd.buf  << "msg count : " << cmd.sq << endl;
        // }
        // comp = cmd.sq;
        //cout << "sender : " << cmd.buf  << endl;

        if (data.empty()) {
            continue;
        }

        message = data.front();
        pop_front(data);
        cout << "message: " << message << endl;
        sleep(1);

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
            //printf("%s\n",message.c_str());
            #ifndef DEBUG
                cout << "communication.cpp" << endl;
            #endif

            strcpy(qmsg.buf, message.c_str());
            //strcpy(qmsg.buf, "10/1/1");
            push(key_id,buf, qmsg);
        } else {
            message = "ERR";
            //printf("%s\n",message.c_str());
            #ifndef DEBUG
                cout << "communication.cpp" << endl;
            #endif
            //printf("timeout occurred\n");
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
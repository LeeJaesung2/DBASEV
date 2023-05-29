#include <DBASEV/vehicle_control.h>

void* vehicle_control(void* arg)
{
    //==================init====================
    unordered_map<int, Vertex> graph = creatingMap();

    int road_id = 1;
    printf("now road: %d\n", road_id);

    //==========================================
    
    string pre_gps="";
    float pre_speed = 0.0, current_speed;
    float current_latitude, current_longitude;
    int pre_waypoint = 0, now_waypoint;
    
    MsgBuf msg;
    key_t key = 1235;
    int key_id = mq_init(key);
    struct msqid_ds buf;


    key_t key2 = 5555;
    int key_id2 = mq_init(key2);
    struct msqid_ds buf2;
    int temp;

    while (1) {  
        msg = pop(key_id, buf);
        string sending_communication;

        if(msg.sq != temp){
            cout << "vehicle_control.cpp : " << msg.buf << "msg num : " <<msg.sq << endl;
        }
        temp = msg.sq;

        if (!isValidGPSData(msg.buf)) {
            pre_gps = "";
            //continue;
        }
         // 데이터 포맷 : 속도 / 도로id/ waypoint
        if (pre_gps ==""){
            //cout << "speed: " << pre_speed << " m/s \n";
            sending_communication += "velocity / ";
            // real : sending_communication += pre_speed;
        } 
        else{
            current_speed = getSpeed(getDistance(pre_gps, msg.buf), pre_gps, msg.buf);
            //cout << "speed: " << current_speed << " m/s \n";
            sending_communication += current_speed;
            pre_speed = current_speed;
        }

        current_latitude = extract_gps_data(msg.buf).latitude;
        current_longitude = extract_gps_data(msg.buf).longitude;

        now_waypoint = calculateClosestWaypoint(road_id, pre_waypoint, current_latitude, current_longitude, graph);
        pre_waypoint = now_waypoint;
        //printf("now_waypoint: %d\n\n", graph[road_id].waypoints[now_waypoint]);

        sending_communication += to_string(road_id) + " / ";
        sending_communication += to_string(now_waypoint);

        int remain_waypoints = graph[road_id].waypoints.back() - now_waypoint;
        
        if (remain_waypoints < 2) {
            int pre_road_id = road_id;
            road_id = findNextRoadId(road_id, current_latitude, current_longitude, graph);

            if(pre_road_id != road_id){
                pre_waypoint = 0;
            }

            //printf("now road: %d\n", road_id);
        }

        pre_gps = msg.buf;
        //cout << "vehicle cmd : " << sending_communication.c_str() << endl;
        strcpy(msg.buf, sending_communication.c_str());
        push(key_id2,buf2, msg);
    }
}
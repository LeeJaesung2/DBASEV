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
    int pre_waypoint = 0, now_waypoint=0;
    
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
        string gps = msg.buf;

        // if(msg.sq != temp){
        //     cout << "vehicle_control.cpp : " << gps << "msg num : " <<msg.sq << endl;
        // }
        // temp = msg.sq;
        cout << "vehicle_control.cpp : " << gps << endl;
        if (!isValidGPSData(gps)) {
            pre_gps = "";
            //continue;
        }
         // data format : velocity / road_id/ now_waypoint
        if (pre_gps ==""){
            sending_communication += to_string(pre_speed)+"/";
        } 
        else{
            current_speed = getSpeed(getDistance(pre_gps, gps), pre_gps, gps);
            sending_communication += to_string(current_speed)+"/";
            pre_speed = current_speed;
        }

        current_latitude = extract_gps_data(gps).latitude;
        current_longitude = extract_gps_data(gps).longitude;

        pre_waypoint = now_waypoint;
        now_waypoint = calculateClosestWaypoint(road_id, pre_waypoint, current_latitude, current_longitude, graph);
        
        cout << "now_waypoint: " << now_waypoint << endl;

        sending_communication += to_string(road_id) + "/";
        sending_communication += to_string(graph[road_id].waypoints[now_waypoint]);

        //int remain_waypoints = graph[road_id].waypoints.back() - now_waypoint;
        
        // if (remain_waypoints < 2) {
        //     int pre_road_id = road_id;
        //     road_id = findNextRoadId(road_id, current_latitude, current_longitude, graph);

        //     if(pre_road_id != road_id){
        //         pre_waypoint = 0;
        //         now_waypoint = 0;
        //     }
        // }

        pre_gps = gps;

        strcpy(msg.buf, sending_communication.c_str());
        push(key_id2,buf2, msg);
    }
}
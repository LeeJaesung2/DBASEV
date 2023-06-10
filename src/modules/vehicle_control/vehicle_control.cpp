#include <DBASEV/vehicle_control.h>

template<typename T>
void pop_front(std::vector<T> &v)
{
    if (v.size() > 0) {
        v.erase(v.begin());
    }
}

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

    string message;

    while (1) {  
        msg = pop(key_id, buf);
        string sending_communication;
        string gps = msg.buf;

        // if(msg.sq != temp){
        //     cout << "vehicle_control.cpp : " << gps << "msg num : " <<msg.sq << endl;
        // }
        // temp = msg.sq;
        //cout << "vehicle_control.cpp : " << gps << endl;
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
        
        sending_communication += to_string(road_id) + "/";
        sending_communication += to_string(graph[road_id].waypoints[now_waypoint]);

        int remain_waypoints = graph[road_id].waypoints.back() - now_waypoint;
        
        if (remain_waypoints < 2) {
            int pre_road_id = road_id;
            road_id = findNextRoadId(road_id, current_latitude, current_longitude, graph);

            if(pre_road_id != road_id){
                pre_waypoint = 0;
                now_waypoint = 0;
            }
        }

        pre_gps = gps;

        if (data.empty()) {
            continue;
        }

        message = data.front();
        pop_front(data);

        strcpy(msg.buf, message.c_str());
        push(key_id2,buf2, msg);
        //sleep(1);
    }
}
#include <DBASEV/drone_control.h>
#include <DBASEV/embedd.h>
void * thread_func1(void *arg)
{   

    const char* src = "struct_embedd_test";
    const char* func = "sturct_embedd";
    carData car_data = {1, 55, 87.4};
    waypoint point1 = {1,10.0,5.2,8.6,6,1};
    waypoint point2 = {3,2.0,7.2,9.6,8,0};
    waypoint point3 = {3,2.0,7.2,9.6,8,0};
    droneData drone_data = {5, 8, 60.8, {point1,point2, point3}};
    /*get data from queue*/
    tbb::concurrent_queue<const char*>* cq = static_cast<tbb::concurrent_queue<const char*>*>(arg);
    const char* msg;
    for (int i = 0; i < 15; i++) {
        if (cq->try_pop(msg)) { // try to pop item from the concurrent queue
            /*call python source code with struct*/
            carAndDroneData data = callPythonStruct(src, func, msg, car_data, drone_data);
            if(data.error==1){
                printf("error : no return value");
            }
            else{
                car_data = data.car;
                drone_data = data.drone;
            }
        } else {
            printf("Concurrent queue is empty\n");
        }
    }

    
    //func = "embedd";
    //callPython(src, func, 1, 1);   
    
    return 0;
}
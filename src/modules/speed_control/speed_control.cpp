#include <DBASEV/speed_control.h>
#include <DBASEV/embedd.h>
void * thread_func1(void *arg)
{   
    tbb::concurrent_queue<int>* cq = static_cast<tbb::concurrent_queue<int>*>(arg);
    int item;
    for (int i = 0; i < 15; i++) {
        if (cq->try_pop(item)) { // try to pop item from the concurrent queue
            //printf("Popped item %d\n", item);
        } else {
            //printf("Concurrent queue is empty\n");
        }
    }
    const char* src = "speed_control";
    const char* func = "speed_control";
    const char* msg = "struct argument test success";
    carData car_data = {1, 55, 87.4};
    droneData drone_data = {5, 8, 60.8, {122.2, 8.33}};
    //callPython(src, func, 1, 1);   
    callPythonStruct(src, func, msg, car_data, drone_data);
    return 0;
}
#include <DBASEV/speed_control.h>
#include <DBASEV/embedd.h>
void * thread_func1(void *arg)
{   
    tbb::concurrent_queue<int>* cq = static_cast<tbb::concurrent_queue<int>*>(arg);
    int item;
    for (int i = 0; i < 15; i++) {
        if (cq->try_pop(item)) { // try to pop item from the concurrent queue
            printf("Popped item %d\n", item);
        } else {
            printf("Concurrent queue is empty\n");
        }
    }
    const char* src = "speed_control";
    const char* func = "speed_control";
    embedd(src, func, 2, 3, 7);   
    return 0;
}
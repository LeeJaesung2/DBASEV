#include <DBASEV/communication.h>

void * thread_func2(void *arg)
{
    tbb::concurrent_queue<int>* cq = static_cast<tbb::concurrent_queue<int>*>(arg);
    for (int i = 0; i < 10; i++) {
        cq->push(i); // push item into the concurrent queue
    }
    return 0;

}
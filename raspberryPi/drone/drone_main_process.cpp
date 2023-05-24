#include <DBASEV/visibility.h>
#include <DBASEV/collision_avoidance.h>
#include <DBASEV/communication.h>
#include <DBASEV/drone_control.h>
#include <DBASEV/announce.h>



#define NUM_THREADS 2



int main()
{
    void *status;
    int thr_id;


    // create each thread
    pthread_t threads[NUM_THREADS];

    //create flight thread
    thr_id = pthread_create(&threads[0], NULL, &flight_control, NULL);
    if(thr_id < 0){
        perror("failure create thread");
    }
    // create receiver thread
    thr_id = pthread_create(&threads[1], NULL, &receiver, NULL);
    if(thr_id < 0){
        perror("failure create thread");
    }


    // create third thread
    // thr_id = pthread_create(&threads[2], NULL, &thread_func3, (void *)&begin);
    // if(thr_id < 0){
    //     perror("failure create thread");
    // }

    // run on main thread
    announceOn();    

    // wait for all of thread dead
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }

    //after dead all of thread
    printf("all of threads are dead");
    //announceOff();

    return 0;
}
#include <DBASEV/visibility.h>
#include <DBASEV/collision_avoidance.h>
#include <DBASEV/communication.h>
#include <DBASEV/drone_control.h>



#define NUM_THREADS 2

//control led and piezo buzzer
void ledAndpiezoControl(){
    int i = 0;
    while(i<1000){
        i++;
        printf("B");
    }
}

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
    ledAndpiezoControl();    

    // wait for all of thread dead
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }

    //after dead all of thread
    printf("all of threads are dead");

    return 0;
}
#include <DBASEV/visibility.h>
#include <DBASEV/collision_avoidance.h>
#include <DBASEV/communication.h>
#include <DBASEV/drone_control.h>
#include <DBASEV/announce.h>



#define NUM_THREADS 0



int main()
{
    void *status;
    int thr_id;


    // create each thread
    pthread_t threads[NUM_THREADS];

    //create flight thread
    // thr_id = pthread_create(&threads[0], NULL, &flight_control, NULL);
    // if(thr_id < 0){
    //     perror("failure create thread");
    // }
    // // create receiver thread
    // thr_id = pthread_create(&threads[1], NULL, &receiver, NULL);
    // if(thr_id < 0){
    //     perror("failure create thread");
    // }



    // run on main thread

    /*check OS version
    only run on raspberry pi*/
    struct utsname linux_info;
    if (uname(&linux_info) != -1) {
        if(strcmp(linux_info.release,"6.1.21-v8+")==0){
        	printf("here");
        //announceOn();
        }   
    } else {
        std::cerr << "Error: Failed to retrieve Linux version." << std::endl;
        return 1;
    }
    if(strcmp(linux_info.release,"5.15.0-71-generic")==0){
        //announceOn(); //only run on raspberry pi
    }   

    // wait for all of thread dead
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }

    //after dead all of thread
    printf("all of threads are dead");
    //announceOff();

    return 0;
}

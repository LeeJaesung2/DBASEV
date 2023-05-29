#include <DBASEV/visibility.h>
#include <DBASEV/gps.h>
#include <DBASEV/vehicle_control.h>
#include <DBASEV/communication.h>

#define NUM_THREADS 3

void main_thred_func(){
    
    while(true){
        printf("A");
    }
}

void * thread_func6(void * arg)
{
  while(true){

        printf("O");
    }
    return 0;
}


int main()
{
    int i;
    void *status;
    int thr_id;
    // 각각의 스레드를 생성
    pthread_t threads[NUM_THREADS];

    //첫번째 스레드 생성
    thr_id = pthread_create(&threads[0], NULL, &sender, NULL);
    if(thr_id < 0){
        perror("failure create thread");
    }

    //두번째 스레드 생성
    thr_id = pthread_create(&threads[0], NULL, &getGPS, NULL);
    if(thr_id < 0){
        perror("failure create thread");
    }

    //세번째 스레드 생성
    thr_id = pthread_create(&threads[1], NULL, &vehicle_control, NULL);
    if(thr_id < 0){
        perror("failure create thread");
    }

    // // test thread
    // thr_id = pthread_create(&threads[0], NULL, &thread_func4, NULL);
    // if(thr_id < 0){
    //     perror("failure create thread");
    // }

    // thr_id = pthread_create(&threads[1], NULL, &thread_func5, NULL);
    // if(thr_id < 0){
    //     perror("failure create thread");
    // }

    //스레드 종료전 메인스레드 기능
    //main_thred_func();    

    // 각각의 스레드가 종료될 때까지 대기
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }

    //모든 스레드 종료시 메인스레드 기능
    //printf("all of threads are dead");

    return 0;
}
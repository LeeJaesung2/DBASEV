#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <time.h>
#include "messageQueue.h"


#define NUM_THREADS 2
using namespace std;

int main()
{
    void *status;
    int thr_id;
    clock_t start = clock();
    // 각각의 스레드를 생성
    pthread_t threads[NUM_THREADS];

    //첫번째 스레드 생성
    thr_id = pthread_create(&threads[0], NULL, &consumer, NULL);
    if(thr_id < 0){
        perror("failure create thread");
    }
    // 두번째 스레드 생성
    thr_id = pthread_create(&threads[1], NULL, &comm, NULL);
    if(thr_id < 0){
        perror("failure create thread");
    }


    // 각각의 스레드가 종료될 때까지 대기
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }

    //모든 스레드 종료시 메인스레드 기능
    printf("all of threads are dead\n");

    cout << ((clock()-start)/CLOCKS_PER_SEC) << "sec" << endl;
    return 0;
}
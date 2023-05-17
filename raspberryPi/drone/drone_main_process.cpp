#include <DBASEV/visibility.h>
#include <DBASEV/collision_avoidance.h>
#include <DBASEV/communication.h>
#include <DBASEV/drone_control.h>



#define NUM_THREADS 2

void main_thred_func(){
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
    time_t begin = clock();
    //tbb::concurrent_queue<const char*> cq; // concurrent queue
    
    // 각각의 스레드를 생성
    pthread_t threads[NUM_THREADS];

    //첫번째 스레드 생성
    thr_id = pthread_create(&threads[0], NULL, &thread_func1, NULL);
    if(thr_id < 0){
        perror("failure create thread");
    }
    // 두번째 스레드 생성
    thr_id = pthread_create(&threads[1], NULL, &thread_func2, NULL);
    if(thr_id < 0){
        perror("failure create thread");
    }

    // create communicate thread
    // thr_id = pthread_create(&threads[1], NULL, &sender, NULL);
    // if(thr_id < 0){
    //     perror("failure create thread");
    // }

    // //세번째 스레드 생성
    // thr_id = pthread_create(&threads[2], NULL, &thread_func3, (void *)&begin);
    // if(thr_id < 0){
    //     perror("failure create thread");
    // }

    // //스레드 종료전 메인스레드 기능
    // main_thred_func();    

    // 각각의 스레드가 종료될 때까지 대기
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }

    //모든 스레드 종료시 메인스레드 기능
    printf("all of threads are dead");

    return 0;
}
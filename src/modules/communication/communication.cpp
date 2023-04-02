#include "communication.h"

void *thread_func2(void *arg)
{
    int i = 0;
    while(i<1000){
        i++;
        // printf("1");
    }
    return 0;


}
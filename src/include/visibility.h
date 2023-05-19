#ifndef __VISIBILITY_H__
#define __VISIBILITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <stdbool.h>
#include <cstring>
#include <cstdlib>
#include <time.h>
#include <vector>


using namespace std;

struct MsgBuf {
    long msgtype; // message type, must be > 0
    int value;
    char buf[26];
};


#endif


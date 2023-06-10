#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#include "visibility.h"
#include <sys/ipc.h>
#include <sys/msg.h>

struct MsgBuf {
    long msgtype; // message type, must be > 0
    int sq;
    char buf[100];
};


int mq_init(key_t key);
void push(int key_id,msqid_ds buf, MsgBuf msg);
MsgBuf pop(int key_id, msqid_ds buf);

#endif
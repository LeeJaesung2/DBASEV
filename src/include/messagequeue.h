#include "visibility.h"
#include <sys/ipc.h>
#include <sys/msg.h>

int mq_init(key_t key);
void push(int key_id,msqid_ds buf, MsgBuf msg);
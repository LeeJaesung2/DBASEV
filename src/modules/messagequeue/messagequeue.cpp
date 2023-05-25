#include <DBASEV/messagequeue.h>

int mq_init(key_t key){

    int key_id = msgget( key, IPC_CREAT|0666);
    if (key_id == -1) {
        cerr << "Message Get Failed!" << endl;
        #ifndef DEBUG
            cout << "messagequeue.cpp" << endl;
        #endif
        exit(EXIT_FAILURE);
    }
    return key_id;
}

void push(int key_id,msqid_ds buf, MsgBuf msg){
    

    if (msgctl(key_id, IPC_STAT, &buf) == -1) {
        std::cerr << "Failed to get message queue status!" << std::endl;
        #ifndef DEBUG
            cout << "messagequeue.cpp" << endl;
        #endif
        return;
    }
    //Check queue is full
    if (buf.msg_qnum >= 409) {
        std::cerr << "Message queue is full. Skipping message send." << std::endl;
        #ifndef DEBUG
            cout << "messagequeue.cpp" << endl;
        #endif
        usleep(1);
        return;
    }
    // Check the number of messages currently in the queue
    //printf("num of data in the queue is %ld\n",buf.msg_qnum);
    while (buf.msg_qnum >0 ) {
        // Remove existing message before pushing a new one
        if (msgrcv(key_id, &msg, sizeof(msg), 0, IPC_NOWAIT) == -1) {
            std::cerr << "Failed to remove existing message from the queue!" << std::endl;
            #ifndef DEBUG
                cout << "messagequeue.cpp" << endl;
            #endif
            return;
        }
    }
    if (msgsnd(key_id, &msg, sizeof(msg), IPC_NOWAIT) == -1) {
            cerr << "Message Sending Failed!" << endl;
            #ifndef DEBUG
                cout << "messagequeue.cpp" << endl;
            #endif
            exit(EXIT_FAILURE);
        }
}

MsgBuf pop(int key_id, msqid_ds buf) {
    MsgBuf msg;
    
    if (msgctl(key_id, IPC_STAT, &buf) == -1) {
        std::cerr << "Failed to get message queue status!" << std::endl;
        #ifndef DEBUG
        cout << "messagequeue.cpp" << endl;
        #endif
        // Return an empty message to indicate failure
        return msg;
    }

    // Check the number of messages currently in the queue
    if (buf.msg_qnum <= 0) {
        std::cerr << "Message queue is empty. Cannot retrieve message." << std::endl;
        #ifndef DEBUG
        cout << "messagequeue.cpp" << endl;
        #endif
        // Return an empty message to indicate failure
        return msg;
    }

    // Receive the message from the queuegi
    if (msgrcv(key_id, &msg, sizeof(msg), 0, IPC_NOWAIT) == -1) {
        std::cerr << "Failed to retrieve message from the queue!" << std::endl;
        #ifndef DEBUG
        cout << "messagequeue.cpp" << endl;
        #endif
        // Return an empty message to indicate failure
        return msg;
    }
    //cout << msg.buf << endl;

    return msg;
}
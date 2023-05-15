import sysv_ipc
from ctypes import Structure, c_long, c_int, c_char
import time
import struct


# Define message buffer structure
class MsgBuf(Structure):
    _fields_ = [
        ("msgtype", c_long),
        ("value", c_int),
        ("buf", c_char * 26)
    ]


def mq_init(key):
     mq = sysv_ipc.MessageQueue(key, sysv_ipc.IPC_CREAT)
     return mq

def pop(mq):
    if mq.current_messages == 0:
        print("queue is empty")
        return -1, "no data"
    try:
        data = mq.receive()
    except sysv_ipc.BusyError:
        time.sleep(1)


    string_data = data[0][4:21] #change this part as a message format
    # Decode the string
    msg = str(string_data.decode('utf-8'))
    value = struct.unpack('<i', data[0][:4])[0]
    return value, msg



def consumer(a):
    BUFFER_SIZE = 26
    msgcount = 0

    # Get message queue ID using same key as C++ program
    key = 5656
    mq = mq_init(key)

    print("Message queue created with ID:", mq)
    # Read messages from queue
    while True:
        # Read message from queue
        value, msg = pop(mq)

        # Print message contents
        print("Received value:", value)
        print("Message data:", msg)
        if (value!=-1):
            msgcount+=1

        if value >= 999999:
            print("All messages received!")
            break

    # Clean up message queue
    mq.close()
    sysv_ipc.unlink_message_queue(key)
    print("received message number is ", msgcount)
    return 0

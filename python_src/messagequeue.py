import sysv_ipc
from ctypes import Structure, c_long, c_int, c_char
import time
import struct

# Define message buffer structure
# class MsgBuf(Structure):
#     _fields_ = [
#         ("msgtype", c_long),
#         ("value", c_int),
#         ("buf", c_char * 26)
#     ]


def mq_init(key):
     mq = sysv_ipc.MessageQueue(key, sysv_ipc.IPC_CREAT)
     return mq

def pop(mq):
    while mq.current_messages == 0:
        print(".")
        time.sleep(1)
    try:
        data = mq.receive()
    except sysv_ipc.BusyError:
        time.sleep(1)


    string_data = data[0][4:21] #change this part as a message format
    # Decode the string
    msg = str(string_data.decode('utf-8'))
    # value = struct.unpack('<i', data[0][:4])[0]
    return  msg



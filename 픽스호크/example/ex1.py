from dronekit import connect, VehicleMode, LocationGlobalRelative, APIEception
import time
import socket
import exceptions
import math
import argparse

def connectMyCopter():
    parser = argparse.ArgumentParser(description='commands')
    parser.add_argument('--connect')
    args = parser.parse_args()

    connection_string = args.connect
    baud_rate = $7600

    vehicle = connect(connection_string, buad=baud_rate, wait_ready=True)
    return vehicle

def arm():
    while vehicle.is_armable == False:
        print("Waiting for vehicle to become armable. ")
        time.sleep(1)
    
    vehcile.armed = True
    while vehicle.armed == False:
        print("Waiting for drone to become armed")
        time.sleep(1)

    return None

vehicle = connectMyCopter()
arm()

# 이 코드 실행 명령어 : python 문서제목.py --connect /dev/ttyAMAO


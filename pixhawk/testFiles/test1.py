from dronekit import connect, VehicleMode, LocationGlobalRelative, LocationGlobal, Command
import time
import math
from pymavlink import mavutil
import dronekit_sitl
import argparse  

# 기본 시작 코드
vehicle = connect("/dev/ttyAMA0", wait_ready=True, baud=57600)
vehicle.mode = VehicleMode("GUIDED")
vehicle.armed = True
while not vehicle.mode.name =='GUIDED' and not vehicle.armed and not api.exit:
    print (" 이륙 중비중 ...")
    time.sleep(1)

# 미션 초기화
def init_commands():
    print("명령어 초기화중 ...")
    cmds = vehicle.commands
    cmds.clear()
    cmds.upload()
    print("명령어 초기화 완료")

# 이륙 코드
def arm_and_takeoff(aTargetAltitude):
    print("기본 사전 검사 ...")
    while not vehicle.is_armable:
        print("초기화 대기중 ...")
        time.sleep(1)

    print("모터 가동 확인!!")
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    while not vehicle.armed:
        print("장비 가동 가능 확인중 ...")
        time.sleep(1)

    print("이륙!!")
    vehicle.simple_takeoff(aTargetAltitude)

    while True:
        print(" 고도: ", vehicle.location.global_relative_frame.alt)
        if vehicle.location.global_relative_frame.alt >= aTargetAltitude*0.95:
            print("목표 고도에 도달했습니다!")
            break
        time.sleep(1)

init_commands()
arm_and_takeoff(5)
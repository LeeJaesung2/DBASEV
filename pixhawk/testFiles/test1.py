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

cmds = None


# 미션 초기화
def init_commands():
    print("명령어 초기화중 ...")
    cmds = vehicle.commands
    cmds.clear()
    cmds.upload()
    cmds.wait_ready()
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

# 미션 시작 세팅
def start_mission():
    print("자동주행 시스템 가동중 ...")
    vehicle.commands.next=0
    vehicle.mode = VehicleMode("AUTO")

# 미션 추가 코드
def add_mission(new_cmd):
    print(f"새로운 명령어 : {new_cmd} 추가중 ...")
    cmds.add(new_cmd)
    cmds.upload()
    print("추가 완료!!")

# 맨 처음으로 미션 추가 코드
def add_mission_first(new_cmd):
    print("새로운 미션 맨 처음으로 넣는중 ...")
    missionlist = [new_cmd]
    for cmd in cmds:
        missionlist.append(cmd)
    
    cmds.clear()
    for cmd in missionlist:
        cmds.add(cmd)

    cmds.upload()
    print("새로운 미션 맨 처음으로 업로드 완료!!")

# 모든 미션 없애버리고 새로운 미션 추가 코드
def add_mission_ignore_all_mission(new_cmd):
    print("모든 명령 삭제중 ...")
    cmds.clear()
    print("모든 명령 삭제 완료!!")

    print(f"새로운 명령어 : {new_cmd} 추가중 ...")
    cmds.add(new_cmd)
    cmds.upload()
    print("추가 완료!!")

# wayPoint 만들어서 명령에 넣기
def way_point(stop_time=0, target_lat=0, target_lon=0, target_alt=0):
    new_cmd = Command(
        0, 0, 0,
        mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
        mavutil.mavlink.MAV_CMD_NAV_WAYPOINT,
        stop_time,
        0, 0, 0,
        target_lat,
        target_lon,
        target_alt,
    )
    return new_cmd

# 특정 위치에 hovering 하기
def hovering_time(stop_time=1.0, target_lat=0, target_lon=0, target_alt=0):
    new_cmd = Command(
        0, 0, 0,
        mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
        mavutil.mavlink.MAV_CMD_NAV_LOITER_TIME,
        stop_time,
        0, 0, 0,
        target_lat,
        target_lon,
        target_alt,
    )

    return new_cmd

# 홈 위치 설정하기
def set_home(target_alt):
    print("\nSet new home location")
    my_location_alt = vehicle.location.global_frame
    my_location_alt.alt = target_alt
    vehicle.home_location = my_location_alt

# 홈 위치 또는 집결 지점으로 돌아가기
def return_home():
    new_cmd = Command(
        0, 0, 0,
        mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
        mavutil.mavlink.MAV_CMD_NAV_RETURN_TO_LAUNCH,
        0,
        0, 0, 0,
        0, 0, 0
    )

    return new_cmd

init_commands()
arm_and_takeoff(5)
# pixhawk와 통신, 명령등의 기능들이 있는 모듈
from dronekit import connect, VehicleMode, LocationGlobalRelative
import time

def connect_to_pixhawk():
    # 연결할 시리얼 포트의 경로를 지정합니다.
    port = "/dev/ttyAMA0" #USB : '/dev/ttyACM0' 
    connection_string = 'com{}'.format(port)

    # 연결합니다.
    vehicle = connect(connection_string, baud=57600, wait_ready=True)

    # 연결된 기체의 정보를 출력합니다.
    print('Connected to vehicle on: {}'.format(connection_string))
    print('Vehicle mode: {}'.format(vehicle.mode.name))

    # 기체를 안전한 모드로 설정합니다.
    vehicle.mode = VehicleMode('STABILIZE')

    return vehicle

# 드론에게 waypoint 리스트를 전달하여 추가하는 함수
"""
waypoint_list의 format :

waypoint_list = [
    [latitude1, longitude1, altitude1],
    [latitude2, longitude2, altitude2],
    [latitude3, longitude3, altitude3],
    ...
]
"""
def add_waypoints(vehicle, waypoint_list):
    # LocationGlobalRelative 객체를 이용해 waypoint 추가
    for waypoint in waypoint_list:
        wp = LocationGlobalRelative(*waypoint)
        vehicle.commands.add(wp)
    vehicle.flush()


# 이륙 함수
def arm_and_takeoff(vehicle, aTargetAltitude):
    print("드론 이륙준비...")
    # 드론 arm
    while not vehicle.is_armable:
        print("드론 arm 가능 대기 중...")
        time.sleep(1)

    print("드론 arm")
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    # 이륙 고도 도달 대기
    while not vehicle.armed:
        print("드론 arming 중...")
        time.sleep(1)

    print("드론 이륙")
    vehicle.simple_takeoff(aTargetAltitude)

    while True:
        print("현재 고도: ", vehicle.location.global_relative_frame.alt)
        if vehicle.location.global_relative_frame.alt >= aTargetAltitude * 0.95:
            print("목표 고도 도달")
            break
        time.sleep(1)

# 착륙 함수
def land(vehicle):
    print("드론 착륙 준비...")
    # RTL 모드 설정
    vehicle.mode = VehicleMode("RTL")
    # 착륙 대기
    while vehicle.location.global_relative_frame.alt > 0.1:
        print("현재 고도: ", vehicle.location.global_relative_frame.alt)
        time.sleep(1)
    print("드론 착륙")

    

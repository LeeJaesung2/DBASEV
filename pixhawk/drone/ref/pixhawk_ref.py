import time
from dronekit import connect, VehicleMode, LocationGlobalRelative
from pymavlink import mavutil

# 연결할 드론의 시리얼 포트나 IP 주소를 입력합니다.
connection_string = '127.0.0.1:14550'

# 드론에 연결합니다.
vehicle = connect(connection_string, wait_ready=True)

# 드론의 목표 속도를 설정합니다. (m/s)
target_speed = 5

# 드론의 현재 위치를 저장합니다.
current_location = vehicle.location.global_frame

# waypoint 리스트를 초기화합니다.
waypoints = []

# waypoint를 생성하고 리스트에 추가합니다.
wp1 = current_location
waypoints.append(wp1)

wp2 = LocationGlobalRelative(37.0, -122.0, 10.0)
waypoints.append(wp2)

wp3 = LocationGlobalRelative(37.0, -122.0, 20.0)
waypoints.append(wp3)

# 드론의 목표 지점을 설정합니다.
for wp in waypoints:
    vehicle.simple_goto(wp)
    time.sleep(5)

# 드론의 현재 속도를 저장하는 변수를 초기화합니다.
current_speed = 0

# 드론의 목표 지점에 도착할 때까지 반복합니다.
while vehicle.mode.name == 'GUIDED':
    # waypoint 리스트에서 다음 목표 지점을 가져옵니다.
    wp = waypoints.pop(0)

    # 드론의 현재 위치와 목표 위치를 확인합니다.
    current_location = vehicle.location.global_frame
    distance_to_target = current_location.distance_to(wp)

    # 드론의 목표 지점에 가까워질수록 속도를 줄입니다.
    if distance_to_target > 10:
        target_speed = 15
    elif distance_to_target > 5:
        target_speed = 10
    else:
        target_speed = 5

    # 드론의 목표 속도와 현재 속도를 비교하여 속도를 조절합니다.
    if current_speed < target_speed:
        set_velocity_body(vehicle, target_speed, 0, 0)
        current_speed = target_speed
    elif current_speed > target_speed:
        set_velocity_body(vehicle, -target_speed, 0, 0)
        current_speed = -target_speed

    # waypoint까지 남은 거리가 5미터 이하일 경우, 다음 waypoint로 넘어갑니다.
    if distance_to_target < 5:
        if not waypoints:
            break
        else:
            wp = waypoints[0]

    # 1초마다 반복합니다.
    time.sleep(1)

# 드론을 RTL 모드로 변경합니다.
vehicle.mode = VehicleMode("RTL")

# 드론을 착륙시킵니다.
vehicle.close()

###################################################################

# 드론의 목표 속도와 방향을 설정합니다.
from dronekit import LocationGlobalRelative

# 드론에게 waypoint 리스트를 전달하여 추가하는 함수
def add_waypoints(vehicle, waypoint_list):
    # LocationGlobalRelative 객체를 이용해 waypoint 추가
    for waypoint in waypoint_list:
        wp = LocationGlobalRelative(*waypoint)
        vehicle.commands.add(wp)
    vehicle.flush()

###################################################################

from dronekit import connect, VehicleMode, LocationGlobalRelative
import time

def set_airspeed(vehicle, airspeed):
    """
    드론의 airspeed 값을 설정합니다.
    """
    msg = vehicle.message_factory.command_long_encode(
        0, 0,                                           # target system, target component
        mavutil.mavlink.MAV_CMD_DO_CHANGE_SPEED,         # command
        0,                                              # confirmation
        1, airspeed, -1, 0, 0, 0)                        # params
    vehicle.send_mavlink(msg)
    vehicle.flush()

# 드론에 연결
vehicle = connect('tcp:127.0.0.1:5760', wait_ready=True)

# 드론 이륙
print("Drone arming motors")
vehicle.mode = VehicleMode("GUIDED")
vehicle.armed = True
while not vehicle.mode.name=='GUIDED' and not vehicle.armed:
    print("Waiting for drone to enter GUIDED flight mode and arm...")
    time.sleep(1)

print("Drone taking off")
vehicle.simple_takeoff(10) # 드론을 10m 높이까지 이륙합니다.
while True:
    altitude = vehicle.location.global_relative_frame.alt
    if altitude >= 10*0.95: # 드론이 10m 이상의 높이에 도달하면
        print("Drone reached target altitude")
        break
    time.sleep(1)

# waypoint 설정
waypoint1 = LocationGlobalRelative(37.12345, 127.67890, 10)
waypoint2 = LocationGlobalRelative(37.23456, 127.78901, 20)
waypoint3 = LocationGlobalRelative(37.34567, 127.89012, 30)

# waypoint 리스트에 추가
waypoint_list = [waypoint1, waypoint2, waypoint3]

# airspeed 리스트
airspeed_list = [10, 20, 30]

# 드론 이동
print("Drone moving to waypoints")
for i, waypoint in enumerate(waypoint_list):
    vehicle.simple_goto(waypoint)
    while True:
        distance = vehicle.location.global_relative_frame.distance_to(waypoint)
        if distance <= 1: # 드론이 waypoint 근처에 도달하면
            print("Drone reached waypoint %d" % (i+1))
            break
        time.sleep(1)

    # airspeed 변경
    set_airspeed(vehicle, airspeed_list[i])

    # 일정 시간 대기
    time.sleep(0.15)

print("Drone mission complete")
vehicle.mode = VehicleMode("LAND") # 드론 착륙
while not vehicle.mode.name=='LAND':
    print("Waiting for drone to land...")
    time.sleep(1)

vehicle.armed = False # 드론 시동 종료
vehicle.close() # 드론 연결 종료

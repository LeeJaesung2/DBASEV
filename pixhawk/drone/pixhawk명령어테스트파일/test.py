from dronekit import connect, VehicleMode, LocationGlobalRelative
import time

#드론 pixhawk 연결함수
def connect_to_pixhawk():
    # 연결할 시리얼 포트의 경로를 지정합니다.
    connection_string = "/dev/ttyAMA0" #USB : '/dev/ttyACM0' 

    # 연결합니다.
    vehicle = connect(connection_string, baud=57600, wait_ready=True)

    # 연결된 기체의 정보를 출력합니다.
    print('Connected to vehicle on: {}'.format(connection_string))
    print('Vehicle mode: {}'.format(vehicle.mode.name))

    return vehicle

# 이륙 함수
def arm_and_takeoff_to_pixhawk(vehicle, aTargetAltitude):
    while not vehicle.is_armable:
        print("드론 arm 가능 대기 중...")
        time.sleep(1)

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


# 드론에게 waypoint 리스트를 전달하여 추가하는 함수
def add_waypoints_to_pixhawk(vehicle, waypoint_list):
    # LocationGlobalRelative 객체를 이용해 waypoint 추가
    for waypoint in waypoint_list:
        waypoint_gps = waypoint[2:]
        wp = LocationGlobalRelative(*waypoint_gps)
        vehicle.commands.add(wp)
    vehicle.flush()

# 드론 속도 변화 
def set_airspeed_to_pixhawk(vehicle, airspeed):
    vehicle.airspeed = airspeed

# 착륙 함수
def land_to_pixhawk(vehicle):

    vehicle.mode = VehicleMode("LAND") # 드론 착륙
    while not vehicle.mode.name=='LAND':
        print("Waiting for drone to land...")
        time.sleep(1)

    vehicle.armed = False # 드론 시동 종료
    vehicle.close() # 드론 연결 종료

"""------------------------------------------------------------------------------------"""

waypoints = [[1, 1, 35.1535098, 128.1002861, 5.0],
            [1, 2, 35.1535017, 128.1003405, 5.0],
            [1, 3, 35.153494, 128.1003952, 5.0],
            [1, 4, 35.1534847, 128.1004491, 5.0],
            [1, 5, 35.153477, 128.1005038, 5.0],
            [1, 6, 35.1534693, 128.1005578, 5.0],
            [1, 7, 35.1534608, 128.1006118, 5.0], 
            [1, 8, 35.1534523, 128.1006654, 5.0], 
            [1, 9, 35.1534447, 128.1007197, 5.0], 
            [1, 10, 35.1534351, 128.100773, 5.0], 
            [1, 11, 35.1534271, 128.1008267, 5.0], 
            [1, 12, 35.1534208, 128.1008813, 5.0], 
            [1, 13, 35.1534126, 128.100935, 5.0], 
            [1, 14, 35.1534038, 128.1009893, 5.0], 
            [1, 15, 35.1533948, 128.1010436, 5.0],
]

#test1 : 연결 함수
def test1():
    vehicle = connect_to_pixhawk()

#test2 : 연결 후 원하는 고도로 이륙
def test2():
    vehicle = connect_to_pixhawk()
    arm_and_takeoff_to_pixhawk(vehicle, 5.0)

# test3 : 연결 -> 이륙 -> 착륙
def test3():
    vehicle = connect_to_pixhawk()
    arm_and_takeoff_to_pixhawk(vehicle, 5.0)
    land_to_pixhawk(vehicle)

# test4 : 연결 -> 이륙 -> waypoint 이동
def test4():
    vehicle = connect_to_pixhawk()
    arm_and_takeoff_to_pixhawk(vehicle, 5.0)
    
    waypoints = [[1, 1, 35.1535098, 128.1002861, 5.0],
                [1, 2, 35.1535017, 128.1003405, 5.0],
                [1, 3, 35.153494, 128.1003952, 5.0],
                [1, 4, 35.1534847, 128.1004491, 5.0],
                [1, 5, 35.153477, 128.1005038, 5.0]
    ]

    add_waypoints_to_pixhawk(vehicle, waypoints)

# test5 : 연결 -> 이륙 -> wapoint이동 -> 이동중 속력 변화
def test5():
    vehicle = connect_to_pixhawk()
    arm_and_takeoff_to_pixhawk(vehicle, 5.0)

    add_waypoints_to_pixhawk(vehicle, waypoints)
    
    velocity = [10, 20, 30, 25, 20, 15, 10, 30]
    cnt = 0
    turnoff =0
    while True:
        # 5초마다 속도 바뀜
        time(5000)
        
        cnt += 1
        turnoff += 1
        if cnt >= len(velocity):
            cnt = 0

        set_airspeed_to_pixhawk(vehicle, velocity[cnt])

        if turnoff * 5 > 60:
            break



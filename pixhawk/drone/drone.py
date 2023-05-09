from collections import deque
from geopy.distance import geodesic
from dronekit import connect, VehicleMode, LocationGlobalRelative
from pymavlink import mavutil
import time

class Drone:
    def __init__(self, drone):
        self.vehicle = drone

        self.waypoint_dist = 5.0
        self.waypoint_num = int(round(100 / self.waypoint_dist))
        
        self.mode = self.vehicle.mode.name
        self.ishovering = False
        
        self.velocity = self.vehicle.airspeed
        self.max_speed = 11.11

        # 현재 gps
        self.gps = self.vehicle.location.global_relative_frame

        # 현재 waypoint
        self.waypoint = 0
        self.road_id = 0

        # 목표 waypoint
        self.target_waypoint = 1,
        self.target_waypoint_gps = LocationGlobalRelative(*(0.0, 0.0, 3.3))
        
        self.will_go_waypoint = deque()
        
    def update_drone_data(self):
        self.velocity= self.vehicle.airspeed
        self.gps = self.vehicle.location.global_relative_frame

    # 업데이트 드론 타겟 다시 짜야 겄다...
    def update_drone_target(self):

        # hovering 상태면 will go waypoint에 값이 있는지 확인
        # 있으면 will go way point에서 pop 해서 target 
        if self.ishovering:
            if self.will_go_waypoint:
                temp = self.will_go_waypoint.popleft()

                self.target_waypoint = temp[1]

                target_waypoint_gps = (temp[2], temp[3], temp[4])
                self.target_waypoint_gps = LocationGlobalRelative(*target_waypoint_gps)
                self.ishovering = False
        # target waypoint gps 와 드론 gps 사이 거리
        dist = self.gps.distance_to(self.target_waypoint)
        
        if dist < 1.5:
            if self.will_go_waypoint:
                temp = self.will_go_waypoint.popleft()

                self.target_waypoint = temp[1]
                target_waypoint_gps = (temp[2], temp[3], temp[4])
                self.target_waypoint_gps = LocationGlobalRelative(*target_waypoint_gps)
                self.ishovering = False
            else:
                self.ishovering = True

    def update_drone_velocity(self, car_data):
        # 같은 도로에 있는 경우
        if car_data.road_id == self.road_id:

            # 거리가 멀어져서 속도 조절이 필요한 상황
            if car_data.waypoint <= self.waypoint - self.waypoint_num:
                ideal_velocity = car_data.velocity - ((16.5 * (self.waypoint - car_data.waypoint - self.waypoint_num))/1000)

                if ideal_velocity < 0:
                    ideal_velocity = 0
                elif ideal_velocity > self.max_speed:
                    ideal_velocity = self.max_speed
                
                self.velocity = ideal_velocity

            # 거리가 가까워서 최대 속도를 내야하는 경우
            else:
                self.velocity = self.max_speed
        
        # 다른 도로에 있어 최대 속도를 내야하는 경우
        else:
            self.velocity = self.max_speed
            
    def update_will_go_waypoint(self, waypoints):
        for waypoint in waypoints:
            self.will_go_waypoint.append(waypoint)
        
    #드론 pixhawk 연결함수
    def connect_to_pixhawk(self):
        # 연결할 시리얼 포트의 경로를 지정합니다.
        port = "/dev/ttyAMA0" #USB : '/dev/ttyACM0' 
        connection_string = 'com{}'.format(port)

        # 연결합니다.
        vehicle = connect(connection_string, baud=57600, wait_ready=True)

        # 연결된 기체의 정보를 출력합니다.
        print('Connected to vehicle on: {}'.format(connection_string))
        print('Vehicle mode: {}'.format(vehicle.mode.name))

        self.vehicle = vehicle

    # 드론에게 waypoint 리스트를 전달하여 추가하는 함수
    def add_waypoints_to_pixhawk(self, waypoint_list):
        """
        waypoint_list의 format :

        waypoint_list = [
            (latitude1, longitude1, altitude1),
            (latitude2, longitude2, altitude2),
            (latitude3, longitude3, altitude3),
            ...
        ]
        """
        
        # LocationGlobalRelative 객체를 이용해 waypoint 추가
        for waypoint in waypoint_list:
            wp = LocationGlobalRelative(*waypoint)
            self.vehicle.commands.add(wp)
        self.vehicle.flush()

    # 드론 속도 변화 
    def set_airspeed(self, airspeed):
        """
        드론의 airspeed 값을 설정합니다.
        """
        msg = self.vehicle.message_factory.command_long_encode(
            0, 0,                                           # target system, target component
            mavutil.mavlink.MAV_CMD_DO_CHANGE_SPEED,         # command
            0,                                              # confirmation
            1, airspeed, -1, 0, 0, 0)                        # params
        self.vehicle.send_mavlink(msg)
        self.vehicle.flush()
    # 이륙 함수
    def arm_and_takeoff_to_pixhawk(self, aTargetAltitude):
        print("드론 이륙준비...")
        # 드론 arm
        while not self.vehicle.is_armable:
            print("드론 arm 가능 대기 중...")
            time.sleep(1)

        print("드론 arm")
        self.vehicle.mode = VehicleMode("GUIDED")
        self.vehicle.armed = True

        # 이륙 고도 도달 대기
        while not self.vehicle.armed:
            print("드론 arming 중...")
            time.sleep(1)

        print("드론 이륙")
        self.vehicle.simple_takeoff(aTargetAltitude)

        while True:
            print("현재 고도: ", self.vehicle.location.global_relative_frame.alt)
            if self.vehicle.location.global_relative_frame.alt >= aTargetAltitude * 0.95:
                print("목표 고도 도달")
                break
            time.sleep(1)

    # 착륙 함수
    def land_to_pixhawk(self):
        self.vehicle.mode = VehicleMode("LAND") # 드론 착륙
        while not self.vehicle.mode.name=='LAND':
            print("Waiting for drone to land...")
            time.sleep(1)

        self.vehicle.armed = False # 드론 시동 종료
        self.vehicle.close() # 드론 연결 종료
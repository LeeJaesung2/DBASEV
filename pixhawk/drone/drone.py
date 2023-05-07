from collections import deque
from geopy.distance import geodesic
from dronekit import connect, VehicleMode, LocationGlobalRelative
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
        self.gps =  {
            "latitude": self.vehicle.gps_0.latitude, #위도
            "longitude" : self.vehicle.gps_0.longitude, #경도
            "altitude" : self.vehicle.gps_0.altitude, #해발고도 
            "relative_alt" : self.vehicle.gps_0.realtive_alt #기체의 상대적 고도
        }

        # 현재 waypoint
        self.waypoint = 0
        self.road_id = 0

        # 목표 waypoint
        self.target_waypoint = 1,
        self.target_waypoint_gps = (0.0, 0.0)
        
        self.will_go_waypoint = deque()
        
    def update_drone_data(self):
        drone_gps = self.vehicle.gps_0

        self.velocity= self.vehicle.airspeed
        self.gps ={ 
            "latitude": drone_gps.latitude, #위도
            "longitude" : drone_gps.longitude, #경도
            "altitude" : drone_gps.altitude, #해발고도 
            "relative_alt" : drone_gps.realtive_alt #기체의 상대적 고도
        }

    def update_drone_target(self):
        # hovering 상태에서 target 어떻게 업데이트 할지 생각하셔야 해요
        if self.ishovering:
            if self.will_go_waypoint:
                temp = self.will_go_waypoint.popleft()
                self.target_waypoint = temp[0]
                self.target_waypoint_gps = temp[1]
                self.ishovering = False
        # target waypoint gps 와 드론 gps 사이 거리
        dist = geodesic((self.gps["latitude"], self.gps["longitude"])
                        ,self.target_waypoint_gps).meters
        
        if dist < 1.5:
            if self.will_go_waypoint:
                temp = self.will_go_waypoint.popleft()
                self.target_waypoint = temp[0]
                self.target_waypoint_gps = temp[1]
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
    def add_waypoints(self, waypoint_list):
        """
        waypoint_list의 format :

        waypoint_list = [
            [latitude1, longitude1, altitude1],
            [latitude2, longitude2, altitude2],
            [latitude3, longitude3, altitude3],
            ...
        ]
        """
        
        # LocationGlobalRelative 객체를 이용해 waypoint 추가
        for waypoint in waypoint_list:
            wp = LocationGlobalRelative(*waypoint)
            self.vehicle.commands.add(wp)
        self.vehicle.flush()

    # 이륙 함수
    def arm_and_takeoff(self, aTargetAltitude):
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
    def land(self):
        print("드론 착륙 준비...")
        # RTL 모드 설정
        self.vehicle.mode = VehicleMode("RTL")
        # 착륙 대기
        while self.vehicle.location.global_relative_frame.alt > 0.1:
            print("현재 고도: ", self.vehicle.location.global_relative_frame.alt)
            time.sleep(1)
        print("드론 착륙")
from collections import deque
from geopy.distance import geodesic

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
            
    def update_will_go_waypoint(self, car_data):
        pass

        

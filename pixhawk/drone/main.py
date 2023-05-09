from drone import Drone
from car import Car
from roadMap import roadMap
import time

road_map = roadMap()
drone = Drone()
car = Car()
pre_car_road_id = car.road_id

def init():
    drone.connect_to_pixhawk()
    drone.arm_and_takeoff_to_pixhawk(3.3)

def update(msg):
    global pre_car_road_id

    drone.update_drone_data()

    pre_car_road_id = car.road_id
    car.readmsg(msg)

    # 자동차의 예전 road와 다른 경우 will go waypoint update
    if pre_car_road_id != car.road_id:
        waypoints = road_map.roadmap[car.road_id]
        drone.update_will_go_waypoint(waypoints)
    
    drone.update_drone_target()
    drone.update_drone_velocity()


    
    

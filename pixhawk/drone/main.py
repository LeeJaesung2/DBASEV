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
    car.update_car_data(msg)

    # 자동차의 예전 road와 다른 경우 will go waypoint update
    if pre_car_road_id != car.road_id:
        waypoints = road_map.roadmap[car.road_id]
        drone.update_will_go_waypoint(waypoints)
        drone.add_waypoints_to_pixhawk(waypoints)
    
    drone.update_drone_target()

    car_data = car.get_car_data()
    drone.update_drone_velocity(car_data)
    
    drone.set_airspeed_to_pixhawk(drone.velocity)

    
    

from drone import Drone
from car import Car
from messagequeue import mq_init, pop
import time

def init(drone):
    drone.connect_to_pixhawk()
    #drone.sim_connect_to_pixhawk()
    drone.arm_and_takeoff_to_pixhawk(7.0)
    

def update(msg, roadMap, car, drone, pre_car_road_id):

    drone.update_drone_data()
    pre_car_road_id = car.road_id

    car.update_car_data(msg)

    if pre_car_road_id != car.road_id:
        waypoints = roadMap[car.road_id]
        # control drone waypoint
        drone.add_waypoints_to_pixhawk(waypoints)
    
    
    car_data = car.get_car_data()

    startpoint = (roadMap[drone.road_id][0][2], roadMap[drone.road_id][0][3], roadMap[drone.road_id][0][4]) 
    drone.update_dist(startpoint,  car_data)
    drone.update_drone_speed(car_data)
    
    # control drone speed
    drone.set_airspeed_to_pixhawk(drone.speed)

    return pre_car_road_id

def init_make_logfile(num):
    title = "logfile{}.txt".format(num)
    with open( title, "w") as file:
        file.write("time,car_speed,car_road_id,car_waypoint_id,drone_target_speed,drone_speed,drone_road_id,drone_waypoint_id\n")

def add_logfile(data):
    with open("logfile.txt", "a") as file:
        file.write(data)

def flight_control(a):
    # Get message queue ID using same key as C++ program
    key = 5656
    mq = mq_init(key)
    starttime = time.time()

    roadMap = {1: [[1, 1, 35.1520036, 128.1010298, 7.0], [1, 2, 35.151636, 128.1022704, 7.0]],
            2: [[2, 1, 35.1516449, 128.1022955, 7.0], [2, 2, 35.1518131, 128.102374, 7.0]],
            3: [[3, 1, 35.1516302, 128.102291, 7.0], [3, 2, 35.1515384, 128.1026009, 7.0]],
            4: [[4, 1, 35.151628, 128.1022865, 7.0], [4, 2, 35.1513796, 128.1021577, 7.0]],
            5: [[5, 1, 35.1513627, 128.1021487, 7.0], [5, 2, 35.1512119, 128.102027, 7.0]],
            6: [[6, 1, 35.1513802, 128.1021448, 7.0], [6, 2, 35.151591, 128.1016595, 7.0]],
            7: [[7, 1, 35.1516036, 128.1016307, 7.0], [7, 2, 35.15177, 128.1012411, 7.0]], 
            8: [[8, 1, 35.1516105, 128.1016628, 7.0], [8, 2, 35.1518857, 128.1019478, 7.0]]}

    drone = Drone()
    car = Car()
    pre_car_road_id = car.road_id

    init_make_logfile()
    init(drone)
    
    while True:
        msg = pop(mq)
    
        pre_car_road_id = update(msg, roadMap, car, drone, pre_car_road_id)
        
        update_time = time.time()
        
        log_data = "{},{},{},{},{},{},{},{}\n".format(update_time-starttime,car.speed,car.road_id,car.waypoint,drone.target_speed,drone.speed,drone.road_id,drone.target_speed)

        add_logfile(log_data)

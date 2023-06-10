from drone import Drone
from car import Car
#from messagequeue import mq_init, pop
import time, datetime

def init(drone):
    #drone.connect_to_pixhawk()
    drone.sim_connect_to_pixhawk()
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
    print(drone.speed)
    drone.set_airspeed_to_pixhawk(drone.target_speed)

    return pre_car_road_id

def init_make_logfile(num):
    title = "{} logfile.txt".format(num)
    with open( title, "w") as file:
        file.write("time,car_road_id,car_speed,car_dist,drone_road_id,drone_speed,drone_target_speed,drone_dist,distance\n")

def add_logfile(data, num):
    title = "{} logfile.txt".format(num)
    with open(title, "a") as file:
        file.write(data)

'''
def flight_control(a):
    # Get message queue ID using same key as C++ program
    key = 5656
    mq = mq_init(key)
    starttime = time.time()

    roadMap = {1: [[1, 1, 35.1554345, 128.1040969, 7.0], [1, 2, 35.1543852, 128.1045824, 7.0]],
            2: [[2, 1, 35.1543528, 128.1045948, 7.0], [2, 2, 35.1541135, 128.1047038, 7.0]],
            3: [[3, 1, 35.1543841, 128.1046156, 7.0], [3, 2, 35.1544301, 128.1048282, 7.0]],
            4: [[4, 1, 35.1543704, 128.1045506, 7.0], [4, 2, 35.1542914, 128.1041778, 7.0]],
            5: [[5, 1, 35.1544608, 128.10485, 7.0], [5, 2, 35.1547827, 128.1046528, 7.0]],
            6: [[6, 1, 35.1544513, 128.1048989, 7.0], [6, 2, 35.1544798, 128.1050029, 7.0]],
            7: [[7, 1, 35.1544175, 128.1048654, 7.0], [7, 2, 35.1542509, 128.1049486, 7.0]]
    }

    drone = Drone()
    car = Car()
    pre_car_road_id = car.road_id

    current_time = datetime.datetime.now()
    formatted_time = current_time.strftime("%Y-%m-%d %H:%M:%S")

    init_make_logfile(formatted_time)
    init(drone)
    
    while True:
        msg = pop(mq)
    
        pre_car_road_id = update(msg, roadMap, car, drone, pre_car_road_id)
        
        update_time = time.time()
        
        log_data = "{},{},{},{},{},{},{},{}\n".format(update_time-starttime, car.road_id, car.speed, car.dist, drone.road_id, drone.speed, drone.target_speed, drone.dist, drone.dist_drone_car)

        add_logfile(log_data, formatted_time)
'''
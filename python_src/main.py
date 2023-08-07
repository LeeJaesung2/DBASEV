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

    # car pre road is not match update will go waypoint update
    if pre_car_road_id != car.road_id:
        waypoints = roadMap[car.road_id]
        drone.update_will_go_waypoint(waypoints)
        drone.add_waypoints_to_pixhawk(waypoints)
    
    drone.update_drone_target()

    car_data = car.get_car_data()
    drone.update_drone_velocity(car_data)
    
    drone.set_airspeed_to_pixhawk(drone.velocity)

    return pre_car_road_id

def init_make_logfile():
    with open("logfile.txt", "w") as file:
        file.write("time,car_speed,car_road_id,car_waypoint_id,drone_target_speed,drone_speed,drone_road_id,drone_waypoint_id\n")

def add_logfile(data):
    with open("logfile.txt", "a") as file:
        file.write(data)

def flight_control(a):
    # Get message queue ID using same key as C++ program
    key = 5656
    mq = mq_init(key)
    starttime = time.time()


    roadMap = {1: [[1, 1, 35.1520036, 128.1010298, 7.0], [1, 2, 35.1519729, 128.1011331, 7.0], [1, 3, 35.1519423, 128.1012366, 7.0], [1, 
4, 35.1519117, 128.10134, 7.0], [1, 5, 35.151881, 128.1014436, 7.0], [1, 6, 35.1518504, 128.1015469, 7.0], [1, 7, 35.1518198, 128.1016501, 7.0], [1, 8, 35.1517889, 128.1017537, 7.0], [1, 9, 35.1517584, 128.101857, 7.0], [1, 10, 35.1517279, 128.1019603, 7.0], [1, 11, 35.1516971, 128.1020642, 7.0], [1, 12, 35.1516666, 128.1021673, 7.0], [1, 13, 35.151636, 128.1022704, 
7.0]], 2: [[2, 1, 35.1516449, 128.1022955, 7.0], [2, 2, 35.1517289, 128.1023351, 7.0], [2, 3, 35.1518131, 128.102374, 7.0]], 3: [[3, 1, 35.1516302, 128.102291, 7.0], [3, 2, 35.1516006, 128.1023948, 7.0], [3, 3, 35.1515695, 128.1024979, 7.0], [3, 
4, 35.1515384, 128.1026009, 7.0]], 4: [[4, 1, 35.151628, 128.1022865, 7.0], [4, 2, 35.1515447, 128.1022449, 7.0], [4, 3, 35.1514622, 128.1022013, 7.0], [4, 4, 35.1513796, 128.1021577, 7.0]], 5: [[5, 1, 35.1513627, 128.1021487, 7.0], [5, 2, 35.1512873, 128.1020879, 7.0], [5, 3, 35.1512119, 128.102027, 7.0]], 6: [[6, 1, 35.1513802, 128.1021448, 7.0], [6, 2, 35.151423, 
128.1020477, 7.0], [6, 3, 35.1514657, 128.1019512, 7.0], [6, 4, 35.1515068, 128.1018533, 7.0], [6, 5, 35.1515489, 128.1017564, 7.0], [6, 6, 35.151591, 128.1016595, 7.0]], 7: [[7, 1, 35.1516036, 128.1016307, 7.0], [7, 2, 35.1516452, 128.1015333, 7.0], [7, 3, 35.1516868, 128.1014359, 7.0], [7, 4, 35.1517284, 128.1013385, 7.0], [7, 5, 35.15177, 128.1012411, 7.0]], 8: [[8, 1, 35.1516105, 128.1016628, 7.0], [8, 2, 35.1516793, 128.1017341, 7.0], [8, 3, 35.1517481, 128.1018053, 7.0], [8, 4, 35.1518169, 128.1018766, 7.0], [8, 5, 35.1518857, 128.1019478, 7.0]]}

    drone = Drone()
    car = Car()
    pre_car_road_id = car.road_id

    init_make_logfile()
    init(drone)
    
    while True:
        msg = pop(mq)
    
        pre_car_road_id = update(msg, roadMap, car, drone, pre_car_road_id)
        
        update_time = time.time()
        
        log_data = "{},{},{},{},{},{},{},{}\n".format(update_time-starttime,car.velocity,car.road_id,car.waypoint,drone.velocity,drone.current_speed,drone.road_id,drone.target_waypoint)

        add_logfile(log_data)
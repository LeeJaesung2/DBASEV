from dronekit import connect, VehicleMode, LocationGlobalRelative, LocationGlobal, Command
import time
import math
from pymavlink import mavutil
import dronekit_sitl
import argparse  

def main(init_flag, vehicle = None, cmds = None , car_data = None, drone_data=None, msg):
    if init_flag:
        vehicle, car_data, drone_data = init_setting(0 ,0)
        return  vehicle, car_data, drone_data

    car_data, drone_data = readmsg(msg, car_data, drone_data)
    drone_data = setting_drone_velocity(car_data, drone_data)
    update_mission

    return  vehicle, car_data, drone_data

######################initialize########################
# init setting
def init_setting(home_road_id, home_waypoint_id):
    car_data ={
        "road_id" : home_road_id,
        "waypoint_id" : home_waypoint_id,
        "velocity" : 0.0,
    }

    drone_data ={
        "road_id" : home_road_id,
        "waypoint_id" : home_waypoint_id,
        "velocity" : 0.0,
        "max_velocity" : 50.0,
        "will_go_waypoints" : []
    }

    vehicle = connect("/dev/ttyAMA0", wait_ready=True, baud=57600)
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True
    while not vehicle.mode.name =='GUIDED' and not vehicle.armed and not api.exit:
        print (" ready to take off ...")
        time.sleep(1)
    
    init_commands(vehicle) # Mission initialization
    vehicle = arm_and_takeoff(vehicle, 3.3) # takeoff
    vehicle = start_mission(vehicle)

    return vehicle, car_data, drone_data

# Mission initialization
def init_commands(vehicle):
    print("Initializing command...")
    cmds = vehicle.commands
    cmds.clear()
    cmds.upload()
    cmds.wait_ready()
    print("Command initialization complete")   


# takeoff code
def arm_and_takeoff(vehicle, aTargetAltitude):
    print("Basic Pre-Scanning...")
    while not vehicle.is_armable:
        print("Turning on the engine...")
        time.sleep(1)

    print("Check if the motor is running!!")
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    while not vehicle.armed:
        print("Checking equipment availability...")
        time.sleep(1)

    print("takeoff!!")
    vehicle.simple_takeoff(aTargetAltitude)

    while True:
        print(" elevation: ", vehicle.location.global_relative_frame.alt)
        if vehicle.location.global_relative_frame.alt >= aTargetAltitude*0.95:
            print("Target elevation reached!")
            break
        time.sleep(1)
    
    return vehicle

# Self-driving system in operation
def start_mission(vehicle):
    print("Automatic driving system running...")
    vehicle.commands.next=0
    vehicle.mode = VehicleMode("AUTO")
    print("Complete")
    return vehicle

######################analysis massage########################

def readmsg(msg, car_data, drone_data):

    mode = None # 0 : enter new road | 1 : update data

    msg_list = msg.strip().split('nxt')
    # determine mode & update common data
    if msg_list[0].strip().split()[0] == "Enter_new_road":
        mode = 0
        print("Enter new road")

    elif msg_list[0].strip().split()[0] == "real_time_value":
        mode = 1
        print("Update car value")
    
    car_data["velocity"] = float(msg_list[1].strip().split()[1])
    car_data["road_id"] = int(msg_list[2].strip().split()[1])
    
    if mode == 1:
        car_data["car_waypoint_id"] = int(msg_list[3].strip().split()[1])
        return car_data, drone_data
    
    # add way points
    elif mode == 0:
        for way_point in msg_list[4:]:
            way_point_data = way_point.strip().split()
            param1 = int(way_point_data[0])
            param2 = float(way_point_data[1])
            param3 = float(way_point_data[2])
            param4 = float(way_point_data[3])
            param5 = int(way_point_data[4])
            param6 = int(way_point_data[5])

            drone_data["will_go_waypoints"].append([car_data["road_id"] , param1, param2, param3, param4, param5, param6])
        
        return car_data, drone_data

def setting_drone_velocity(car_data, drone_data):
    # if they are same road.
    if car_data["road_id"] == drone_data["road_id"]:
        print("car and drone was in same road")
        # if drone far more than 6 way point
        if car_data["waypoint_id"] <= drone_data["waypoint_id"] - 6:
            
            temp_velocity = car_data["velocity"] - ((16.5 * (drone_data["waypoint_id"] - car_data["waypoint_id"] - 6))/1000)

            if temp_velocity < 0 :
                drone_data["velocity"] = 0
            elif temp_velocity > drone_data["max_velocity"]:
                drone_data["velocity"] = drone_data["max_velocity"]
            else:
                drone_data["velocity"] = temp_velocity
        # if drone less than 6 way point.    
        else:
            drone_data["velocity"] = drone_data["max_velocity"]

    # if they are different road.
    else:
        drone_data["velocity"] = drone_data["max_velocity"]

    return drone_data
    

def update_mission(vehicle, car_data, drone_data):
    cmds_list = {
        0:"MAV_CMD_NAV_TAKEOFF",
        1:"MAV_CMD_NAV_LAND",
        2:"MAV_CMD_NAV_WAYPOINT",
        3:"MAV_CMD_NAV_LOITER_TIME",
        4:"MAV_CMD_NAV_RETURN_TO_LAUNCH",
        5:"MAV_CMD_NAV_CONDITION_YAW",
        6:"MAV_CMD_NAV_CONDITION_CHANGE_ALT",
        7:"MAV_CMD_DO_CHANGE_SPEED",
    }

    cmds = vehicle.cmds
    cmds_length = len(cmds)
    pass






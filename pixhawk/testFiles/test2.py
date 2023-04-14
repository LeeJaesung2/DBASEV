from dronekit import connect, VehicleMode, LocationGlobalRelative, LocationGlobal, Command
import time
import math
from pymavlink import mavutil
import dronekit_sitl
import argparse  

def main():
    vehicle, cmds = init_setting()


######################초기화########################
# init setting
def init_setting():
    vehicle = connect("/dev/ttyAMA0", wait_ready=True, baud=57600)
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True
    while not vehicle.mode.name =='GUIDED' and not vehicle.armed and not api.exit:
        print (" ready to take off ...")
        time.sleep(1)
    
    cmds = init_commands(vehicle) # Mission initialization
    vehicle = arm_and_takeoff(vehicle, 3.3) # takeoff
    vehicle = start_mission(vehicle)

    return vehicle, cmds

# Mission initialization
def init_commands(vehicle):
    print("Initializing command...")
    cmds = vehicle.commands
    cmds.clear()
    cmds.upload()
    cmds.wait_ready()
    print("Command initialization complete")   
    return cmds

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

######################초기화########################

######################메세지 분석########################
def readmsg(msg, pre_road_id, pre_car_waypoint_id, pre_car_velocity):
    msg_list = msg.strip().split('nxt')
    car_data ={
        "car_road_id" : pre_road_id,
        "car_waypoint_id" : pre_car_waypoint_id,
        "car_velocity" : pre_car_velocity
    }
    will_go_way_points = []
    mode = None # 0 : enter new road | 1 : update data

    # determine mode & update common data
    for idx in range(4):
        clean_data  = msg_list[idx].strip().split()
        
        # data format info
        if idx == 0: 
            if clean_data[0] == "Enter_new_road":
                mode = 0
                print("Enter new road data")
            elif clean_data[0] == "real_time_value":
                mode = 1
                print("Update car value")
        
        # car velocity
        elif idx == 1:
            car_data["car_velocity"] = float(clean_data[1])
        
        # car road id
        elif idx == 2:
            car_data["car_road_id"] = int(clean_data[1])

        # car waypoint id    
        elif idx == 3:    
            if mode == 1:
                car_data["car_waypoint_id"] = int(clean_data[1])

    # add way points
    if mode == 0:
        for way_point in msg_list[4:]:
            way_point_data = way_point.strip().split()
            param1 = int(way_point_data[0])
            param2 = float(way_point_data[0])
            param3 = float(way_point_data[0])
            param4 = float(way_point_data[0])
            param5 = int(way_point_data[0])
            param6 = int(way_point_data[0])

            will_go_way_points.append([param1, param2, param3, param4, param5, param6])
        
        return car_data, will_go_way_points
    else:
        return car_data, None



def send_mission():
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

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
def readmission(msg):
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

    print("\n Reading mission")
    missionlist = []

    

    msg_list = msg.split('nxt')
    for i, cur_msg in enumerate(msg_list):
        if i == 0:
            temp = cur_msg.split(":")
            car_velocity = int(temp[1])
        else:
            parm_str = cur_msg.split("\t")
            parm0 = int(parm_str[0]) # command
            parm1 = float(parm_str[1])
            parm2 = float(parm_str[2])
            parm3 = float(parm_str[3])
            parm4 = float(parm_str[4])
            parm5 = float(parm_str[5])  
            parm6 = float(parm_str[6]) 
            parm7 = float(parm_str[7]) 

            new_command = Command(0, 0, 0,
                            mavutil.mavlink.MAV_FAME_GLOBAL_RLATIVE_ALT_INT, 
                            cmds_list[parm0],
                            parm1, parm2, parm3, parm4, parm5, parm6, parm7 )    

            missionlist.append(new_command)
    
    return car_velocity, missionlist

def control_velocity(car_velocity, vehicle_velocity):
    #1.
    pass

def addmissionlist(vehicle):
    vehicle.velocity
    vehicle.gps_0
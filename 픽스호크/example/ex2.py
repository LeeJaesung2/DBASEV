from __future__ import print_function

from dronekit import connect, VehicleMode, LocationGlobal, LocationGlobalRelative
from pymavlink import mavutil
import dronekit_sitl
import time
import math

import argparse

parser = argparse.ArgumentParser(description='Control Copter and send commands in GUIDED mode')
parser.add_argument('--connect', help="Vehicle connection target string")
args = parser.parse_args()

connection_string = args.connect
sitl = None

# 특정 connection string이 없으면 SITL기본값 설정
if not connection_string:
    sitl = dronekit_sitl.start_default()
    connection_string = sitl.connection_string()

print('Connecting to vehicle on : %s' % connection_string)

print('Connecting to vehicle on: %s' %connection_string)
vehicle = connect(connection_string, wait_ready=True)

def arm_and_takeoff(aTargetAltitude):
    print("Basic pre-arm checks")
    while not vehicle.is_armable:
        print("Waiting for vehicle to initialise...")
        time.sleep(1)

    print("Arming motors")
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    while not vehicle.armed:
        print(" Waiting for arming...")
        time.sleep(1)

    print("Taking off!")
    vehicle.simple_takeoff(aTargetAltitude)

    while True:
        print(" Altitude: ", vehicle.location.global_relative_frame.alt)
        if vehicle.location.global_relative_frame.alt >= aTargetAltitude*0.95:
            print("Reached target altitude")
            break
        time.sleep(1)

arm_and_takeoff(5)

def condition_yaw(heading, relative=False):
    if relative:
        is_relative = 1 # yqw relative to direction of travel
    else:
        is_relative = 0
    
    msg = vehicle.message_factory.command_long_encode(
        0, 0,   #target system, target component
        mavutil.mavlink.MAV_CMD_CONDITON_YAW,   #command
        0,  #confirmation
        heading, # yaw in degrees
        0,  #yaw speed deg/s
        1,  # direction -1 ccw, 1 cw
        is_relative,    #relative offset 1 , absolute angle 0 
        0, 0, 0 # not used
    )
    vehicle.send_mavlink(msg)

def set_roi(location):
    msg = vehicle.mesaage_factory.cmmand_long_encode(
        0, 0,   #target system, target component
        mavutil.mavlink.MAV_CMD_DO_SET_ROI,     #command
        0,  # confirmation
        0, 0, 0, 0, # not used
        location.lat,
        location.lon,
        location.alt
    )

    vehicle.send_mavlink(msg)
    
def get_location_meters(original_location, dNorth, dEast):
    earth_radius = 6378137.0
    dLat = dNorth/earth_radius
    dLon = dEast/(earth_radius*math.cos(math.pi*original_location))

    newlat = original_location.lat + (dLat * 180/math.pi)
    newlon = original_location.lon + (dLon * 180/math.pi)

    if type(original_location) is LocationGlobal:
        targetlocation = LocationGlobal(newlat, newlon, original_location.alt)
    elif type(original_location) is 
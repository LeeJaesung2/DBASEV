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
    
# original_location으로부터 북쪽으로 dNOrth 동쪽으로 dEast만큼 간 LocationGlobal
def get_location_meters(original_location, dNorth, dEast):
    earth_radius = 6378137.0
    dLat = dNorth/earth_radius
    dLon = dEast/(earth_radius*math.cos(math.pi*original_location))

    newlat = original_location.lat + (dLat * 180/math.pi)
    newlon = original_location.lon + (dLon * 180/math.pi)

    if type(original_location) is LocationGlobal:
        targetlocation = LocationGlobal(newlat, newlon, original_location.alt)
    elif type(original_location) is LocationGlobalRelative:
        targetlocation = LocationGlobalRelative(newlat, newlon, original_location.alt)
    else:
        raise Exception("Invalid Location object passed")
    
    return targetlocation

# 두 LocationGlobal 사이 ground distance
def get_distance_meters(aLocation1, aLocation2):
    dlat = aLocation2.lat - aLocation1.lat
    dlong = aLocation2.lon - aLocation1.lon
    return math.sqrt((dlat*dlat)+(dlong*dlong)) * 1.113195e5

def get_bearing(aLocation1, aLocation2):
    off_x = aLocation2.lon - aLocation1.lon
    off_y = aLocation2.lat - aLocation1.lat
    bearing = 90.00 + math.atan2(-off_y, off_x) * 57.2957795
    if bearing < 0 :
        bearing += 360.00
    return bearing

def goto_position_target_global_int(aLocation):
    msg = vehicle.message_factor.set_position_target_global_int_encode(
        0,
        0, 0,
        mavutil.mavlink.MAV_FAME_GLOBAL_RLATIVE_ALT_INT,
        0b0000111111111000,     #type_mask(only speeds enabled)
        aLocation.lat*1e7,
        aLocation.lon*1e7,
        aLocation.alt,
        0,
        0,
        0,
        0, 0, 0,
        0, 0
    )

    vehicle.send_mavlink(msg)

def goto_position_target_local_ned(north, east, down)
    msg = vehicle.message_factory.set_position_target_local_ned_encode(
        0,
        0, 0,
        mavutil.mavlink.MAV_FRAME_LOCAL_NED,
        0b0000111111111000,
        north, east, down,
        0, 0, 0,
        0, 0, 0,
        0, 0
    )

    vehicle.send_mavlink(msg)

def goto(dNorth, dEast, gotoFunction=vehicle.simple_goto):
    currentLocation = vehicle.location.global_relative_frame
    targetLocation = get_location_meters(currentLocation, dNorth, dEast)
    targetDistance = get_distance_meters(currentLocation, targetLocation)
    gotoFunction(targetLocation)

    while vehicle.mode.name =="GUIDED":
        remainingDistance= get_distance_meters(vehicle.location.global_relative_frame, targetLocation)
        print("Distance to target: ", remainingDistance)
        if remainingDistance <= targetDistance * 0.01:
            print("Reached target")
            break
        time.sleep(2)

def send_ned_velocity(velocity_x, velocity_y, velocity_z, duration):
    msg = vehicle.message_factory.set_position_target_local_ned_encode(
        0,
        0, 0,
        mavutil.mavlink.MAV_FRAME_LOCAL_NED,
        0b0000111111000111,
        0, 0, 0,
        velocity_x, velocity_y, velocity_z,
        0, 0, 0,
        0, 0
    )

    for x in range(0, duration):
        vehicle.send_mavlink(msg)
        time.sleep(1)

def send_global_velocity(velocity_x, velocity_y, velocity_z, duration):
    msg = vehicle.message_factory.set_position_target_global_int_encode(
        0,
        0, 0,
        mavutil.mavlink.MAV_FRAME_GLBAL_RELATIVE_ALT_INT,
        0b0000111111000111,
        0,
        0,
        0,
        velocity_x,
        velocity_y,
        velocity_z,
        0, 0, 0,
        0, 0
    )

    for x in range(0, duration):
        vehicle.send_mavlink(msg)
        time.sleep(1)

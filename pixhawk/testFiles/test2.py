from dronekit import connect, VehicleMode, LocationGlobalRelative, LocationGlobal, Command
from pymavlink.enums import MAV_CMD
import time
import math
from pymavlink import mavutil
import dronekit_sitl
import argparse  

def main(init_flag, msg, vehicle = None, cmds = None , car_data = None, drone_data=None,):
    if init_flag:
        vehicle, car_data, drone_data = init_setting(0 ,0)
        return  vehicle, car_data, drone_data

    car_data, drone_data = readmsg(msg, car_data, drone_data)
    drone_data = setting_drone_velocity(vehicle, car_data, drone_data)

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
        car_data["waypoint_id"] = int(msg_list[3].strip().split()[1])
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

def setting_drone_velocity(vehicle, car_data, drone_data):
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

    update_mission(vehicle, drone_data, 0)
    return drone_data
    

def update_mission(vehicle, drone_data, update_waypoint):

    if update_waypoint: # waypoint 넣는 명령어 일경우
        for item in drone_data["will_go_points"]:
            
            if item[5]:
                new_cmd = Command(
                    0, 0, 0,
                    mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
                    mavutil.mavlink.MAV_CMD_NAV_WAYPOINT,
                    0,
                    0, 0, 0,
                    item[1],
                    item[2],
                    item[3],
                )
            else: # 마지막 waypoint는 30초 교차로 호버링
                new_cmd = Command(
                    0, 0, 0,
                    mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
                    mavutil.mavlink.MAV_CMD_NAV_WAYPOINT,
                    30,
                    0, 0, 0,
                    item[1],
                    item[2],
                    item[3],
                )


            vehicle.commands.add(new_cmd)
        vehicle.commands.upload()
        return drone_data

    
    else: # 속도 변환 메세지 인경우
        if vehicle.commands.next: # 만약에 다음 가야 할 곳이 있다면
            waypoint = vehicle.commands.next

            current_speed = vehicle.groundspeed
            new_speed = drone_data["velocity"]
    
            # 현재 위치를 얻어옵니다.
            current_location = vehicle.location.global_relative_frame

            # 현재 위치와 이동 중인 waypoint 사이의 거리를 계산합니다.
            distance_to_waypoint = current_location.distance_to(waypoint)

            # 만약, 현재 위치와 이동 중인 waypoint 사이의 거리가 3m 미만이면, 다음 waypoint로 이동합니다.
            if distance_to_waypoint < 3:
                vehicle.commands.next
                waypoint = vehicle.commands.next
                
            
            # 속도를 업데이트합니다.
            if abs(current_speed - new_speed) > 0.1:
                msg = vehicle.message_factory.command_long_encode(
                    0, 0,    # target system, target component
                    mavutil.mavlink.MAV_CMD_DO_CHANGE_SPEED,  # command
                    0,       # confirmation
                    1,       # speed type (1 = airspeed, 2 = groundspeed)
                    new_speed,  # new speed in m/s
                    -1,      # throttle (not used)
                    0, 0, 0)  # parameters not used
                vehicle.send_mavlink(msg)
                vehicle.flush()

            time.sleep(0.1)


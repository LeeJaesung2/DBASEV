from dronekit import connect, VehicleMode, LocationGlobalRelative, LocationGlobal, Command
import time
import math
from pymavlink import mavutil
import dronekit_sitl

#Set up option parsing to get connection string
import argparse  
parser = argparse.ArgumentParser(description='Control Copter and send commands in GUIDED mode ')
parser.add_argument('--connect', 
                help="Vehicle connection target string. If not specified, SITL automatically started and used.")
args = parser.parse_args()

connection_string = args.connect
sitl = None

#Start SITL if no connection string specified
if not connection_string:
    sitl = dronekit_sitl.start_default()
    connection_string = sitl.connection_string()

# Connect to the Vehicle
print('Connecting to vehicle on: %s' % connection_string)
vehicle = connect(connection_string, wait_ready=True)
cmds = vehicle.cmds


# Read the mission file
def readmission(aFileName):
    print("\nRead from the following file : %s" % aFileName)

    missionlist=[]
    with open(aFileName) as f:
        for i, line in enumerate(f):
            if i==0:
                if not line.startswith('QGC WPL 110'):
                    raise Exception('File is not supported WP version')
            else:
                linearray=line.strip().split('\t')

                ln_index=int(linearray[0])  # Command Index
                ln_command=int(linearray[1]) # Command
                ln_param1=float(linearray[2]) # Parameters 1
                ln_param2=float(linearray[3]) # Parameters 2
                ln_param3=float(linearray[4]) # Parameters 3
                ln_param4=float(linearray[5]) # Parameters 4
                ln_param5=float(linearray[6]) # Target latitude, 0 means current latitude
                ln_param6=float(linearray[7]) # Target hardness, if zero, current hardness
                ln_param7=float(linearray[8]) # Target altitude, 0 means current altitude
                
    return missionlist

# Mission initialization
def init_commands():
    print("Initializing command...")
    cmds = vehicle.commands
    cmds.clear()
    cmds.upload()
    cmds.wait_ready()
    print("Command initialization complete!!")

# takeoff code
def arm_and_takeoff(aTargetAltitude):
    print("Basic Pre-Scanning...")
    while not vehicle.is_armable:
        print("Waiting for initialization...")
        time.sleep(1)

    print("Check operation of motor operation!!")
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    while not vehicle.armed:
        print("Checking equipment availability...")
        time.sleep(1)

    print("Takeoff!!")
    vehicle.simple_takeoff(aTargetAltitude)

    while True:
        print("altitude: ", vehicle.location.global_relative_frame.alt)
        if vehicle.location.global_relative_frame.alt >= aTargetAltitude*0.95:
            print("You've reached your target altitude!!")
            break
        time.sleep(1)

# Setting up to start the mission
def start_mission():
    print("Automatic driving system in operation...")
    vehicle.commands.next=0
    vehicle.mode = VehicleMode("AUTO")

# Mission Additional Code
def add_mission(new_cmd):
    print(f"new command: {new_cmd} Adding...")
    cmds.add(new_cmd)
    cmds.upload()
    print("Addition Complete!!")

# The first mission addition code
def add_mission_first(new_cmd):
    print("Putting in a new mission for the first time...")
    missionlist = [new_cmd]
    for cmd in cmds:
        missionlist.append(cmd)
    
    cmds.clear()
    for cmd in missionlist:
        cmds.add(cmd)

    cmds.upload()
    print("New mission completed uploading for the first time!!")

# Get rid of all the missions and add a new mission code
def add_mission_ignore_all_mission(new_cmd):
    print("Deleting all commands ...")
    cmds.clear()
    print("Delete all commands complete!!")

    print(f"New command : {new_cmd} adding ...")
    cmds.add(new_cmd)
    cmds.upload()
    print("Addition Complete!!")

# Create a waypoint and put it in the command
def way_point(stop_time=0, target_lat=0, target_lon=0, target_alt=0):
    new_cmd = Command(
        0, 0, 0,
        mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
        mavutil.mavlink.MAV_CMD_NAV_WAYPOINT,
        stop_time,
        0, 0, 0,
        target_lat,
        target_lon,
        target_alt,
    )
    return new_cmd

# Hovering in a Specific Location
def hovering_time(stop_time=1.0, target_lat=0, target_lon=0, target_alt=0):
    new_cmd = Command(
        0, 0, 0,
        mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
        mavutil.mavlink.MAV_CMD_NAV_LOITER_TIME,
        stop_time,
        0, 0, 0,
        target_lat,
        target_lon,
        target_alt,
    )

    return new_cmd

# Setting the Home Location
def set_home(target_alt):
    print("\nSet new home location")
    my_location_alt = vehicle.location.global_frame
    my_location_alt.alt = target_alt
    vehicle.home_location = my_location_alt

# Return to Home Location or Gathering Point
def return_home():
    new_cmd = Command(
        0, 0, 0,
        mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
        mavutil.mavlink.MAV_CMD_NAV_RETURN_TO_LAUNCH,
        0,
        0, 0, 0,
        0, 0, 0
    )

    return new_cmd

init_commands()
arm_and_takeoff(5)
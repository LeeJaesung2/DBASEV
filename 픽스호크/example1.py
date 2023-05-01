from dronekit import connect, VehicleMode, LocationGlobal, LocationGlobalRelative
from pymavlink import mavutil
import time
import math
import argparse

def connectMyCopter():
    parser = argparse.ArgumentParser(description="commands")
    parser.add_argument('--connect')
    args = parser.parse_args()
    connection_string = args.connect
    baud_rate = 921600

    vehicle = connect(connection_string, baud=baud_rate, wait_ready=True)

    return vehicle

vehicle = connectMyCopter()

def arm_and_takeoff_nogps(aTargetAltitude):
    DEFAULT_TAKEOFF_THRUST = 0.7
    SMOOTH_TAKEOFF_TURST = 0.6
    print("Basic pre-arm checks")

    print("Arming motors")

    vehicle.mode = VehicleMode("GUIDED_NOGPS")
    vehicle.armed = True
    
    while not vehicle.armed:
        print("Waiting for arming...")
        vehicle.armed = True
        time.sleep(1)

    print("Taking off!")

    thrust = DEFAULT_TAKEOFF_THRUST

    while True:
        current_altitude = vehicle.location.global_relative_frame.alt
        print("Altitude: %f Desired: %f" %(current_altitude, aTargetAltitude))

        print(vehicle.mode)

        if current_altitude >= aTargetAltitude * 0.95:
            print("Reached target altitude")
            break

        elif current_altitude >= aTargetAltitude * 0.6:
            thrust = SMOOTH_TAKEOFF_TURST
        
        set_attitude(thrust = thrust)
        time.sleep(0.2)

def send_attitude_target(roll_angle = 0.0, pitch_angle = 0.0, yaw_angle = None, use_yaw_rate = False, thrust = 0.5):
    if yaw_angle is None :
        yaw_angle = vehicle.attitude.yaw

    msg = vehicle.message_factory.set_attitude_target_encode(0, 1, 1, 0b00000000 if use_yaw_rate else 0b00000100, to_quaternion(roll_angle, pitch_angle, yaw_angle), 0, 0, math.radians(yaw_rate), thrust)
    vehicle.send_mavlink(msg)

def set_attitude(roll_angle = 0.0, pitch_angle = 0.0, yaw_angle = None, yaw_rate = 0.0, use_yaw_rate = False, thrust = 0.5, duration = 0):
    send_attitude_target(roll_angle, pitch_angle, yaw_angle, yaw_rate, False, thrust)
    start = time.time()

    while time.time() - start < duration:
        send_attitude_target(roll_angle, pitch_angle, yaw_angle, yaw_rate, False, thrust)
        time.sleep(0.1)
        send_attitude_target(0, 0, 0, 0, True, thrust)

def to_quaternion(roll = 0.0, pitch = 0.0, yaw = 0.0):
    t0 = math.cos(math.radians(yaw * 0.5))
    t1 = math.sin(math.radians(yaw * 0.5))
    t2 = math.cos(math.radians(roll* 0.5))
    t3 = math.sin(math.radians(roll * 0.5))
    t4 = math.cos(math.radians(pitch * 0.5))
    t5 = math.sin(math.radians(pitch * 0.5))

    w = t0 * t2 * t4 + t1 * t3 * t5
    x = t0 * t3 * t4 - t1 * t2 * t5
    y = t0 * t2 * t5 + t1 * t3 * t4
    z = t1 * t2 * t4 - t0 * t3 * t5

    return [w, x, y, z]

arm_and_takeoff_nogps(2.5)

print("Hold position for  seconds")
set_attitude(duration = 1)

print("Move forward")
set_attitude(pitch_angle = - 10, thrust = 0.5, duration = 1.21)

print("Move backward")
set_attitude(pitch_angle = 10, thrust = 0.5, duration = 1)

print("Setting LAND mode ...")
vehicle.mode = VehicleMode("LAND")
time.sleep(1)

print("Close vehivle object")
vehicle.close()

if sitl is not NOne : 
    sitl.stop()

print("Completed")
from dronekit import connect, VehicleMode
import time

vehicle = connect("/dev/ttyAMA0", wait_ready=True, baud=57600)
vehicle.mode = VehicleMode("GUIDED")
vehicle.armed = True
while not vehicle.mode.name =='GUIDED' and not vehicle.armed and not api.exit:
    print (" Getting ready to take off ...")
    time.sleep(1)

# 특정 고도로 이륙 코드
# GUIDED모드 일때 simple_takeoff 함수 제공
def arm_and_takeoff(aTargetAltitude):
    print("Taking off!")
    vehicle.simple_takeoff(aTargetAltitude)

    while True:
        print("Altitude : ", vehicle.location.global_relative_frame.alt)
        if vehicle.location.global_relative_frame.alt >= aTargetAltitude * 0.95:
            print("Reached target altitude")
            break
        time.sleep(1)

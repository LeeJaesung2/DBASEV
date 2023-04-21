# orientation | region of interest | servos 등
# target position 이나 velocity 속성으로 드론 움직임 조종 가능
from dronekit import connect, VehicleMode
import time

# 기본 시작 코드
vehicle = connect("/dev/ttyAMA0", wait_ready=True, baud=57600)
vehicle.mode = VehicleMode("GUIDED")
vehicle.armed = True
while not vehicle.mode.name =='GUIDED' and not vehicle.armed and not api.exit:
    print (" Getting ready to take off ...")
    time.sleep(1)

# 특정 (위도 경도 고도) 에 가라
a_location = LocationGlobalRelative(-34.3641114, 149.166022, 30)
vehicle.simple_goto(a_location)

# 특정 속도를 내어라
vehicle.airspeed = 5 # m/s
vehicle.groundspeed = 7.5

# 특정 위치로 특정 속도로 가라
vehicle.simple_goto(a_location, groundspeed=10)

def send_ned_velocity(velocity_x, velocity_y, velocity_z, duration):
    msg = vehicle.message_factory.set_position_target_local_ned_encode(
        0, 0, 0,
        mavutil.mavlink.MAV_FRAME_LOCAL_NED,
        
    )
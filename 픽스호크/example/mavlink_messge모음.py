# yaw 움직이는 메세지
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

# roi 움직이는 메세지
msg = vehicle.mesaage_factory.cmmand_long_encode(
    0, 0,   #target system, target component
    mavutil.mavlink.MAV_CMD_DO_SET_ROI,     #command
    0,  # confirmation
    0, 0, 0, 0, # not used
    location.lat,
    location.lon,
    location.alt
)

# goto_position_target_global_int 메세지
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

# goto_position_target_local_ned
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

# send_ned_velocity
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

# send_global_velocity
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
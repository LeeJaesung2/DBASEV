from collections import deque
from dronekit import connect, VehicleMode, LocationGlobalRelative, Command
from pymavlink import mavutil
import time

class Drone:
    def __init__(self):
        self.vehicle = None

        self.waypoint_dist = 5.0
        self.waypoint_num = int(round(100 / self.waypoint_dist))
        
        self.ishovering = False
        
        self.velocity = 0
        self.current_speed = 0
        self.max_speed = 15.0

        # current mission num
        self.cur_mission_num = 0

        # Current waypoint
        self.waypoint = 0
        self.road_id = 0

        # Target waypoint
        self.target_waypoint = 1
        self.target_waypoint_gps = LocationGlobalRelative(0.0, 0.0, 3.3)
        
        self.will_go_waypoint = deque()

    def update_drone_data(self):
        self.velocity= self.vehicle.groundspeed
        self.gps = self.vehicle.location.global_relative_frame


    def update_drone_target(self):
        # Distance between target waypoint gps and drone gps
        '''
        dist = self.gps.distance_to(self.target_waypoint_gps)
        
        if dist < 1.5:
            nxt_target = True
        else:
            nxt_target = False
        '''
        cmds = self.vehicle.commands
        

        # Update next waypoint
        if self.cur_mission_num < cmds.next:
            self.cur_mission_num = cmds.next

            if self.will_go_waypoint:
                self.ishovering = False
                cur_target_waypoint = self.will_go_waypoint.popleft()

                self.road_id = cur_target_waypoint[0]
                self.target_waypoint = cur_target_waypoint[1]

                self.target_waypoint_gps = LocationGlobalRelative(cur_target_waypoint[2], cur_target_waypoint[3], cur_target_waypoint[4])

            else:
                self.ishovering = True

    def update_drone_velocity(self, car_data):
        if self.ishovering:
            self.velocity = 0
            return
        
        # Same road
        if car_data["road_id"] == self.road_id:

            # Need to adjust velocity as the distance gets larger
            if car_data["waypoint"] <= self.waypoint - self.waypoint_num:
                ideal_velocity = car_data["velocity"] - (self.waypoint_dist * (self.waypoint - car_data.waypoint - self.waypoint_num))

                if ideal_velocity < 0:
                    ideal_velocity = 0
                elif ideal_velocity > self.max_speed:
                    ideal_velocity = self.max_speed
                
                self.velocity = ideal_velocity

            # Need to reach maximum speed as the distance gets closer
            else:
                self.velocity = self.max_speed
        
        # Need to reach maximum speed as it is on a different road
        else:
            self.velocity = self.max_speed
            
    def update_will_go_waypoint(self, waypoints):
        for waypoint in waypoints:
            self.will_go_waypoint.append(waypoint)
        


    """--------------------------------------------------------------------------------------------------------"""

    

    # Connect to Pixhawk
    def sim_connect_to_pixhawk(self):
        connection_string = 'udp:127.0.0.1:1450'
        vehicle = connect(connection_string, wait_ready=True)
        

        cmds = vehicle.commands
        cmds.download()
        cmds.wait_ready()
        cmds.clear()
        cmds.upload()

        self.vehicle = vehicle
        

        
    def connect_to_pixhawk(self):
        # Specify the path of the serial port to connect
        connection_string = "/dev/ttyAMA0" # USB: '/dev/ttyACM0' 

        # Connect to the vehicle
        vehicle = connect(connection_string, baud=57600, wait_ready=True)

        # Print information about the connected vehicle
        print('Connected to vehicle on: {}'.format(connection_string))
        print('Vehicle mode: {}'.format(vehicle.mode.name))


        cmds = vehicle.commands
        cmds.download()
        cmds.wait_ready()
        cmds.clear()
        cmds.upload()

        self.vehicle = vehicle

    # Takeoff function
    def arm_and_takeoff_to_pixhawk(self, aTargetAltitude):
        print("Drone takeoff preparation...")
        # Arm the drone
        '''
        while not self.vehicle.is_armable:
            print("Waiting for drone to become armable...")
            time.sleep(1)
        
        print("Arming the drone")
        '''
        
        self.vehicle.armed = True
        self.vehicle.mode = VehicleMode("GUIDED")
        time.sleep(1)

        # Wait for the drone to be armed
        while not self.vehicle.armed:
            print("Waiting for the drone to become armed...")
            self.vehicle.armed = True
            self.vehicle.mode = VehicleMode("GUIDED")
            time.sleep(1)

        print("Taking off")
        self.vehicle.simple_takeoff(aTargetAltitude)

        while True:
            print("Current altitude: ", self.vehicle.location.global_relative_frame.alt)
            if self.vehicle.location.global_relative_frame.alt >= aTargetAltitude * 0.95:
                print("Target altitude reached")
                break
            time.sleep(1)


    # Add waypoints to the drone
    def add_waypoints_to_pixhawk(self, waypoint_list):
        
        cmd = self.vehicle.commands
        
        for waypoint in waypoint_list:
            print("wapoint : ", waypoint[2], waypoint[3], waypoint[4])
            wp = LocationGlobalRelative(waypoint[2], waypoint[3], waypoint[4])
            cmd.add(
                Command(0,0,0, 
                    mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
                    mavutil.mavlink.MAV_CMD_NAV_WAYPOINT,
                    0, 1, 0, 0, 0, 0,
                    wp.lat, wp.lon, wp.alt
                )
            )    
        cmd.upload()
        
        self.change_vehicleMode("AUTO")


    # Set drone airspeed
    def set_airspeed_to_pixhawk(self, airspeed):
        self.current_speed = self.vehicle.groundspeed
        self.vehicle.groundspeed = airspeed

    # Landing function
    def land_to_pixhawk(self):
        self.vehicle.mode = VehicleMode("LAND") # Land the drone
        while not self.vehicle.mode.name == 'LAND':
            print("Waiting for the drone to land...")
            time.sleep(1)

        self.vehicle.armed = False # Disarm the drone
        self.vehicle.close() # Close the connection to the drone

    def change_vehicleMode(self, new_mode):
        while self.vehicle.mode.name != new_mode:
            print("Vehicl mode : ", self.vehicle.mode.name)
            print("Changing Vehcile mode to ", new_mode, "....")
            
            self.vehicle.mode = VehicleMode(new_mode)
            time.sleep(1)

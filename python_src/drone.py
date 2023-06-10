from collections import deque
from dronekit import connect, VehicleMode, LocationGlobalRelative, Command
from pymavlink import mavutil
import time
import math

class Drone:
    def __init__(self):
        self.vehicle = None

        # speed / target_speed
        self.speed = 0
        self.target_speed = 0
        self.max_speed = 15.0

        # Current waypoint
        self.waypoint = 0
        self.road_id = 0

        # GPS
        self.cur_gps = LocationGlobalRelative(0.0, 0.0, 0.0)
        
        # distanc between car and drone
        self.dist_drone_car = 0
        self.dist = 0

    def update_drone_data(self):
        self.speed= self.vehicle.groundspeed
        self.cur_gps = self.vehicle.location.global_frame

    def update_dist(self, start_point, car_data):
        # Start point coordinates
        lat1, lon1, alt1 = start_point

        # Convert degrees to radians
        alt1_rad = math.radians(alt1)
        lat1_rad = math.radians(lat1)
        lon1_rad = math.radians(lon1)

        # Current location coordinates
        current_location = self.cur_gps
        alt2_rad = math.radians(current_location.alt)
        lat2_rad = math.radians(current_location.lat)
        lon2_rad = math.radians(current_location.lon)

        # Calculate differences
        delta_lat = lat2_rad - lat1_rad
        delta_lon = lon2_rad - lon1_rad
        delta_alt = alt2_rad - alt1_rad

        # Haversine formula
        a = math.sin(delta_lat / 2) ** 2 + math.cos(lat1_rad) * math.cos(lat2_rad) * math.sin(delta_lon / 2) ** 2
        c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
        distance_2d = c * 6371 * 1000  # Earth radius = 6371km, 2D distance (ignoring altitude difference)

        # Calculate 3D distance
        self.dist = math.sqrt(distance_2d ** 2 + delta_alt ** 2)
        
        self.dist_drone_car = self.dist - car_data.dist 


    def update_drone_speed(self, car_data):
        # Same road
        if car_data["road_id"] == self.road_id:

            # if dist under 100m
            # drone have to fly to max speed
            if self.dist_drone_car < 100:
                self.target_speed = self.max_speed

            # if dist over 100m
            else:
                self.target_speed = car_data.speed - (self.dist_drone_car - 100)  
                if self.target_speed < 0 :
                    self.target_speed = 0
                elif self.target_speed > self.max_speed:
                    self.target_speed = self.max_speed
        # different road
        else:
            self.velocity = self.max_speed


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
        self.update_drone_data()
        
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
        
        self.change_vehicleMode("GUIDED")

        # Wait for the drone to be armed
        while not self.vehicle.armed:
            print("Waiting for the drone to become armed...")
            self.change_vehicleMode("GUIDED")

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
            
            if new_mode == "GUIDED":
                self.vehicle.arm = True

            self.vehicle.mode = VehicleMode(new_mode)
            time.sleep(1)
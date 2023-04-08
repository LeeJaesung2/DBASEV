from __future__ import print_function
from dronekit import connect, VehicleMode
import time
import argparse

parser = argparse.ArgumentParser(description='Print out vehicle state information. Connects to SITL on local PC by default.')
parser.add_argument('--connect', 
                    help="vehicle connection target string. If not specified, SITL automatically started and used.")
args = parser.parse_args()

connection_string = args.connect
sitl = None

#Start SITL if no connection string specified
if not connection_string:
    import dronekit_sitl
    sitl = dronekit_sitl.start_default()
    connection_string = sitl.connection_string()
    
#   Set `wait_ready=True` to ensure default attributes are populated before `connect()` returns.
print("\nConnecting to vehicle on: %s" % connection_string)
vehicle = connect(connection_string, wait_ready=True)

vehicle.wait_ready('autopilot_version')

# Get Vehicle Home location - will be `None` until first set by autopilot
while not vehicle.home_location:
    cmds = vehicle.commands
    cmds.download()
    cmds.wait_ready()
    if not vehicle.home_location:
        print(" Waiting for home location ...")
# We have a home location, so print it!        
print("\n Home location: %s" % vehicle.home_location)

print("\nSet new home location")
# Home location must be within 50km of EKF home location (or setting will fail silently)
# In this case, just set value to current location with an easily recognisable altitude (222)
my_location_alt = vehicle.location.global_frame
my_location_alt.alt = 222.0
vehicle.home_location = my_location_alt
print(" New Home Location (from attribute - altitude should be 222): %s" % vehicle.home_location)

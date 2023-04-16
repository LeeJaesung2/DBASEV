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
        car_data["car_waypoint_id"] = int(msg_list[3].strip().split()[1])
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

def setting_drone_velocity(car_data, drone_data):
    # if they are same road.
    if car_data["road_id"] == drone_data["road_id"]:
        print("car and drone was in same road")
        # if drone far more than 6 way point
        if car_data["waypoint_id"] <= drone_data["waypoint_id"] - 6:
            
            temp_velocity = car_data["velocity"] - ((16.5 * (drone_data["waypoint_id"] - car_data["waypoint_id"] - 6))/1000)
            print(f"temp_velocity : {temp_velocity}")

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

    return drone_data
    
def main():
    teststr1 = "Enter_new_road nxt velocity 42.6 nxt road_id 5 nxt way_points : nxt 0 35.18413 128.0713 3.3 1 0 nxt 1 35.18413 128.0713 3.3 1 0 nxt 2 35.18513 128.0673 3.3 1 0 nxt 3 35.18603 128.0653 3.3 1 0 nxt 4 35.18413 128.0713 3.3 1 0 nxt 5 35.18413 128.0713 3.3 1 0 nxt 6 35.18413 128.0713 3.3 1 0 nxt 7 35.18413 128.0713 3.3 1 0 nxt 8 35.18413 128.0713 3.3 1 0 nxt 9 35.18413 128.0713 3.3 1 0 nxt 10 35.18413 128.0713 3.3 1 1"
    teststr2 = "real_time_value nxt velocity 37.8 nxt road_id 5 nxt waypoint_id 6"

    car_data ={
        "road_id" : 0,
        "waypoint_id" : 0,
        "velocity" : 36.2,
    }

    drone_data ={
        "road_id" : 5,
        "waypoint_id" : 7,
        "velocity" : 0.0,
        "max_velocity" : 50.0,
        "will_go_waypoints" : []
    }

    print("\n----------------------STAGE1. SEND NEW ROAD MESSAGE----------------------\n")
    print(f"SEND DATA : {teststr1}")
    car_data, drone_data = readmsg(teststr1, car_data, drone_data)

    print("\n----------------------PRINT CAR DATA----------------------\n")
    print(car_data)
    print("\n----------------------PRINT DRONE DATA----------------------\n")
    print(drone_data)

    print("\n----------------------STAGE2. SEND NEW ROAD MESSAGE----------------------\n")
    print(f"SEND DATA : {teststr2}")
    car_data, drone_data = readmsg(teststr2, car_data, drone_data)

    print("\n----------------------PRINT CAR DATA----------------------\n")
    print(car_data)
    print("\n----------------------PRINT DRONE DATA----------------------\n")
    print(drone_data)

    print("\n----------------------STAGE3. SETTING DRONE VELOCITY----------------------\n")
    drone_data = setting_drone_velocity(car_data, drone_data)

    print(f"car velocity : {car_data['velocity']} \t car waypoint : {car_data['waypoint_id']}")
    print("\n----------------------PRINT DRONE DATA----------------------\n")
    print(drone_data)

main()
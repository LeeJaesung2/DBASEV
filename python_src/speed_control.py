def speed_control1(a, b):
    print(a+b)
    return a+b

def speed_control(msg, car_data, drone_data):
    print(msg)
    print(car_data["road_id"], car_data["waypoint_id"], car_data["velocity"])
    print(drone_data['road_id'], drone_data['waypoint_id'], drone_data['velocity'])
    print(drone_data['will_go_waypoint'][0]['latitude'])
    
    car_data["road_id"] = 123
    car_data["waypoint_id"] = 55555
    car_data["velocity"] = 10.36
    return car_data
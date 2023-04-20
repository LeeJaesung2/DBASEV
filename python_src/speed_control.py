def speed_control1(a, b):
    print(a+b)
    return a+b

def speed_control(msg, car_data, drone_data):
    print(msg)
    print(car_data[0], car_data[1], car_data[2])
    print(int(drone_data['road_id']), int(drone_data['waypoint_id']), drone_data['velocity'])
    print(drone_data['will_go_waypoint'][0]['latitude'])
    car_data ={
            "road_id" : car_data[0],
            "waypoint_id" : car_data[1],
            "velocity" : car_data[2],
        }
    return car_data
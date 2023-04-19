def speed_control1(a, b):
    print(a+b)
    return a+b

def speed_control(msg, car_data):
    print(msg)
    print(car_data[0], car_data[1], car_data[2])
    #print(drone_data[0], drone_data[1], drone_data[2], drone_data[3])
    car_data ={
            "road_id" : 11,
            "waypoint_id" : 52,
            "velocity" : 36.2,
        }
    return car_data
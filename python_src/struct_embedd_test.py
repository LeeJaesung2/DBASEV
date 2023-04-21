def embedd(a, b):
    print(a+b)
    return a+b

def sturct_embedd(msg, car_data, drone_data):
    print(msg)
    print(car_data["road_id"], car_data["waypoint_id"], car_data["velocity"])
    print(drone_data['road_id'], drone_data['waypoint_id'], drone_data['velocity'])
    print(drone_data['will_go_waypoint'][0]['latitude'])
    
    car_data["road_id"] = 123
    car_data["waypoint_id"] = 55555
    car_data["velocity"] = 10.36


    drone_data["road_id"] = 54545
    drone_data["waypoint_id"] = 1212
    drone_data["velocity"] = 80.2
    drone_data['will_go_waypoint'][0]['latitude'] = 132.5
    drone_data['will_go_waypoint'][0]['longitude'] = 56.21
    drone_data['will_go_waypoint'][0]['altitude'] = 11.2
    drone_data['will_go_waypoint'][0]['countable'] = 1
    drone_data['will_go_waypoint'][0]['last_point'] = 0


    car_drone_data = {
        'car' : car_data,
        'done' : drone_data
    }
    return car_drone_data
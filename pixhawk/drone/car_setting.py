def readmsg(msg):
    if msg == "error":
        return None
    
    msg_list = msg.strip().split()

    road_id = int(msg_list[0])
    current_waypoint = int(msg_list[1])
    velocity = float(msg_list[2])

    return [road_id, current_waypoint, velocity]
    
def init_car_data():
    car_data = {
        "position": {
            "current_waypoint" : 0,
            "road_id" : 0,
        },
        "velocity" : 0.0,
    }

    return car_data

def update_car_data(car_data, msg):
    msg_decode = readmsg(msg)
    if msg_decode:
        car_data["position"]["road_id"] = msg_decode[0]
        car_data["position"]["current_waypoint"] = msg_decode[1]
        car_data["velocity"] = msg_decode[2]
    
    return car_data

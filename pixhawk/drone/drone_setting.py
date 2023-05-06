from collections import deque


# 드론의 데이터를 초기화
def inint_drone_data(drone):
    drone_data = {
        "vehicle": drone,
        "mode" : drone.mode.name,
        "position": {
            "gps" : {
                "latitude": drone.gps_0.latitude, #위도
                "longitude" : drone.gps_0.longitude, #경도
                "altitude" : drone.gps_0.altitude, #해발고도 
                "relative_alt" : drone.gps_0.realtive_alt #기체의 상대적 고도
            },
            "current_waypoint" : 0,
            "road_id" : 0,
        },
        "target" :{
            "ishovering" : False,
            "waypoint_id" : 1,
            "waypoint_gps" : (0.0, 0.0),  # (위도, 경도)
            "will_go_waypoint" : deque(), # item format : [waypoint_id , waypoint_latitude, waypoint_logitude]
        },
        # 속력 단위 m/s로 통일
        "velocity":{ 
            "max_velocity" : 11.11,
            "current" : drone.airspeed,
        }
    }

    print("compltet init drone_data")
    return drone_data

#드론의 위치, 속도를 알고 현재 어느 
def update_drone_data(drone_data):
    vehicle = drone_data["vehicle"]
    drone_airspeed = vehicle.airspeed
    drone_gps = vehicle.gps_0

    drone_data["velocity"]["current"] = drone_airspeed
    drone_data["position"]["gps"] = {
        "latitude": drone_gps.latitude, #위도
        "longitude" : drone_gps.longitude, #경도
        "altitude" : drone_gps.altitude, #해발고도 
        "relative_alt" : drone_gps.realtive_alt #기체의 상대적 고도
    },
    
    drone_data = update_drone_target(drone_data)

    return drone_data

def update_drone_target(drone_data):
    target_pos = drone_data["target"]["target_waypoint_gps"]
    drone_pos = (drone_data["position"]["gps"]["latitude"], drone_data["position"]["gps"]["longitude"])
    dist = distance(drone_pos, target_pos)

    if dist < 1.5:
        if drone_data["target"]["will_go_waypoint"] :
            target_waypoint = drone_data["target"]["will_go_waypoint"].popleft()

            drone_data["target"]["ishovering"] = False
            drone_data["target"]["waypoint_id"] = target_waypoint[0]
            drone_data["target"]["waypoint_gps"] = (target_waypoint[1], target_waypoint[2])
        else:
            drone_data["target"]["ishovering"] = True
    
def update_drone_velocity(drone_data, car_data):
    pass
    





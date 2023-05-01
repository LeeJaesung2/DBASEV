import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import math

waypoint_dist = 10
waypoint_num = int(100/ waypoint_dist) 

def readmsg(msg, car_data, drone_data, real_car_data, error_occur):
    
    mode = None # 0 : enter new road | 1 : update data

    msg_list = msg.strip().split('nxt')
    # determine mode & update common data
    if msg_list[0].strip().split()[0] == "Enter_new_road":
        mode = 0

    elif msg_list[0].strip().split()[0] == "real_time_value":
        mode = 1
        
    if error_occur == False:    
        car_data["velocity"] = float(msg_list[1].strip().split()[1])
        car_data["road_id"] = int(msg_list[2].strip().split()[1])
        
    real_car_data["velocity"] = float(msg_list[1].strip().split()[1])
    real_car_data["road_id"] = int(msg_list[2].strip().split()[1])
    
    if mode == 1:
        if error_occur == False:
            car_data["car_waypoint_id"] = int(msg_list[3].strip().split()[1])

        real_car_data["car_waypoint_id"] = int(msg_list[3].strip().split()[1])

        return car_data, drone_data, real_car_data
    
    # add way points
    elif mode == 0:
        if error_occur == False:
            for way_point in msg_list[4:]:
                way_point_data = way_point.strip().split()
                param1 = int(way_point_data[0])
                param2 = float(way_point_data[1])
                param3 = float(way_point_data[2])
                param4 = float(way_point_data[3])
                param5 = int(way_point_data[4])
                param6 = int(way_point_data[5])

                drone_data["will_go_waypoints"].append([car_data["road_id"] , param1, param2, param3, param4, param5, param6])
        
        return car_data, drone_data, real_car_data

def setting_drone_velocity(time_value, car_data, drone_data):
    # if they are same road.
    if car_data["road_id"] == drone_data["road_id"]:
        # if drone far more than 6 way point"
        if drone_data["waypoint_id"] - car_data["waypoint_id"] <= waypoint_num: # 최고속도를 내야하는 곳
            
            max_velocity = drone_data["velocity"] + (drone_data["acelate"] * time_value) * 18 / 5
            
            if max_velocity > drone_data["max_velocity"]:
                drone_data["velocity"] = drone_data["max_velocity"]
                # print("최고속도를 내고있어요")
            else:
                drone_data["velocity"] = max_velocity
               
        # if drone less than 6 way point.    
        else:   # 6개 보다 더 감속이 필요한 경우
            ideal_value = car_data["velocity"] - (((drone_data["waypoint_id"] - waypoint_num) - car_data["waypoint_id"]) * waypoint_dist * 18/5)  #(m/s)
            min_drone_velocity = drone_data["velocity"] - (drone_data["acelate"] * time_value) * 18 / 5
            max_drone_velocity = drone_data["velocity"] + (drone_data["acelate"] * time_value) * 18 / 5
            
            if ideal_value < min_drone_velocity:
                drone_data["velocity"] = min_drone_velocity
            elif ideal_value > max_drone_velocity:
                drone_data["velocity"] = max_drone_velocity
            else:
                drone_data["velocity"] = ideal_value

    # if they are different road.
    else: # 최고속도를 내야해요
        ideal_value = drone_data["velocity"] + (drone_data["acelate"] * time_value) * 18 / 5
            
        if ideal_value > drone_data["max_velocity"]:
            drone_data["velocity"] = drone_data["max_velocity"]
            # print("최고속도를 내고있어요")
        else:
            drone_data["velocity"] = ideal_value

    return drone_data
    
def data_update(initflag, time_value, msg, car_data = None, drone_data=None, real_car_data =None, error_occur = False):
    if initflag :
        car_data ={
            "road_id" : 0,
            "waypoint_id" : 0,
            "velocity" : 36.2,
            "distance" : 0
        }

        real_car_data = {
            "road_id" : 0,
            "waypoint_id" : 0,
            "velocity" : 36.2,
            "distance" : 0     
        }

        drone_data ={
            "road_id" : 31,
            "waypoint_id" : 0,
            "velocity" : 0.0, #(km/h)
            "max_velocity" : 40.0, #(km/h)
            "acelate" : 3.7, #(m/s^2)
            "distance" : 0, #(m)
            "will_go_waypoints" : []
        }

        car_data, drone_data, real_car_data = readmsg(msg, car_data, drone_data, real_car_data, error_occur)
        return car_data, drone_data, real_car_data

    pre_drone_velocity = drone_data["velocity"]
    pre_car_velocity = car_data["velocity"]
    pre_real_car_velocity = real_car_data["velocity"]
    
    car_data, drone_data, real_car_data = readmsg(msg, car_data, drone_data,real_car_data, error_occur)
    drone_data = setting_drone_velocity(time_value, car_data, drone_data)


    drone_data["distance"] += ((pre_drone_velocity+drone_data["velocity"]) * 0.5 * 1000 / 3600) * time_value
    drone_data["waypoint_id"] = int(drone_data["distance"] / waypoint_dist)


    car_data["distance"] += ((pre_car_velocity + car_data["velocity"]) * 0.5 * 1000 / 3600) * time_value
    car_data["waypoint_id"] = int(car_data["distance"] / waypoint_dist)
    
    real_car_data["distance"] += ((pre_real_car_velocity + real_car_data["velocity"]) * 0.5 * 1000 / 3600) * time_value
    real_car_data["waypoint_id"] = int(real_car_data["distance"] / waypoint_dist)


    return car_data, drone_data, real_car_data

def set_data(time_value, random_speed, is_error):
    time = 0

    error_data = []

    time_data = []

    real_car_dist_data =[]
    real_car_velocity_data = []

    drone_dist_data = []
    drone_velocity_data = []
    
    distance_data = []

    init_reach = False
    error_occur = False

    for i in range(500):
        if i == 0:
            car_data, drone_data, real_car_data = data_update(1, time_value,  "Enter_new_road nxt velocity 30 nxt road_id 31 nxt way_points nxt 0 35.1545167671935 128.093429803848 3.3 1 0 nxt 1 35.1544905 128.0936088 3.3 1 0 nxt 2 35.1544647 128.1937879 3.3 1 0 nxt 3 35.154489 128.0939662 3.3 1 0 nxt 4 35.154411 128.0941446 3.3 1 0 nxt 5 35.1543874 128.0943243 3.3 1 0 nxt 6 35.1543594 128.094502 3.3 1 0 nxt 7 35.1543342 128.0946804 3.3 1 0 nxt 8 35.1543079 128.0948594 3.3 1 0 nxt 9 35.1542827 128.0950378 3.3 1 0 nxt 10 35.1542553 128.0952162 3.3 1 0 nxt 11 35.15423 128.0953945 3.3 1 0 nxt 12 35.1542048 128.0955729 3.3 1 0 nxt 13 35.154178 128.0957513 3.3 1 0 nxt 14 35.1541533 128.0959303 3.3 1 0 nxt 15 35.1541264 128.0961087 3.3 1 0 nxt 16 35.1540996 128.0962877 3.3 1 0 nxt 17 35.1540743 128.0964667 3.3 1 0 nxt 18 35.154048 128.0966451 3.3 1 0 nxt 19 35.1540222 128.0968241 3.3 1 0 nxt 20 35.1539948 128.0970025 3.3 1 0 nxt 21 35.1539696 128.0971809 3.3 1 0 nxt 22 35.1539438 128.0973592 3.3 1 0 nxt 23 35.1539159 128.0975369 3.3 1 0 nxt 24 35.1538907 128.0977153 3.3 1 0 nxt 25 35.1538654 128.0978937 3.3 1 0 nxt 26 35.1538391 128.098072 3.3 1 0 nxt 27 35.1538128 128.0982511 3.3 1 0 nxt 28 35.153787 128.0984294 3.3 1 0 nxt 29 35.1537608 128.0986078 3.3 1 0 nxt 30 35.1537344 128.0987862 3.3 1 0 nxt 31 35.1537092 128.0989652 3.3 1 0 nxt 32 35.1536818 128.0991436 3.3 1 0 nxt 33 35.153656 128.0993226 3.3 1 0 nxt 34 35.153603 128.099501 3.3 1 0 nxt 35 35.1536056 128.09968 3.3 1 0 nxt 36 35.1535793 128.0998591 3.3 1 0 nxt 37 35.1535519 128.1000374 3.3 1 0 nxt 38 35.1535261 128.1002158 3.3 1 0 nxt 39 35.1534998 128.1003942 3.3 1 0 nxt 40 35.153474 128.1005725 3.3 1 0 nxt 41 35.1534488 128.1007509 3.3 1 0 nxt 42 35.1534219 128.1009299 3.3 1 0 nxt 43 35.1533961 128.1011083 3.3 1 0 nxt 44 35.1533704 128.01012867 3.3 1 0 nxt 45 35.1533441 128.101465 3.3 1 0 nxt 46 35.1533172 128.1016434 3.3 1 0 nxt 47 35.1532925 128.1018224 3.3 1 0 nxt 48 35.1532657 128.1020005 3.3 1 0 nxt 49 35.152448 128.102147 3.3 1 1")
            
            time_data.append(time)
            
            # car_dist_data.append(car_data["distance"])
            # car_velocity_data.append(car_data["velocity"])

            real_car_dist_data.append(real_car_data["distance"])
            real_car_velocity_data.append(real_car_data["velocity"])

            drone_dist_data.append(drone_data["distance"])
            drone_velocity_data.append(drone_data["velocity"])
            
            distance_data.append(abs(drone_data["distance"]-real_car_data["distance"]))
            continue

        for j in range(4):
            time += time_value

            if (is_error == 1) and (j < 2) and ( ( i == 20) or (i==30) or ( i==120 ) or (i==230 ) or (i==340)):
                error_occur = True
            else:
                error_occur = False
            
            current_car_waypoint = int(car_data["distance"]/waypoint_dist)
            
            if random_speed == 1:
                current_car_speed = abs(math.sin(i)) * 45
            else : 
                current_car_speed = 30

            data = f"real_time_value nxt velocity {current_car_speed} nxt road_id 31 nxt waypoint_id {current_car_waypoint}"
            
            car_data, drone_data, real_car_data = data_update(0, time_value, data, car_data, drone_data, real_car_data, error_occur)
            
            time_data.append(time)

            # car_dist_data.append(car_data["distance"])
            # car_velocity_data.append(car_data["velocity"])

            real_car_dist_data.append(real_car_data["distance"])
            real_car_velocity_data.append(real_car_data["velocity"])
            
            drone_dist_data.append(drone_data["distance"])
            drone_velocity_data.append(drone_data["velocity"])
            
            distance_data.append(abs(drone_data["distance"]-real_car_data["distance"]))

            # 이상치 와 오차 관련 코드

            if (init_reach == False) and abs(drone_data["distance"]-real_car_data["distance"]) >= 100:
                init_reach = True
                init_reach_time.append(time)

            if init_reach :
                error_data.append(abs(drone_data["distance"]-real_car_data["distance"]))


    time_arr = np.array(time_data)

    real_car_dist_arr = np.array(real_car_dist_data)
    real_car_velocity_arr = np.array(real_car_velocity_data)
    
    drone_dist_arr = np.array(drone_dist_data)
    drone_velocity_arr = np.array(drone_velocity_data)
    
    distance_arr = np.array(distance_data)

    time_plots.append(time_arr)
    
    real_car_dist_plots.append(real_car_dist_arr)
    real_car_velocity_plots.append(real_car_velocity_arr)

    drone_dist_plots.append(drone_dist_arr)
    drone_velocity_plots.append(drone_velocity_arr)
    
    dist_plots.append(distance_arr)
    
    if error_data:
      max_error = max(error_data)
      min_error = min(error_data)
      errors_dist.append((max_error, min_error))


def draw_plot( ):
    fig, axes = plt.subplots(nrows = 4, ncols=3, figsize=(30, 30))
    for idx, time, car_velocity, drone_velocity, car_dist, drone_dist, dist in zip(range(4), time_plots, real_car_velocity_plots, drone_velocity_plots, real_car_dist_plots, drone_dist_plots, dist_plots):
        axes[idx, 0].text(-0.5, 0.5, f'{situation[idx]}', transform=axes[idx, 0].transAxes, fontsize=18, fontweight='bold', va='center',  bbox=dict(facecolor='white', alpha=0.5, pad=10))

        axes[idx, 0].plot(time, car_velocity, color="blue", label="car")
        axes[idx, 0].plot(time, drone_velocity, color="red", label="drone")
        axes[idx, 0].set_title("car vs drone velocity", fontsize=28)
        axes[idx, 0].set_xlabel("time", fontsize=18)
        axes[idx, 0].set_ylabel("velocity", fontsize=18)
        axes[idx, 0].set_xlim(0, 100)
        axes[idx, 0].set_ylim(0, 50)
        axes[idx, 0].legend()

        axes[idx, 1].plot(time, car_dist, color= "blue", label="car")
        axes[idx, 1].plot(time, drone_dist, color="red", label="drone")
        axes[idx, 1].set_title("car vs drone distance", fontsize=28)
        axes[idx, 1].set_xlabel("time", fontsize=18)
        axes[idx, 1].set_ylabel("distance", fontsize=18)
        axes[idx, 1].set_xlim(0, 100)
        axes[idx, 1].set_ylim(0, 1000)
        axes[idx, 1].legend()
        
        axes[idx, 2].plot(time, dist)
        axes[idx, 2].set_title("Distance between car and drone", fontsize=28)
        axes[idx, 2].set_xlabel("time", fontsize=18)
        axes[idx, 2].set_ylabel("velocity", fontsize=18)
        axes[idx, 2].set_xlim(0, 100)
        axes[idx, 2].set_ylim(0, 120)

    plt.tight_layout()
    plt.show()

situation =["30km/h constant speed \n communication error X", "30km/h constant speed \n communication error O", "random speed change \n communication error X", "random speed change \n communication error O"]

time_plots = []
real_car_dist_plots = []
real_car_velocity_plots =[]
drone_dist_plots = []
drone_velocity_plots =[]
dist_plots = []
errors_dist = []
init_reach_time = []

set_data(0.5, 0, 0)
set_data(0.5, 0, 1)
set_data(0.5, 1, 0)
set_data(0.5, 1, 1)

draw_plot()

for item in init_reach_time:
  print(item, end="\t")
print()


for i in errors_dist:
    print(i, end="\t")
print()
    



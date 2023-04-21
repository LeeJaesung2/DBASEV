import matplotlib.pyplot as plt
import numpy as np
import math


def readmsg(msg, car_data, drone_data):
    if msg == "Error":
        return car_data, drone_data
    
    mode = None # 0 : enter new road | 1 : update data

    msg_list = msg.strip().split('nxt')
    # determine mode & update common data
    if msg_list[0].strip().split()[0] == "Enter_new_road":
        mode = 0
        print("Enter new road")

    elif msg_list[0].strip().split()[0] == "real_time_value":
        mode = 1
        
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

def setting_drone_velocity(time_value, car_data, drone_data):
    # if they are same road.
    if car_data["road_id"] == drone_data["road_id"]:
        # if drone far more than 6 way point"
        if drone_data["waypoint_id"] - car_data["waypoint_id"] <= 6: # 최고속도를 내야하는 곳
            
            max_velocity = drone_data["velocity"] + (drone_data["acelate"] * time_value) * 18 / 5
            
            if max_velocity > drone_data["max_velocity"]:
                drone_data["velocity"] = drone_data["max_velocity"]
                # print("최고속도를 내고있어요")
            else:
                drone_data["velocity"] = max_velocity

        elif drone_data["waypoint_id"] == car_data["waypoint_id"] + 6:
                min_drone_velocity = drone_data["velocity"] - drone_data["acelate"] * time_value
                max_drone_velocity = drone_data["velocity"] + drone_data["acelate"] * time_value

                if car_data["velocity"] > min_drone_velocity and car_data["velocity"] < max_drone_velocity:
                    drone_data["velocity"] = car_data["velocity"]
                elif car_data["velocity"] <= min_drone_velocity:
                    drone_data["velocity"] = min_drone_velocity
                elif car_data["velovity"] >= max_drone_velocity:
                    drone_data = max_drone_velocity
        # if drone less than 6 way point.    
        else:   # 감속이 필요한 경우
            ideal_value = car_data["velocity"] - (((drone_data["waypoint_id"]-6) - car_data["waypoint_id"]) * 16.5 * 18/5)  #(m/s)

            if ideal_value < 0:
                drone_data["velocity"] = 0
                # print("호버링")
            else:
                if ideal_value > drone_data["max_velocity"]:
                    # print("최고속도가 나올 수 있나 감속하는데?")
                    pass
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
    
def main(initflag, time_value, msg, car_data = None, drone_data=None):
    #car_data = None, drone_data = None
    if initflag :
        car_data ={
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
        car_data, drone_data = readmsg(msg, car_data, drone_data)
        return car_data, drone_data

    pre_drone_velocity = drone_data["velocity"]
    pre_car_velocity = car_data["velocity"]
    
    
    car_data, drone_data = readmsg(msg, car_data, drone_data)
    drone_data = setting_drone_velocity(time_value, car_data, drone_data)


    drone_data["distance"] += ((pre_drone_velocity+drone_data["velocity"]) * 0.5 * 1000 / 3600) * time_value
    drone_data["waypoint_id"] = int(drone_data["distance"] / 16.5)

    car_data["distance"] += ((pre_car_velocity + car_data["velocity"]) * 0.5 * 1000 / 3600) * time_value
    car_data["waypoint_id"] = int(car_data["distance"] / 16.5)


#   print(f"\ndrone way point : {drone_data['waypoint_id']}")
#   print(f"drone velocity : {drone_data['velocity']}") 
#   print(f"\ndrone distance : {drone_data['distance']}")
#   print(f"car way point : {car_data['waypoint_id']}")
#   print(f"car distance : {car_data['distance']}\n")
#   print(f"car velocity : {car_data['velocity']}\n")
    

    return car_data, drone_data

def draw_data(time_value, random_speed, is_error):
    car_data, drone_data = main(1, time_value,  "Enter_new_road nxt velocity 30 nxt road_id 31 nxt way_points nxt 0 35.1545167671935 128.093429803848 3.3 1 0 nxt 1 35.1544905 128.0936088 3.3 1 0 nxt 2 35.1544647 128.1937879 3.3 1 0 nxt 3 35.154489 128.0939662 3.3 1 0 nxt 4 35.154411 128.0941446 3.3 1 0 nxt 5 35.1543874 128.0943243 3.3 1 0 nxt 6 35.1543594 128.094502 3.3 1 0 nxt 7 35.1543342 128.0946804 3.3 1 0 nxt 8 35.1543079 128.0948594 3.3 1 0 nxt 9 35.1542827 128.0950378 3.3 1 0 nxt 10 35.1542553 128.0952162 3.3 1 0 nxt 11 35.15423 128.0953945 3.3 1 0 nxt 12 35.1542048 128.0955729 3.3 1 0 nxt 13 35.154178 128.0957513 3.3 1 0 nxt 14 35.1541533 128.0959303 3.3 1 0 nxt 15 35.1541264 128.0961087 3.3 1 0 nxt 16 35.1540996 128.0962877 3.3 1 0 nxt 17 35.1540743 128.0964667 3.3 1 0 nxt 18 35.154048 128.0966451 3.3 1 0 nxt 19 35.1540222 128.0968241 3.3 1 0 nxt 20 35.1539948 128.0970025 3.3 1 0 nxt 21 35.1539696 128.0971809 3.3 1 0 nxt 22 35.1539438 128.0973592 3.3 1 0 nxt 23 35.1539159 128.0975369 3.3 1 0 nxt 24 35.1538907 128.0977153 3.3 1 0 nxt 25 35.1538654 128.0978937 3.3 1 0 nxt 26 35.1538391 128.098072 3.3 1 0 nxt 27 35.1538128 128.0982511 3.3 1 0 nxt 28 35.153787 128.0984294 3.3 1 0 nxt 29 35.1537608 128.0986078 3.3 1 0 nxt 30 35.1537344 128.0987862 3.3 1 0 nxt 31 35.1537092 128.0989652 3.3 1 0 nxt 32 35.1536818 128.0991436 3.3 1 0 nxt 33 35.153656 128.0993226 3.3 1 0 nxt 34 35.153603 128.099501 3.3 1 0 nxt 35 35.1536056 128.09968 3.3 1 0 nxt 36 35.1535793 128.0998591 3.3 1 0 nxt 37 35.1535519 128.1000374 3.3 1 0 nxt 38 35.1535261 128.1002158 3.3 1 0 nxt 39 35.1534998 128.1003942 3.3 1 0 nxt 40 35.153474 128.1005725 3.3 1 0 nxt 41 35.1534488 128.1007509 3.3 1 0 nxt 42 35.1534219 128.1009299 3.3 1 0 nxt 43 35.1533961 128.1011083 3.3 1 0 nxt 44 35.1533704 128.01012867 3.3 1 0 nxt 45 35.1533441 128.101465 3.3 1 0 nxt 46 35.1533172 128.1016434 3.3 1 0 nxt 47 35.1532925 128.1018224 3.3 1 0 nxt 48 35.1532657 128.1020005 3.3 1 0 nxt 49 35.152448 128.102147 3.3 1 1")
    time = 0

    time_data = [0]
    car_dist_data =[0]
    drone_dist_data = [0]
    distance_data = [0]

    for i in range(500):
        for j in range(4):
            
            time += time_value
            # print("time : ", time)
            time_data.append(time)
            if is_error == 1 and i % 100 == 0 and j < 2:
                  data = "Error"
            else:
                  current_car_waypoint = int(car_data["distance"]/16.5)
                  if random_speed == 1:
                        current_car_speed = abs(math.sin(i)) * 40
                  else : 
                        current_car_speed = 30

                  data = f"real_time_value nxt velocity {current_car_speed} nxt road_id 31 nxt waypoint_id {current_car_waypoint}"
            
            car_data, drone_data = main(0, time_value, data, car_data, drone_data)
            
            car_dist_data.append(car_data["distance"])
            drone_dist_data.append(drone_data["distance"])
            distance_data.append(abs(drone_data["distance"]-car_data["distance"]))

    time_numpy = np.array(time_data)
    car_numpy = np.array(car_dist_data)
    drone_numpy = np.array(drone_dist_data)
    distance_numpy = np.array(distance_data)

    return time_numpy, car_numpy, drone_numpy, distance_numpy


time_plot_data1, car_plot_data1, drone_plot_data1, dis_plot_data1 = draw_data(0.5, 0, 0)
time_plot_data2, car_plot_data2, drone_plot_data2, dis_plot_data2 = draw_data(0.5, 0, 1)
time_plot_data3, car_plot_data3, drone_plot_data3, dis_plot_data3 = draw_data(0.5, 1, 0)
time_plot_data4, car_plot_data4, drone_plot_data4, dis_plot_data4 = draw_data(0.5, 1, 1)


plt.subplot(4, 2, 1)
plt.plot(time_plot_data1, car_plot_data1, time_plot_data1, drone_plot_data1, 'r-')
plt.xlim([0, 100])
plt.ylim([0, 1000])

plt.subplot(4, 2, 2)
plt.plot(time_plot_data1, dis_plot_data1)
plt.xlim([0, 100])
plt.ylim([0, 120])

plt.subplot(4, 2, 3)
plt.plot(time_plot_data2, car_plot_data2, time_plot_data2, drone_plot_data2, 'r-')
plt.xlim([0, 100])
plt.ylim([0, 1000])

plt.subplot(4, 2, 4)
plt.plot(time_plot_data2, dis_plot_data2)
plt.xlim([0, 100])
plt.ylim([0, 120])

plt.subplot(4, 2, 5)
plt.plot(time_plot_data3, car_plot_data3, time_plot_data3, drone_plot_data3, 'r-')
plt.xlim([0, 100])
plt.ylim([0, 1000])

plt.subplot(4, 2, 6)
plt.plot(time_plot_data3, dis_plot_data3)
plt.xlim([0, 100])
plt.ylim([0, 120])

plt.subplot(4, 2, 7)
plt.plot(time_plot_data4, car_plot_data4, time_plot_data4, drone_plot_data4, 'r-')
plt.xlim([0, 100])
plt.ylim([0, 1000])

plt.subplot(4, 2, 8)
plt.plot(time_plot_data4, dis_plot_data4)
plt.xlim([0, 100])
plt.ylim([0, 120])

plt.show()

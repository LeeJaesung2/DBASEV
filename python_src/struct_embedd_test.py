from messagequeue import *
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

def consumer(a):
    BUFFER_SIZE = 26
    msgcount = 0

    # Get message queue ID using same key as C++ program
    key = 5656
    mq = mq_init(key)

    print("Message queue created with ID:", mq)
    # Read messages from queue
    while True:
        # Read message from queue
        value, msg = pop(mq)

        # Print message contents
        print("Received value:", value)
        print("Message data:", msg)
        if (value!=-1):
            msgcount+=1

        if value >= 99999:
            print("All messages received!")
            break

    # Clean up message queue
    #mq.close()
    #sysv_ipc.unlink_message_queue(key)
    print("received message number is ", msgcount)
    return 0

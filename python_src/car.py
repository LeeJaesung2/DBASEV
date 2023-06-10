# Object that decodes communication messages and stores the state of the car

class Car:
    def __init__(self):
        self.waypoint = 0
        self.road_id = 0
        self.speed = 0.0
        self.dist = 0.0
        
    def update_car_data(self, msg):
        if msg == "error":
            return None
        
        msg_list = msg.strip().split('/')

        self.speed = float(msg_list[0])
        self.road_id = int(msg_list[1])
        self.waypoint = int(msg_list[2])
        self.dist = float(msg_list[3])

    def get_car_data(self):
        return {
            "waypoint": self.waypoint,
            "road_id": self.road_id,
            "speed": self.speed,
            "dist" : self.dist
        }

# 통신 메세지를 디코딩하고 자동차의 상태를 저장하는 객체

class Car:
    def __init__(self) :
        self.waypoint = 0
        self.road_id = 0
        self.velocity = 0.0
    
    def readmsg(self, msg):
        if msg == "error":
            return None
        
        msg_list = msg.strip().split()

        self.velocity = float(msg_list[0])
        self.road_id = int(msg_list[1])
        self.waypoint = int(msg_list[2])
        
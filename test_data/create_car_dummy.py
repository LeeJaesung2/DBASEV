def testCarMsg():
    """
    1 - 37
    2 38 - 78
    3 
    """

    waypoint = 0
    road_id = 1
    speed = 5

    msgs = []

    for i in range(93):
        waypoint += 1
        if waypoint < 38:
            road_id = 1
        elif waypoint < 79:
            road_id = 2
        else:
            road_id = 3 
            
        #20번 마다 waypoint 바뀜
        for _ in range(20):
            msg = f"{speed} {road_id} {waypoint}"
            msgs.append(msg)
            
    return msgs

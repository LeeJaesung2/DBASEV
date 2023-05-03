from dronekit import connect, VehicleMode

def connect_to_pixhawk():
    # 연결할 시리얼 포트의 경로를 지정합니다.
    port = "/dev/ttyAMA0" #USB : '/dev/ttyACM0' 
    connection_string = 'com{}'.format(port)

    # 연결합니다.
    vehicle = connect(connection_string, baud=57600, wait_ready=True)

    # 연결된 기체의 정보를 출력합니다.
    print('Connected to vehicle on: {}'.format(connection_string))
    print('Vehicle mode: {}'.format(vehicle.mode.name))

    # 기체를 안전한 모드로 설정합니다.
    vehicle.mode = VehicleMode('STABILIZE')

    return vehicle
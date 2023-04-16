from dronekit import connect

# 첫번째 파라미터
vehicle = connect('127.0.0.1:14550', wait_ready=True)
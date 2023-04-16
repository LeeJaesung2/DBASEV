# 기본 시작 코드
vehicle = connect("/dev/ttyAMA0", wait_ready=True, baud=57600)
vehicle.mode = VehicleMode("GUIDED")
vehicle.armed = True
while not vehicle.mode.name =='GUIDED' and not vehicle.armed and not api.exit:
    print (" Getting ready to take off ...")
    time.sleep(1)
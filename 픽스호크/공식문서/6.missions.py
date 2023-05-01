# https://dronekit-python.readthedocs.io/en/latest/guide/auto_mode.html
# mission commands: MAV_CMD_NAV_TAKEOFF, MAV_CMD_NAV_WAYPOINT

import tkinter


vehicle = connect('127.0.0.1:14550', wait_ready=True)

# Download the vehicle waypoints (commands). Wait until download is complete.
cmds = vehicle.commands
cmds.download()
cmds.wait_ready()

cmds.clear() # 명령어 초기화
cmds.upload()

#명령어 생성 & 추가
cmd1=Command( 0, 0, 0, mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT, mavutil.mavlink.MAV_CMD_NAV_TAKEOFF, 0, 0, 0, 0, 0, 0, 0, 0, 10)
cmd2=Command( 0, 0, 0, mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT, mavutil.mavlink.MAV_CMD_NAV_WAYPOINT, 0, 0, 0, 0, 0, 0, 10, 10, 10)
cmds.add(cmd1)
cmds.add(cmd2)
cmds.upload() # Send commands

# 미션 수정 
missionlist = []
for cmd in cmds:
    missionlist.append(cmd)

missionlist[0].command = mavutil.mavlink.MAV_CMD_NAV_TAKEOFF # 수정 하는 코드

cmds.clear()
for cmd in missionlist:
    cmds.add(cmd)

cmds.upload()

# 미션을 시작하기 위해서 AUTO모드로 바꾸기(공중에 있을때 바꾸기)
vehicle.mode = VehicleMode("AUTO")

# vehicle.commands.next : 현재 커맨드 번호를 얻음 & 현재 커맨드 번호를 바꿈
vehicle.commands.next=2
print ("Current Waypoint: %s" % vehicle.commands.next)
vehicle.commands.next=4
print ("Current Waypoint: %s" % vehicle.commands.next)

# 미션 업로드 함수
def upload_mission(aFileName):
        """
        Upload a mission from a file.
        """
        #Read mission from file
        missionlist = readmission(aFileName)

        print ("\nUpload mission from a file: %s" % import_mission_filename)
        #Clear existing mission from vehicle
        print (' Clear mission')
        cmds = vehicle.commands
        cmds.clear()
        #Add new mission to vehicle
        for command in missionlist:
            cmds.add(command)
        print (' Upload mission')
        vehicle.commands.upload()

# 미션 read 함수
# 파일의 형식은 아래 함수의 else 참고
def readmission(aFileName):
    print ("Reading mission from file: %s\n" % aFileName)
    cmds = vehicle.commands
    missionlist=[]
    with open(aFileName) as f:
        for i, line in enumerate(f):
            if i==0:
                if not line.startswith('QGC WPL 110'):
                    raise Exception('File is not supported WP version')
            else:
                linearray=line.split('\t')
                ln_index=int(linearray[0])
                ln_currentwp=int(linearray[1])
                ln_frame=int(linearray[2])
                ln_command=int(linearray[3])
                ln_param1=float(linearray[4])
                ln_param2=float(linearray[5])
                ln_param3=float(linearray[6])
                ln_param4=float(linearray[7])
                ln_param5=float(linearray[8])
                ln_param6=float(linearray[9])
                ln_param7=float(linearray[10])
                ln_autocontinue=int(linearray[11].strip())
                cmd = Command( 0, 0, 0, ln_frame, ln_command, ln_currentwp, ln_autocontinue, ln_param1, ln_param2, ln_param3, ln_param4, ln_param5, ln_param6, ln_param7)
                missionlist.append(cmd)
    return missionlist

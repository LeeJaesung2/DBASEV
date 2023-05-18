# g++ -c src/modules/speed_control/speed_control.cpp -I/usr/include/python2.7 -lpython2.7
# g++ -c raspberryPi/drone/main_process.cpp
# g++ -c src/modules/embedd/embedd.cpp -I/usr/include/python2.7 -lpython2.7
# g++ main_process.o speed_control.o embedd.o -o main -I/usr/include -lpthread -I/usr/include/python2.7 -lpython2.7


# make object file all of modules
# make object file main process 
# make execution file with all of object file

CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpthread -ltbb -std=c++14
PYFLAGS = -I/usr/include/python2.7 -lpython2.7
SRC_DIR=./src
OBJ_DIR=./build
DRONE_DIR=./raspberryPi/drone
VEHICLE_DIR=./raspberryPi/vehicle
PYTHONE_DIR=./python_src
INSTALL_DIR=/usr/include/DBASEV


# Create build directories if they don't exist
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(OBJ_DIR)/drivers/gps)
$(shell mkdir -p $(OBJ_DIR)/drivers/radar)
$(shell mkdir -p $(OBJ_DIR)/drivers/telemetry)
$(shell mkdir -p $(OBJ_DIR)/modules/collision_avoidance)
$(shell mkdir -p $(OBJ_DIR)/modules/communication)
$(shell mkdir -p $(OBJ_DIR)/modules/embedd)
$(shell mkdir -p $(OBJ_DIR)/modules/drone_control)
$(shell mkdir -p $(OBJ_DIR)/modules/messagequeue)
$(shell mkdir -p $(OBJ_DIR)/modules/vehicle_control)

# List all objcet files to be compiled
SRC=$(wildcard $(SRC_DIR)/**/**/*.cpp)
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

#path of drone
DRONE_MAIN=$(DRONE_DIR)/drone_main_process.o
DRONE_EXECUTABLE=$(OBJ_DIR)/drone.exe

#path of vehicle
VEHICLE_MAIN=$(VEHICLE_DIR)/vehicle_main_process.o
VEHICLE_EXECUTABLE=$(OBJ_DIR)/vehicle.exe


#target for execution file
all : $(DRONE_EXECUTABLE) $(VEHICLE_EXECUTABLE)

#make drone.exe
$(DRONE_EXECUTABLE): $(OBJ) $(DRONE_MAIN)
		$(CC) $^ -o $(DRONE_EXECUTABLE) $(LDFLAGS) $(PYFLAGS)

#make vehicle.exe
$(VEHICLE_EXECUTABLE): $(OBJ) $(VEHICLE_MAIN)
		$(CC) $^ -o $(VEHICLE_EXECUTABLE) $(LDFLAGS) $(PYFLAGS)

#make dreon_main_process.o
$(DRONE_MAIN) : $(DRONE_DIR)/drone_main_process.cpp
		$(CC) $(CFLAGS) $(DRONE_DIR)/drone_main_process.cpp -o $(DRONE_MAIN)

#make vehicle_main_process.o
$(VEHICLE_MAIN) : $(VEHICLE_DIR)/vehicle_main_process.cpp
		$(CC) $(CFLAGS) $(VEHICLE_DIR)/vehicle_main_process.cpp -o $(VEHICLE_MAIN)

#make all of object file from modules and drivers
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(PYFLAGS) $< -o $@ 


# Clean all object files and executable
clean:
	rm -rf $(OBJ_DIR)/**/**/*.o $(DRONE_DIR)/drone_main_process.o $(OBJ_DIR)/drone.exe $(VEHICLE_DIR)/vehicle_main_process.o $(OBJ_DIR)/vehicle.exe  $(PYTHONE_DIR)/*.pyc

install:
	mkdir -p $(INSTALL_DIR)
	$(shell mkdir -p $(INSTALL_DIR)/telemetry)
	cp -r src/include/*.h $(INSTALL_DIR)
	cp -r src/include/telemetry/*.h $(INSTALL_DIR)/telemetry

#program start command
exedrone:
	./$(DRONE_EXECUTABLE)

exevehicle:
	./$(VEHICLE_EXECUTABLE)
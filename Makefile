CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpthread
SRC_DIR=./src
#INC_DIR=./src/include
OBJ_DIR=./build
DRONE_DIR=./raspberryPi/drone
VEHICLE_DIR=./raspberryPi/vehicle

# List all source files to be compiled
SRC=$(wildcard $(SRC_DIR)/drivers/**/*.cpp $(SRC_DIR)/modules/**/*.cpp)
OBJ=$(SRC:$(SRC_DIR)/**/**/%.cpp=$(OBJ_DIR)/**/**/%.o)
DRONE_MAIN=$(DRONE_DIR)/main_process.o
DRONE_EXECUTABLE=$(DRONE_DIR)/main_process.exe

VEHICLE_MAIN=$(VEHICLE_DIR)/main_process.o
VEHICLE_EXECUTABLE=$(VEHICLE_DIR)/main_process.exe

all : $(DRONE_EXECUTABLE) $(VEHICLE_EXECUTABLE)

# Build executable from object files
$(DRONE_EXECUTABLE): $(OBJ) $(DRONE_MAIN)
	$(CC) $^ -o $(DRONE_EXECUTABLE) $(LDFLAGS)

$(VEHICLE_EXECUTABLE): $(OBJ) $(VEHICLE_MAIN)
	$(CC) $^ -o $(VEHICLE_EXECUTABLE) $(LDFLAGS)

$(DRONE_MAIN) : $(DRONE_DIR)/main_process.cpp
	$(CC) $(CFLAGS) -o $(DRONE_MAIN) $(DRONE_DIR)/main_process.cpp

$(VEHICLE_MAIN) : $(VEHICLE_DIR)/main_process.cpp
	$(CC) $(CFLAGS) -o $(VEHICLE_MAIN) $(VEHICLE_DIR)/main_process.cpp

# Rules for building object files
#$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
#	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@






# Clean all object files and executable
clean:
	rm $(DRONE_DIR)/main_process.o $(DRONE_DIR)/main_process.exe $(VEHICLE_DIR)/main_process.o $(VEHICLE_DIR)/main_process.exe

exedrone:
	./$(DRONE_EXECUTABLE)

exevehicle:
	./$(VEHICLE_EXECUTABLE)
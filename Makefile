CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpthread
SRC_DIR=./src
#INC_DIR=./src/include
OBJ_DIR=./build
DRONE_DIR=./rasberryPi/drone
VEHICLE_DIR=./raspberryPi/vehicle

# List all source files to be compiled
SRC=$(wildcard $(SRC_DIR)/drivers/**/*.cpp $(SRC_DIR)/modules/**/*.cpp)
OBJ=$(SRC:$(SRC_DIR)/**/**/%.cpp=$(OBJ_DIR)/**/**/%.o)
MAIN=$(DRONE_DIR)/main_process.o
EXECUTABLE=$(DRONE_DIR)/main_process.exe

all : $(EXECUTABLE)	

# Build executable from object files
$(EXECUTABLE): $(OBJ) $(MAIN)
	$(CC) $^ -o $(EXECUTABLE) $(LDFLAGS)

# Rules for building object files
#$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
#	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@






# Clean all object files and executable
clean:
	rm $(DRONE_DIR)/main_process.o $(DRONE_DIR)/main_process.exe

execute:
	./rasberryPi/drone/main_process.exe	
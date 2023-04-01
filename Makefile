CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SRC_DIR=./src
INC_DIR=./src/include
OBJ_DIR=./src/obj
DRONE_DIR=./rasberryPi/drone
VEHICLE_DIR=./raspberryPi/vehicle

# List all source files to be compiled
SRC=$(wildcard $(SRC_DIR)/drivers/*.cpp $(SRC_DIR)/modules/*.cpp)
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
MAIN=$(DRONE_DIR)/main_process
EXECUTABLE=$(DRONE_DIR)/main_process.cpp

# Rules for building object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@

# Build executable from object files
all: $(OBJ) $(MAIN)
	$(CC) $(LDFLAGS) $^ -o $(EXECUTABLE)

# Clean all object files and executable
clean:
	rm -rf $(OBJ_DIR)/*.o $(DRONE_DIR)/main_process.exe

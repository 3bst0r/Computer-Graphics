CC = g++

OBJ = Cube.o Cylinder.o LoadShader.o Shape.o Transformation.o
TARGET = MerryGoRound

CFLAGS = -g -Wall
LDLIBS = -lm -lglut -lGLEW -lGL
INCLUDES = -Isource

SRC_DIR = source
BUILD_DIR = build
VPATH = source

# Rules
all: $(TARGET)

$(TARGET).o: $(TARGET).cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

clean:
	rm -f $(BUILD_DIR)/*.o *.o $(TARGET) 

.PHONY: clean

# Dependencies
$(TARGET): $(BUILD_DIR)/Cube.o $(BUILD_DIR)/Cylinder.o $(BUILD_DIR)/LoadShader.o $(BUILD_DIR)/Shape.o  $(BUILD_DIR)/Transformation.o | $(BUILD_DIR)


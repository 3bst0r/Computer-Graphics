CC = g++
OBJ = MerryGoRound.o LoadShader.o Transformation.o Shape.o Cylinder.o
CFLAGS = -g -Wall -Wextra

LDLIBS=-lm -lglut -lGLEW -lGL

MerryGoRound: $(OBJ)
	 $(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o MerryGoRound 
.PHONY: all clean

#ifndef CUBE_H
	#define CUBE_H
	
	#include "Shape.h"
	
	class Cube : public Shape {
		public:
			Cube(double size, double x_val, double y_val, double z_val);
			~Cube();
	};
	
#endif

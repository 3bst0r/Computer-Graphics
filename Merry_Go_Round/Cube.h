#ifndef CUBE_H
	#define CUBE_H
	
	#include "Form.h"
	
	class Cube : public Form {
		public:
			Cube(double size, double x_val, double y_val, double z_val);
			~Cube();
	};
	
#endif

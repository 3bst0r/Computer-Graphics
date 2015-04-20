#ifndef CYLINDER_H
	#define CYLINDER_H
	
	#include "Form.h"
	
	class Cylinder : public Form {
		public:
			Cylinder(int points, double radius, double height);
			~Cylinder();
	};
	
#endif

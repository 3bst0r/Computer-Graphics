#ifndef CYLINDER_H
	#define CYLINDER_H
	
	
	
	class Cylinder : public Shape {
		public:
			Cylinder(int points, double radius, double height, double x_offset, double y_offset, double z_offset);
			~Cylinder();
	};
	
#endif

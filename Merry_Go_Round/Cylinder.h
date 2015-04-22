#ifndef CYLINDER_H
	#define CYLINDER_H
	
	
	
	class Cylinder : public Shape {
		public:
			Cylinder(int points, double radius, double height, double y_offset);
			~Cylinder();
	};
	
#endif

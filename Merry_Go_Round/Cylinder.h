/******************************************************************
*
* Cylinder.h
*
* Description: Class to create a cylinder shape
*
*******************************************************************/

#ifndef __CYLINDER_H__
	#define __CYLINDER_H__
	
	
	
	class Cylinder : public Shape {
		public:
			Cylinder(int points, double radius, double height, double x_offset, double y_offset, double z_offset);
			~Cylinder();
	};
	
#endif

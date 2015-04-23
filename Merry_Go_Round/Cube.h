/******************************************************************
*
* Cube.h
*
* Description: Class to create a cube shape
*
*******************************************************************/

#ifndef __CUBE_H__
	#define __CUBE_H__
	
	class Cube : public Shape {
		public:
			Cube(double size, double x_val, double y_val, double z_val);
			~Cube();
	};
	
#endif

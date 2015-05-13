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
			Cube(float size, float x_val, float y_val, float z_val);
			~Cube();
	};
	
#endif

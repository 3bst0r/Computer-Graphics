/******************************************************************
*
* Lightsource.h
*
* Description: Class representing a point light with its coordinates 
* 			   and RGB color values
*
*******************************************************************/

#ifndef __LIGHTSOURCE_H__
	#define __LIGHTSOURCE_H__
	
	#include <GL/glut.h>
	#include <GL/gl.h>
	
	class Lightsource {
		public:
			Lightsource(double x_offset, double y_offset, double z_offset, float color_r, float color_g, float color_b);
			~Lightsource();
			GLfloat* vertex_buffer_data;
			GLfloat* color_buffer_data;
			GLushort* index_buffer_data;
	};
	
#endif

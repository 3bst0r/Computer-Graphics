/******************************************************************
*
* Shape.h
*
* Description: Class to create basic shapes
*
*******************************************************************/

#ifndef __SHAPE_H__
	#define __SHAPE_H__
	
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include "Texture.h"
	
	class Shape {
		public:
			Shape(int vertex_number, int triangle_number);
			~Shape();
			void compute_normals();
			void print_vertices();
			void print_indices();
			void print_normals();
			void add_shape(Shape* shape);

			Texture* texture;
			GLfloat* vertex_buffer_data;
			GLfloat* color_buffer_data;
			GLfloat* normal_buffer_data;
			GLushort* index_buffer_data;
			GLfloat* uv_buffer_data;
			int vertex_number;
			int triangle_number;
			double center_x;
			double center_y;
			double center_z;
			GLfloat kD;
			GLfloat kA;
			GLfloat kS;
	};
	
#endif

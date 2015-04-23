#ifndef SHAPE_H
	#define SHAPE_H
	
	#include <GL/glut.h>
	#include <GL/gl.h>
	
	class Shape {
		public:
			Shape(int vertex_number, int triangle_number);
			~Shape();
			void print_vertices();
			void print_indices();
			void add_shape(Shape* shape);
			
			GLfloat* vertex_buffer_data;
			GLfloat* color_buffer_data;
			GLushort* index_buffer_data;
			int vertex_number;
			int triangle_number;
			double center_x;
			double center_y;
			double center_z;
	};
	
#endif

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
			void add_form(Shape shape);
			GLfloat* vertex_buffer_data = NULL;
			GLfloat* color_buffer_data = NULL;
			GLushort* index_buffer_data = NULL;
			int vertex_number;
			int triangle_number;
	};
	
#endif

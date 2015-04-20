#ifndef FORM_H
	#define FORM_H
	
	#include <GL/glut.h>
	#include <GL/gl.h>
	
	class Form {
		public:
			Form(int vertex_number, int triangle_number);
			~Form();
			void print_vertices();
			void print_indices();
			GLfloat* vertex_buffer_data = NULL;
			GLfloat* color_buffer_data = NULL;
			GLushort* index_buffer_data = NULL;
			int vertex_number;
			int triangle_number;
	};
	
#endif

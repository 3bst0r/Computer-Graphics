#include <iostream>
#include <cstring>

#include "Shape.h"
#include "Cube.h"

using namespace std;

Cube::Cube(double size, double x_val, double y_val, double z_val) /* XYZ values of the cubes center and half of the edge length(size)*/
: Shape(8, 12){
	center_x = x_val;
	center_y = y_val;
	center_z = z_val;
	GLfloat vertex_buffer_data_tmp[] = { /* 8 cube vertices XYZ */
		 (x_val - size), (y_val - size),  (z_val + size),
		 (x_val + size), (y_val - size),  (z_val + size),
		 (x_val + size), (y_val + size),  (z_val + size),
		 (x_val - size), (y_val + size),  (z_val + size),
		 (x_val - size), (y_val - size),  (z_val - size),
		 (x_val + size), (y_val - size),  (z_val - size),
		 (x_val + size), (y_val + size),  (z_val - size),
		 (x_val - size), (y_val + size),  (z_val - size),
	};   

	GLfloat color_buffer_data_tmp[] = { /* RGB color values for 8 vertices */
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
	}; 

	GLushort index_buffer_data_tmp[] = { /* Indices of 6*2 triangles (6 sides) */
		0, 1, 2,
		2, 3, 0,
		1, 5, 6,
		6, 2, 1,
		7, 6, 5,
		5, 4, 7,
		4, 0, 3,
		3, 7, 4,
		4, 5, 1,
		1, 0, 4,
		3, 2, 6,
		6, 7, 3,
	};
	
	memcpy(vertex_buffer_data, vertex_buffer_data_tmp, 3 * 8 * sizeof(GLfloat));
	memcpy(color_buffer_data, color_buffer_data_tmp, 3 * 8 * sizeof(GLfloat));
	memcpy(index_buffer_data, index_buffer_data_tmp, 3 * 12 * sizeof(GLshort));
}

Cube::~Cube(){
	
}

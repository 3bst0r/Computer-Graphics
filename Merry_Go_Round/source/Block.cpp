/******************************************************************
*
* Block.cpp
*
* Description: Class to create a block-like shape with variable side lengths
*
*******************************************************************/

#include <iostream>
#include <cstring>

#include "Shape.h"
#include "Block.h"

using namespace std;

/******************************************************************
*
* Block::Block
*
* creates a block object with the given specification
*
*******************************************************************/
Block::Block(float x_val, float y_val, float z_val, float height, float width, float thickness) /* XYZ values of the blocks center and length of the 3 different edge sizes*/
: Shape(36, 12){
	center_x = x_val;
	center_y = y_val;
	center_z = z_val;
	GLfloat vertex_help[] = {
			(x_val - (width / 2)), (y_val - (height / 2)), (z_val + (thickness / 2)), // 0
			(x_val + (width / 2)), (y_val - (height / 2)), (z_val + (thickness / 2)), // 1
			(x_val + (width / 2)), (y_val + (height / 2)), (z_val + (thickness / 2)), // 2
			(x_val - (width / 2)), (y_val + (height / 2)), (z_val + (thickness / 2)), // 3
			(x_val - (width / 2)), (y_val - (height / 2)), (z_val - (thickness / 2)), // 4
			(x_val + (width / 2)), (y_val - (height / 2)), (z_val - (thickness / 2)), // 5
			(x_val + (width / 2)), (y_val + (height / 2)), (z_val - (thickness / 2)), // 6
			(x_val - (width / 2)), (y_val + (height / 2)), (z_val - (thickness / 2)) // 7
	};
	
	GLfloat color_help[] = {
			0.0, 0.0, 1.0,
			1.0, 0.0, 1.0,
			1.0, 1.0, 1.0,
			0.0, 1.0, 1.0,
			0.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 1.0, 0.0,
			0.0, 1.0, 0.0
	};

	GLfloat uv_help[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1,
			0, 0,
			1, 0,
			1, 1,
			0, 1
	};


	int indices[] = {
			0,1,2,
			0,2,3,
			1,5,6,
			1,6,2,
			4,0,3,
			4,3,7,
			4,5,1,
			4,1,0,
			3,2,6,
			3,6,7,
			5,4,7,
			5,7,6
	};
	for (int i = 0; i < 36; i++) {
		vertex_buffer_data[3*i] = vertex_help[3*indices[i]];
		vertex_buffer_data[3*i+1] = vertex_help[3*indices[i]+1];
		vertex_buffer_data[3*i+2] = vertex_help[3*indices[i]+2];
		uv_buffer_data[2 * i] = uv_help[2 * indices[i]];
		uv_buffer_data[2 * i] = uv_help[2 * indices[i] + 1];

		color_buffer_data[3*i] = color_help[3*indices[i]];
		color_buffer_data[3*i+1] = color_help[3*indices[i]+1];
		color_buffer_data[3*i+2] = color_help[3*indices[i]+2];
	}

	GLshort* index_buffer_data_tmp = (GLshort *) malloc(36 * sizeof(GLshort));
	for (GLushort i = 0; i < 36; i++) {
		index_buffer_data[i] = i;
	}

	compute_normals();
}

/******************************************************************
*
* Block::~Block
*
* default destructor
*
*******************************************************************/
Block::~Block(){
	
}

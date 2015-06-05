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
: Shape(8, 12){
	center_x = x_val;
	center_y = y_val;
	center_z = z_val;
	GLfloat vertex_buffer_data_tmp[] = { /* 8 block vertices XYZ */
		 (x_val - (width/2)), (y_val - (height/2)),  (z_val + (thickness/2)),
		 (x_val + (width/2)), (y_val - (height/2)),  (z_val + (thickness/2)),
		 (x_val + (width/2)), (y_val + (height/2)),  (z_val + (thickness/2)),
		 (x_val - (width/2)), (y_val + (height/2)),  (z_val + (thickness/2)),
		 (x_val - (width/2)), (y_val - (height/2)),  (z_val - (thickness/2)),
		 (x_val + (width/2)), (y_val - (height/2)),  (z_val - (thickness/2)),
		 (x_val + (width/2)), (y_val + (height/2)),  (z_val - (thickness/2)),
		 (x_val - (width/2)), (y_val + (height/2)),  (z_val - (thickness/2)),
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
	/* copy tmp data buffer into our buffer */
	memcpy(vertex_buffer_data, vertex_buffer_data_tmp, 3 * 8 * sizeof(GLfloat));
	memcpy(color_buffer_data, color_buffer_data_tmp, 3 * 8 * sizeof(GLfloat));
	memcpy(index_buffer_data, index_buffer_data_tmp, 3 * 12 * sizeof(GLshort));
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

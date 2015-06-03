/******************************************************************
*
* Cylinder.cpp
*
* Description: Class to create a cylinder shape
*
*******************************************************************/


#include <iostream>
#include <cmath>

#include "Shape.h"
#include "Cylinder.h"

using namespace std;

/******************************************************************
*
* Cylinder::Cylinder
*
* creates a basic cylinder object with the given specification
*
*******************************************************************/
Cylinder::Cylinder(int points, double radius, double height, double x_offset, double y_offset, double z_offset, double top_center_offset, float color_r, float color_g, float color_b)
: Shape(points * 2 + 2, 4 * points){
	center_x = x_offset;
	center_y = y_offset + height / 2;
	center_z = z_offset;

	for(int i = 0; i < 2 * points + 2; i++){
		color_buffer_data[3 * i] = color_r;
		color_buffer_data[3 * i + 1] = color_g;
		color_buffer_data[3 * i + 2] = color_b;
	}
	
	for(int i = 0; i < points; i++){
		vertex_buffer_data[3 * i] = radius * cos(M_PI * 2 * i / points) + x_offset;
		vertex_buffer_data[3 * i + 1] = y_offset;
		vertex_buffer_data[3 * i + 2] = radius * sin(M_PI * 2 * i / points) + z_offset;
		
		vertex_buffer_data[3 * points + 3 * i] = radius * cos(M_PI * 2 * i / points) + x_offset;
		vertex_buffer_data[3 * points + 3 * i + 1] = y_offset + height;
		vertex_buffer_data[3 * points + 3 * i + 2] = radius * sin(M_PI * 2 * i / points) + z_offset;
	}
	
	// set center
	vertex_buffer_data[6 * points] = x_offset;
	vertex_buffer_data[6 * points + 1] = y_offset;
	vertex_buffer_data[6 * points + 2] = z_offset;
	
	vertex_buffer_data[6 * points + 3] = x_offset;
	vertex_buffer_data[6 * points + 4] = y_offset + height + top_center_offset;
	vertex_buffer_data[6 * points + 5] = z_offset;
	
	for(int i = 0; i < points; i++){
		index_buffer_data[12 * i + 0] = i;
		index_buffer_data[12 * i + 1] = (i + 1) % points;
		index_buffer_data[12 * i + 2] = i + points;
		
		index_buffer_data[12 * i + 3] = i + points;
		index_buffer_data[12 * i + 4] = (i + 1) % points + points;
		index_buffer_data[12 * i + 5] = (i + 1) % points;
		
		index_buffer_data[12 * i + 6] = i;
		index_buffer_data[12 * i + 7] = (i + 1) % points;
		index_buffer_data[12 * i + 8] = 2 * points;
		
		index_buffer_data[12 * i + 9] = points + i;
		index_buffer_data[12 * i + 10] = (i + 1) % points + points;
		index_buffer_data[12 * i + 11] = 2 * points + 1;
	}	
}

/******************************************************************
*
* Cylinder::~Cylinder
*
* default destructor
*
*******************************************************************/
Cylinder::~Cylinder(){

}

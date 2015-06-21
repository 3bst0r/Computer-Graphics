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
: Shape(points * 2 * 2 + 2, 4 * points) {
	center_x = x_offset;
	center_y = y_offset + height / 2;
	center_z = z_offset;

	for (int i = 0; i < 4 * points + 2; i++) {
		color_buffer_data[3 * i] = color_r;
		color_buffer_data[3 * i + 1] = color_g;
		color_buffer_data[3 * i + 2] = color_b;
	}
	/* create circles */
	for (int i = 0; i < points; i++) {
		vertex_buffer_data[3 * i] = radius * cos(M_PI * 2 * i / points) + x_offset;
		vertex_buffer_data[3 * i + 1] = y_offset;
		vertex_buffer_data[3 * i + 2] = radius * sin(M_PI * 2 * i / points) + z_offset;
		uv_buffer_data[2 * i] = 0.5 + (0.5 - height) * cos(M_PI * 2 * i / points);
		uv_buffer_data[2 * i + 1] = 0.5 + (0.5 - height) * sin(M_PI * 2 * i / points);

		vertex_buffer_data[3 * points + 3 * i] = radius * cos(M_PI * 2 * i / points) + x_offset;
		vertex_buffer_data[3 * points + 3 * i + 1] = y_offset + height;
		vertex_buffer_data[3 * points + 3 * i + 2] = radius * sin(M_PI * 2 * i / points) + z_offset;
		uv_buffer_data[2 * points + 2 * i] = 0.5 + 0.5 * cos(M_PI * 2 * i / points);
		uv_buffer_data[2 * points + 2 * i + 1] = 0.5 + 0.5 * sin(M_PI * 2 * i / points);

		// duplicate vertices for hard edges
		vertex_buffer_data[3 * i + 2 * 3 * points] = radius * cos(M_PI * 2 * i / points) + x_offset;
		vertex_buffer_data[3 * i + 1 + 2 * 3 * points] = y_offset;
		vertex_buffer_data[3 * i + 2 + 2 * 3 * points] = radius * sin(M_PI * 2 * i / points) + z_offset;
		uv_buffer_data[2 * i] = 0.5 + (0.5 - height) * cos(M_PI * 2 * i / points);
		uv_buffer_data[2 * i + 1] = 0.5 + (0.5 - height) * sin(M_PI * 2 * i / points);

		vertex_buffer_data[3 * points + 3 * i + 2 * 3 * points] = radius * cos(M_PI * 2 * i / points) + x_offset;
		vertex_buffer_data[3 * points + 3 * i + 1 + 2 * 3 * points] = y_offset + height;
		vertex_buffer_data[3 * points + 3 * i + 2 + 2 * 3 * points] = radius * sin(M_PI * 2 * i / points) + z_offset;
		uv_buffer_data[2 * points + 2 * i] = 0.5 + 0.5 * cos(M_PI * 2 * i / points);
		uv_buffer_data[2 * points + 2 * i + 1] = 0.5 + 0.5 * sin(M_PI * 2 * i / points);

	}

	// set center
	vertex_buffer_data[4 * 3 * points] = x_offset;
	vertex_buffer_data[4 * 3 * points + 1] = y_offset;
	vertex_buffer_data[4 * 3 * points + 2] = z_offset;
	vertex_buffer_data[4 * 3 * points + 3] = x_offset;
	vertex_buffer_data[4 * 3 * points + 4] = y_offset + height + top_center_offset;
	vertex_buffer_data[4 * 3 * points + 5] = z_offset;
	uv_buffer_data[4 * 2 * points] = 0.5;
	uv_buffer_data[4 * 2 * points + 1] = 0.5;
	uv_buffer_data[4 * 2 * points + 2] = 0.5;
	uv_buffer_data[4 * 2 * points + 3] = 0.5;


	for (int i = 0; i < points; i++) {
		// bottom
		index_buffer_data[12 * i + 6] = i;
		index_buffer_data[12 * i + 7] = (i + 1) % points;
		index_buffer_data[12 * i + 8] = 4 * points;
		// top
		index_buffer_data[12 * i + 9] = points + i;
		index_buffer_data[12 * i + 11] = (i + 1) % points + points;
		index_buffer_data[12 * i + 10] = 4 * points + 1;

		// sides (using duplicate circle points)
		int j = i + 2 * points;
		index_buffer_data[12 * i + 0] = j;
		index_buffer_data[12 * i + 2] = j + points;
		index_buffer_data[12 * i + 1] = 2 * points + (i + 1) % points + points;

		index_buffer_data[12 * i + 3] = 2 * points + (i + 1) % points + points;
		index_buffer_data[12 * i + 4] = j;
		index_buffer_data[12 * i + 5] = 2 * points + (i + 1) % points;
	}
	compute_normals();
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

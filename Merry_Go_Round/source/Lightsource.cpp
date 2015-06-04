#include "Lightsource.h"

using namespace std;

/******************************************************************
*
* Lightsource.h
*
* Description: Class representing a point light with its coordinates 
* 			   and RGB color values
*
*******************************************************************/

Lightsource::Lightsource(double x_offset, double y_offset, double z_offset, float color_r, float color_g, float color_b){
	
			vertex_buffer_data = (GLfloat*)malloc(3 * sizeof(GLfloat));
			vertex_buffer_data[0] = x_offset;
			vertex_buffer_data[1] = y_offset;
			vertex_buffer_data[2] = z_offset;
			color_buffer_data = (GLfloat*)malloc(3 * sizeof(GLfloat));
			color_buffer_data[0] = color_r;
			color_buffer_data[1] = color_g;
			color_buffer_data[2] = color_b;
			index_buffer_data = (GLushort*)malloc(1 * sizeof(GLushort));
			index_buffer_data[0] = 0;
}

/******************************************************************
*
* Lightsource::~Lightsource
*
* free the allocated memory
*
*******************************************************************/
Lightsource::~Lightsource(){
	if(vertex_buffer_data != NULL)
		free(vertex_buffer_data);
	if(color_buffer_data != NULL)
		free(color_buffer_data);
}

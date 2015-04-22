#include <iostream>
#include <cmath>

#include "Shape.h"
#include "Cylinder.h"

using namespace std;

Cylinder::Cylinder(int points, double radius, double height, double y_offset)
: Form(points * 2 + 2, 4 * points){
	//set color(white)
	for(int i = 0; i < 6 * points + 2; i++){
		color_buffer_data[i] = (float)rand() / RAND_MAX;
	}
	/*
	for(int i = 0; i < (points/3); i++){
		color_buffer_data[3*i] = 1.;
		color_buffer_data[3*i + points] = 1.;
	}
	for(int i = (points/3); i < (2 * (points/3)); i++){
		color_buffer_data[3*i+1] = 1.;
		color_buffer_data[3*i+1 + points] = 1.;
	}
	for(int i = (2 * (points/3)); i < points; i++){
		color_buffer_data[3*i+2] = 1.;
		color_buffer_data[3*i+2 + points] = 1.;
	}*/
	
	for(int i = 0; i < 6; i++){
		color_buffer_data[3 * 2 * points + i] = 1.;
	}
	
	for(int i = 0; i < points; i++){
		vertex_buffer_data[3 * i] = radius * cos(M_PI * 2 * i / points);
		vertex_buffer_data[3 * i + 1] = y_offset;
		vertex_buffer_data[3 * i + 2] = radius * sin(M_PI * 2 * i / points);
		
		vertex_buffer_data[3 * points + 3 * i] = radius * cos(M_PI * 2 * i / points);
		vertex_buffer_data[3 * points + 3 * i + 1] = y_offset + height;
		vertex_buffer_data[3 * points + 3 * i + 2] = radius * sin(M_PI * 2 * i / points);
	}
	
	// set center
	vertex_buffer_data[6 * points] = 0.;
	vertex_buffer_data[6 * points + 1] = y_offset;
	vertex_buffer_data[6 * points + 2] = 0.;
	
	vertex_buffer_data[6 * points + 3] = 0.;
	vertex_buffer_data[6 * points + 4] = y_offset + height;
	vertex_buffer_data[6 * points + 5] = 0.;
	
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

Cylinder::~Cylinder(){

}
